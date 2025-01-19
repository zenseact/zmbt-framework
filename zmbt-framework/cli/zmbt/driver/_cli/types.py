# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

import click


class CommaSeparatedList(click.ParamType):
    name = "Comma separated list of values, mapped to a Python set"

    def __init__(self, transform = lambda x: x) -> None:
        super().__init__()
        self.transform = transform

    def __call__(self, value, param: click.Parameter = None, ctx: click.Context = None):
        if value is None:
            value = set()
        return super().__call__(value, param, ctx)

    def convert(self, value, param, ctx) -> set:
        if value is None:
            return set()
        elif isinstance(value, str):
            return set(map(self.transform, value.split(',')))
        else:
            try:
                return set(value)
            except:
                self.fail(f"{value!r} is not a valid list", param, ctx)


class Path(click.Path):
    STD = ('?stdout', '?stdin')

    def __call__(self, value, param: click.Parameter = None, ctx: click.Context = None):
        if value in Path.STD:
            value = "-" # stdout/stdin in click.File
        return super().__call__(value, param, ctx)


def resolve_type_in(parameter_definition):
    "update parameter type in place"

    if parameter_definition.get('is_flag', None):
        parameter_definition['type'] = click.BOOL
        return parameter_definition

    parameter_type = parameter_definition.get('type', click.STRING)
    parameter_type = {
        'bool': click.BOOL,
        'str': click.STRING,
        'Path': Path(allow_dash=True),
        'Choice': click.Choice(parameter_definition.pop('choices', None), case_sensitive=False),
        'CommaSeparatedList': CommaSeparatedList(),
    }.get(parameter_type, parameter_type)
    parameter_definition['type'] = parameter_type
