/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/math/constants/constants.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "zmbt/expr.hpp"
#include "zmbt/decor.hpp"


namespace utf = boost::unit_test;

using namespace zmbt;
using namespace zmbt::reflect;
using namespace zmbt::expr;
using namespace zmbt::decor;
using namespace boost::json;

using V = boost::json::value;
using L = boost::json::array;
using zmbt::lang::Keyword;
using zmbt::lang::Expression;
using zmbt::lang::EvalContext;
using zmbt::lang::EvalLog;

namespace {

std::set<Keyword> const NotImplemented {
    Keyword::Find,
    Keyword::FindPtr,
    Keyword::FindIdx,
    Keyword::Link,
    Keyword::Let,
    Keyword::Capture,
    Keyword::Refer,
};


BOOST_DEFINE_FIXED_ENUM_CLASS(Foo, int, Bar, Baz)


struct TestEvalSample
{
    Expression expr;
    Expression x;
    Expression expected;

    friend std::ostream& operator<<(std::ostream& os, TestEvalSample const& sample)
    {
        return os << "{expr: " << sample.expr << ", x: " << sample.x << ", expected: " << sample.expected << "}";
    }
};


std::vector<TestEvalSample> const TestSamples
{ // expr                       , x                     , expected
    // identity
    {Id                         , 42                    , 42                    },
    {Id                         , ""                    , ""                    },
    {Id                         , nullptr               , nullptr               },
    {Id                         , {1,2,3}               , {1,2,3}               },

    /// Capture
    {"$x"                       , 42                    , 42                    },
    {"$x"|Mul("$x")             , 3                     , 9                     },
    {Q(3)|"$x"|Mul("$x")        , {}                    , 9                  },



    {Default(42)                , nullptr               , 42                    },
    {Default(42)                , ""                    , ""                    },
    {Default(42)                , {1,2,3}               , {1,2,3}               },

    // boolean cast
    {Bool                       , true                  , true                  },
    {Bool                       , 42                    , true                  },
    {Bool                       , 42.1                  , true                  },
    {Bool                       , "false"               , true                  },
    {Bool                       , {1,2}                 , true                  },
    {Bool                       , false                 , false                 },
    {Bool                       , 0                     , false                 },
    {Bool                       , 0.0                   , false                 },
    {Bool                       , ""                    , false                 },
    {Bool                       , L()                   , false                 },

    {Nil                       , ""                    , true                   },
    {Nil                       , L()                   , true                   },
    {Nil                       , "false"               , false                  },
    {Nil                       , {1,2  }               , false                  },

    // basic constants
    {True                       , {}                    , true                  },
    {False                      , {}                    , false                 },
    {Null                       , {}                    , nullptr               },
    {Noop                       , {}                    , true                  },
    {Noop                       , true                  , true                  },
    {Noop                       , false                 , true                  },
    {Noop                       , "foo"                 , true                  },

    // math constants
    { Pi                        , {}                    , *Pi                   },
    { E                         , {}                    , *E                    },
    { Inf                       , {}                    , *Inf                  },
    { Eps                       , {}                    , *Eps                  },

    { Thread                    , {}                    , zmbt::get_tid().c_str()},


    // comparison
    {Eq(42)                     , 42                    , true                  },
    {Eq(42)                     , 13                    , false                 },
    {Eq                         , {42, 42}              , true                  },
    {Eq                         , {13, 42}              , false                 },
    {Ne(42)                     , 42                    , false                 },
    {Ne(42)                     , 13                    , true                  },
    {Ne                         , {42, 42}              , false                 },
    {Ne                         , {13, 42}              , true                  },

    {Q(42)                      , 13                    , 42                    },
    {Q                          , 13                    , nullptr               },
    {Q(42)                      , nullptr               , 42                    },
    {Q(42)                      , nullptr               , 42                    },

    {Approx(42)                 , 42                    , true                  },
    {Approx(42)                 , 42.0 + 1e-09          , true                  },
    {Approx(42.0 + 1e-09)       , 42                    , true                  },
    {Approx                     , {42, 42.0 + 1e-09}    , true                  },
    {Approx(42.001)             , 42                    , false                 },

    // relative tolerance
    {Approx({3.14, 0.001})      , *Pi                   , true                  },
    {Approx({2.71, 0.005})      , *E                    , true                  },
    // absolute tolerance
    {Approx({3.14, 0, 0.01})    , *Pi                   , true                  },
    {Approx({2.71, 0, 0.01})    , *E                    , true                  },
    {Approx                     , {*Pi, {3.14, 0.001}}  , true                  },
    {Approx                     , {*Pi, {3.15, 0.001}}  , false                 },
    // range check
    {All(Gt(3.14), Lt(3.15))    , *Pi                   , true                  },

    {Round                      ,  1.49999               ,  1                   },
    {Round                      ,  1.5                   ,  2                   },
    {Round                      ,  0.49999               ,  0                   },
    {Round                      , -0.49999               ,  0                   },
    {Round                      , -0.5                   , -1                   },
    {Round                      , -1.49999               , -1                   },
    {Round                      , -1.5                   , -2                   },

    {Mul(2)|Round|Div(2)        ,  1.49999               ,  1.5                 },

    // order relation
    {Lt(42)                     , 41                    , true                  },
    {Lt(42)                     , 42                    , false                 },
    {Lt                         , {41, 42}              , true                  },
    {Lt                         , {42, 42}              , false                 },
    {Gt(42)                     , 43                    , true                  },
    {Gt(42)                     , 42                    , false                 },
    {Gt                         , {43, 42}              , true                  },
    {Gt                         , {42, 42}              , false                 },
    {Le(42)                     , 41                    , true                  },
    {Le(42)                     , 42                    , true                  },
    {Le(42)                     , 43                    , false                 },
    {Le                         , {41, 42}              , true                  },
    {Le                         , {42, 42}              , true                  },
    {Le                         , {43, 42}              , false                 },
    {Ge(42)                     , 43                    , true                  },
    {Ge(42)                     , 42                    , true                  },
    {Ge(42)                     , 41                    , false                 },
    {Ge                         , {43, 42}              , true                  },
    {Ge                         , {42, 42}              , true                  },
    {Ge                         , {41, 42}              , false                 },

    // lexicographical_compare
    {Gt("aaaa")                 , "bbbb"                , true                  },
    {Gt("aa")                   , "aaaa"                , true                  },
    {Gt({1,1,1})                , {1,2,3}               , true                  },

    // set relation
    {        Subset({1,2,3})    , {1,3}                 , true                  },
    {        Subset({1,3}  )    , {1,2,3}               , false                 },
    {        Subset({1,2}  )    , {1,2}                 , true                  },
    {      Superset({1,3}  )    , {1,2,3}               , true                  },
    {      Superset({1,2,3})    , {1,3}                 , false                 },
    {      Superset({1,2})      , {1,2}                 , true                  },
    {  PSubset({1,2,3})         , {1,3}                 , true                  },
    {  PSubset({1,3}  )         , {1,2,3}               , false                 },
    {  PSubset({1,2}  )         , {1,2}                 , false                 },
    {PSuperset({1,3}  )         , {1,2,3}               , true                  },
    {PSuperset({1,2,3})         , {1,3}                 , false                 },
    {PSuperset({1,2})           , {1,2}                 , false                 },

    {        Subset, {{1,3}     , {1,2,3}}              , true                  },
    {      Superset, {{1,2,3}   , {1,3}  }              , true                  },
    {  PSubset, {{1,3}          , {1,2,3}}              , true                  },
    {PSuperset, {{1,2,3}        , {1,3}  }              , true                  },

    {        Subset({1})        , L{}                   , true                  },
    {      Superset({1})        , L{}                   , false                 },
    {  PSubset({1})             , L{}                   , true                  },
    {PSuperset({1})             , L{}                   , false                 },

    {        Subset(L{})        , L{1}                  , false                 },
    {      Superset(L{})        , L{1}                  , true                  },
    {  PSubset(L{})             , L{1}                  , false                 },
    {PSuperset(L{})             , L{1}                  , true                  },

    {        Subset(L{})        , L{}                   , true                  },
    {      Superset(L{})        , L{}                   , true                  },
    {  PSubset(L{})             , L{}                   , false                 },
    {PSuperset(L{})             , L{}                   , false                 },


    {SetEq({1,2,3})             , {1,2,3}               , true                  },
    {SetEq({1,2,3})             , {1,3,2}               , true                  },
    {SetEq({1,2,3})             , {1,2}                 , false                 },

    {Uniques                    , {1,2,2,4,3,4}         , {3,4,2,1}             },

    {Union|Sort                 , {{1,2,3},{2,3,4}}     , {1,2,3,4}             },
    {Intersect|Sort             , {{1,2,3},{2,3,4}}     , {2,3}                 },
    {Diff                       , {{1,2,3},{2,3,4}}     , L{1}                  },
    {Reverse|Diff               , {{1,2,3},{2,3,4}}     , L{4}                  },
    {Union({2,3,4})|Sort        , L{1,2,3}              , {1,2,3,4}             },
    {Intersect({2,3,4})|Sort    , L{1,2,3}              , {2,3}                 },
    {Diff({2,3,4})              , L{1,2,3}              , L{1}                  },
    {Flip(Diff({2,3,4}))        , L{1,2,3}              , L{4}                  },
    {   ~(Diff({2,3,4}))        , L{1,2,3}              , L{4}                  },

    {Union                      , {L{},L{}}             , L{}                   },
    {Intersect                  , {L{},L{}}             , L{}                   },
    {Diff                       , {L{},L{}}             , L{}                   },
    {Reverse|Diff               , {L{},L{}}             , L{}                   },

    // element in set
    {In({1, 2})                 , 2                     , true                  },
    {In({1, 2})                 , 3                     , false                 },
    {NotIn({1, 2})              , 3                     , true                  },
    {NotIn({1, 2})              , 2                     , false                 },
    {In                         , {2, {1, 2}}           , true                  },
    {In                         , {3, {1, 2}}           , false                 },
    {NotIn                      , {3, {1, 2}}           , true                  },
    {NotIn                      , {2, {1, 2}}           , false                 },

    {Ni(2)                      , {1, 2}                , true                  },
    {Ni(3)                      , {1, 2}                , false                 },
    {NotNi(3)                   , {1, 2}                , true                  },
    {NotNi(2)                   , {1, 2}                , false                 },
    {Ni                         , {{1, 2}, 2}           , true                  },
    {Ni                         , {{1, 2}, 3}           , false                 },
    {NotNi                      , {{1, 2}, 3}           , true                  },
    {NotNi                      , {{1, 2}, 2}           , false                 },

    {Size                       , {1,2,2,2}             , 4                     },
    {Card                       , {1,2,2,2}             , 2                     },
    {Card                       , {1,2,2,4,3,4}         , 4                     },
    {Sort                       , {1,2}                 , {1,2}                 },
    {Sort                       , {2,1}                 , {1,2}                 },
    {Sort                       , {4,1,3,1,2,1}         , {1,1,1,2,3,4}         },
    {Sort|Reverse               , {4,1,3,1,2,1}         , {4,3,2,1,1,1}         },
    {Sort(Neg)                  , {4,1,3,1,2,1}         , {4,3,2,1,1,1}         },
    {Sort                       , {{1,3},{2,1},{3,2}}   , {{1,3},{2,1},{3,2}}   },
    {Sort                       , {{3,2},{2,1},{1,3}}   , {{1,3},{2,1},{3,2}}   },
    {Sort(At(1))                , {{1,3},{2,1},{3,2}}   , {{2,1},{3,2},{1,3}}   },
    {Sort(At(1))                , {{3,2},{2,1},{1,3}}   , {{2,1},{3,2},{1,3}}   },

    {Min                        , {1,2,2,4,3,4,-1}      , -1                    },
    {Max                        , {1,2,2,4,3,4,-1}      , 4                     },
    {Argmin                     , {1,2,2,4,3,4,-1}      , 6                     },
    {Argmax                     , {1,2,2,4,3,4,-1}      , 3                     },
    {Min                        , {{3, 2},{2, 1},{1, 3}}, {1,3}                 },
    {Max                        , {{3, 2},{2, 1},{1, 3}}, {3,2}                 },
    {Min(At(1))                 , {{3, 2},{2, 1},{1, 3}}, {2,1}                 },
    {Max(At(1))                 , {{3, 2},{2, 1},{1, 3}}, {1,3}                 },
    {Argmin(At(1))              , {{3, 2},{2, 1},{1, 3}}, 1                     },
    {Argmax(At(1))              , {{3, 2},{2, 1},{1, 3}}, 2                     },
    {Min                        , L{}                   , nullptr               },
    {Max                        , L{}                   , nullptr               },
    {Argmin                     , L{}                   , nullptr               },
    {Argmax                     , L{}                   , nullptr               },

    {Sort                       , L{}                   , L{}                   },
    {Reverse                    , L{}                   , L{}                   },

    {Slide(3)                   , {1,2,3,4,5}           , {{1,2,3},{2,3,4},{3,4,5}}},
    {Slide(3)|Map(Sum)          , {1,2,3,4,5}           , {6      , 9      , 12}},
    {Slide(3)|Map(Prod)         , {1,2,3,4,5}           , {6      , 24     , 60}},
    {Slide(3)|Map(Avg)          , {1,2,3,4,5}           , {2      , 3      ,  4}},
    {Slide(42)                  , {1,2,3}               , L{}                   },
    {Slide(42)                  , L{}                   , L{}                   },

    {Slide(-1) | Kwrd           , L{}                   , "Err"                 },
    {Try(Slide(-1))             , L{}                   , nullptr               },
    {42 | Try(Div(0)) | D(13)   , {}                    , 13                    },


    {Fork(Size|4, Size, Card)   , {2,2,3,3}             , {true, 4, 2}          },
    {(Size|4) & Size & Card     , {2,2,3,3}             , {{true, 4}, 2}        },
    {At(0) & At(2)              , {1,2,3}               , {1,3}                 },
    {Reduce(Add) & Size | Div   , {2,2,3,3}             , 2.5                   },


    {Stride(2)                  , {1,2,3,4,5,6}         , {{1,2},{3,4},{5,6}}   },
    {Stride(3)                  , {1,2,3,4,5}           , L{{1,2,3}}            },
    {Stride(42)                 , {1,2,3}               , L{}                   },
    {Stride(42)                 , L{}                   , L{}                   },

    {Chunks(2)                  , {1,2,3,4,5,6}         , {{1,2},{3,4},{5,6}}   },
    {Chunks(3)                  , {1,2,3,4,5}           , {{1,2,3},{4,5}}       },

    {Arange                     , 4                     , {0,1,2,3}             },
    {Arange                     , L{4}                  , {0,1,2,3}             },
    {4|Arange                   , nullptr               , {0,1,2,3}             },
    {Arange                     , "2:6"                 , {2,3,4,5}             },
    {"2:6"   |Arange            , nullptr               , {2,3,4,5}             },
    {"1:9:2" |Arange            , nullptr               , {1,3,5,7}             },
    {"5:1:-1"|Arange            , nullptr               , {5,4,3,2}             },

    {L{2,6}   |Arange           , nullptr               , {2,3,4,5}             },
    {L{1,9,2} |Arange           , nullptr               , {1,3,5,7}             },
    {L{5,1,-1}|Arange           , nullptr               , {5,4,3,2}             },

    {0        | Arange          , nullptr               , L{}                   },
    {"1:9:-1" | Arange          , nullptr               , L{}                   },


    {Items                      , {{"a", 1}, {"b", 2}}  , {L{"a", 1}, L{"b", 2}}},
    {Keys                       , {{"a", 1}, {"b", 2}}  , {"a", "b"}            },
    {Values                     , {{"a", 1}, {"b", 2}}  , {1, 2}                },
    {Enumerate                  , {1,2,3}               , {{0,1},{1,2},{2,3}}   },
    {Flatten                    , {{1,2},{3,4}}         , {1,2,3,4}             },
    {ToList                     , 42                    , L{42}                 },


    // composition
    {Pipe(Size, Eq(2))          , {1,2}                 , true                  },
    {Size|Eq(2)                 , {1,2}                 , true                  },
    {Size|2                     , {1,2}                 , true                  },

    {42 | (Size + Card)           , {}                  , {Size, Card}          },

    // arithmetic
    {Neg                        , 42                    , -42                   },
    {Neg                        , -42                   ,  42                   },

    {Sign                       , 42                    ,  1                    },
    {Sign                       , -42                   , -1                    },

    {Abs                        , 42                    ,  42                   },
    {Abs                        , -42                   ,  42                   },

    {Ceil                       , 42.1                  ,  43                   },
    {Ceil                       , 42.9                  ,  43                   },
    {Ceil                       , -42.1                 , -42                   },

    {Floor                      , 42.1                  ,  42                   },
    {Floor                      , 42.9                  ,  42                   },
    {Floor                      , -42.1                 , -43                   },

    {BitNot                     , 42ul                  , ~42ul                 },
    {BitAnd                     , {1ul, 2ul}            , 2ul & 1ul             },
    {BitOr                      , {1ul, 2ul}            , 2ul | 1ul             },
    {BitXor                     , {1ul, 2ul}            , 2ul xor 1ul           },

    {Lshift                     , {1, 1}                , 2                     },
    {Rshift                     , {2, 1}                , 1                     },

    {Add                        , {3,  2}               , 5                     },
    {Add                        , {.5, 1}               , 1.5                   },
    {Add                        , {.5, 1.5}             , 2                     },

    {Add                        , {3, -2}               , 1                     },
    {Add(2)                     , 3                     , 5                     },
    {Add(-2)                    , 3                     , 1                     },

    {Sub                        , {3,  2}               , 1                     },
    {Sub                        , {3, -2}               , 5                     },
    {Sub(2)                     , 3                     , 1                     },
    {Sub(-2)                    , 3                     , 5                     },

    {Mul                        , {3,  2}               ,  6                    },
    {Mul                        , {3, -2}               , -6                    },
    {Mul(2)                     , 3                     ,  6                    },
    {Mul(-2)                    , 3                     , -6                    },
    {Mul(0.5)                   , 3                     , 1.5                   },
    {Mul(3)                     , 0.5                   , 1.5                   },

    {Div                        , {3,  2}               ,  1.5                  },
    {Div                        , {3, -2}               , -1.5                  },
    {Div(2)                     , 3                     ,  1.5                  },
    {Div(-2)                    , 3                     , -1.5                  },
    {Reverse|Div                , {-2, 3}               , -1.5                  },

    {Mod                        , {4,  2}               ,  0                    },
    {Mod                        , {7,  4}               ,  3                    },
    {Mod                        , {7, -4}               ,  3                    },
    {Mod(2)                     , 4                     ,  0                    },
    {Mod(-4)                    , 7                     ,  3                    },
    {Mod(2)                     , 11                    ,  1                    },
    {Mod(2)                     , 12                    ,  0                    },
    {Mod(2)                     , 13                    ,  1                    },
    {Mod(2)                     , 14                    ,  0                    },

    {Quot                       , {4 ,  2}              ,  2                    },
    {Quot                       , {17, -4}              , -4                    },
    {Quot(2)                    , 11                    ,  5                    },
    {Quot(-4)                   , 7                     , -1                    },

    {Pow(2)                     , 3                     ,  9                    },
    {Pow(2)                     , 4                     , 16                    },
    {Pow(0.5)                   , 9                     ,  3                    },
    {Pow(0.5)                   , 16                    ,  4                    },
    {Pow(0.5)                   , 0.25                  ,  0.5                  },
    {Flip(Pow(3))               , 2                     ,  9                    },
    {Flip(Pow(9))               , 0.5                   ,  3                    },
    {Pow                        , {3, 2}                ,  9                    },

    {Log(2)                     , 8                     , 3                     },
    {Log(2)                     , 16                    , 4                     },
    {Log(0.5)                   , 0.125                 , 3                     },
    {Log(0.5)                   , 0.25                  , 2                     },
    {Log(0.5)                   , 0.5                   , 1                     },
    {Log(E.eval())              , E.eval()              , 1                     },
    {Flip(Log(16   ))           , 2                     , 4                     },
    {Flip(Log(0.125))           , 0.5                   , 3                     },
    {Log                        , {8, 2}                , 3                     },

    {Sqrt                       , 9                     , 3                     },
    {Sqrt                       , 16                    , 4                     },
    {Sqrt                       , 0.25                  , 0.5                   },

    {Sin                        , 0                     , 0                     },
    {Cos                        , 0                     , 1                     },
    {Tan                        , 0                     , 0                     },
    {Asin                       , 0                     , 0                     },
    {Acos                       , 1                     , 0                     },
    {Atan                       , 0                     , 0                     },
    {Sinh                       , 0                     , 0                     },
    {Cosh                       , 0                     , 1                     },
    {Tanh                       , 0                     , 0                     },
    {Asinh                      , 0                     , 0                     },
    {Acosh                      , 1                     , 0                     },
    {Atanh                      , 0                     , 0                     },

    {Exp                        , 1.0/3                 , std::exp(1.0/3)       },
    {Erf                        , 1.0/3                 , std::erf(1.0/3)       },
    {Erfc                       , 1.0/3                 , std::erfc(1.0/3)      },
    {Gamma                      , 1.0/3                 , std::tgamma(1.0/3)    },

    // compose const and fn
    {3|Add(2)                   , {}                    , 5                     },
    {3|Pow(2)                   , {}                    , 9                     },

    // eval operator yields literal which interpreted as Q(x)
    {2|Add(-1)                  , {}                    , 1                     },

    {Repeat(4)                  ,  1                    , {1,1,1,1}             },
    {Repeat(3)                  , 42                    , {42,42,42}            },
    {Repeat(3)|Repeat(2)        ,  1                    , {{1,1,1}, {1,1,1}}    },

    {Reduce(Add)                , {2,2,2,2}             ,  8                    },
    {Reduce(Add)                , {1,2,3,4}             , 10                    },
    {Push(3)|Reduce(Add)        , {1,2,3,4}             , 13                    },
    {Reduce(Add)                , L{42}                 , 42                    },
    {Reduce(Add)                , L{}                   , nullptr               },
    {Reduce                     , {{2,2,2,2}, Add}      ,  8                },
    {L{2,2,2,2} + Add | Reduce  , {}                    , 8                     },


    // ternary and or
    {And(42)|Or(13)             , true                  , 42                    },
    {And(42)|Or(13)             , false                 , 13                    },
    {And(42)|Or(13)|Not         , true                  , false                 },
    {And(E)|Or(NaN)             , true                  , *E                    },

    {Push("baz")|Reduce(And)    , {"foo", "bar"}        , "bar"                 },
    {Push(""   )|Reduce(And)    , {"foo", "bar"}        , ""                    },
    {Push(42   )|Reduce(Or )    , {"foo", "bar"}        , 42                    },
    {Push(""   )|Reduce(Or )    , {"foo", "bar"}        , "foo"                 },

    {Re("42")                   , "42"                  , true                  },
    {Re("42")                   , "43"                  , false                 },
    {Re("^.{3}$")               , "123"                 , true                  },
    {Re("^.{3}$")               , "1234"                , false                 },
    {Str|Re("\\[1,2\\]")        , {1,2}                 , true                  },
    {Str|Re("42")               , 42                    , true                  },
    {(At(0)|Str)&At(1)|Re       , {42, "42"}            , true                  },
    {(At(0)|Str)&At(1)|Re       , {{1,2}, "\\[1,2\\]"}  , true                  },


    {At(0)                      , {1,2,3}               , 1                     },
    {At(1)                      , {1,2,3}               , 2                     },
    {At(2)                      , {1,2,3}               , 3                     },
    {At(3)                      , {1,2,3}               , nullptr               },
    {At("")                     , "foo"                 , "foo"                 },
    {At(0)                      , 42                    , nullptr               },
    {At("/a")                   , {{"a",42}, {"b",13}}  , 42                    },
    {At("/b")                   , {{"a",42}, {"b",13}}  , 13                    },
    {At({"/a", "/b"})           , {{"a",42}, {"b",13}}  , {42, 13}              },
    {At({"/a", "/b"})           , {{"a",42}, {"b",13}}  , {42, 13}              },
    {At({{"f","/a"},{"g","/b"}}), {{"a",42}, {"b",13}}  , {{"f",42},{"g",13}}   },
    {At({{"$/b","/a"}})         , {{"a",42}, {"b",13}}  , {{"13",42}}           },
    {At("::2")                  , {1,2,3,4,5,6,7,8}     , {1,3,5,7}             },
    {At("4:")                   , {1,2,3,4,5,6,7,8}     , {5,6,7,8}             },
    {At("-1:0:-1")              , {1,2,3,4,5,6,7,8}     , {8,7,6,5,4,3,2,1}     },

    {At                         , {{1,2,3}, 0}           , 1                    },

    {At({"", ""})               , 42                     , {42, 42}             },
    // single-element bracket init is always an array in Expression ctor
    {At({""})                   , 42                     , L{42}                },

    {Delete({"/lol"})           , 42                     , 42                   },
    {Delete("")                 , 42                     , nullptr              },
    {Delete({""})               , 42                     , nullptr              },
    {Delete(2)                  , {1,2,3,4,5}            , {1,2,4,5}            },
    {Delete(-2)                 , {1,2,3,4,5}            , {1,2,3,5}            },
    {Delete("/0/1")             , {{1,2,3},4}            , {{1,3}, 4}           },
    {Del({"/a/b/0", "/a/b/1"})  , {{"a", {{"b", {1,2,3,4}}}}} , {{"a", {{"b", {3,4}}}}}},
    {Del({0, 1, 42, "/2"})      , {1,2,3,4,5}            , {4,5}                },

    {Flip(At({1,2,3}))          , 0                      , 1                     },
    {Lookup({1,2,3})            , 0                      , 1                     },
    {Lookup({{"42", "lol"}})    , 42                     , nullptr               },
    {Lookup({{"42", "lol"}})    , 0                      , {"42", "lol"}         },
    {Lookup({{"42", "lol"}})    , -1                     , {"42", "lol"}         },
    {Lookup({{"42", "lol"}})    , "42"                   , "lol"                 },
    {Lookup({{"42", "lol"}})    , "/42"                  , "lol"                 },

    // TODO: string query
    // {At(0)                   , "foo"                 , "f"                   },

    {Map(Add(10))               , {1,2,3,4}              , {11,12,13,14}        },
    {Map(Mod(2))                , {1,2,3,4}              , {1,0,1,0}            },
    {Map(Pow(0.5))              , {1,4,9,16}             , {1,2,3,4}            },
    {Map(Flip(Pow(2)))          , {1,2,3,4}              , {2,4,8,16}           },
    {Map(Flip(Pow(0.5)))        , {1,2,3,4}              , {.5,.25,.125,.0625}  },

    {Filter(Mod(2)|0)           , {1,2,3,4}              , {2,4}                },
    {Filter(Mod(2)|1)           , {1,2,3,4}              , {1,3}                },

    {Recur( 0 + Add(1) )         ,  4                     , 4                   },
    {Recur(42 + Add(-1))         , 41                     , 1                   },
    {Recur(42 + Sub(1) )         , 41                     , 1                   },
    {Recur( 2 + Pow(2) )         ,  4                     , 65536               },

    {Unfold(0 + Add(1))         , 4                      , {0,1,2,3,4}          },
    {Unfold(1 + Add(1))         , 4                      , {1,2,3,4,5}          },

    {Recur( 4 + Add(1))         ,  Ge(12)                , 11                   },
    {Unfold(8 + Add(1))         ,  Ge(12)                , {8,9,10,11}          },
    {Q(Ge(12)) | Recur( 4 + Add(1)),  {}                 , 11                   },
    {Q(Ge(12)) | Unfold(8 + Add(1)),  {}                 , {8,9,10,11}          },


    {4|Recur(Q({0,0}) + ((At(0)|Add(1)) & (At(1)|Sub(1)))),{}, {4, -4}          },


    {All(Gt(5), Mod(2)|0)       , 6                      , true                 },
    {All(Gt(5), Mod(2)|0)       , 7                      , false                },
    {All(Gt(5), Mod(2)|0)       , 5                      , false                },

    {Any(Gt(5), Mod(2)|0)       , 6                      , true                 },
    {Any(Gt(5), Mod(2)|0)       , 7                      , true                 },
    {Any(Gt(5), Mod(2)|0)       , 5                      , false                },

    {Any(Eq(42),Eq(13))         , 42                     , true                 },
    {Any(42,13)                 , 42                     , true                 },
    {Any(42,13)                 , 13                     , true                 },
    {Any(42,13)                 , 2                      , false                },

    {Count(Mod(2)|0)            , {1,2,3,4,5}            , 2                    },
    {Count(Mod(2)|1)            , {1,2,3,4,5}            , 3                    },

    {Each(Gt(2))                , {1, 2, 3, 4}           , false                },
    {Slide(2)|Each(Lt)          , {1, 2, 3, 4}           , true                 },
    {Each(1|Add(1))             , {2, 2, 2, 2}           , true                 },

    {Saturate(Eq(42), Mod(2)|0) , {2,4,8,42,1,2}         , true                 },
    {Saturate(42, Mod(2)|0)     , {2,4,8,42,1,2}         , true                 },
    {Saturate(42, Mod(2)|0)     , {2,4,8,41,2}           , false                },
    {Saturate(42, Mod(2)|0)     , {2,4,8,42}             , false                },

    {Concat("World!")           , "Hello, "              , "Hello, World!"      },
    {Flip(Concat("Hello, "))    , "World!"               , "Hello, World!"      },
    {Concat                     , L{"Hello, ", "World!"} , "Hello, World!"      },
    {Concat({3,4})              , {1,2}                  , {1,2,3,4}            },

    {Format()                   , "Hello, World!"        , "Hello, World!"      },
    {Format("Hello", "World")   , "%s, %s!"              , "Hello, World!"      },
    {Format(2,2,4)              , "%d + %d = %d"         , "2 + 2 = 4"          },
    {Format({1,2,3})            , "list: %s"             , "list: [1,2,3]"      },
    {Format                     , {"Hello, %s!", L{"World"}}, "Hello, World!"   },
    {"%s"|Format(Pi) |Parse     , {}                     , *Pi                  },
    {"%s"|Format(Inf)|Parse     , {}                     , *Inf                 },

    {Parse                      , "[1,2,3]"              , {1,2,3}              },
    {Str                  , {1,2,3}                , "[1,2,3]"            },

    {Transp                     , L{}                    , L{}                  },
    {Transp                     , L{{1,2}}               , {L{1},L{2}}          },
    {Transp                     , {{1,2},{3,4},{5,6}}    , {{1,3,5},{2,4,6}}    },
    {Transp                     , {{1,2},L{"a", "b"}}    , {{1, "a"},{2, "b"}}  },

    {Cartesian                  , L{}                   , L{}                   },
    {Cartesian                  , L{{1,2}}              , {L{1},L{2}}           },

    {Cartesian                  , {{1,2},L{"a", "b"}}   ,
                                                            {{1, "a"},
                                                             {1, "b"},
                                                             {2, "a"},
                                                             {2, "b"}}          },

    {Keyword::Noop              , nullptr               , "Noop"                },
    {Noop                       , nullptr               , true                  },

    // Binary
    {Str                        , Cat({Keyword::At})    , R"(Cat(["At"]))"      },
    {Str                        , Cat({42, Keyword::At, precise<float>()})
                                , R"(Cat([42,"At","0x0p+0"]))"                  },
    // HiOrd
    {Str                        , Max({Keyword::At})    , R"(Max(["At"]))"      },

    {2|Add(1|Add(1))            , {}                    , 4                     },

    {Pi|Cos                     , {}                    , -1                    },
    {Pi|Div(2)|Sin              , {}                    , 1                     },
    {Pi|Div(6)|Sin|Approx(0.5)  , {}                    , true                  },
    {Pi|Div(4)|Tan|Approx(1)    , {}                    , true                  },

    {Try(Cast(type<int>))       , 42.5                  , nullptr               },
    {Cast(type<float>)          , 0.2                   , 0.2f                  },

    {Cast(Underlying<Foo>)      , "Bar"                 ,  int(Foo::Bar)        },
    {Cast(Underlying<Foo>)      , "Baz"                 ,  int(Foo::Baz)        },
    {Uncast(Underlying<Foo>)    , int(Foo::Bar)         , "Bar"                 },
    {Uncast(Underlying<Foo>)    , int(Foo::Baz)         , "Baz"                 },
    {Cast(type<Foo>)            , int(Foo::Bar)         , "Bar"                 },
    {Cast(type<Foo>)            , int(Foo::Baz)         , "Baz"                 },


    {42 | Bind(Add)             , {}                    , Add(42)               },
    {42 & Q(Add) | Bind         , {}                    , Add(42)               },

    {Id & Q(Add(1)) | Bind(Pipe)      , 42           , 42 | Add(1)           },
    {Id & Q(Add(1)) | Bind(Pipe)|Eval , 42           , 43                    },

    {42 | Id & Q(Add(1)) | Bind(Pipe)      , {}      , 42 | Add(1)           },
    {42 | Id & Q(Add(1)) | Bind(Pipe)|Eval , {}      , 43                    },

    { (ToList & Id) | Bind          , Fmt     , Fmt(Fmt)              },
    { Q(Fmt) | (ToList & Id) | Bind , {}      , Fmt(Fmt)              },

    {42 + Add(1) + Sub(1) + Mul(1) | Bind(Pipe) , {}
                                                , 42 | Add(1) | Sub(1) | Mul(1) },

    { Q(Fmt) | Bind(Q) | Bind(Q), {}, Q(Q(Fmt))                                 },

    { 3 | Recur(Q(Fmt) + Bind(Q)), {}                   , Q(Q(Q(Fmt)))          },


    {Eval                       , {}                    , {}                    },
    {Eval                       , 42                    , 42                    },
    {Eval(42)                   , Sub(2)                , 40                    },
    {Eval                       , Eq(nullptr)           , true                  },
    {Dbg(42|Trace(ZMBT_CUR_LOC)|Sub(2)) , {}                    , 40            },
    {Debug(Sub(2))              , 42                    , 40                    },
    {Debug                      , 42                    , nullptr               },
    {Debug(Trace("foo")|Add(2)|Debug(Trace("bar")|Sub(2))),    40   , 40        },

    {Kwrd                       , Fold(Add)             , "Fold"                },
    {Q(Fold(Add)) | Kwrd        , {}                    , "Fold"                },
    {Q(Fold(Add)) | Prms        , {}                    , Tuple(Add)            },
    {Q(Pipe(Add, Sub)) | Prms   , {}                    , Add + Sub             },

    {Op(type<unsigned>, Eq(42))            , 42     , true                },
    {Try(Op(type<unsigned>, Eq(42)))       , -42    , nullptr             },
    {Op(type<std::complex<double>>, Add(1)), {.5, 2}, {1.5, 2}            },

    {Op(type<int>, Eq(42) | Not|Not)       , 41     , false               }, // overload by default ignores boolean keywords

    {Op(type<int>, Add(1)) | Str | Eq("42"), 41, true               },
    {Op(type<int>, Add(1) | Str | Op("", Eq("42"))), 41, true },

    {Error("foo")           , {}              , Err({
                                                    {"message", "foo"}
                                                })                             },

    {Error("foo", "bar")    , {}              , Err({
                                                    {"message", "foo"},
                                                    {"context", "bar"}
                                                })                             },

    {Error(type<int>, "foo", "bar"), {}       , Err({
                                                    {"type"   , "int"},
                                                    {"message", "foo"},
                                                    {"context", "bar"}
                                                })                             },

    {0 | Assert(Ne(0)) | Flip(Div(1)), {}     , Err("assertion failure", "Ne(0)")},
    {1 | Assert(Ne(0)) | Flip(Div(1)), {}     , 1                              },

    {Error("foo") | IsErr            , {}     , true                           },


    {PreProc(1)             , {}              , "$[1]"                         },
    {PreProc("lol")         , {}              , "$[lol]"                       },

};


std::set<Keyword> const CoveredInTestEval = []{
    std::set<Keyword> covered;
    for (auto const& sample : TestSamples)
    {
        auto const& keywords = sample.expr.encoding().keywords;
        covered.insert(keywords.cbegin(), keywords.cend());
    }
    return covered;
}();

} // namespace


BOOST_DATA_TEST_CASE(ExpressionEval, TestSamples)
{
    auto context = lang::EvalContext::make();


    Logger::set_max_level(Logger::DEBUG);

    try
    {
        BOOST_TEST_INFO("Expression: " << sample.expr.prettify());
        BOOST_TEST_INFO("  encoding: " << json_from(sample.expr.encoding()));
        BOOST_TEST_INFO("Input: " << sample.x.prettify());
        BOOST_TEST_INFO("Expected: " << sample.expected.prettify());

        auto const result = sample.expr.eval_e(sample.x, context);
        BOOST_TEST_INFO("Observed: " << result.prettify());
        BOOST_TEST_INFO("Eval log: \n" << context.log);
        BOOST_CHECK_EQUAL(result, sample.expected);
    }
    catch(const std::exception& e)
    {
        BOOST_FAIL("Exception thrown: " << e.what());
    }
}

BOOST_DATA_TEST_CASE(EvalTestCoverage, utf::data::xrange(std::size_t{1ul}, static_cast<std::size_t>(Keyword::_count)))
{
    Keyword const keyword = static_cast<Keyword>(sample);
    if (keyword == Keyword::Void) return;
    if (NotImplemented.count(keyword) == 0 && CoveredInTestEval.count(keyword) == 0)
    {
        BOOST_FAIL("Keyword " << json_from(keyword) << " is not covered in TestEval");
    }
}

BOOST_DATA_TEST_CASE(ImplementationCoverage, utf::data::xrange(std::size_t{1ul}, static_cast<std::size_t>(Keyword::_count)))
{
    BOOST_TEST_INFO("Testing coverage for " << sample);

    Keyword const keyword = static_cast<Keyword>(sample);
    if (keyword == Keyword::Literal || keyword == Keyword::PreProc)
    {
        return;
    }
    auto const expr = Expression(keyword);
    bool const expect_impl = NotImplemented.count(keyword) == 0;
    BOOST_TEST_INFO(expr.prettify());
    try
    {
        auto const maybe_err = expr.eval_e({}, {});
        if (expect_impl
            && maybe_err.is_error()
            && maybe_err.has_subexpr()
            && maybe_err.as_object().at("message") == "not implemented"
        )
        {
            BOOST_FAIL("Keyword " << json_from(keyword) << " is not implemented");
        }
        else if(not expect_impl && not maybe_err.is_error())
        {
            BOOST_FAIL("Keyword " << json_from(keyword)
                << " expected to yield 'not implemented' error"
                << "\n, but got  " << maybe_err.prettify()
                << "\n, to_json: " << maybe_err.to_json()
            );
        }
    }
    catch(const std::exception& e)
    {
        BOOST_FAIL("Keyword " << json_from(keyword) << " throws unexpectedly: " << e.what());
    }
}


BOOST_AUTO_TEST_CASE(ExprFromKeyword)
{
    {
        Expression e(Keyword::Noop);
        BOOST_CHECK_EQUAL(e, Noop);
    }
    {
        Logger::set_max_level(Logger::DEBUG);

        Expression e = Keyword::Noop;
        BOOST_CHECK_NE(e, Noop);
    }
}

BOOST_AUTO_TEST_CASE(NaNConst)
{
    BOOST_CHECK(std::isnan((*NaN).as_double()));
}


BOOST_AUTO_TEST_CASE(InvalidSetError)
{
    BOOST_CHECK(Expression( Subset  ({1,2,3}).eval(13)).is_error());
    BOOST_CHECK(Expression( Superset({1,2,3}).eval(13)).is_error());
    BOOST_CHECK(Expression(PSubset  ({1,2,3}).eval(13)).is_error());
    BOOST_CHECK(Expression(PSuperset({1,2,3}).eval(13)).is_error());
}




BOOST_AUTO_TEST_CASE(SetOperationsOnObject)
{

    BOOST_CHECK(Subset({{"lol", 42}, {"kek", 13}}).match({{"lol", 42}}));
    BOOST_CHECK(In({{"lol", 42}, {"kek", 13}}).match({"lol", 42}));

    boost::json::object set {{"lol", 42}, {"kek", 13}};
    boost::json::object obj {{"lol", 42}, {"lol", 0}}; // will create {{"lol", 42}}
    boost::json::array  kvp {"lol", 42};
    boost::json::string key {"lol"};

    BOOST_CHECK(Subset(set).match(obj));
    BOOST_CHECK(In    (set).match(kvp));
    BOOST_CHECK(In    (set).match(key));

    BOOST_CHECK(Expression(Subset(set).eval(kvp)).is_error());
    BOOST_CHECK(Expression(Subset(set).eval(key)).is_error());
    BOOST_CHECK(Expression(In    (set).eval(obj)).is_error());
}


BOOST_AUTO_TEST_CASE(AtQueryMatch)
{
    auto const obj = boost::json::parse(R"({
        "home": {
            "user": {
                "Desktop":    ["foo", "bar", "baz"],
                "Downloads":  ["lol", "kek"]
            }
        }
    })");


    BOOST_CHECK_MESSAGE((At("/home/user/Downloads/1")| "kek").match(obj), obj);
    BOOST_CHECK((At("/home/user/Desktop/2")|"baz").match(obj));
    BOOST_CHECK((At("/home/user/Desktop") | At(1) | Eq("bar")).match(obj));

    // recursive At
    BOOST_CHECK((At("/home")|At("/user")|At("/Desktop")   | At(0) | Eq("foo")).match(obj));
    BOOST_CHECK((At("/home")|At("/user")|At("/Downloads") | Contains("kek")).match(obj));
    BOOST_CHECK((At("/undefined") | nullptr).match(obj));


    // joining At
    // query array
    BOOST_CHECK((At({"/home/user/Downloads/1", "/home/user/Desktop/2"}) | Eq({"kek", "baz"})).match(obj));
    // query nested array
    BOOST_CHECK((At({{"/home/user/Downloads/1", "/home/user/Desktop/2"}, "/undefined" }) | Eq({{"kek", "baz"}, nullptr})).match(obj));
    // query object
    BOOST_CHECK((At({{"a", "/home/user/Downloads/1"}, {"b", "/home/user/Desktop/2"}}) | Eq({{"a", "kek"}, {"b", "baz"}})).match(obj));
    // query nested structures, a: array, b: object
    BOOST_CHECK((At({{"a", {"/home/user/Downloads/1", "/home/user/Desktop/2"}}, {"b", {{"null", "/undefined"}}}}) | Eq({{"a", {"kek", "baz"}}, {"b", {{"null", nullptr}}}})).match(obj));

    // dynamic keys
    BOOST_CHECK((At({{"$/home/user/Desktop/0", "/home/user/Desktop/1"}}) | Eq({{"foo", "bar"}}) ).match(obj));

}


BOOST_AUTO_TEST_CASE(TestComposePipe)
{
    auto const test = boost::json::parse(R"({
        "foo": {
            "bar": {
                "baz": 42
            }
        }
    })");

    BOOST_CHECK_EQUAL(42, Pipe(At("/foo") | At("/bar") | At("/baz")).eval(test));
    BOOST_CHECK_EQUAL(42, (At("/foo") | At("/bar") | At("/baz")).eval(test));
}


BOOST_AUTO_TEST_CASE(TestComposeMapFilterAt)
{
    auto const AllTrueFirst = Pipe(Filter(At(1)|true), Map(At(0)));
    auto const AllFalseFirst  = Filter(At(1) | Eq(false)) | Map(At(0));

    boost::json::array const pairs {
        {"lol", true},
        {"kek", true},
        {"foo", false},
        {"bar", false},
    };

    BOOST_CHECK_EQUAL(AllTrueFirst.eval(pairs), V({"lol", "kek"}));
    BOOST_CHECK_EQUAL(AllFalseFirst.eval(pairs), V({"foo", "bar"}));
}


BOOST_AUTO_TEST_CASE(ExpressionEvalLog)
{
    auto ctx = EvalContext::make();

    auto const f = Debug(Reduce(Add) & Size | Div);
    auto const x = L{1,2,3,42.5};
    f.eval(x, ctx);
    BOOST_CHECK(!ctx.log.str().empty());
}



BOOST_AUTO_TEST_CASE(TestPreprocessing)
{
    // Param const p1 {1};
    // Param const p2 {2};

    auto p1 = PreProc(1);
    auto p2 = PreProc(2);

    auto const expr = Filter(At(p1)|false) | p2;
    BOOST_TEST_INFO(expr.prettify());
    BOOST_TEST_INFO(expr.to_json());
    auto const pp = expr.preprocessing_parameters();
    BOOST_CHECK_EQUAL(pp.size(), 2);

    auto as_json = expr.to_json();

    BOOST_TEST_INFO(p1.eval());
    BOOST_TEST_INFO(p2.eval());
    boost::json::object values {
        {p1.eval().as_string(), 42},
        {p2.eval().as_string(),  Map(At(13)).to_json()},
    };

    for (auto const& kp: pp)
    {
        auto const& key = kp.first;
        auto const& json_pointer = kp.second;
        as_json.set_at_pointer(json_pointer, values.at(key));
    }

    BOOST_TEST_INFO(as_json);
    BOOST_CHECK_EQUAL(Expression(as_json).prettify(), (Filter(At(42)|false) | Map(At(13))).prettify());
}



BOOST_AUTO_TEST_CASE(SerializationUndefinedSpeed, *utf::timeout(1))
{
    for (int i = 0; i < 1000*1000; i++)
    {
        auto const kw = dejsonize<Keyword>("ArbitraryStringNotPresentInKeywords");
        if (kw != Keyword::Undefined)
        {
            BOOST_FAIL("ITER FAILED = " << i);
            break;
        }
    }
}


BOOST_AUTO_TEST_CASE(SerializationSpeed, *utf::timeout(1))
{
    boost::json::string const test_kw = json_from(Keyword::PSuperset).as_string();
    for (int i = 0; i < 1000*1000; i++)
    {
        auto const kw = dejsonize<Keyword>(test_kw);
        if (kw != Keyword::PSuperset)
        {
            BOOST_FAIL("ITER FAILED = " << i);
            break;
        }
    }
}


BOOST_AUTO_TEST_CASE(PrettifyExpressionTostaticBuffer)
{
    auto e = (Fold(Add) & Size) | Div | Eq(2.5E0) | Not;
    char buff[100];

    e.prettify_to(buff);
    BOOST_CHECK_EQUAL(buff, "(Fold(Add) & Size) | Div | Eq(2.5E0) | Not");
}

BOOST_AUTO_TEST_CASE(DebugExample)
{
    {
        auto const f = Debug(Reduce(Add) & Size | Div);
        auto const x = L{1,2,3,42.5};
        BOOST_CHECK_EQUAL(f.eval(x), 12.125);
    }
    {
        auto const f = Trace(ZMBT_CUR_LOC) | Reduce(Add) & Size | Div;
        auto const x = L{1,2,3,42.5};
        BOOST_CHECK_EQUAL(f.eval(x), 12.125);
    }
}


BOOST_AUTO_TEST_CASE(PrettifyExpression)
{

    #define TEST_PRETIFY(e) \
    BOOST_TEST_INFO("Expected: " #e); \
    BOOST_TEST_INFO("     Got: " << (e).prettify()); \
    { auto const ps = (e).to_json(); std::stringstream ss; pretty_print(ss, ps); \
         BOOST_TEST_INFO("to_json: \n" << ss.str()); } \
    {BOOST_CHECK_EQUAL((e).prettify(), #e); }\
    { std::stringstream ss; (e).prettify_to(ss); \
        BOOST_CHECK_EQUAL(ss.str(), #e); }

    TEST_PRETIFY(   (Fold(Add) & Size) | Div | Eq(2.5E0) | Not      )
    TEST_PRETIFY(   Eq(Pi | Div(2))                                 )
    TEST_PRETIFY(   "%s%d" | Fmt(Pi | Div(2), 2 | Add(2))           )
    TEST_PRETIFY(   Recur(42 + Map(Add(2) | Div(E)))                )
    TEST_PRETIFY(   Unfold(13 + Recur(42 + Map(Add(2) | Div(E))))   )

    TEST_PRETIFY(   Min                                             )
    TEST_PRETIFY(   Min(At("/%s" | Fmt("foo")))                     )

    // check parentheses
    TEST_PRETIFY(   All                                             )
    // TEST_PRETIFY(All()) // FIXME: empty variadic != incomplete kw

    TEST_PRETIFY(   Q(Q | (Q | Q) | Q)                              )
    TEST_PRETIFY(   Q((Q & (Q & Q)) & (Q | (Q | (Q | Q))))          )

    TEST_PRETIFY(   (Q | Q) & (Q | Q)                               )
    TEST_PRETIFY(   (Q & Q) | (Q & Q)                               )

     // right grouping - parentheses preserved
    TEST_PRETIFY(   Q & (Q & (Q & Q))                               )
     // left associativity - unfold groups
    TEST_PRETIFY(   ((Q & Q) & Q) & Q                               );

    // no infix sugar for singleton Pipe and Fork
    TEST_PRETIFY(   Pipe(Fork(All))                                 )
    TEST_PRETIFY(   Pipe(All & Any)                                 )
    TEST_PRETIFY(   Saturate(Eq(23), 42, Any(27, 13) | Not)         )

    TEST_PRETIFY(   Size | 3                                        )

    TEST_PRETIFY(   ~Div(1)                                         )

    // Symbolic links
    TEST_PRETIFY(
        "$f" << (Let("$x") | Assert(Ge(0)) | Lt(2) | And(1) | Or(("$x" & ("$x" | Sub(1) | "$f")) | Mul))
    )
    TEST_PRETIFY(
        Q("$f" << (Let("$x") | Assert(Ge(0)) | Lt(2) | And(1) | Or(("$x" & ("$x" | Sub(1) | "$f")) | Mul)))
    )



    BOOST_CHECK_EQUAL((~Diff({2,3,4})).prettify(), "~Diff([2,3,4])"); // true JSON syntax
    BOOST_CHECK_EQUAL((Q & Q | Q & Q).prettify(), "(Q & Q) | (Q & Q)"); // prec(&) > prec(|)
    BOOST_CHECK_EQUAL((Q | Q & Q | Q).prettify(), "Q | (Q & Q) | Q"); // prec(&) > prec(|)

}


BOOST_AUTO_TEST_CASE(BracketInit)
{
    Expression const e1{Add}; // API
    // Expression const e2{e1}; // error: conflicts with init list ctor
    BOOST_CHECK_EQUAL(e1, Add);
}

BOOST_AUTO_TEST_CASE(TestCapture)
{
    auto inv = "$x" | Ne(0) | And("$x" | Flip(Div(1))) | Or("$x");

    BOOST_CHECK_EQUAL(*(42 | inv), 1.0/42);
    BOOST_CHECK_EQUAL(*(0 | inv), 0);
}

BOOST_AUTO_TEST_CASE(SymbolicLink)
{
    auto fact = "$f" << (Let("$x")
        | Assert(Ge(0))
        | Lt(2)
        | And(1)
        | Or("$x" & ("$x" | Sub(1) | "$f") | Mul)
    );

    BOOST_TEST_INFO(fact.prettify());
    BOOST_CHECK_EQUAL(fact.eval(0),   1);
    BOOST_CHECK_EQUAL(fact.eval(1),   1);
    BOOST_CHECK_EQUAL(fact.eval(2),   2);
    BOOST_CHECK_EQUAL(fact.eval(3),   6);
    BOOST_CHECK_EQUAL(fact.eval(4),  24);
    BOOST_CHECK_EQUAL(fact.eval(5), 120);
    BOOST_CHECK_EQUAL((fact | Kwrd).eval(-5), "Err");
}
