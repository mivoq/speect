"""
    doxybridge.renderer.rst.doxygen
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :copyright: Copyright (c) 2009, Michael Jones           
    :license: MIT, see LICENSE for details. 
"""

from doxybridge.renderer.rst.doxygen.base import Renderer
from doxybridge.renderer.rst.doxygen import index as indexrenderer
from doxybridge.renderer.rst.doxygen import compound as compoundrenderer

from doxybridge.parser.doxygen import index, compound, compoundsuper

class RstContentCreator(object):

    def __init__(self, list_type, dedent):

        self.list_type = list_type
        self.dedent = dedent

    def __call__(self, text):

        # Remove the first line
        text = "\n".join(text.split(u"\n")[1:])

        # Remove starting whitespace
        text = self.dedent(text)

        # Inspired by autodoc.py in Sphinx
        result = self.list_type()
        for line in text.split("\n"):
            result.append(line, "<doxybridge>")

        return result

class UnicodeRenderer(Renderer):

    def render(self):

        # Skip any nodes that are pure whitespace
        # Probably need a better way to do this as currently we're only doing
        # it skip whitespace between higher-level nodes, but this will also
        # skip any pure whitespace entries in actual content nodes
        if self.data_object.strip():
            return [self.node_factory.Text(self.data_object)]
        else:
            return []

class NullRenderer(Renderer):

    def __init__(self):
        pass

    def render(self):
        return []

class DoxygenToRstRendererFactory(object):

    def __init__(
            self,
            node_type,
            type_,
            renderers,
            renderer_factory_creator,
            node_factory,
            project_info,
            state,
            document,
            rst_content_creator,
            filter_
            ):

        self.node_type = node_type
        self.type_ = type_
        self.node_factory = node_factory
        self.project_info = project_info
        self.renderers = renderers
        self.renderer_factory_creator = renderer_factory_creator
        self.state = state
        self.document = document
        self.rst_content_creator = rst_content_creator
        self.filter_ = filter_


    def create_renderer(self,
                        parent_data_object,
                        data_object,
                        directive
                        ):

        if not self.filter_.allow(parent_data_object, data_object):
            return NullRenderer()

        child_renderer_factory = self.renderer_factory_creator.create_child_factory(data_object, self)
        
        try:
            node_type = data_object.node_type
        except AttributeError, e:
            
            # Horrible hack to silence errors on filtering unicode objects
            # until we fix the parsing
            if type(data_object) == unicode:
                node_type = "unicode"
            else:
                raise e

        Renderer = self.renderers[node_type]

        if node_type == "docmarkup":

            creator = self.node_factory.inline
            if data_object.type_ == "emphasis":
                creator = self.node_factory.emphasis
            elif data_object.type_ == "computeroutput":
                creator = self.node_factory.literal
            elif data_object.type_ == "ulink":
                Renderer = compoundrenderer.URLMarkupTypeSubRenderer
                return Renderer(self.project_info,
                                data_object,
                                child_renderer_factory,
                                self.node_factory,
                                self.state,
                                self.document,
                                directive
                                )
            elif data_object.type_ == "bold":
                creator = self.node_factory.strong
            elif data_object.type_ == "superscript":
                creator = self.node_factory.superscript
            elif data_object.type_ == "subscript":
                creator = self.node_factory.subscript
            elif data_object.type_ == "center":
                print "Warning: does not currently handle 'center' text display"
            elif data_object.type_ == "small":
                print "Warning: does not currently handle 'small' text display"

            return Renderer(creator,
                            self.project_info,
                            data_object,
                            child_renderer_factory,
                            self.node_factory,
                            self.state,
                            self.document,
                            directive
                            )

        if node_type == "verbatim":

            return Renderer(self.rst_content_creator,
                            self.project_info,
                            data_object,
                            child_renderer_factory,
                            self.node_factory,
                            self.state,
                            self.document,
                            directive
                            )

        if node_type == "memberdef":

            if data_object.kind == "function":
                Renderer = compoundrenderer.FuncMemberDefTypeSubRenderer
            elif data_object.kind == "enum":
                Renderer = compoundrenderer.EnumMemberDefTypeSubRenderer
            elif data_object.kind == "typedef enum":
                Renderer = compoundrenderer.TypedefEnumMemberDefTypeSubRenderer
            elif data_object.kind == "typedef":
                Renderer = compoundrenderer.TypedefMemberDefTypeSubRenderer
            elif data_object.kind == "typedef function pointer":
                Renderer = compoundrenderer.TypedefFunctionPointerMemberDefTypeSubRenderer
            elif data_object.kind == "variable":
                Renderer = compoundrenderer.VariableMemberDefTypeSubRenderer
            elif data_object.kind == "macro":
                Renderer = compoundrenderer.MacroMemberDefTypeSubRenderer
            elif data_object.kind == "define":
                Renderer = compoundrenderer.DefineMemberDefTypeSubRenderer
            elif data_object.kind == "define hidden":
                Renderer = compoundrenderer.DefineHiddenMemberDefTypeSubRenderer

        if node_type == "docsimplesect":
            if data_object.kind == "par":
                Renderer = compoundrenderer.ParDocSimpleSectTypeSubRenderer

        return Renderer(self.project_info,
                        data_object,
                        child_renderer_factory,
                        self.node_factory,
                        self.state,
                        self.document,
                        directive
                        )


class CreateCompoundTypeSubRenderer(object):

    def __init__(self, parser_factory):

        self.parser_factory = parser_factory

    def __call__(self, project_info, *args):

        compound_parser = self.parser_factory.create_compound_parser(project_info)
        return indexrenderer.CompoundTypeSubRenderer(compound_parser, project_info, *args)


class CreateRefTypeSubRenderer(object):

    def __init__(self, parser_factory):

        self.parser_factory = parser_factory

    def __call__(self, project_info, *args):
        compound_parser = self.parser_factory.create_compound_parser(project_info)
        return compoundrenderer.RefTypeSubRenderer(compound_parser, project_info, *args)


class DoxygenToRstRendererFactoryCreator(object):

    def __init__(
            self,
            node_factory,
            parser_factory,
            rst_content_creator
            ):

        self.node_factory = node_factory
        self.parser_factory = parser_factory
        self.rst_content_creator = rst_content_creator

    def create_factory(self, project_info, state, document, filter_):

        renderers = {
            "doxygen" : indexrenderer.DoxygenTypeSubRenderer,
            "compound" : CreateCompoundTypeSubRenderer(self.parser_factory),
            "doxygendef" : compoundrenderer.DoxygenTypeSubRenderer,
            "compounddef" : compoundrenderer.CompoundDefTypeSubRenderer,
            "sectiondef" : compoundrenderer.SectionDefTypeSubRenderer,
            "memberdef" : compoundrenderer.MemberDefTypeSubRenderer,
            "enumvalue" : compoundrenderer.EnumvalueTypeSubRenderer,
            "linkedtext" : compoundrenderer.LinkedTextTypeSubRenderer,
            "description" : compoundrenderer.DescriptionTypeSubRenderer,
            "param" : compoundrenderer.ParamTypeSubRenderer,
            "docreftext" : compoundrenderer.DocRefTextTypeSubRenderer,
            "docpara" : compoundrenderer.DocParaTypeSubRenderer,
            "docmarkup" : compoundrenderer.DocMarkupTypeSubRenderer,
            "docparamlist" : compoundrenderer.DocParamListTypeSubRenderer,
            "docparamlistitem" : compoundrenderer.DocParamListItemSubRenderer,
            "docparamnamelist" : compoundrenderer.DocParamNameListSubRenderer,
            "docparamname" : compoundrenderer.DocParamNameSubRenderer,
            "docsect1" : compoundrenderer.DocSect1TypeSubRenderer,
            "docsimplesect" : compoundrenderer.DocSimpleSectTypeSubRenderer,
            "doctitle" : compoundrenderer.DocTitleTypeSubRenderer,
            "ref" : CreateRefTypeSubRenderer(self.parser_factory),
            "verbatim" : compoundrenderer.VerbatimTypeSubRenderer,
            "mixedcontainer" : compoundrenderer.MixedContainerRenderer,
            "unicode" : UnicodeRenderer,
            }

        return DoxygenToRstRendererFactory(
                "root",
                "basic",
                renderers,
                self,
                self.node_factory,
                project_info,
                state,
                document,
                self.rst_content_creator,
                filter_
                )

    def create_child_factory( self, data_object, parent_renderer_factory ):
        type_ = "basic"
        renderer_factory_class = DoxygenToRstRendererFactory

        try:
            node_type = data_object.node_type
        except AttributeError, e:

            # Horrible hack to silence errors on filtering unicode objects
            # until we fix the parsing
            if type(data_object) == unicode:
                node_type = "unicode"
            else:
                raise e

        return renderer_factory_class(node_type,
                                      type_,
                                      parent_renderer_factory.renderers,
                                      self,
                                      self.node_factory,
                                      parent_renderer_factory.project_info,
                                      parent_renderer_factory.state,
                                      parent_renderer_factory.document,
                                      self.rst_content_creator,
                                      parent_renderer_factory.filter_
                                      )

