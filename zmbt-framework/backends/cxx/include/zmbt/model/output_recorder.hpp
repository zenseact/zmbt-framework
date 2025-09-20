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
    using extract_fn_t = std::function<std::size_t(Registry&, consumer_fn_t const)>;


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
        };

        using FramesBuffMap = std::shared_ptr<boost::concurrent_flat_map<std::thread::id, FrameBuffs>>;

        std::type_index data_typeid;
        boost::json::string interface_name;

        extract_fn_t extract_fn;
        FramesBuffMap frame_buff_map;

        std::mutex mtx;
        boost::json::array serialized_frames{};
        std::atomic<std::size_t> count{0};
        std::atomic<std::size_t> lost_count{0};

        std::bitset<static_cast<unsigned>(ChannelKind::Undefined)> enable_categories_;

        Registry(
            std::type_index typid,
            boost::json::string name,
            extract_fn_t efn,
            FramesBuffMap fbm
        )
            : data_typeid(typid)
            , interface_name{name}
            , extract_fn(efn)
            , frame_buff_map(fbm)
        {
        }

        ~Registry()
        {
        }

        template <class I>
        static std::shared_ptr<Registry> Make()
        {
            using args_t = reflect::invocation_args_unqf_t<I const&>;
            using ret_t = reflect::invocation_ret_unqf_or_nullptr_t<I const&>;

            std::type_index const typid {typeid(std::tuple<args_t, ret_t>)};

            auto extract_fn = [](Registry& registry, consumer_fn_t const& consumer_fn) -> std::size_t {

                return 0;
            };

            return std::make_shared<Registry>(typid, type_name<I>().c_str(), extract_fn, nullptr);
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


    OutputRecorder();
    ~OutputRecorder();

    OutputRecorder(OutputRecorder const&) = default;
    OutputRecorder(OutputRecorder &&) = default;
    OutputRecorder& operator=(OutputRecorder const&) = default;
    OutputRecorder& operator=(OutputRecorder &&) = default;

    /// Flush interal buffers and complete serialization
    void flush();

    template <class I, class II = ifc_pointer_t<I>>
    void setup_handlers()
    {
        if (registry_ != nullptr) return;
        registry_ = Registry::Make<II>();
    }


    /// Multi-producer push
    template <class ArgsTuple, class Return>
    void push(ArgsTuple const& args, ErrorOr<Return> const& return_or_error)
    {

        if (!registry_)
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
            ErrorInfo e;
            e.type = type_name<output_recorder_error>();
            e.what = "push to output recorder outside of managed test";
            e.context = "OutputRecorder";
            report_test_error(e);
            return;
        }

        registry_->count++;

        boost::json::object f {
            {"ts"  , 0UL      },
        };

        if (registry_->enable_categories_[static_cast<unsigned>(ChannelKind::ThreadId)])
        {
            f["tid"] = get_tid();
        }

        if (registry_->enable_categories_[static_cast<unsigned>(ChannelKind::Args)])
        {
            f["args"] = json_from(args);
        }

        if (return_or_error.is_error())
        {
            return_or_error.dump_to(f);
            // let the test fail if no error expectations
            // and stop capturing outputs
            if (not registry_->enable_categories_[static_cast<unsigned>(ChannelKind::Exception)])
            {
                report_test_error(return_or_error.as_error());
                registry_->enable_categories_.reset();
            }
        }
        else if (registry_->enable_categories_[static_cast<unsigned>(ChannelKind::Return)] && return_or_error.is_return())
        {
            return_or_error.dump_to(f);
        }

        auto& ts = f["ts"];

        std::lock_guard<std::mutex> l(registry_->mtx);
        ts = get_ts();
        registry_->serialized_frames.push_back(std::move(f));
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

    std::shared_ptr<Registry> registry_;
};

}

#endif
