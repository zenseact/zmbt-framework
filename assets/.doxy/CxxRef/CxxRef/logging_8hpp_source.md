

# File logging.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**logging.hpp**](logging_8hpp.md)

[Go to the documentation of this file](logging_8hpp.md)


```C++

#ifndef ZMBT_LOGGING_HPP_
#define ZMBT_LOGGING_HPP_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>

#include <boost/current_function.hpp>


#define ZMBT_LOG_WITH_SRCLOC(logger, sev) \
   BOOST_LOG_STREAM_WITH_PARAMS( \
      (logger), \
         (::zmbt::logging::upd_attr<std::string>("File", __FILE__)) \
         (::zmbt::logging::upd_attr<int>("Line", __LINE__)) \
         (::zmbt::logging::upd_attr<std::string>("Func", BOOST_CURRENT_FUNCTION)) \
         (::boost::log::keywords::severity = (boost::log::trivial::sev)) \
   )

#define ZMBT_LOG_WITH_JSON_TAG(logger, sev) \
   BOOST_LOG_STREAM_WITH_PARAMS( \
      (logger), \
         (::zmbt::logging::upd_attr<std::string>("Tag", "JSON")) \
         (::boost::log::keywords::severity = (boost::log::trivial::sev)) \
   )


#define ZMBT_LOG_INSTANCE ::boost::log::trivial::logger::get()
#define ZMBT_LOG_INSTANCE_INTERNAL ::boost::log::trivial::logger::get()

#define ZMBT_LOG(sev) ZMBT_LOG_WITH_SRCLOC(ZMBT_LOG_INSTANCE, sev)
#define ZMBT_LOG_INTERNAL(sev) ZMBT_LOG_WITH_SRCLOC(ZMBT_LOG_INSTANCE_INTERNAL, sev)

#define ZMBT_LOG_JSON(sev) ZMBT_LOG_WITH_JSON_TAG(ZMBT_LOG_INSTANCE_INTERNAL, sev)


#define ZMBT_LOG_FORMAT_FULL "%LineID% %File%#Line% (%Func%) <%Severity%>\t%Message%"
#define ZMBT_LOG_FORMAT_BRIEF "%LineID% <%Severity%>: %Message%"
#define ZMBT_LOG_FORMAT ZMBT_LOG_FORMAT_BRIEF


namespace zmbt {
namespace logging {


template <class T>
T upd_attr(std::string const name, T value) {
    auto attr = boost::log::attribute_cast<boost::log::attributes::mutable_constant<T>>(boost::log::core::get()->get_thread_attributes()[name]);
    attr.set(value);
    return attr.get();
}


} // namespace logging
} // namespace zmbt

#endif
```


