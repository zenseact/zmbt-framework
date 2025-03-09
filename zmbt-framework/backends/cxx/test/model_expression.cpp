/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <cmath>
#include <limits>
#include <boost/math/constants/constants.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "zmbt/model.hpp"
#include "zmbt/decor.hpp"


namespace utf = boost::unit_test;

using namespace zmbt;
using namespace zmbt::reflect;
using namespace zmbt::api;
using namespace boost::json;

using V = boost::json::value;
using L = boost::json::array;
using zmbt::expr::Keyword;

namespace {

std::set<Keyword> const NotImplemented {
    Keyword::Diff,
    Keyword::DiffFrom,
    Keyword::Union,
    Keyword::Intersect,
    Keyword::Min,
    Keyword::Max,
    Keyword::Argmin,
    Keyword::Argmax,
    Keyword::Bind,
    Keyword::Round,
    Keyword::BitLshift,
    Keyword::BitRshift,
    Keyword::BitLshiftFrom,
    Keyword::BitRshiftFrom,
    Keyword::Void,
};


double const pi = boost::math::constants::pi<double>();
double const e = boost::math::constants::e<double>();
double const inf = std::numeric_limits<double>::infinity();
double const eps = std::numeric_limits<double>::epsilon();

struct TestEvalSample
{
    Expression expr;
    V x;
    V expected;

    friend std::ostream& operator<<(std::ostream& os, TestEvalSample const& sample)
    {
        return os << "{expr: " << sample.expr << ", x: " << sample.x << ", expected: " << sample.expected << "}";
    }
};


std::vector<TestEvalSample> const TestSamples
{ // expr             , x                     , expected
    // identity
    {Id                         , 42                    , 42                    },
    {Id                         , ""                    , ""                    },
    {Id                         , nullptr               , nullptr               },
    {Id                         , {1,2,3}               , {1,2,3}               },


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
    { Pi                        , {}                    , pi                    },
    { E                         , {}                    , e                     },
    { Inf                       , {}                    , inf                   },
    { Eps                       , {}                    , eps                   },

    // comparison
    {Eq(42)                     , 42                    , true                  },
    {Eq(42)                     , 13                    , false                 },
    {Eq                         , {42, 42}              , true                  },
    {Eq                         , {13, 42}              , false                 },
    {Ne(42)                     , 42                    , false                 },
    {Ne(42)                     , 13                    , true                  },
    {Ne                         , {42, 42}              , false                 },
    {Ne                         , {13, 42}              , true                  },

    // literal is equivalent to Eq(x) in direct evaluation
    {Expression(42)             , 42                    , true                  },
    {Expression(42)             , 13                    , false                 },
    {Expression(42)|Not         , 42                    , false                 },
    {Expression(42)|Not         , 13                    , true                  },
    {Expression(42)|Not|Not     , 42                    , true                  },
    {Expression(42)|Not|Not     , 13                    , false                 },

    {Approx({3.14, 0.001})      , pi                    , true                 },
    {Approx({2.71, 0.005})      , e                     , true                 },
    {Approx({3.14, 0, 0.01})    , pi                    , true                 },
    {Approx({2.71, 0, 0.01})    , e                     , true                 },
    {Approx                     , {pi, {3.14, 0.001}}   , true                 },
    {All(Gt(3.14), Lt(3.15))    , pi                    , true                 },

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
    {  ProperSubset({1,2,3})    , {1,3}                 , true                  },
    {  ProperSubset({1,3}  )    , {1,2,3}               , false                 },
    {  ProperSubset({1,2}  )    , {1,2}                 , false                 },
    {ProperSuperset({1,3}  )    , {1,2,3}               , true                  },
    {ProperSuperset({1,2,3})    , {1,3}                 , false                 },
    {ProperSuperset({1,2})      , {1,2}                 , false                 },

    {        Subset, {{1,3}     , {1,2,3}}              , true                  },
    {      Superset, {{1,2,3}   , {1,3}  }              , true                  },
    {  ProperSubset, {{1,3}     , {1,2,3}}              , true                  },
    {ProperSuperset, {{1,2,3}   , {1,3}  }              , true                  },

    {SetEq({1,2,3})             , {1,2,3}               , true                  },
    {SetEq({1,2,3})             , {1,3,2}               , true                  },
    {SetEq({1,2,3})             , {1,2}                 , false                 },

    {Uniques                    , {1,2,2,4,3,4}         , {3,4,2,1}             },


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
    {Sort                       , {4,1,3,1,2,1}         , {1,1,1,2,3,4}         },
    {Sort|Reverse               , {4,1,3,1,2,1}         , {4,3,2,1,1,1}         },
    {Sort(Neg)                  , {4,1,3,1,2,1}         , {4,3,2,1,1,1}         },
    {Sort                       , {{1, 3},{2, 1},{3, 2}}, {{1, 3},{2, 1},{3, 2}}},
    {Sort                       , {{3, 2},{2, 1},{1, 3}}, {{1, 3},{2, 1},{3, 2}}},
    {Sort(At(1))                , {{1, 3},{2, 1},{3, 2}}, {{2, 1},{3, 2},{1, 3}}},
    {Sort(At(1))                , {{3, 2},{2, 1},{1, 3}}, {{2, 1},{3, 2},{1, 3}}},

    {Sort                       , L{}                   , L{}                   },
    {Reverse                    , L{}                   , L{}                   },

    {Slide(3)                   , {1,2,3,4,5}           , {{1,2,3},{2,3,4},{3,4,5}}},
    {Slide(3)|Map(Sum)          , {1,2,3,4,5}           , {6      , 9      , 12}},
    {Slide(3)|Map(Prod)         , {1,2,3,4,5}           , {6      , 24     , 60}},
    {Slide(3)|Map(Avg)          , {1,2,3,4,5}           , {2      , 3      ,  4}},
    {Slide(42)                  , {1,2,3}               , L{{1,2,3}}            },
    {Slide(42)                  , L{}                   , L{}                   },
    {Try(Slide(-1))             , L{}                   , nullptr               },

    {Pack(Size|4, Size, Card)   , {2,2,3,3}             , {true, 4, 2}          },
    {(Size|4) + Size + Card     , {2,2,3,3}             , {true, 4, 2}          },
    {At(0) + At(1) + At(2)      , {1,2,3}               , {1,2,3}               },
    {(Reduce(Add) + Size) | Div , {2,2,3,3}             , 2.5                   },


    {Stride(2)                  , {1,2,3,4,5,6}         , {{1,2},{3,4},{5,6}}   },
    {Stride(3)                  , {1,2,3,4,5}           , {{1,2,3},{4,5}}       },
    {Stride(42)                 , {1,2,3}               , L{{1,2,3}}            },
    {Stride(42)                 , L{}                   , L{}                   },


    // composition
    {Compose(Eq(2), Size)       , {1,2}                 , true                  },
    {Size|Eq(2)                 , {1,2}                 , true                  },
    {Size|2                     , {1,2}                 , true                  },

    // arithmetic
    {Neg                        , 42                    , -42                   },
    {Neg                        , -42                   ,  42                   },

    {Sign                       , 42                    ,  1                    },
    {Sign                       , -42                   , -1                    },

    {Abs                        , 42                    ,  42.0                 },
    {Abs                        , -42                   ,  42.0                 },

    {Ceil                       , 42.1                  ,  43.0                 },
    {Ceil                       , 42.9                  ,  43.0                 },
    {Ceil                       , -42.1                 , -42.0                 },

    {Floor                      , 42.1                  ,  42.0                 },
    {Floor                      , 42.9                  ,  42.0                 },
    {Floor                      , -42.1                 , -43.0                 },

    {BitNot                     , 42ul                  , ~42ul                 },
    {BitAnd                     , {1ul, 2ul}            , 2ul & 1ul             },
    {BitOr                      , {1ul, 2ul}            , 2ul | 1ul             },
    {BitXor                     , {1ul, 2ul}            , 2ul xor 1ul           },

    {Add                        , {3,  2}               , 5                     },
    {Add                        , {3, -2}               , 1                     },
    {Add(2)                     , 3                     , 5                     },
    {Add(-2)                    , 3                     , 1                     },

    {Sub                        , {3,  2}               , 1                     },
    {Sub                        , {3, -2}               , 5                     },
    {Sub(2)                     , 3                     , 1                     },
    {Sub(-2)                    , 3                     , 5                     },
    {SubFrom                    , { 2, 3}               , 1                     },
    {SubFrom                    , {-2, 3}               , 5                     },

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
    {DivFrom                    , { 2, 3}               ,  1.5                  },
    {DivFrom                    , {-2, 3}               , -1.5                  },

    {Mod                        , {4,  2}               ,  0                    },
    {Mod                        , {7,  4}               ,  3                    },
    {Mod                        , {7, -4}               ,  3                    },
    {Mod(2)                     , 4                     ,  0                    },
    {Mod(-4)                    , 7                     ,  3                    },
    {Mod(2)                     , 11                    ,  1                    },
    {Mod(2)                     , 12                    ,  0                    },
    {Mod(2)                     , 13                    ,  1                    },
    {Mod(2)                     , 14                    ,  0                    },
    {ModFrom                    , { 2, 4}               ,  0                    },
    {ModFrom                    , { 3, 4}               ,  1                    },

    {Quot                       , {4 ,  2}              ,  2                    },
    {Quot                       , {17, -4}              , -4                    },
    {Quot(2)                    , 11                    ,  5                    },
    {Quot(-4)                   , 7                     , -1                    },
    {QuotFrom                   , { 2, 4}               ,  2                    },
    {QuotFrom                   , { 3, 4}               ,  1                    },

    {Pow(2)                     , 3                     ,  9                    },
    {Pow(2)                     , 4                     , 16                    },
    {Pow(0.5)                   , 9                     ,  3                    },
    {Pow(0.5)                   , 16                    ,  4                    },
    {Pow(0.5)                   , 0.25                  ,  0.5                  },
    {PowFrom(3)                 , 2                     ,  9                    },
    {PowFrom(9)                 , 0.5                   ,  3                    },
    {Pow                        , {3, 2}                ,  9                    },
    {PowFrom                    , {2, 3}                ,  9                    },

    {Log(2)                     , 8                     , 3                     },
    {Log(2)                     , 16                    , 4                     },
    {Log(0.5)                   , 0.125                 , 3                     },
    {Log(0.5)                   , 0.25                  , 2                     },
    {Log(0.5)                   , 0.5                   , 1                     },
    {Log(E.eval())              , E.eval()              , 1                     },
    {LogFrom(16   )             , 2                     , 4                     },
    {LogFrom(0.125)             , 0.5                   , 3                     },
    {Log                        , {8, 2}                , 3                     },
    {LogFrom                    , {2, 8}                , 3                     },

    {Sqrt                       , 9                     , 3.0                   },
    {Sqrt                       , 16                    , 4.0                   },
    {Sqrt                       , 0.25                  , 0.5                   },

    {Sin                        , 0                     , 0.0                   },
    {Cos                        , 0                     , 1.0                   },
    {Tan                        , 0                     , 0.0                   },
    {Asin                       , 0                     , 0.0                   },
    {Acos                       , 1                     , 0.0                   },
    {Atan                       , 0                     , 0.0                   },
    {Sinh                       , 0                     , 0.0                   },
    {Cosh                       , 0                     , 1.0                   },
    {Tanh                       , 0                     , 0.0                   },
    {Asinh                      , 0                     , 0.0                   },
    {Acosh                      , 1                     , 0.0                   },
    {Atanh                      , 0                     , 0.0                   },

    {Exp                        , 1.0/3                 , std::exp(1.0/3)       },
    {Erf                        , 1.0/3                 , std::erf(1.0/3)       },
    {Erfc                       , 1.0/3                 , std::erfc(1.0/3)      },
    {Gamma                      , 1.0/3                 , std::tgamma(1.0/3)    },

    // apply fn
    {Apply(Add(2), 3)           , {}                    , 5                     },
    {Apply(Pow(2), 3)           , {}                    , 9                     },
    {Add(2) <<= 3               , {}                    , 5                     },
    {Pow(2) <<= 3               , {}                    , 9                     },

    {Repeat(4)                  ,  1                    , {1,1,1,1}             },
    {Repeat(3)                  , 42                    , {42,42,42}            },
    {Repeat(3)|Repeat(2)        ,  1                    , {{1,1,1}, {1,1,1}}    },

    {Reduce(Add, 0)             , {2,2,2,2}             ,  8                    },
    {Reduce(Add, 0)             , {1,2,3,4}             , 10                    },
    {Reduce(Add, 3)             , {1,2,3,4}             , 13                    },

    // ternary and or
    {And(42)|Or(13)             , true                  , 42                    },
    {And(42)|Or(13)             , false                 , 13                    },
    {And(42)|Or(13)|Not         , true                  , false                 },
    {And(E)|Or(NaN)             , true                  , E                     },

    {Reduce(And, "baz")         , {"foo", "bar"}        , "bar"                 },
    {Reduce(And, ""   )         , {"foo", "bar"}        , ""                    },
    {Reduce(Or , 42   )         , {"foo", "bar"}        , 42                    },
    {Reduce(Or , ""   )         , {"foo", "bar"}        , "foo"                 },

    {Re("42")                   , "42"                  , true                  },
    {Re("42")                   , "43"                  , false                 },
    {Re("^.{3}$")               , "123"                 , true                  },
    {Re("^.{3}$")               , "1234"                , false                 },
    {Re("\\[1,2\\]")            , {1,2}                 , true                  },
    {Re("42")                   , 42                    , true                  },
    {Re                         , {42, "42"}            , true                  },
    {Re                         , {{1,2}, "\\[1,2\\]"}  , true                  },


    {At(0)                      , {1,2,3}               , 1                     },
    {At(1)                      , {1,2,3}               , 2                     },
    {At(2)                      , {1,2,3}               , 3                     },
    {At(3)                      , {1,2,3}               , nullptr               },
    {At("")                     , "foo"                 , "foo"                 },
    {At(0)                      , 42                    , nullptr               },
    {At("/a")                   , {{"a",42}, {"b",13}}   , 42                   },
    {At("/b")                   , {{"a",42}, {"b",13}}   , 13                   },
    {At({"/a", "/b"})           , {{"a",42}, {"b",13}}   , {42, 13}             },
    {At({"/a", "/b"})           , {{"a",42}, {"b",13}}   , {42, 13}             },
    {At({{"f","/a"},{"g","/b"}}), {{"a",42}, {"b",13}}   , {{"f",42},{"g",13}}  },
    {At({{"$/b","/a"}})         , {{"a",42}, {"b",13}}   , {{"13",42}}          },
    {At("::2")                  , {1,2,3,4,5,6,7,8}      , {1,3,5,7}            },
    {At("4:")                   , {1,2,3,4,5,6,7,8}      , {5,6,7,8}            },
    {At("-1:0:-1")              , {1,2,3,4,5,6,7,8}      , {8,7,6,5,4,3,2,1}    },

    {At                         , {{1,2,3}, 0}           , 1                    },

    // TODO: string query
    // {At(0)                   , "foo"                 , "f"                   },

    {Map(Add(10))               , {1,2,3,4}              , {11,12,13,14}        },
    {Map(Mod(2))                , {1,2,3,4}              , {1,0,1,0}            },
    {Map(Pow(0.5))              , {1,4,9,16}             , {1,2,3,4}            },
    {Map(PowFrom(2))            , {1,2,3,4}              , {2,4,8,16}           },
    {Map(PowFrom(0.5))          , {1,2,3,4}              , {.5,.25,.125,.0625}  },

    {Filter(Mod(2)|0)           , {1,2,3,4}              , {2,4}                },
    {Filter(Mod(2)|1)           , {1,2,3,4}              , {1,3}                },

    {Recur(Add(1), 4)           , 0                      , 4                    },
    {Recur(Add(-1), 41)         , 42                     , 1                    },
    {Recur(Sub(1) , 41)         , 42                     , 1                    },
    {Recur(Pow(2), 4)           , 2                      , 65536                },

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

    {Saturate(Eq(42), Mod(2)|0) , {2,4,8,42,1,2}         , true                 },
    {Saturate(42, Mod(2)|0)     , {2,4,8,42,1,2}         , true                 },
    {Saturate(42, Mod(2)|0)     , {2,4,8,41,2}           , false                },
    {Saturate(42, Mod(2)|0)     , {2,4,8,42}             , false                },

    {List(42)                   , nullptr                , L{42}                },
    {List(1,2,3)                , nullptr                , {1,2,3}              },

    {Concat("World!")           , "Hello, "              , "Hello, World!"      },
    {Concat                     , L{"Hello, ", "World!"} , "Hello, World!"      },
    {Concat({3,4})              , {1,2}                  , {1,2,3,4}            },

    {Format({"Hello", "World"}) , "%s, %s!"              , "Hello, World!"      },
    {Format("World")            , "Hello, %s!"           , "Hello, World!"      },
    {Format                     , {"Hello, %s!", "World"}, "Hello, World!"      },
    {Format({2,2,4})            , "%d + %d = %d"         , "2 + 2 = 4"          },
    {Format(L{{1,2,3}})         , "list: %s"             , "list: [1,2,3]"      },

    {Parse                      , "[1,2,3]"              , {1,2,3}              },
    {Serialize                  , {1,2,3}                , "[1,2,3]"            },

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

    {Try(Div(2))                , 42                    , 21                    },
    {Try(Div(0))                , 42                    , nullptr               },
    {Try                        , {42, Div(0)}          , nullptr               },

    {TryCatch(Div(2))           , 42                    , 21                    },
    {TryCatch(Div(0))           , 42                    ,
                                        {{"err", "zero division"},
                                         {"fn" , {{":div",0}}   },
                                         {"x"  , 42              },
                                         {"op" , "zmbt::GenericSignalOperator"}}},
};


std::set<Keyword> const CoveredInTestEval = []{
    std::set<Keyword> covered;
    for (auto const& sample : TestSamples)
    {
        std::set<Keyword> covered_here;
        JsonTraverse([&](boost::json::value const& v, std::string const){
            Expression expr(v);
            covered.insert(expr.keyword());
            covered_here.insert(expr.keyword());
            return false;
        })(sample.expr.underlying());
    }
    return covered;
}();

} // namespace


BOOST_DATA_TEST_CASE(ExpressionEval, TestSamples)
{
    BOOST_CHECK_EQUAL(sample.expr.eval(sample.x), sample.expected);

    V js = json_from(sample.expr);
    Expression converted_implicitly = js;
    Expression converted_explicitly = dejsonize<Expression>(js);

    BOOST_CHECK_EQUAL(sample.expr, converted_explicitly);
    BOOST_CHECK_EQUAL(sample.expr, converted_implicitly);
}

BOOST_DATA_TEST_CASE(EvalTestCoverage, utf::data::xrange(std::size_t{1ul}, static_cast<std::size_t>(Keyword::_count)))
{
    Keyword const keyword = static_cast<Keyword>(sample);
    if (NotImplemented.count(keyword) == 0 && CoveredInTestEval.count(keyword) == 0)
    {
        BOOST_FAIL("Keyword " << json_from(keyword) << " is not covered in TestEval");
    }
}

BOOST_DATA_TEST_CASE(ImplementationCoverage, utf::data::xrange(std::size_t{1ul}, static_cast<std::size_t>(Keyword::_count)))
{
    Keyword const keyword = static_cast<Keyword>(sample);
    Expression const expr(keyword, {});
    bool implementation_error_thrown = false;
    try
    {
        expr.eval();
    }
    catch(const expression_not_implemented& e)
    {
        implementation_error_thrown = true;
        if (NotImplemented.count(keyword) == 0)
        {
            std::cerr << e.what() << '\n';
            BOOST_FAIL("Keyword " << json_from(keyword) << " is not implemented");
        }
    }
    catch(const std::exception& e)
    {
        std::ignore = e;
    }
    if (!implementation_error_thrown && NotImplemented.count(keyword) != 0)
    {
        BOOST_FAIL("Keyword " << json_from(keyword) << " expected to throw expression_not_implemented error");
    }
}



BOOST_AUTO_TEST_CASE(NaNConst)
{
    std::isnan(NaN.eval().as_double());
}

BOOST_AUTO_TEST_CASE(TrigonometricFunctions)
{
    BOOST_CHECK_CLOSE((Pi | Div(2) | Sin)   .eval().as_double(),  1.0, eps);
    BOOST_CHECK_CLOSE((Pi | Cos)            .eval().as_double(), -1.0, eps);
    BOOST_CHECK_CLOSE((Pi | Div(6) | Sin)   .eval().as_double(),  0.5, 0.00001);
    BOOST_CHECK_CLOSE((Pi | Div(4) | Tan)   .eval().as_double(),  1.0, 0.00001);
}

BOOST_AUTO_TEST_CASE(InvalidSetThrow)
{
    BOOST_CHECK_THROW(      Subset  ({1,2,3}).match(13), std::exception);
    BOOST_CHECK_THROW(      Superset({1,2,3}).match(13), std::exception);
    BOOST_CHECK_THROW(ProperSubset  ({1,2,3}).match(13), std::exception);
    BOOST_CHECK_THROW(ProperSuperset({1,2,3}).match(13), std::exception);
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

    BOOST_CHECK_THROW(Subset(set).match(kvp), std::exception);
    BOOST_CHECK_THROW(Subset(set).match(key), std::exception);
    BOOST_CHECK_THROW(In    (set).match(obj), std::exception);
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

    BOOST_CHECK_EQUAL(42, Compose(At("/baz"), At("/bar"), At("/foo")).eval(test));
    BOOST_CHECK_EQUAL(42, (At("/foo") | At("/bar") | At("/baz")).eval(test));
}


BOOST_AUTO_TEST_CASE(TestComposeMapFilterAt)
{
    auto const AllTrueFirst = Compose(Map(At(0)), Filter(At(1)|true));
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




BOOST_AUTO_TEST_CASE(TestNestedParam)
{
    Param const p1 {1};
    Param const p2 {2};

    auto const expr = Compose(Map(At(p1)), Filter(At(p2)|false));
    boost::json::array list{};

    JsonTraverse([&](boost::json::value const& v, std::string const jp){
        if (Param::isParam(v)) list.push_back(value_from(jp));
        return false;
    })(expr.underlying());
    BOOST_CHECK_EQUAL(list.size(), 2);
}



BOOST_AUTO_TEST_CASE(SerializationUndefinedSpeed, *utf::timeout(1))
{
    for (int i = 0; i < 1000*1000; i++)
    {
        auto const kw = dejsonize<Keyword>(":ArbitraryStringNotPresentInKeywords");
        if (kw != Keyword::Undefined)
        {
            BOOST_FAIL("ITER FAILED = " << i);
            break;
        }
    }
}


BOOST_AUTO_TEST_CASE(SerializationSpeed, *utf::timeout(1))
{
    boost::json::string const test_kw = json_from(Keyword::ProperSuperset).as_string();
    for (int i = 0; i < 1000*1000; i++)
    {
        auto const kw = dejsonize<Keyword>(test_kw);
        if (kw != Keyword::ProperSuperset)
        {
            BOOST_FAIL("ITER FAILED = " << i);
            break;
        }
    }
}


BOOST_AUTO_TEST_CASE(TestComposeVsApplyPrecedence)
{
    // <<= has lower precedence than |

    // Invalid expression: Add(2) <<= 2 | Eq(4) is Add(2) <<= (2 | Eq(4))
    BOOST_CHECK_NE(Add(2) <<= 2 | Eq(4), Compose(Eq(4), Apply(Add(2), 2)));

    BOOST_CHECK_EQUAL((Add(2) <<= 2) | Eq(4), Compose(Eq(4), Apply(Add(2), 2)));
    BOOST_CHECK_EQUAL(((Add(2) <<= 2) | Eq(4)).eval(), true);
}


BOOST_AUTO_TEST_CASE(CodegenType)
{
    auto const test = zmbt::expr::detail::getCodegenType(Keyword::Abs);
    BOOST_CHECK(test == zmbt::expr::detail::CodegenType::CodegenFn);
}
