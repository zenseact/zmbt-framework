

# File json\_iter.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**core**](dir_1dfd3566c4a6f6e15f69daa4a04e2d4f.md) **>** [**json\_iter.hpp**](json__iter_8hpp.md)

[Go to the documentation of this file](json__iter_8hpp.md)


```C++

#ifndef ZMBT_CORE_JSON_ITER_HPP_
#define ZMBT_CORE_JSON_ITER_HPP_


#include "aliases.hpp"

#include "exceptions.hpp"


namespace zmbt {



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

    virtual bool halt() const = 0;

    virtual boost::json::array get() = 0;

    JsonIter& operator++(int)
    {
        if (!halt())
        {
            this->step();
        }
        return *(this);
    };

    boost::json::array operator*()
    {
        return get();
    }
};


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


class JsonProdIter : public virtual JsonIterZipProdBase
{
    void step() final;

    public:
    using JsonIterZipProdBase::JsonIterZipProdBase;

};


class JsonZipIter : public virtual JsonIterZipProdBase
{
    void step() final;
    public:
    using JsonIterZipProdBase::JsonIterZipProdBase;
};


}

#endif
```


