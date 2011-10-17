"""
    doxybridge.utils
    ~~~~~~~~~~~~~~~~

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :copyright: Copyright (c) 2009, Michael Jones
    :license: MIT, see LICENSE for details.
"""

import fnmatch
import re
import textwrap
import os

from doxybridge.factories import ProjectInfoFactory, NodeFactory, RootDataObject
from doxybridge.directive import DirectiveFactory
from doxybridge.directive.std import BUILTIN_DOMAINS
from doxybridge.directive.summary import BUILTIN_SUMMARIES
from doxybridge.parser import DoxygenParserFactory
from doxybridge.finder.doxygen import ItemMatcherFactory, DoxygenItemFinderFactoryCreator
from doxybridge.finder import FinderFactory, NoMatchesError, MultipleMatchesError
from doxybridge.parser import DoxygenIndexParser
from doxybridge.renderer.rst.doxygen import DoxygenToRstRendererFactoryCreator, RstContentCreator
from doxybridge.renderer.rst.doxygen.filter import FilterFactory, GlobFactory, PathHandler


from sphinx import addnodes
from docutils.statemachine import ViewList
from docutils import nodes

from sphinx.domains.cpp import DefinitionParser


class DoxybridgeUtils(object):
    def __init__(self):
        self.data_objects_cache = {}
        self.project_info_factory = ProjectInfoFactory(fnmatch.fnmatch)
        self.directive_factory = DirectiveFactory(BUILTIN_DOMAINS, BUILTIN_SUMMARIES)
        self.parser_factory = DoxygenParserFactory()
        self.matcher_factory = ItemMatcherFactory()
        self.index_parser = DoxygenIndexParser()
        self.item_finder_factory_creator = DoxygenItemFinderFactoryCreator(self.parser_factory,
                                                                           self.matcher_factory)
        self.finder_factory = FinderFactory(self.index_parser, self.item_finder_factory_creator)
        self.node_factory = NodeFactory(nodes, addnodes)
        self.rst_content_creator = RstContentCreator( ViewList, textwrap.dedent )
        self.path_handler = PathHandler(os.sep, os.path.basename)
        self.glob_factory = GlobFactory(fnmatch.fnmatch)
        self.filter_factory = FilterFactory(self.glob_factory, self.path_handler)
        self.root_data_object = RootDataObject()

        self.renderer_factory_creator = DoxygenToRstRendererFactoryCreator(self.node_factory,
                                                                           self.parser_factory,
                                                                           self.rst_content_creator
                                                                           )


        self.callbacks = {"create_project_info" : self.project_info_factory.create_project_info,
                          "create_domain_directive" : self.directive_factory.create_domain_directive,
                          "create_summary_directive" : self.directive_factory.create_summary_directive,
                          "create_finder" : self.finder_factory.create_finder,
                          "create_name_matcher" :  self.matcher_factory.create_name_matcher,
                          "create_name_type_matcher" :  self.matcher_factory.create_name_type_matcher,
                          "create_matcher_stack" : self.matcher_factory.create_matcher_stack,
                          "create_name_type_matcher" : self.matcher_factory.create_name_type_matcher,
                          "create_filter" : self.create_filter,
                          "create_renderer_factory" : self.renderer_factory_creator.create_factory,
                          "get_root_data_object" : self.get_root_data_object,
                          "standard_finder" : self.standard_finder
                          }


    def create_filter(self, options):
        if options.has_key("members"):
            filter_ = self.filter_factory.create_class_filter(options)
        else:
            filter_ = self.filter_factory.create_outline_filter(options)

        return filter_


    def get_root_data_object(self):
        return self.root_data_object

    def service_emitter(self, app, *args):
        if args[0] not in self.callbacks:
            string = "no callback named '%s' registered in DoxygenUtils" % args[0]
            raise RuntimeError(string)
        nargs = args[1:len(args)]
        return self.callbacks[args[0]](*nargs)
          

    def register_and_connect_emitters(self, app):
        app.connect("builder-inited", self.get_config_values)
        app.add_event("doxybridge-emitter")
        app.connect("doxybridge-emitter", self.service_emitter)


    def get_config_values(self, app):
        # All DirectiveContainers maintain references to the project info factory
        # so we can update this to update them
        self.project_info_factory.update(app.config.doxybridge_projects,
                                         app.config.doxybridge_default_project,
                                         )


    def standard_finder(self, entity_name, namespace, finder, options, kind):
        if entity_name in self.data_objects_cache:
            return self.data_objects_cache[entity_name]
        
        mf_compound = self.matcher_factory.create_name_matcher(namespace)
        mf_member = self.matcher_factory.create_name_matcher(entity_name)
        matcher_stack = self.matcher_factory.create_matcher_stack({"compound" : mf_compound,
                                                                   "member" : mf_member,
                                                                   },
                                                                  "member")
        try:
            data_object = None
            if (('type' in options) and (options['type'] == "macro")) or (kind == "macro"):
                data_objects = finder.find(matcher_stack)
                for do in data_objects:
                    if do.kind == "define":
                        data_object = do
                        break
            else:
                data_object = finder.find_one(matcher_stack)

            if not data_object:
                raise NoMatchesError

        except NoMatchesError, e:
            xml_name = "%s::%s" % (namespace, entity_name) if namespace else entity_name
            mf_compound = self.matcher_factory.create_name_matcher(xml_name)
            matcher_stack = self.matcher_factory.create_matcher_stack({"compound" : mf_compound},
                                                                      "compound")
            try:
                data_object = finder.find_one(matcher_stack)
            except NoMatchesError, e:
                raise NoMatchesError

        self.data_objects_cache[entity_name] = data_object

        return data_object


