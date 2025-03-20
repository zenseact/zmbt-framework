
import os

from codegen.cli import Config
from codegen.mdm.model_definition import DefinitionMachine


TEMPLATES_DIR = os.path.join(os.path.dirname(__file__), 'templates')
DATA_DEFINITION = os.path.join(os.path.dirname(__file__), 'model_definition.yaml')

CONFIG = Config(app_name='codegen.expr',
                app_descr='Code generator for expression grammar',
                transform=DefinitionMachine,
                templates_dir=TEMPLATES_DIR,
                data=DATA_DEFINITION)
