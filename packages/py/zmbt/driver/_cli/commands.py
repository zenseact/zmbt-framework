# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0


from zmbt.driver._cli.group import command_group
from zmbt.driver._cli.config import configure_command


def main():
    # default_map = ... TODO: get CLI default map from config
    command_group(
        # default_map=default_map
    )


@configure_command
def test(**kwargs):
    print(f'zmbt test {kwargs}')



@configure_command
def validate(**kwargs):
    print(f'zmbt validate {kwargs}')
