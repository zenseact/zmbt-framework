

# Struct zmbt::lang::ExpressionGrammar

**template &lt;typename OutputIterator&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**ExpressionGrammar**](structzmbt_1_1lang_1_1ExpressionGrammar.md)








Inherits the following classes: boost::spirit::karma::grammar< std::back_insert_iterator< std::string >, Expression()>


















## Public Attributes

| Type | Name |
| ---: | :--- |
|  boost::spirit::karma::rule&lt; OutputIterator, std::list&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt;()&gt; | [**composition**](#variable-composition)  <br> |
|  boost::spirit::karma::rule&lt; OutputIterator, std::list&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt;()&gt; | [**fork**](#variable-fork)  <br> |
|  boost::spirit::karma::rule&lt; OutputIterator, [**Expression**](classzmbt_1_1lang_1_1Expression.md)()&gt; | [**keyword**](#variable-keyword)  <br> |
|  boost::spirit::karma::rule&lt; OutputIterator, [**Expression**](classzmbt_1_1lang_1_1Expression.md)()&gt; | [**literal**](#variable-literal)  <br> |
|  boost::spirit::karma::rule&lt; OutputIterator, std::list&lt; [**Expression**](classzmbt_1_1lang_1_1Expression.md) &gt;()&gt; | [**parameters**](#variable-parameters)  <br> |
|  boost::spirit::karma::rule&lt; OutputIterator, [**Expression**](classzmbt_1_1lang_1_1Expression.md)()&gt; | [**start**](#variable-start)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ExpressionGrammar**](#function-expressiongrammar) () <br> |




























## Public Attributes Documentation




### variable composition 

```C++
boost::spirit::karma::rule<OutputIterator, std::list<Expression>()> zmbt::lang::ExpressionGrammar< OutputIterator >::composition;
```




<hr>



### variable fork 

```C++
boost::spirit::karma::rule<OutputIterator, std::list<Expression>()> zmbt::lang::ExpressionGrammar< OutputIterator >::fork;
```




<hr>



### variable keyword 

```C++
boost::spirit::karma::rule<OutputIterator, Expression()> zmbt::lang::ExpressionGrammar< OutputIterator >::keyword;
```




<hr>



### variable literal 

```C++
boost::spirit::karma::rule<OutputIterator, Expression()> zmbt::lang::ExpressionGrammar< OutputIterator >::literal;
```




<hr>



### variable parameters 

```C++
boost::spirit::karma::rule<OutputIterator, std::list<Expression>()> zmbt::lang::ExpressionGrammar< OutputIterator >::parameters;
```




<hr>



### variable start 

```C++
boost::spirit::karma::rule<OutputIterator, Expression()> zmbt::lang::ExpressionGrammar< OutputIterator >::start;
```




<hr>
## Public Functions Documentation




### function ExpressionGrammar 

```C++
inline zmbt::lang::ExpressionGrammar::ExpressionGrammar () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/expression_grammar.hpp`

