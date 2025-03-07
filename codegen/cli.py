# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

import argparse
from datetime import datetime, timezone
import os
import sys

from dataclasses import dataclass

from ruamel.yaml import YAML

from wheezy.template.engine import Engine
from wheezy.template.ext.code import CodeExtension
from wheezy.template.ext.core import CoreExtension
from wheezy.template.loader import FileLoader

from codegen import WORKDIR


# see CoreExtension and CodeExtension init
DEFAULT_TOKEN_START = '@'
DEFAULT_LINE_JOIN = '\\'
DEFAULT_APP_DESCR = "Code generator based on wheezy.template"
FORMAT_APP_USAGE = "{} <template_name>"


yaml = YAML(typ="safe")
DEFAULT_YAML_SPEC = '1.2'

def load_dict(path: str, yaml_spec = DEFAULT_YAML_SPEC) -> dict:
    "load YAML file"
    with open(path) as file:
        yaml.version = yaml_spec
        return yaml.load(file)


@dataclass
class Config:
    app_name: str = sys.argv[0]
    app_descr: str = DEFAULT_APP_DESCR
    transform: type = dict
    token_start: str = DEFAULT_TOKEN_START
    line_join: str = DEFAULT_LINE_JOIN
    templates_dir: str = None
    data: str = None
    output: str = None

def build_argparser(config: Config) -> argparse.ArgumentParser:
    app_info = {}
    app_info.setdefault('prog', config.app_name)
    app_info.setdefault('description', config.app_descr)
    app_info.setdefault('usage', FORMAT_APP_USAGE.format(config.app_name))

    parser = argparse.ArgumentParser(**app_info)
    parser.add_argument('template',
        choices=os.listdir(config.templates_dir),
        help = 'Template name, choices: [{}]'.format(', '.join(os.listdir(config.templates_dir)))
    )
    return parser

def render(template_entry: str, data: dict, config: Config) -> str:

    TEMPLATES_DIR = os.path.dirname(template_entry)
    data = config.transform(data)

    TEMPLATE_ENGINE = Engine(
        loader=FileLoader([TEMPLATES_DIR]),
        extensions=[CoreExtension(config.token_start, config.line_join)]
    )
    TEMPLATE_ENGINE.global_vars.update({'appname': config.app_name})
    TEMPLATE_ENGINE.global_vars.update({'timestamp': str(datetime.now(timezone.utc))})
    TEMPLATE_ENGINE.global_vars.update({'template': os.path.relpath(template_entry, WORKDIR)})

    return TEMPLATE_ENGINE.get_template(template_entry).render({'data':data})


def run(template: str, config = Config) -> str:
    data = os.path.abspath(config.data)
    template = os.path.join(config.templates_dir, template)

    assert os.path.isfile(data)
    data = load_dict(data)

    generated = render(template, data, config)

    if config.output == None:
        sys.stdout.write(generated)
    else:
        with open(config.output, 'w') as f:
            f.write(generated)
            print(f'done: {config.output}', file=sys.stderr)


def run_from_cli(config = Config) -> None:
    argparser = build_argparser(config)
    args = argparser.parse_args(sys.argv[1:])
    data = os.path.abspath(config.data)
    return run(args.template, config)
