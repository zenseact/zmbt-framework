

# File test\_diagnostics.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**mapping**](dir_84d9d905044f75949470ced2679fed92.md) **>** [**test\_diagnostics.hpp**](test__diagnostics_8hpp.md)

[Go to the documentation of this file](test__diagnostics_8hpp.md)


```C++

#ifndef ZMBT_MAPPING_TEST_DIAGNOSTICS_HPP_
#define ZMBT_MAPPING_TEST_DIAGNOSTICS_HPP_

#include <boost/json.hpp>
#include <zmbt/expr/expression.hpp>
#include <zmbt/expr/eval_log.hpp>

namespace zmbt {
namespace mapping {

struct TestDiagnostics
{
    enum class Result
    {
        Success,
        Fail,
        Error
    };


    boost::json::string model_name;
    boost::json::string message    {};
    boost::json::string description{};
    boost::json::string comment    {};

    boost::json::value expected    {};
    boost::json::value observed    {};
    boost::json::value vector      {};
    boost::json::value error      {};
    size_t tr  {};
    size_t tc  {};

    boost::json::value channel_id  {};
    boost::json::value pipe_id  {};

    boost::json::array traces      {};
    boost::json::array eval_stack  {};
    Result result                  {};
    bool tabular_condition_failure_{};


    TestDiagnostics(boost::json::string_view model_name) : model_name{model_name} {}

    TestDiagnostics(TestDiagnostics const&) = default;
    TestDiagnostics(TestDiagnostics &&) = default;

    TestDiagnostics& operator=(TestDiagnostics const&) = default;
    TestDiagnostics& operator=(TestDiagnostics &&) = default;
    ~TestDiagnostics() = default;

    TestDiagnostics& Description(boost::json::string_view msg)
    {
        this->description = msg; return *this;
    }

    TestDiagnostics& Comment(boost::json::string_view msg)
    {
        this->comment = msg; return *this;
    }

    TestDiagnostics& Vector(boost::json::value val)
    {
        this->vector = val; return *this;
    }
    TestDiagnostics& TabularConditionFailure(std::size_t const col)
    {
        tabular_condition_failure_ = true;
        this->tc = col; return *this;
    }
    TestDiagnostics& TestRow(std::size_t const r)
    {
        this->tr = r; return *this;
    }
    TestDiagnostics& ChannelId(boost::json::value const& id)
    {
        this->channel_id = id; return *this;
    }
    TestDiagnostics& PipeId(boost::json::value const& id)
    {
        this->pipe_id = id; return *this;
    }
    TestDiagnostics& EvalStack(lang::EvalLog const& log)
    {
        if (log.stack)
        {
            this->eval_stack = *log.stack;
        }
        return *this;
    }

    TestDiagnostics& Traces(std::shared_ptr<boost::json::array> const traces_ptr)
    {
        if (traces_ptr)
        {
            traces = *traces_ptr;
        }
        return *this;
    }

    TestDiagnostics& Error(boost::json::string_view origin, boost::json::value const& err);

    TestDiagnostics& Fail(lang::Expression const& expected, boost::json::value const& observed);

    boost::json::value to_json() const;
};

} // namespace mapping
} // namespace zmbt

#endif // ZMBT_MAPPING_EXPECT_VARIANTS_HPP_
```


