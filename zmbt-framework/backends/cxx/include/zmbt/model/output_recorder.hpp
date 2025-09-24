/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_ENV_OUTPUT_RECORDER_HPP_
#define ZMBT_ENV_OUTPUT_RECORDER_HPP_

#include <atomic>
#include <bitset>
#include <deque>
#include <functional>
#include <memory>
#include <thread>
#include <typeindex>
#include <typeinfo>

#include <boost/json.hpp>
#define BOOST_UNORDERED_DISABLE_REENTRANCY_CHECK
#include <boost/unordered/concurrent_flat_map.hpp>

#include "zmbt/application/log.hpp"
#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"

#include "channel_kind.hpp"
#include "global_flags.hpp"
#include "error_or_return.hpp"


namespace zmbt {

struct output_recorder_error : public base_error {
    using base_error::base_error;
};

class OutputRecorder
{
  public:

    template <class T>
    struct Frame
    {
        std::size_t ts;
        T data {reflect::signal_traits<T>::init()};
    };

  private:
    class Registry;

    using consumer_fn_t = std::function<void(boost::json::value&&)>;


    template <class I>
    static std::type_index get_args_typeid(I const& interface)
    {
        using reflection  = reflect::invocation<I const&>;
        using args_t      = typename reflection::args_t;
        using unqf_args_t = tuple_unqf_t<args_t>;
        return {typeid(unqf_args_t)};
    }

    struct Registry
    {
        struct FrameBuffs
        {
            std::shared_ptr<void> args{};
            std::shared_ptr<void> ret{};
            std::shared_ptr<void> err{};
            bool include_tid{false};
        };

        using FramesBuffMap = boost::concurrent_flat_map<std::thread::id, FrameBuffs, std::hash<std::thread::id>>;
        using extract_fn_t = std::function<std::size_t(std::shared_ptr<FramesBuffMap>, consumer_fn_t const)>;

        std::type_index data_typeid;
        boost::json::string interface_name;
        extract_fn_t extract_fn;
        std::shared_ptr<FramesBuffMap> frame_buff_map;

        boost::json::array serialized_frames{};
        std::atomic<std::size_t> count{0};
        std::atomic<std::size_t> lost_count{0};

        std::bitset<static_cast<unsigned>(ChannelKind::Undefined)> enable_categories_{};

        Registry(
            std::type_index typid,
            boost::json::string name,
            extract_fn_t efn,
            std::shared_ptr<FramesBuffMap> fbm
        )
            : data_typeid(typid)
            , interface_name{name}
            , extract_fn(efn)
            , frame_buff_map{fbm}
        {
        }

        ~Registry()
        {
        }


        template <class I>
        static std::shared_ptr<Registry> Make()
        {
            using ArgsTuple = reflect::invocation_args_unqf_t<I const&>;
            using Return = reflect::invocation_ret_unqf_or_nullptr_t<I const&>;

            std::type_index const typid {typeid(std::tuple<ArgsTuple, Return>)};

            auto extract_fn = [](std::shared_ptr<FramesBuffMap> frame_buff_map, consumer_fn_t const& consumer_fn) -> std::size_t {
                using FB = Registry::FrameBuffs;
                using ThreadFrameBuffers = std::pair<std::thread::id, FB>;

                constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

                // Snapshot thread buffers
                std::vector<ThreadFrameBuffers> fbs;
                fbs.reserve(frame_buff_map->size());
                frame_buff_map->visit_all([&](auto& record){
                    fbs.push_back(record);
                });

                auto get_next_ts = [](FB& fb) -> std::size_t {
                    std::size_t ts = npos;
                    if (fb.args) {
                        auto& dq = *std::static_pointer_cast<std::deque<Frame<ArgsTuple>>>(fb.args);
                        if (!dq.empty()) ts = std::min(ts, dq.front().ts);
                    }
                    if (fb.ret) {
                        auto& dq = *std::static_pointer_cast<std::deque<Frame<Return>>>(fb.ret);
                        if (!dq.empty()) ts = std::min(ts, dq.front().ts);
                    }
                    if (fb.err) {
                        auto& dq = *std::static_pointer_cast<std::deque<Frame<ErrorInfo>>>(fb.err);
                        if (!dq.empty()) ts = std::min(ts, dq.front().ts);
                    }
                    return ts;
                };

                struct Cursor {
                    std::size_t ts {npos}; // next timestamp
                    ThreadFrameBuffers* buffs{nullptr};
                };

                // initialize candidates
                std::vector<Cursor> cursors;
                cursors.reserve(fbs.size());
                for (auto& tfbs : fbs) {
                    cursors.push_back({get_next_ts(tfbs.second), &tfbs});
                }

                std::size_t frame_count = 0;
                Cursor null{};
                for (;;) {
                    // find candidate with smallest ts
                    Cursor* cursor = &null;

                    for (auto& candidate: cursors)
                    {
                        if (candidate.ts < cursor->ts) {
                            cursor = &candidate;
                        }
                    }
                    if (cursor == &null)
                        break; // all empty

                    boost::json::object json_frame{
                        {"ts",  cursor->ts},
                    };

                    FB& fb = cursor->buffs->second;

                    if (fb.include_tid)
                    {
                        json_frame["tid"] = tid2str(cursor->buffs->first);
                    }

                    // consume exactly one element of each deque matching this ts
                    if (fb.args) {
                        auto& dq = *std::static_pointer_cast<std::deque<Frame<ArgsTuple>>>(fb.args);
                        if (!dq.empty() && dq.front().ts == cursor->ts) {
                            json_frame["args"] = json_from(dq.front().data);
                            dq.pop_front();
                        }
                    }
                    if (fb.ret) {
                        auto& dq = *std::static_pointer_cast<std::deque<Frame<Return>>>(fb.ret);
                        if (!dq.empty() && dq.front().ts == cursor->ts) {
                            json_frame["return"] = json_from(dq.front().data);
                            dq.pop_front();
                        }
                    }
                    if (fb.err) {
                        auto& dq = *std::static_pointer_cast<std::deque<Frame<ErrorInfo>>>(fb.err);
                        if (!dq.empty() && dq.front().ts == cursor->ts) {
                            json_frame["exception"] = dq.front().data.to_json();
                            dq.pop_front();
                        }
                    }

                    consumer_fn(std::move(json_frame));
                    ++frame_count;

                    // refresh this candidate’s ts
                    cursor->ts = get_next_ts(fb);
                }

                return frame_count;
            };
            auto frame_buff_map = std::make_shared<FramesBuffMap>();

            return std::make_shared<Registry>(typid, type_name<I>().c_str(), extract_fn, frame_buff_map);
        }
    };

    /// Single-consumer extraction
    std::size_t consume_all(consumer_fn_t const consume_fn);

    template <ChannelKind ck>
    bool check_filter()
    {
        return registry_->enable_categories_[static_cast<unsigned>(ck)];
    }

    void report_test_error(ErrorInfo const&) const;


  public:


    OutputRecorder(interface_id const& ifc_id, object_id const& obj_id);
    ~OutputRecorder();

    OutputRecorder(OutputRecorder const&) = default;
    OutputRecorder(OutputRecorder &&) = default;
    OutputRecorder& operator=(OutputRecorder const&) = default;
    OutputRecorder& operator=(OutputRecorder &&) = default;

    /// Flush interal buffers and complete serialization
    void flush();

    template <class I>
    void setup_handlers()
    {
        if (registry_ != nullptr) return;
        registry_ = Registry::Make<ifc_pointer_t<I>>();
    }


    /// Multi-producer push
    template <class ArgsTuple, class Return>
    void push(ArgsTuple const& args, ErrorOr<Return> const& return_or_error)
    {
        if (!ensure_registry())
        {
            ErrorInfo e;
            e.type = type_name<output_recorder_error>();
            e.what = "push to unregistered output recorder";
            e.context = "OutputRecorder";
            report_test_error(e);
            return;
        }

        std::type_index const ti {typeid(std::tuple<ArgsTuple, Return>)};

        if (ti != registry_->data_typeid)
        {
            ErrorInfo e;
            e.type = type_name<output_recorder_error>();
            e.what = "invalid type on push";
            e.context = "OutputRecorder";
            report_test_error(e);
            return;
        }

        if (!flags::TestIsRunning::status())
        {
            registry_->lost_count++;
            // TODO: redesign me
            // lost call may come from SUT initialization
            // before expectations are st, which may be ok to skip
            return;
        }

        registry_->count++;

        auto const ts = get_ts();

        bool const args_en = registry_->enable_categories_[static_cast<unsigned>(ChannelKind::Args)];
        bool const ret_en = registry_->enable_categories_[static_cast<unsigned>(ChannelKind::Return)];
        bool const err_en = registry_->enable_categories_[static_cast<unsigned>(ChannelKind::Exception)];
        bool const tid_en = registry_->enable_categories_[static_cast<unsigned>(ChannelKind::ThreadId)];

        boost::optional<Registry::FrameBuffs&> maybe_fb_ref{};

        registry_->frame_buff_map->try_emplace_or_visit(std::this_thread::get_id(), Registry::FrameBuffs{},
            [&maybe_fb_ref](auto& record){
            maybe_fb_ref = record.second;
        });

        if (!maybe_fb_ref.has_value())
        {
            registry_->frame_buff_map->visit(std::this_thread::get_id(),
                [&maybe_fb_ref](auto& record){
                maybe_fb_ref = record.second;
            });
        }

        if (!maybe_fb_ref.has_value())
        {
            ErrorInfo e;
            e.type = type_name<output_recorder_error>();
            e.what = "corrupted output recorder";
            e.context = "OutputRecorder";
            report_test_error(e);
            return;
        }

        Registry::FrameBuffs& fb = maybe_fb_ref.value();

        if (!fb.args) fb.args = std::make_shared<std::deque<Frame<ArgsTuple>>>();
        if (!fb.ret ) fb.ret  = std::make_shared<std::deque<Frame<Return>>>();
        if (!fb.err ) fb.err  = std::make_shared<std::deque<Frame<ErrorInfo>>>();
        if (tid_en) fb.include_tid = true;

        std::deque<Frame<ArgsTuple>>& fb_args = *std::static_pointer_cast<std::deque<Frame<ArgsTuple>>>(fb.args);
        std::deque<Frame<Return>>&    fb_ret  = *std::static_pointer_cast<std::deque<Frame<Return>>>(fb.ret);
        std::deque<Frame<ErrorInfo>>& fb_err  = *std::static_pointer_cast<std::deque<Frame<ErrorInfo>>>(fb.err);

        if (args_en) fb_args.push_back({ts, args});
        if (ret_en && return_or_error.is_return()) fb_ret.push_back({ts, return_or_error.as_return()});

        if (return_or_error.is_error())
        {
            if (err_en)
            {
                fb_err.push_back({ts, return_or_error.as_error()});
            }
            else
            {
                report_test_error(return_or_error.as_error());
                registry_->enable_categories_.reset();
            }
        }
    }

    /// Push calls count
    std::size_t count() const
    {
        return registry_ ? registry_->count.load(std::memory_order_relaxed) : 0UL;
    }

    /// Push calls count after the capture is closed
    std::size_t lost_count() const
    {
        return registry_ ? registry_->lost_count.load(std::memory_order_relaxed) : 0UL;
    }

    /// OutputRecorder
    bool is_registered() const
    {
        return registry_ != nullptr;
    }

    boost::json::array const& data_frames() const;

    void clear();

    void enable_category(ChannelKind const ck);

  private:

    interface_id ifc_id_;
    object_id obj_id_;
    std::shared_ptr<Registry> registry_;
    bool ensure_registry();
};

}

#endif
