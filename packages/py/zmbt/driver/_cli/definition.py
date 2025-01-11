# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0


import os


from zmbt import __version__ as VERSION
from zmbt._internal import yaml
from zmbt.driver._cli.types import resolve_type_in


def _resolve_click_formatting(text: str):
    return text.encode().decode('unicode-escape')

def _get_cli_definition(definition_file_path: str):

    with open(definition_file_path) as file:
        cli = yaml.safe_load(file, spec='1.1')

    cli['help'] = _resolve_click_formatting(cli['help'].format(version=VERSION))
    # prepare global definition
    for command_def in cli['commands'].values():
        command_def['help'] = _resolve_click_formatting(command_def['help'])

        for option_def in command_def.get('options', {}).values():
            option_def['help'] = _resolve_click_formatting(option_def['help'])
            resolve_type_in(option_def)

        for arg_def in command_def.get('arguments', {}).values():
            # no 'help' in args
            resolve_type_in(arg_def)
    return cli


CLI_DEFINITION_FILE = os.path.join(os.path.dirname(__file__), 'definition.yaml')
CLI = _get_cli_definition(CLI_DEFINITION_FILE)
