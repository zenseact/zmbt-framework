from functools import cached_property

class Keyword:
    def __init__(self, signature: str, definition: dict):
        self._signature = signature
        self._definition = definition

    @staticmethod
    def map(signature, definitions):
        return map(lambda d: Keyword(signature, d), definitions)

    @cached_property
    def Class(self):
        if self._signature == 'non-template':
            return f"{self.Enum}"
        else:
            return f"{self._signature}<Keyword::{self.Enum}>"

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
