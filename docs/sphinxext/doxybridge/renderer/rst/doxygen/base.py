"""
    doxybridge.renderer.rst.doxygen.base
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :copyright: Copyright (c) 2009, Michael Jones           
    :license: MIT, see LICENSE for details.
"""

class Renderer(object):

    def __init__(self,
                 project_info,
                 data_object,
                 renderer_factory,
                 node_factory,
                 state,
                 document,
                 directive
                 ):

        self.project_info = project_info
        self.data_object = data_object
        self.renderer_factory = renderer_factory
        self.node_factory = node_factory
        self.state = state
        self.document = document
        self.directive = directive


