from functools import cached_property
from typing import Generator


class Keyword:
    def __init__(self, definition: dict):
        self._definition = definition

    @staticmethod
    def map(signature, definitions):
        return map(lambda d: Keyword(signature, d), definitions)

    @property
    def Internal(self) -> str:
        return self._definition.get('internal', False)

    @property
    def Signature(self) -> str:
        return self._definition.get('signature', None)

    @cached_property
    def Class(self):
        if self.Internal:
            return None
        elif self.Signature:
            return f"Signature{self.Signature}<::zmbt::expr::Keyword::{self.Enum}>"
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
        return self._definition.get('details', None)

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
        self._keywords = tuple(Keyword(
            {**group.get('common', {}), **keyword}
        ) for group in keyword_groups for keyword in group['keywords'])

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
        return (k for k in self._keywords if not k.Internal)

    @property
    def Signatures(self) -> tuple[str]:
        return tuple(k.Signature for k in self._keywords)

    def filterSignature(self, signature: str) -> list[Keyword]:
        return list(filter(lambda k: k.Signature == signature, self._keywords))
