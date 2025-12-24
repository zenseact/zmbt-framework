

# File injection\_table.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**injection\_table.hpp**](injection__table_8hpp.md)

[Go to the documentation of this file](injection__table_8hpp.md)


```C++

#ifndef ZMBT_MODEL_INJECTION_TABLE_HPP_
#define ZMBT_MODEL_INJECTION_TABLE_HPP_

#include <memory>
#include <vector>

#include "zmbt/core.hpp"
#include "zmbt/expr.hpp"
#include "generator.hpp"
#include "channel_kind.hpp"


namespace zmbt {


class InjectionTable
{
  public:

    struct Record
    {
        boost::json::string jptr;
        Generator::Shared generator;
        lang::Expression transform;
        boost::optional<boost::json::value> maybe_constant;

        Record(
            boost::json::string_view jp,
            Generator::Shared gen,
            lang::Expression const& tf
        );
    };

  private:
    interface_id ifc_id_;
    object_id obj_id_;
    reflect::Prototypes prototypes_;

    std::vector<Record> args_;
    std::vector<Record> ret_;
    std::vector<Record> exception_;

    boost::optional<boost::json::value> maybe_constant_args_;
    boost::optional<boost::json::value> maybe_constant_ret_;
    boost::optional<boost::json::value> maybe_constant_exception_;


    std::vector<Record>& get_records(ChannelKind const& ck) &;
    boost::optional<boost::json::value>& get_maybe_const(ChannelKind const& ck) &;

  public:

    InjectionTable(interface_id const& ifc_id, object_id const& obj_id);
    void add_record(ChannelKind const& ck, Record&& rec);
    Record& last_record(ChannelKind const& ck) &;

    boost::json::value yield(ChannelKind const& ck, boost::json::value& result_value);

    static std::shared_ptr<InjectionTable> Make(interface_id const& ifc_id, object_id const& obj_id);
};

} // namespace

#endif
```


