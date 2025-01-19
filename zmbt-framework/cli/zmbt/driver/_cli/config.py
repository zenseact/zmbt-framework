# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0


from typing import Callable
import click

from zmbt.driver._cli.group import command_group
from zmbt.driver._cli.definition import CLI


def _add_option(command, option_name, option_definition: dict) -> Callable:

    option_definition = option_definition.copy()
    option_definition['help'] = option_definition.get('help', '') #.encode().decode('unicode-escape')

    param_decls = ['--' + option_name]
    alias = option_definition.pop('alias', None)
    if alias:
        param_decls.insert(0, '-' + alias)

    command = click.option(*param_decls,**option_definition)(command)


def configure_command(command: Callable):

    # Command entry function requirements:
    # - shall start with underscore
    # - Kebab case
    CMD_NAME = (command.__name__).replace('_', '-')

    CMD_DEF = CLI['commands'][CMD_NAME]

    CMD_HELP = CMD_DEF.get('help', '') #.encode().decode('unicode-escape')

    command = command_group.command(help=CMD_HELP, name=CMD_NAME)(command)

    # add verbose option to each command
    command = click.option('-v', '--verbose', count=True)(command)

    for option_name, option_definition in CMD_DEF.get('options', {}).items():
        _add_option(command, option_name, option_definition)


    for argument_name, argument_definition in CMD_DEF.get('arguments', {}).items():
        argument_decor = click.argument(argument_name, **argument_definition)
        command = argument_decor(command)
    return command
