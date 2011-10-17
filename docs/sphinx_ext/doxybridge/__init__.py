"""
    Doxybridge
    ~~~~~~~~~~
    A bridge between doxygen generated xml and the Sphinx documentation toolchain.

    :copyright: Copyright 2011 by Aby Louw
    :license: BSD, see LICENSE for details.
"""

from sphinx.directives import ObjectDescription

from sphinx.ext.autodoc import members_option, bool_option
from sphinx.ext.inheritance_diagram import html_visit_inheritance_diagram, skip
from sphinx.ext.autosummary import Autosummary


from docutils.parsers.rst.directives import unchanged_required, unchanged, flag

from doxybridge.utils import DoxybridgeUtils
from doxybridge.domains.inheritance_diagram import speect_inheritance_diagram


import re



class DoxygenAutosummary(Autosummary):
    """
    Specialized Doxygen autosummary directive for doxygen generated xml.
    """

    required_arguments = 0
    optional_arguments = 0
    final_argument_whitespace = False
    has_content = True
    option_spec = {
        'toctree': unchanged,
        'nosignatures': flag,
        'template': unchanged,
        'path' : unchanged_required,
        'project' : unchanged_required,
        'domain' : unchanged_required,
        'title' : unchanged_required,
    }

    def run(self):
        self.env = env = self.state.document.settings.env
        self.genopt = {}
        self.warnings = []

        names = [x.strip() for x in self.content
                 if x.strip() and re.search(r'^[~a-zA-Z_]', x.strip()[0])]
        items = self.get_items(names)
        nodes = self.get_table(items)

        if 'toctree' in self.options:
            suffix = env.config.source_suffix
            dirname = posixpath.dirname(env.docname)

            tree_prefix = self.options['toctree'].strip()
            docnames = []
            for name, sig, summary, real_name in items:
                docname = posixpath.join(tree_prefix, real_name)
                if docname.endswith(suffix):
                    docname = docname[:-len(suffix)]
                docname = posixpath.normpath(posixpath.join(dirname, docname))
                if docname not in env.found_docs:
                    self.warn('toctree references unknown document %r'
                              % docname)
                docnames.append(docname)

            tocnode = addnodes.toctree()
            tocnode['includefiles'] = docnames
            tocnode['entries'] = [(None, docname) for docname in docnames]
            tocnode['maxdepth'] = -1
            tocnode['glob'] = None

            tocnode = autosummary_toc('', '', tocnode)
            nodes.append(tocnode)

        return self.warnings + nodes


    def get_items(self, names):
        this_env = self.state.document.settings.env

        # get the project info
        project_info = this_env.app.emit("doxybridge-emitter",
                                         "create_project_info",
                                         self.options)[0]

        # create a directive based on the project info
        # and the specified options
        try:
            self.directive = this_env.app.emit("doxybridge-emitter",
                                               "create_summary_directive",
                                               self,
                                               project_info,
                                               names)[0]
        except ValueError:
            self.directive = None
            return []
        
        # run the directive and return its results to sphinx
        return self.directive.get_items()

    def get_table(self, items):
        if self.directive:
            return self.directive.get_table(items)
        else:
            return []



class DoxygenAutoDirective(ObjectDescription):
    """
    Specialized Doxygen directive subclass for doxygen generated xml.
    
    Creates a domain specific directive based on the project information
    and/or specified options and calls this directive's run method.

    'option_spec' entries:
        * path    - Top level path to doxygen generated xml files that should
                    be queried for this directive entry.
        * project - The project name keyword in the 'doxybridge_projects'
                    dict to be queried for this directive entry.
        * domain  - The domain for this directive entry.
        * type    - The type of this entry.
    """

    # docutils Directive attributes
    has_content = True
    optional_arguments = 4
    final_argument_whitespace = True

    option_spec = {
        "path" : unchanged_required,
        "project" : unchanged_required,
        "domain" : unchanged_required,
        "type" : unchanged_required,
        "members" : unchanged,
        'noindex': flag,
        'inheritance': unchanged_required,
        }


    def run(self):
        """
        Main directive entry function, called by sphinx upon encountering the
        'doxybridge' directive.
        """

        this_env = self.state.document.settings.env

        # get the project info
        project_info = this_env.app.emit("doxybridge-emitter",
                                         "create_project_info",
                                         self.options)[0]

        # create a directive based on the project info
        # and the specified options
        try:
            directive = this_env.app.emit("doxybridge-emitter",
                                          "create_domain_directive",
                                          self,
                                          project_info)[0]
        except ValueError:
            return []
        
        # run the directive and return its results to sphinx
        return directive.run()

        


def setup(app):
    doxybridgeUtils = DoxybridgeUtils()
    doxybridgeUtils.register_and_connect_emitters(app)
    app.setup_extension('sphinx.ext.graphviz')
    app.setup_extension('sphinx.ext.autosummary')
    app.add_node(
        speect_inheritance_diagram,
        latex=(skip, None),
        html=(html_visit_inheritance_diagram, None),
        text=(skip, None),
        man=(skip, None),
        texinfo=(skip, None))
    
    app.add_directive("doxybridge", DoxygenAutoDirective)
    app.add_directive("doxybridge-autosummary", DoxygenAutosummary)
    app.add_config_value("doxybridge_projects", {}, True)
    app.add_config_value("doxybridge_default_project", "", True)

    # cmake role & directive for CMake variables
    app.add_object_type("cmake", "cmake", '%s (CMake variable)',
                        parse_node=None, ref_nodeclass=None,
                        objname='CMake variable')

    # cmake role & directive for CMake functions
    app.add_object_type("cmake_func", "cmake_func", '%s (CMake function)',
                        parse_node=None, ref_nodeclass=None,
                        objname='CMake function')

    # cmake role & directive for CMake macros
    app.add_object_type("cmake_macro", "cmake_macro", '%s (CMake macro)',
                        parse_node=None, ref_nodeclass=None,
                        objname='CMake macro')
    
    app.add_stylesheet("doxybridge.css")
