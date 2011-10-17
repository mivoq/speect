"""
    doxybridge.autosummary.c
    ~~~~~~~~~~~~~~~~~~~~~~~~
    Autosummary handler for c types

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :license: MIT, see LICENSE for details.
"""

import re

from doxybridge.finder import NoMatchesError, MultipleMatchesError
from doxybridge.directive.summary import DoxygenSummary
from doxybridge.domains.c import xml_extractor_re

from sphinx.domains.c import c_sig_re, c_funcptr_sig_re

xml_summary_extractor_re = re.compile("<.+?><.+?>(.+)</.+?></.+?>")
arg_parser_re = re.compile("(%*)(.+)")
pending_xref_parser_re = re.compile("(<pending_xref.+?reftype=\"ref\">)")

class CSummary(DoxygenSummary):
    def __init__(self, sphinx_directive, project_info, app, data_objects, names):
        super(CSummary, self).__init__(sphinx_directive,
                                       project_info,
                                       app,
                                       data_objects,
                                       names)
        self.domain = "c"


    @classmethod
    def get_handler(cls, sphinx_directive, project_info, names):
        app = sphinx_directive.state.document.settings.env.app
        finder = app.emit("doxybridge-emitter", "create_finder", project_info)[0]

        try:
            data_objects = cls._get_data_objects(app, finder, sphinx_directive, names)
        except NoMatchesError, e:
            raise ValueError

        objectHandler = CSummaryHandler(sphinx_directive, project_info, app, data_objects, names)
        return objectHandler


    @classmethod
    def _get_data_objects(cls, app, finder, sphinx_directive, names):
        """
        Find all the data objects to document in the xml file and return them.
        """

        namespace = ""
        object_list = []

        for entity_name in names:
            name_options = entity_name.split(' ')
            if len(name_options) > 1:
                entity_name = name_options[1]
                kind = name_options[0]
            else:
                kind = None

            try:
                data_object = app.emit("doxybridge-emitter", "standard_finder",
                                       entity_name,
                                       namespace,
                                       finder,
                                       sphinx_directive.options,
                                       kind)[0]
            except NoMatchesError:
                this_env = sphinx_directive.state.document.settings.env
                warn_string = "Unable to find definition '%s' in doxygen generated xml"
                this_env.warn(this_env.docname, warn_string % sphinx_directive.arguments[0],
                              sphinx_directive.lineno)
                raise NoMatchesError

            if kind:
                data_object.kind = kind

            ## if hasattr(data_object, "get_location"):
            ##     # add dependency
            ##     filename = data_object.get_location().get_file()
            ##     sphinx_directive.state.document.settings.env.note_dependency(filename)

            object_list.append(data_object)
                
        return object_list
    


class CSummaryHandler(CSummary):
    def __init__(self, sphinx_directive, project_info, app, data_objects, names):
        super(CSummaryHandler, self).__init__(sphinx_directive,
                                              project_info,
                                              app,
                                              data_objects,
                                              names)

    def get_display_name(self, name, renderer):
        type_name = name.split(' ')
        if len(type_name) > 1:
            return type_name[1]
        else:
            return name

        
    def get_real_name(self, name, renderer):
        type_name = name.split(' ')
        if len(type_name) > 1:
            return type_name[1]
        else:
            return name

    
    def get_signature(self, renderer):
        title = renderer.title()
        try:
            signature = self._get_signature_from_title(title)
        except ValueError:
            signature = ''

        sig = "(%s)" % self._format_signature(signature)
        return sig


    def _format_signature(self, signature):
        args = signature.strip().split(',')
        new_args = []
        for argument in args:
            new_arg = []
            this_arg = argument.split(' ')
            for arg in this_arg:
                new_arg.append(arg.replace("*", "%"))

            last_one = new_arg.pop()
            if arg_parser_re.match(last_one):
                match = arg_parser_re.findall(last_one)[0]
                new_last_one = "%s\\ *%s*\\" % (match[0], match[1])
            else:
                new_last_one = last_one

            new_arg.append(new_last_one)
            very_new_arg = []
            for arg in new_arg:
                very_new_arg.append(arg.replace("%", "\*"))

            combined = ' '.join(very_new_arg)
            new_args.append(combined)

        formatted_args = ','.join(new_args)
        return formatted_args
            
    def _get_signature_from_title(self, title):
        title_elements = []

        for i in title:
            if xml_extractor_re.match(str(i)):
                elements = xml_extractor_re.findall(str(i))
                title_elements.extend(elements)
            else:
                elements = str(i)
                title_elements.append(elements)
        
        signature = "".join(title_elements)

        # replace double spaces "  " with single space " "
        signature = re.sub("  ", " ", signature)

        # replace pointer space "* " with pointer "*"
        signature = re.sub("\* ", "*", signature)

        m = c_funcptr_sig_re.match(signature)
        if m is None:
            m = c_sig_re.match(signature)
        if m is None:
            raise ValueError('no match')
        rettype, name, arglist, const = m.groups()

        return arglist 
    

    def get_summary(self, renderer):
        summary = renderer.briefdescription()
        summary_elements = []

        for i in summary:
            i_string = str(i)
            #print i_string
            i_string = i_string.replace("<emphasis>", "*")
            i_string = i_string.replace("</emphasis>", "*")
            i_string = i_string.replace("<strong>", "**")
            i_string = i_string.replace("</strong>", "**")
            i_string = i_string.replace("<literal>", "``")
            i_string = i_string.replace("</literal>", "``")
            i_string = i_string.replace("</pending_xref>", "`")
            i_string = pending_xref_parser_re.sub(":c:type:`", i_string)
            
            if xml_summary_extractor_re.match(i_string):
                elements = xml_summary_extractor_re.findall(i_string)
                summary_elements.extend(elements)
            else:
                elements = i_string
                summary_elements.append(elements)

        clean_summary = "".join(summary_elements)
        #print clean_summary
        return clean_summary


    def get_qualifier(self, name):
        # qualifier for all c is c:type

        return "c:type"
