
import logging, re
import markdown
import mkdocs.plugins
from mkdocs.structure.pages import Page

log = logging.getLogger('mkdocs')

SM_GUIDE = 'user-guide/signal-mapping/'

SM_DSL = 'dsl-reference/signal-mapping/'

MD_COMMENTS = re.compile("(?<=>)((/\*)|(\*/))(?=<)")
ANNOTATIONS = re.compile("(?<=>)\/\*(\(\s*\d+\s*\))\*\/(?=<)")
ANNOTATIONS_ENDLINE = re.compile("(?<=>)\/\/\s*(\(\d+\))(?=<)")
API_LINK = re.compile("<code>@T_(\w+)</code>")

@mkdocs.plugins.event_priority(-50)
def on_post_page(output: str, page: Page, **kwargs) -> str:

    if page.file.url == SM_GUIDE:
        log.info(f'preprocessing {page.file.url}')
        output = re.sub(MD_COMMENTS, "  ", output)
        output = re.sub(ANNOTATIONS, r'\1    ', output)
        output = re.sub(ANNOTATIONS_ENDLINE, r'\1', output)
        return output
    elif page.file.url == SM_DSL:
        log.info(f'preprocessing {page.file.url}')
        output = re.sub(API_LINK, r'<a href="../../CxxRef/structzmbt_1_1mapping_1_1ModelDefinition_1_1T__\1/">T_\1</a>', output)
        return output
    else:
        return output