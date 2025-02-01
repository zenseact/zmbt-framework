#! /usr/bin/bash

bazel run //templates/expr:keyword_hpp > zmbt-framework/backends/cxx/include/zmbt/model/keyword.hpp
bazel run //templates/expr:keyword_cpp > zmbt-framework/backends/cxx/src/model/keyword.cpp
bazel run //templates/expr:keyword_grammar_cpp > zmbt-framework/backends/cxx/src/model/keyword_grammar.cpp
bazel run //templates/expr:expression_api_hpp > zmbt-framework/backends/cxx/include/zmbt/model/expression_api.hpp
bazel run //templates/expr:expression_api_cpp > zmbt-framework/backends/cxx/src/model/expression_api.cpp
