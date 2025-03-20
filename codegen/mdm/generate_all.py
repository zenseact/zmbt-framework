
import os
from codegen import WORKDIR
from codegen.cli import Config, run
from codegen.mdm import CONFIG, TEMPLATES_DIR


EXPR_HEADER_OUT = os.path.join(WORKDIR, 'zmbt-framework/backends/cxx/include/zmbt/mapping/')
EXPR_SOURCE_OUT = os.path.join(WORKDIR, 'zmbt-framework/backends/cxx/src/mapping/')

def generate_all():
    for template in os.listdir(TEMPLATES_DIR):
        if template.endswith('.hpp'):
            out = os.path.join(EXPR_HEADER_OUT, template)
        elif template.endswith('.cpp'):
            out = os.path.join(EXPR_SOURCE_OUT, template)
        config = Config(**CONFIG.__dict__)
        config.output = out
        run(template, config)
