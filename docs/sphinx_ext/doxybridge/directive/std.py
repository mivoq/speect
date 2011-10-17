"""
    doxybridge.directive.std
    ~~~~~~~~~~~~~~~~~~~~~~~~

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :license: MIT, see LICENSE for details.
"""


from sphinx import addnodes
from sphinx.util.docfields import DocFieldTransformer

from docutils import nodes



class DoxygenDirective(object):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        self.sphinx_directive = sphinx_directive
        self.project_info = project_info
        self.app = app
        self.data_object = data_object
        self.options = sphinx_directive.options


    @classmethod
    def get_handler(cls, sphinx_directive, project_info):
        """
        Get a handler for this domain. 

        self.objtype must be set to the object's type
        self.domain must be set to the domain
        """

        raise NotImplementedError('must be implemented in subclasses')


    def run(self):
        """
        Main directive entry function, called by docutils upon encountering the
        directive.

        This directive is meant to be quite easily subclassable, so it delegates
        to several additional methods.  What it does:

        * find out if called as a domain-specific directive, set self.domain
        * create a `desc` node to fit all description inside
        * parse standard options, currently `noindex`
        * create an index node if needed as self.indexnode
        * parse all given signatures (as returned by self.get_signatures())
          using self.handle_signature(), which should either return a name
          or raise ValueError
        * add index entries using self.add_target_and_index()
        * parse the content and handle doc fields in it
        """


        # set info for sphinx_directive
        self.sphinx_directive.objtype = self.objtype
        self.sphinx_directive.domain = self.domain
        self.sphinx_directive.env = self.sphinx_directive.state.document.settings.env
        self.sphinx_directive.indexnode = addnodes.index(entries=[])

        # setup this node
        self.node = addnodes.desc()
        self.node.document = self.sphinx_directive.state.document
        self.node['domain'] = self.domain

        # 'desctype' is a backwards compatible attribute
        self.node['objtype'] = self.node['desctype'] = self.sphinx_directive.objtype
        self.node['noindex'] = noindex = ('noindex' in self.sphinx_directive.options)
        self.noindex = noindex 

        self.sphinx_directive.names = []

        # get renderer
        self.data_object_renderer = self.get_data_object_renderer()

        #self.data_object_renderer.handle()
        self.node.extend(self.data_object_renderer.render())


        return [self.sphinx_directive.indexnode, self.node]


    def get_data_object_renderer(self):
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

        return renderer_factory.create_renderer(root_data_object, self.data_object, self)


    def before_content(self):
        """
        Called before parsing content. Used to set information about the current
        directive context on the build environment.
        """
        pass


    def after_content(self):
        """
        Called after parsing content. Used to reset information about the
        current directive context on the build environment.
        """
        pass

      


from doxybridge.domains.c import CDomain


BUILTIN_DOMAINS = {
    'c': CDomain,
}
