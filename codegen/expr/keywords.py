from functools import cached_property
from typing import Callable, Generator


class Keyword:
    def __init__(self, definition: dict, backend: str = 'cxx'):
        self._definition = definition
        self._backend = backend

    @staticmethod
    def map(signature, definitions):
        return map(lambda d: Keyword(signature, d), definitions)

    @property
    def IsInternal(self) -> bool:
        return self._definition.get('internal', False)

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
    def IsConst(self) -> bool:
        return self.Signature == 'Const'

    @property
    def IsOperator(self) -> bool:
        return 'operator' in self.Tags

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
        if domain is None:
            return 0
        elif type(domain) is list:
            return len(domain)
        elif domain.startswith('list['):
            return '*'
        else:
            return 1

    @property
    def Imports(self) -> list[str]:
        return self._definition.get('imports', {}).get(self._backend, [])

    @cached_property
    def Class(self):
        if self.IsInternal:
            return None
        elif self.Signature != 'Special':
            return f"Signature{self.Signature}<::zmbt::dsl::Keyword::{self.Enum}>"
        else:
            return f"Signature{self.Enum}"


    @property
    def Name(self) -> str:
        return self._definition['name']

    @property
    def Enum(self) -> str:
        return self._definition.get('enum', self._definition['name'].replace('-',' ').title().replace(' ', ''))

    @property
    def DocBrief(self) -> str:
        return self._definition.get('brief', self._definition['name']).capitalize()

    @property
    def DocDetails(self) -> str:
        details = self._definition.get('details', "")
        if details:
            details = f"{details}\n\n"

        domain = self.Domain
        default = self.DefaultValue
        if default and self.IsBinary:
            domain = f"[{domain[0]} [,{domain[1]} = {default}]]"
        else:
            domain = str(domain).replace("'", "")
        domain = f"Domain: {domain}\n\n"
        codomain = f"Codomain: {self.Codomain}\n\n"

        signature_ref = ""
        if self.Signature != 'Special':
            signature_ref = f'\see \\ref {self.Signature.lower()}-syntactic-forms "{self.Signature} Syntatic Forms"'

        complete_doc = details + domain + codomain + signature_ref
        return complete_doc
    @property
    def Short(self) -> str:
        return self._definition.get('short', None)

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
        keyword_groups: list = data['keyword_groups']
        self._keywords = tuple(sorted(
            (Keyword({**group.get('common', {}), **keyword}) for group in keyword_groups for keyword in group['keywords']),
            key=lambda item: item.Name
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
