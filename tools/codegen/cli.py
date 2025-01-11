# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

import argparse

APP_INFO = {
    "prog": "codegen",
    "description": "Code generator based on wheezy.template",
    "usage": "codegen --data <data.yaml> --template <main_template>",
}

def build_argparser() -> argparse.ArgumentParser:

    parser = argparse.ArgumentParser(**APP_INFO)
    parser.add_argument('--data',
        type=str,
        help = 'Path to rendiring data in YAML'
    )
    parser.add_argument('--template',
        type=str,
        help = 'Path to entry wheezy template file'
    )
    parser.add_argument('--output',
        type=argparse.FileType('w'),
        default='-',
        help = 'Path to entry wheezy template file'
    )
    return parser

ARGPARSER = build_argparser()
