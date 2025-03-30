
import logging, re
import markdown
import mkdocs.plugins
from mkdocs.structure.pages import Page

log = logging.getLogger('mkdocs')

FILES_TO_PROCESS = [
    'user-guide/signal-mapping/',
]

MD_COMMENTS = re.compile("(?<=>)((/\*)|(\*/))(?=<)")
ANNOTATIONS = re.compile("(?<=>)\/\*(\(\s*\d+\s*\))\*\/(?=<)")
ANNOTATIONS_ENDLINE = re.compile("(?<=>)\/\/\s*(\(\d+\))(?=<)")



@mkdocs.plugins.event_priority(-50)
def on_post_page(output: str, page: Page, **kwargs) -> str:

    if page.file.url in FILES_TO_PROCESS:
        log.info(f'preprocessing {page.file.url}')
        output = re.sub(MD_COMMENTS, "  ", output)
        output = re.sub(ANNOTATIONS, r'\1    ', output)
        output = re.sub(ANNOTATIONS_ENDLINE, r'\1', output)
        return output
    else:
        return output