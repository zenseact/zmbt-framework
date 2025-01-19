# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

import click

from zmbt import __version__ as VERSION
from zmbt.driver._cli.definition import CLI



@click.group(context_settings=CLI['context_settings'],help=CLI['help'])
@click.version_option(VERSION)
def command_group():
    pass
