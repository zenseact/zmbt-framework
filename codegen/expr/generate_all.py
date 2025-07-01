
import os
from codegen import WORKDIR
from codegen.cli import Config, run
from codegen.expr import CONFIG, TEMPLATES_DIR
from glob import iglob


def generate_all():

    for template in iglob(TEMPLATES_DIR + '/**/*.*', recursive=True):
        config = Config(**CONFIG.__dict__)
        config.output = os.path.join(WORKDIR, os.path.relpath(template, TEMPLATES_DIR))
        run(template, config)
