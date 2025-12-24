/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_CORE_JSON_ITER_HPP_
#define ZMBT_CORE_JSON_ITER_HPP_


#include "aliases.hpp"

#include "exceptions.hpp"


namespace zmbt {



/// Boost JSON array iterator interface
class JsonIter
{
    virtual void step() = 0;
public:


    JsonIter() = default;
    virtual ~JsonIter() = default;
    JsonIter(JsonIter const&) = default;
    JsonIter& operator=(JsonIter const&) = default;
    JsonIter(JsonIter &&) = default;
    JsonIter& operator=(JsonIter &&) = default;

    /// Iterator halted
    virtual bool halt() const = 0;

    /// iterator value
    virtual boost::json::array get() = 0;

    /// iterator post-increment
    JsonIter& operator++(int)
    {
        if (!halt())
        {
            this->step();
        }
        return *(this);
    };

    /// iterator value
    boost::json::array operator*()
    {
        return get();
    }
};


/// Boost JSON array iterator base for zip or cartesian product
class JsonIterZipProdBase : public virtual JsonIter
{
protected:

    bool halt_{false};
    std::vector<boost::json::array::const_iterator> begins_{};
    std::vector<boost::json::array::const_iterator> ends_{};
    std::vector<boost::json::array::const_iterator> its_{};

    boost::json::array owned_sequences_; // capture temporaries

private:

    void init_iters(boost::json::array const& seqences);

public:

    bool halt() const final
    {
        return halt_;
    }

    explicit JsonIterZipProdBase(boost::json::array const& sequences) : JsonIter(), owned_sequences_{}
    {
        init_iters(sequences);
    }

    explicit JsonIterZipProdBase(boost::json::array && seqences) : JsonIter(), owned_sequences_(std::move(seqences))
    {
        init_iters(owned_sequences_);
    }

    JsonIterZipProdBase() : JsonIterZipProdBase([]{
            boost::json::array arr {};
            arr.push_back(boost::json::array{});
            return arr;
        }()) // default iter yields empty set
    {
    }

    virtual ~JsonIterZipProdBase() = default;

    JsonIterZipProdBase(JsonIterZipProdBase const&) = default;
    JsonIterZipProdBase& operator=(JsonIterZipProdBase const&) = default;

    JsonIterZipProdBase(JsonIterZipProdBase &&) = default;
    JsonIterZipProdBase& operator=(JsonIterZipProdBase &&) = default;


    boost::json::array get() final;
};


/// Cartesian Product Iterator
class JsonProdIter : public virtual JsonIterZipProdBase
{
    void step() final;

    public:
    using JsonIterZipProdBase::JsonIterZipProdBase;

};


/// Zip Iterator
class JsonZipIter : public virtual JsonIterZipProdBase
{
    void step() final;
    public:
    using JsonIterZipProdBase::JsonIterZipProdBase;
};


}

#endif