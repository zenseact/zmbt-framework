
/**
 * \file
 * \copyright (c) Copyright 2024-2025 Zenseact AB
 * \license SPDX-License-Identifier: Apache-2.0
 *
 * \details Autogenerated file - do not change!
 *
 * How to update:
 * 1. Update the template at `templates/expr/expression_api.hpp`
 * 2. Update the keyword data at `templates/expr/keywords.yaml`
 * 3. Regenerate file with `bazel run //templates/expr:expression_api_cpp > zmbt-framework/backends/cxx/src/model/expression_api.cpp`
 * 4. Commit changes
 */


#include "zmbt/model/expression_api.hpp"

namespace zmbt {

expr::signature::Base<Keyword::Id> Id = {};
expr::signature::Base<Keyword::Noop> Noop = {};
expr::signature::Base<Keyword::Noop> _ = Noop;
expr::signature::Base<Keyword::True> True = {};
expr::signature::Base<Keyword::False> False = {};
expr::signature::Base<Keyword::Null> Null = {};
expr::signature::Base<Keyword::Pi> Pi = {};
expr::signature::Base<Keyword::E> E = {};
expr::signature::Base<Keyword::Inf> Inf = {};
expr::signature::Base<Keyword::Eps> Eps = {};
expr::signature::Base<Keyword::NaN> NaN = {};
expr::signature::Base<Keyword::Bool> Bool = {};
expr::signature::Base<Keyword::Nil> Nil = {};
expr::signature::Base<Keyword::Neg> Neg = {};
expr::signature::Base<Keyword::Abs> Abs = {};
expr::signature::Base<Keyword::Sign> Sign = {};
expr::signature::Base<Keyword::Ceil> Ceil = {};
expr::signature::Base<Keyword::Floor> Floor = {};
expr::signature::Base<Keyword::Round> Round = {};
expr::signature::Base<Keyword::BitNot> BitNot = {};
expr::signature::Base<Keyword::Concat> Concat = {};
expr::signature::Base<Keyword::Union> Union = {};
expr::signature::Base<Keyword::Intersect> Intersect = {};
expr::signature::Base<Keyword::Diff> Diff = {};
expr::signature::Base<Keyword::Cartesian> Cartesian = {};
expr::signature::Base<Keyword::Transp> Transp = {};
expr::signature::Base<Keyword::Set> Set = {};
expr::signature::Base<Keyword::List> List = {};
expr::signature::OneParam<Keyword::Not> Not = {};
expr::signature::OneParam<Keyword::Eq> Eq = {};
expr::signature::OneParam<Keyword::Ne> Ne = {};
expr::signature::OneParam<Keyword::Lt> Lt = {};
expr::signature::OneParam<Keyword::Le> Le = {};
expr::signature::OneParam<Keyword::Gt> Gt = {};
expr::signature::OneParam<Keyword::Ge> Ge = {};
expr::signature::OneParam<Keyword::Ni> Ni = {};
expr::signature::OneParam<Keyword::Ni> Contains = Ni;
expr::signature::OneParam<Keyword::NotNi> NotNi = {};
expr::signature::OneParam<Keyword::Add> Add = {};
expr::signature::OneParam<Keyword::Sub> Sub = {};
expr::signature::OneParam<Keyword::Mul> Mul = {};
expr::signature::OneParam<Keyword::Div> Div = {};
expr::signature::OneParam<Keyword::Pow> Pow = {};
expr::signature::OneParam<Keyword::Log> Log = {};
expr::signature::OneParam<Keyword::Mod> Mod = {};
expr::signature::OneParam<Keyword::Quot> Quot = {};
expr::signature::OneParam<Keyword::Sqrt> Sqrt = {};
expr::signature::OneParam<Keyword::Sin> Sin = {};
expr::signature::OneParam<Keyword::Cos> Cos = {};
expr::signature::OneParam<Keyword::Tan> Tan = {};
expr::signature::OneParam<Keyword::Asin> Asin = {};
expr::signature::OneParam<Keyword::Acos> Acos = {};
expr::signature::OneParam<Keyword::Atan> Atan = {};
expr::signature::OneParam<Keyword::Sinh> Sinh = {};
expr::signature::OneParam<Keyword::Cosh> Cosh = {};
expr::signature::OneParam<Keyword::Tanh> Tanh = {};
expr::signature::OneParam<Keyword::Asinh> Asinh = {};
expr::signature::OneParam<Keyword::Acosh> Acosh = {};
expr::signature::OneParam<Keyword::Atanh> Atanh = {};
expr::signature::OneParam<Keyword::Exp> Exp = {};
expr::signature::OneParam<Keyword::Erf> Erf = {};
expr::signature::OneParam<Keyword::Erfc> Erfc = {};
expr::signature::OneParam<Keyword::Gamma> Gamma = {};
expr::signature::OneParam<Keyword::BitAnd> BitAnd = {};
expr::signature::OneParam<Keyword::BitOr> BitOr = {};
expr::signature::OneParam<Keyword::BitXor> BitXor = {};
expr::signature::OneParam<Keyword::BitLshift> BitLshift = {};
expr::signature::OneParam<Keyword::BitRshift> BitRshift = {};
expr::signature::OneParam<Keyword::Slide> Slide = {};
expr::signature::OneParam<Keyword::Stride> Stride = {};
expr::signature::OneParam<Keyword::Slice> Slice = {};
expr::signature::OneParam<Keyword::Sort> Sort = {};
expr::signature::OneParam<Keyword::Repeat> Repeat = {};
expr::signature::OneParam<Keyword::Re> Re = {};
expr::signature::OneParam<Keyword::Re> Regex = Re;
expr::signature::VariadicExpr<Keyword::And> And = {};
expr::signature::VariadicExpr<Keyword::Or> Or = {};
expr::signature::VariadicExpr<Keyword::Any> Any = {};
expr::signature::VariadicExpr<Keyword::All> All = {};
expr::signature::VariadicExpr<Keyword::Saturate> Saturate = {};
expr::signature::VariadicExpr<Keyword::Compose> Compose = {};
expr::signature::TwoExpr<Keyword::Xor> Xor = {};
expr::signature::SetParam<Keyword::SetEq> SetEq = {};
expr::signature::SetParam<Keyword::Subset> Subset = {};
expr::signature::SetParam<Keyword::Superset> Superset = {};
expr::signature::SetParam<Keyword::ProperSubset> ProperSubset = {};
expr::signature::SetParam<Keyword::ProperSuperset> ProperSuperset = {};
expr::signature::SetParam<Keyword::In> In = {};
expr::signature::SetParam<Keyword::NotIn> NotIn = {};
expr::signature::OneExpr<Keyword::Map> Map = {};
expr::signature::OneExpr<Keyword::Filter> Filter = {};
expr::signature::ExprAndOptionalParam<Keyword::Recur> Recur = {};
expr::signature::ExprAndOptionalParam<Keyword::Apply> Apply = {};
expr::signature::ExprAndOptionalParam<Keyword::Partial> Partial = {};
expr::signature::ExprAndOptionalParam<Keyword::Reduce> Reduce = {};
expr::signature::ExprAndOptionalParam<Keyword::Sum> Sum = {};
expr::signature::ExprAndOptionalParam<Keyword::Prod> Prod = {};
expr::signature::ParamAndOptionalExpr<Keyword::At> At = {};
expr::signature::ParamAndOptionalExpr<Keyword::Card> Card = {};
expr::signature::ParamAndOptionalExpr<Keyword::Size> Size = {};
expr::signature::ParamAndOptionalExpr<Keyword::Count> Count = {};
expr::signature::ParamAndOptionalExpr<Keyword::Min> Min = {};
expr::signature::ParamAndOptionalExpr<Keyword::Max> Max = {};
expr::signature::ParamAndOptionalExpr<Keyword::Argmin> Argmin = {};
expr::signature::ParamAndOptionalExpr<Keyword::Argmax> Argmax = {};
expr::signature::Approx Approx = {};
expr::signature::Approx Near = Approx;



namespace detail
{
    boost::json::value as_set(std::initializer_list<boost::json::value_ref> set)
    {
        boost::json::value val(set);
        if (set.size() != 1 || val.is_object())
        {
            return val;
        }
        else
        {
            return boost::json::array{val};
        }
    }
}

Expression operator|(Expression const& lhs, Expression const& rhs)
{
    return expr::Compose(rhs, lhs);
}

} // namespace zmbt

