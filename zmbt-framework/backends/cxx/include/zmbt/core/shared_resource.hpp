/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_SHARED_EMPLACER_HPP_
#define ZMBT_CORE_SHARED_EMPLACER_HPP_


#include <boost/json.hpp>
#include <string>
#include <utility>

namespace zmbt {

namespace detail
{
struct emplace_shared_tag {};
}

static constexpr detail::emplace_shared_tag emplace_shared;

template <class T>
class shared_resource
{
    std::shared_ptr<T> ptr_;

  public:

    shared_resource(T&& v)
        : ptr_{std::make_shared<T>(std::forward<T>(v))}
    {
    }

    shared_resource(std::function<T()> f)
        : ptr_{std::make_shared<T>(f())}
    {
    }

    shared_resource(std::function<std::shared_ptr<T>()> f)
        : ptr_{f()}
    {
    }

    shared_resource(std::shared_ptr<T> ptr)
        : ptr_{ptr}
    {
    }

    template <class... A>
    shared_resource(detail::emplace_shared_tag, A&&... args)
        : ptr_{std::make_shared<T>(std::forward<A>(args)...)}
    {
    }

    shared_resource() = default;
    shared_resource(shared_resource const&) = default;
    shared_resource(shared_resource&&) = default;

    shared_resource& operator=(shared_resource const&) = default;
    shared_resource& operator=(shared_resource&&) = default;

    ~shared_resource() = default;

    std::shared_ptr<T> ptr() const
    {
        return ptr_;
    }

    T&       operator*()       { return *ptr_; }
    T const& operator*() const { return *ptr_; }

    std::shared_ptr<T>       operator->()       { return  ptr_; }
    std::shared_ptr<T> const operator->() const { return  ptr_; }

    operator std::shared_ptr<T>()
    {
        return ptr_;
    }
};

} // namespace zmbt

#endif
