# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

import os
import sys

from argparse import Namespace

from cli import ARGPARSER

from ruamel.yaml import YAML

from wheezy.template.engine import Engine
from wheezy.template.ext.code import CodeExtension
from wheezy.template.ext.core import CoreExtension
from wheezy.template.loader import FileLoader

# see CoreExtension and CodeExtension init
TOKEN_START = '@'
LINE_JOIN = '\\'

def render(template_entry: str, data: dict):

    TEMPLATES_DIR = os.path.dirname(template_entry)

    TEMPLATE_ENGINE = Engine(
        loader=FileLoader([TEMPLATES_DIR]),
        extensions=[CoreExtension(TOKEN_START, LINE_JOIN), CodeExtension(TOKEN_START)]
    )

    return TEMPLATE_ENGINE.get_template(template_entry).render(data)


yaml = YAML(typ="safe")
DEFAULT_YAML_SPEC = '1.2'

def load_dict(path: str, yaml_spec = DEFAULT_YAML_SPEC) -> dict:
    "load YAML file"
    with open(path) as file:
        yaml.version = yaml_spec
        return yaml.load(file)


if __name__ == "__main__":

    args = ARGPARSER.parse_args(sys.argv[1:])
    data = os.path.abspath(args.data)
    template = os.path.abspath(args.template)

    assert os.path.isfile(data)
    data = load_dict(data)

    generated = render(template, data)

    print(generated, file=args.output)

