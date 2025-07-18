# (c) Copyright 2025 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

from functools import cached_property
from typing import Callable, Generator

EXCLUDE_OPERATORS = (
    'Near',
)

class Keyword:
    def __init__(self, definition: dict, backend: str = 'cxx'):
        self._definition = definition
        self._backend = backend

        if 'examples' in definition:
            examples: str = definition['examples']
            lines = []
            for line in examples.splitlines(keepends=False):
                if '↦' in line or '≡' in line:
                    line = line.replace('↦', '`$\mapsto$`').replace('≡', '`$\equiv$`')
                    line = f" * `{line}`"
                lines.append(line)
            definition['examples'] = '\n'.join(lines)

    @staticmethod
    def map(signature, definitions):
        return map(lambda d: Keyword(signature, d), definitions)

    @property
    def IsInternal(self) -> bool:
        return self.Group == 'Internal'

    @property
    def IsHiOrd(self) -> bool:
        return self.Group == 'High-Order'

    @property
    def Signature(self) -> str:
        return self._definition.get('signature', 'Special')

    @property
    def IsBinary(self) -> str:
        return self.Arity == 2

    @property
    def Tags(self) -> tuple[str]:
        return self._definition.get('tags', tuple())

    @property
    def Group(self) -> str:
        return self._definition['group']

    @property
    def IsConst(self) -> bool:
        return self.Signature == 'Const'

    @property
    def IsOperator(self) -> bool:
        return 'op' in self.Tags and not self.Name in EXCLUDE_OPERATORS

    @property
    def CodegenValue(self) -> str:
        return self._definition.get('codegen-value', {}).get(self._backend, None)

    @property
    def DefaultValue(self) -> str:
        default = self._definition.get('default', None)
        if default and not self.IsBinary:
            raise Exception("Default value is only handled for binary expressions")
        return default

    @property
    def Arity(self) -> str:
        domain = self._definition.get('domain', None)
        if self.Signature == 'Const':
            return 0
        elif self.Signature == 'Unary':
            return 1
        elif self.Signature == 'Binary':
            return 2
        elif self.Signature == 'Variadic':
            return 3
        else:
            return 1

    @property
    def Attributes(self) -> str:

        attrs = [
            (
                'is_const',
                'is_unary',
                'is_binary',
                'is_variadic'
            )[self.Arity]
        ]

        if self.Codomain == 'bool':
            attrs.append('is_predicate')
        if self.IsOperator:
            attrs.append('is_operator')
        if self.IsHiOrd:
            attrs.append('is_hiord')
        if self.CodegenValue:
            attrs.append('is_autogen')
        if self.Name == 'Noop':
            attrs.append('is_noop')
        if self.Name == 'Literal':
            attrs.append('is_literal')
        if self.Name == 'PreProc':
            attrs.append('is_preproc')
        if self.Name == 'Err':
            attrs.append('is_error')
        if self.Name == 'Q':
            attrs.append('is_quote')

        return ' | '.join(attrs)

    @property
    def Imports(self) -> list[str]:
        return self._definition.get('imports', {}).get(self._backend, [])

    @cached_property
    def Class(self):
        if self.IsInternal:
            return None
        elif self.Signature != 'Special':
            return f"Signature{self.Signature}<::zmbt::lang::Keyword::{self.Name}>"
        else:
            return f"Signature{self.Name}"


    @property
    def Name(self) -> str:
        return self._definition['name']

    @property
    def DocBrief(self) -> str:
        return self._definition.get('brief', self._definition['name']).capitalize()

    @property
    def DocDetails(self) -> str:
        return self._definition.get('details', "")
        # if details:
        #     details = f"{details}\n\n"

        # domain = self.Domain
        # default = self.DefaultValue
        # if default and self.IsBinary:
        #     domain = f"[{domain[0]} [,{domain[1]} = {default}]]"
        # else:
        #     domain = str(domain).replace("'", "")
        # domain = f"Domain: {domain}\n\n"
        # codomain = f"Codomain: {self.Codomain}\n\n"

        # complete_doc = details + domain + codomain
        # return complete_doc

    @property
    def Symbol(self) -> str:
        return self._definition.get('symbol', "")

    @property
    def Examples(self) -> str:
        return self._definition.get('examples', "")

    @property
    def Aliases(self) -> str:
        return self._definition.get('aliases', [])

    @property
    def Domain(self) -> str:
        return self._definition.get('domain', 'any')

    @property
    def Codomain(self) -> str:
        return self._definition.get('codomain', 'any')

    @property
    def Params(self) -> list:
        return self._definition.get('params', 'any')


class KeywordGrammar:
    def __init__(self, data: dict):
        keyword_groups: dict = data['keyword_groups']

        self._groups = tuple(filter(lambda x: x != 'Internal', keyword_groups.keys()))
        self._groups = tuple(map(lambda x: (x, x.lower().replace(' ', '-')), self._groups))
        self._keywords = tuple((Keyword(
            {'group': group, **group_data.get('defaults', {}), **keyword}) \
                for group, group_data in keyword_groups.items() for keyword in group_data['keywords']
        ))
        # check unique names
        names = [k.Name for k in self._keywords]
        for k in self._keywords:
            names.extend(k.Aliases)
        for name in names:
            if names.count(name) > 1:
                raise ValueError(f"Duplicate keyword name: {name}")

    @property
    def Keywords(self) -> tuple[Keyword]:
        return self._keywords

    @property
    def ApiKeywords(self) -> Generator[Keyword, None, None]:
        return (k for k in self._keywords if not k.IsInternal)

    @property
    def Signatures(self) -> tuple[str]:
        return tuple(k.Signature for k in self._keywords)

    def where(self, pred: Callable) -> list[Keyword]:
        "filter Keywords by attributes conjunction"
        return list(filter(pred, self._keywords))

    def imports_for(self, keywords: list[Keyword]) -> list[str]:
        "get imports for Keywords"
        imports = list(set(lib for k in keywords for lib in k.Imports))
        imports.sort(key=lambda x: chr(255) + x if '/' in x else x)
        return imports

    @property
    def Internals(self) -> list[Keyword]:
        return self.where(lambda x: x.IsInternal)

    @property
    def Constants(self) -> list[Keyword]:
        return self.where(lambda x: x.IsConst)

    @property
    def Operators(self) -> list[Keyword]:
        return self.where(lambda x: x.IsOperator)

    @property
    def HiOrdFns(self) -> list[Keyword]:
        return self.where(lambda x: x.IsHiOrd)

    @property
    def UnaryOperators(self) -> list[Keyword]:
        return self.where(lambda x: x.IsOperator and x.Arity == 1)

    @property
    def BinaryOperators(self) -> list[Keyword]:
        return self.where(lambda x: x.IsOperator and x.Arity == 2)

    @property
    def BinarySignatures(self) -> list[Keyword]:
        return self.where(lambda x: x.IsBinary)

    @property
    def CodegenFns(self) -> list[Keyword]:
        return self.where(lambda x: x.CodegenValue and not x.IsOperator and not x.IsConst)

    @property
    def Groups(self) -> tuple[tuple[str, str]]:
        "list of (group_name, group_ref)"
        return self._groups

    def GetGroup(self, group: str) -> list[Keyword]:
        return self.where(lambda x: x.Group == group)