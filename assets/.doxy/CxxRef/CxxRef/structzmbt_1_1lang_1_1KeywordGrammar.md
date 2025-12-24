

# Struct zmbt::lang::KeywordGrammar



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**KeywordGrammar**](structzmbt_1_1lang_1_1KeywordGrammar.md)



_Boost.Sirit.Qi Grammar for_ [_**zmbt::lang::Keyword**_](namespacezmbt_1_1lang.md#enum-keyword) _parsing._

* `#include <keyword_grammar.hpp>`



Inherits the following classes: boost::spirit::qi::grammar< boost::json::string::const_iterator, Keyword()>


















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**KeywordSymbol**](structzmbt_1_1lang_1_1KeywordSymbol.md) | [**keyword\_symbol**](#variable-keyword_symbol)  <br> |
|  boost::spirit::qi::rule&lt; boost::json::string::const\_iterator, [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword)()&gt; | [**start**](#variable-start)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**KeywordGrammar**](#function-keywordgrammar) () <br> |




























## Public Attributes Documentation




### variable keyword\_symbol 

```C++
KeywordSymbol zmbt::lang::KeywordGrammar::keyword_symbol;
```




<hr>



### variable start 

```C++
boost::spirit::qi::rule<boost::json::string::const_iterator, Keyword()> zmbt::lang::KeywordGrammar::start;
```




<hr>
## Public Functions Documentation




### function KeywordGrammar 

```C++
zmbt::lang::KeywordGrammar::KeywordGrammar () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/keyword_grammar.hpp`

