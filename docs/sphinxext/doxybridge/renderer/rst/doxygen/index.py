"""
    doxybridge.renderer.rst.doxygen.index
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :copyright: Copyright (c) 2009, Michael Jones           
    :license: MIT, see LICENSE for details. 
"""

from sphinx import addnodes
from doxybridge.renderer.rst.doxygen.base import Renderer

class DoxygenTypeSubRenderer(Renderer):

    def render(self):

        nodelist = []

        # Process all the compound children
        for compound in self.data_object.get_compound():
            compound_renderer = self.renderer_factory.create_renderer(self.data_object,
                                                                      compound,
                                                                      self.directive)
            nodelist.extend(compound_renderer.render())

        return nodelist


class CompoundTypeSubRenderer(Renderer):

    def __init__(self, compound_parser, *args):
        Renderer.__init__(self, *args)

        self.compound_parser = compound_parser


    def render(self):

        title = self.title()
        rettype, name, arglist, const, signature = self.directive.parse_title(title)
        target = self.directive.create_target(name)

        # add a signature node 
        signode = addnodes.desc_signature(signature, '')
        signode['first'] = False

        signode += addnodes.desc_type('', '')
        
        self.directive.add_return_type(signode[-1] , rettype)
        name = self.directive.add_sig_name(signode, name)
        self.directive.set_name(name)
        self.directive.add_sig_args(signode, arglist, const)


        self.directive.before_content()
        description_nodes = self.description()
        content_node = addnodes.desc_content('')
        content_node += description_nodes
        self.directive.after_content()

        target.extend(signode)
        term = self.node_factory.term("","", ids=[name], *target )
        entry = self.node_factory.definition_list_item("",term, content_node)

        
        return [entry]
         
    def title(self):
        kind = self.node_factory.emphasis(text=self.data_object.kind)
        name = self.node_factory.strong(text=self.data_object.name)

        args = []
        args.extend(kind)
        args.extend([self.node_factory.Text(" "), name])

        return args


    def description(self):
        description_nodes = []

        # Read in the corresponding xml file and process
        file_data = self.compound_parser.parse(self.data_object.refid)
        data_renderer = self.renderer_factory.create_renderer(self.data_object,
                                                              file_data,
                                                              self.directive)
        nodes = data_renderer.render()

        return nodes
