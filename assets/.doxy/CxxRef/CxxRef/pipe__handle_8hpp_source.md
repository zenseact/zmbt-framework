

# File pipe\_handle.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**mapping**](dir_84d9d905044f75949470ced2679fed92.md) **>** [**pipe\_handle.hpp**](pipe__handle_8hpp.md)

[Go to the documentation of this file](pipe__handle_8hpp.md)


```C++

#ifndef ZMBT_MAPPING_PIPE_HANDLE_HPP_
#define ZMBT_MAPPING_PIPE_HANDLE_HPP_

#include <list>
#include <boost/json.hpp>

#include "zmbt/core.hpp"
#include "zmbt/model/environment.hpp"
#include "zmbt/expr.hpp"
#include "channel_handle.hpp"


namespace zmbt {
namespace mapping {


class PipeHandle
{
    JsonNode data_;
    Environment env;
    bool should_flatten_;

    std::list<ChannelHandle> channels_;
    boost::json::value observe_blend() const;
    mutable boost::json::value id_cache_{};

public:


    PipeHandle(JsonNode& model, std::size_t const pipe_idx);

    ~PipeHandle() = default;

    boost::json::value type() const;
    bool is_input() const;
    bool is_output() const;
    bool has_expression() const;
    lang::Expression expression() const;
    bool overload(lang::Expression& e) const;

    void inject(lang::Expression expr) const;
    boost::json::value observe() const;

    int column() const;
    boost::json::value index() const;
    boost::json::value id() const;

    std::list<ChannelHandle> const& channels() const
    {
        return channels_;
    }

};


} // namespace mapping
} // namespace zmbt

#endif
```


