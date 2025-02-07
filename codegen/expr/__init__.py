
import os

from codegen.cli import Config
from codegen.expr.keywords import KeywordGrammar


TEMPLATES_DIR = os.path.join(os.path.dirname(__file__), 'templates')
DATA_DEFINITION = os.path.join(os.path.dirname(__file__), 'keywords.yaml')

CONFIG = Config(app_name='codegen.expr',
                app_descr='Code generator for expression grammar',
                transform=KeywordGrammar,
                templates_dir=TEMPLATES_DIR,
                data=DATA_DEFINITION)
