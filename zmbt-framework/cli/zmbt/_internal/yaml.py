# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0


from ruamel.yaml import YAML

YAML_11 = YAML(typ="safe")
YAML_11.version = '1.1'
YAML_11.default_flow_style = False

YAML_12 = YAML(typ="safe")
YAML_12.version = '1.2'
YAML_12.default_flow_style = False

DEFAULT_SPEC = '1.2'

def safe_load(stream, spec = DEFAULT_SPEC):
    yaml = YAML_11 if spec == '1.1' else YAML_12
    return yaml.load(stream)


def safe_dump(data, stream, spec = DEFAULT_SPEC):
    yaml = YAML_11 if spec == '1.1' else YAML_12
    return yaml.dump(data, stream)
