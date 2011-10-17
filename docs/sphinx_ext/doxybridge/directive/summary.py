"""
    doxybridge.directive.summary
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :license: MIT, see LICENSE for details.
"""

from sphinx import addnodes
from sphinx.ext.autosummary import autosummary_table

from docutils import nodes
from docutils.statemachine import ViewList

class DoxygenSummary(object):
    def __init__(self, sphinx_directive, project_info, app, data_objects, names):
        self.sphinx_directive = sphinx_directive
        self.project_info = project_info
        self.app = app
        self.data_objects = data_objects
        self.options = sphinx_directive.options
        self.names = names


    @classmethod
    def get_handler(cls, sphinx_directive, project_info, names):
        """
        Get a handler for this domain. 

        self.domain must be set to the domain
        """

        raise NotImplementedError('must be implemented in subclasses')


    def get_data_object_renderer(self, data_object):
        """
        Find the renderer that will render this data object
        """

        filter_ =  self.app.emit("doxybridge-emitter", "create_filter",
                                 self.sphinx_directive.options)[0]
        renderer_factory = self.app.emit("doxybridge-emitter", "create_renderer_factory",
                                         self.project_info, 
                                         self.sphinx_directive.state,
                                         self.sphinx_directive.state.document,
                                         filter_
                                         )[0]

        root_data_object = self.app.emit("doxybridge-emitter", "get_root_data_object")[0]

        return renderer_factory.create_renderer(root_data_object, data_object, self)


    def get_items(self):
        """Try to import the given names, and return a list of
        ``[(name, signature, summary_string, real_name), ...]``.
        """
        env = self.sphinx_directive.state.document.settings.env

        items = []
        max_item_chars = 50

        for name, dobject in zip(self.names, self.data_objects):

            renderer = self.get_data_object_renderer(dobject)
            display_name = self.get_display_name(name, renderer)
#            print display_name
            real_name = self.get_real_name(name, renderer)
            signature = self.get_signature(renderer)
            summary = self.get_summary(renderer)
            items.append((display_name, signature, summary, real_name))

        return items


    def make_title(self, title_text):
        text_nodes = nodes.inline(title_text, title_text)
        title = nodes.title(title_text, '', *text_nodes)
        return title
    

    def get_table(self, items):
        """Generate a proper list of table nodes for autosummary:: directive.

        *items* is a list produced by :meth:`get_items`.
        """
        table_spec = addnodes.tabular_col_spec()
        table_spec['spec'] = 'LL'

        table = autosummary_table('')
        real_table = nodes.table('')

        if "title" in self.options:
            title_node = self.make_title(self.options["title"])
            real_table.insert(0, title_node)
            
        table.append(real_table)
        group = nodes.tgroup('', cols=2)
        real_table.append(group)
        group.append(nodes.colspec('', colwidth=10))
        group.append(nodes.colspec('', colwidth=90))
        body = nodes.tbody('')
        group.append(body)

        def append_row(*column_texts):
            row = nodes.row('')
            for text in column_texts:
                node = nodes.paragraph('')
                vl = ViewList()
                vl.append(text, '<doxybridge-autosummary>')
                self.sphinx_directive.state.nested_parse(vl, 0, node)
                try:
                    if isinstance(node[0], nodes.paragraph):
                        node = node[0]
                except IndexError:
                    pass
                row.append(nodes.entry('', node))
            body.append(row)

        for name, sig, summary, real_name in items:
            qualifier = self.get_qualifier(name)
            if 'nosignatures' not in self.options:
                col1 = ':%s:`%s <%s>`\ %s' % (qualifier, name, real_name, sig)
            else:
                col1 = ':%s:`%s <%s>`' % (qualifier, name, real_name)
            col2 = summary
            append_row(col1, col2)

        return [table_spec, table]


from doxybridge.autosummary.c import CSummary


BUILTIN_SUMMARIES = {
    'c': CSummary,
}
