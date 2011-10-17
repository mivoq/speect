"""
    doxybridge.domains.c
    ~~~~~~~~~~~~~~~~~~~~

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :license: MIT, see LICENSE for details.
"""

import re
import string


from docutils import nodes


from sphinx.domains.c import CObject, c_sig_re, c_funcptr_sig_re, c_funcptr_name_re, wsplit_re
from sphinx.locale import l_, _
from sphinx import addnodes

from doxybridge.directive.std import DoxygenDirective
from doxybridge.finder import NoMatchesError, MultipleMatchesError
from doxybridge.domains.inheritance_diagram import speect_inheritance_diagram, SpeectInheritanceException, SpeectInheritanceGraph

xml_extractor_re = re.compile("<.+>(.+)</.+>")

class CDomain(DoxygenDirective):

    # These C types aren't described anywhere, so don't try to create
    # a cross-reference to them
    stopwords = set(('const', 'void', 'char', 'int', 'long', 'FILE', 'struct'))

    
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CDomain, self).__init__(sphinx_directive,
                                      project_info,
                                      app,
                                      data_object)
        self.domain = "c"


    @classmethod
    def get_handler(cls, sphinx_directive, project_info):
        app = sphinx_directive.state.document.settings.env.app
        finder = app.emit("doxybridge-emitter", "create_finder", project_info)[0]

        try:
            data_object = cls._get_data_object(app, finder, sphinx_directive)
        except NoMatchesError, e:
            raise ValueError

        objtype = data_object.get_kind()

        # check if an explicit type was given
        if 'type' in sphinx_directive.options:
            objtype = data_object.kind = sphinx_directive.options['type']
            
        try:
            objectClass = C_OBJECT_TYPES[objtype]
            objectHandler = objectClass(sphinx_directive, project_info, app, data_object)
        except KeyError:
            objectClass = CDomain
            objectHandler = objectClass(sphinx_directive, project_info, app, data_object)
            objectHandler.objtype = objtype
            this_env = sphinx_directive.state.document.settings.env
            warn_string = ("Unable to create a c type handler for object type '%s', " +
                           "using default doxybridge c domain directive")
            this_env.warn(this_env.docname, warn_string % objtype, sphinx_directive.lineno)

        return objectHandler
        

    @classmethod
    def _get_data_object(cls, app, finder, sphinx_directive):
        """
        Find the data object to document in the xml file and return it.
        """

        try:
            (namespace, entity_name) = sphinx_directive.arguments[0].rsplit("::", 1)
        except ValueError:
            (namespace, entity_name) = "", sphinx_directive.arguments[0]

        try:
            data_object = app.emit("doxybridge-emitter", "standard_finder",
                                   entity_name,
                                   namespace,
                                   finder,
                                   sphinx_directive.options,
                                   None)[0]
        except NoMatchesError, e:
            this_env = sphinx_directive.state.document.settings.env
            warn_string = "Unable to find definition '%s' in doxygen generated xml"
            this_env.warn(this_env.docname, warn_string % sphinx_directive.arguments[0],
                          sphinx_directive.lineno)
            raise NoMatchesError

        ## if hasattr(data_object, "get_location"):
        ##      # add dependency
        ##      filename = data_object.get_location().get_file()
        ##      sphinx_directive.state.document.settings.env.note_dependency(filename)

        return data_object

    
    def get_signature_from_title(self, title):
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

        return unicode(signature)


    def add_cross_ref(self, node, ctype):
        # add cross-ref nodes for all words
        for part in filter(None, wsplit_re.split(ctype)):
            tnode = nodes.Text(part, part)
            if part[0] in string.ascii_letters+'_' and \
                   part not in self.stopwords:
                pnode = addnodes.pending_xref(
                    '', refdomain='c', reftype='type', reftarget=part,
                    modname=None, classname=None)
                pnode += tnode
                node += pnode
            else:
                node += tnode


    def add_return_type(self, node, ctype):
        # emphasize the 'return type'
        # used for macros, defines, structs, enums, typedefs and
        # so on where 'return type' is actually the type name
        for part in filter(None, wsplit_re.split(ctype)):
            tnode = nodes.emphasis(part, part)
            node += tnode


    def create_target(self, name):
        if not self.noindex and name not in self.sphinx_directive.names:
            # only add target and index entry if this is the first
            # description of the object with this name in this desc block
            self.sphinx_directive.names.append(name)
            if name not in self.sphinx_directive.state.document.ids:
                signode = addnodes.desc_signature()
                signode['names'].append(name)
                signode['ids'].append(name)
                signode['first'] = (not self.sphinx_directive.names)
                self.sphinx_directive.state.document.note_explicit_target(signode)
                inv = self.sphinx_directive.env.domaindata['c']['objects']
                if name in inv:
                    self.sphinx_directive.env.warn(
                        self.sphinx_directive.env.docname,
                        'duplicate C object description of %s, ' % name +
                        'other instance in ' + self.sphinx_directive.env.doc2path(inv[name][0]),
                        self.sphinx_directive.lineno)
                inv[name] = (self.sphinx_directive.env.docname, self.sphinx_directive.objtype)
            else:
                signode = []

            indextext = self.get_index_text(name)
            if indextext:
                self.sphinx_directive.indexnode['entries'].append(('single', indextext, name, ''))

            return signode
        
        return []


    def parse_title(self, title):
        signature = self.get_signature_from_title(title)
        m = c_funcptr_sig_re.match(signature)
        if m is None:
            m = c_sig_re.match(signature)
        if m is None:
            raise ValueError('no match')
        rettype, name, arglist, const = m.groups()

        return rettype, name, arglist, const, signature


    def get_full_name(self, name):
        typename = self.sphinx_directive.env.temp_data.get('c:type')
        if self.objtype == 'struct' and typename:
            fullname = typename + '.' + name
        else:
            fullname = name
   
        return fullname


    def add_sig_name(self, signode, name):
        try:
            classname, funcname = name.split('::', 1)
            classname += '::'
            signode += addnodes.desc_addname(classname, classname)
            signode += addnodes.desc_name(funcname, funcname)
            # name (the full name) is still both parts
        except ValueError:
            signode += addnodes.desc_name(name, name)

        # clean up parentheses from canonical name
        m = c_funcptr_name_re.match(name)
        if m:
            name = m.group(1)

        return self.get_full_name(name)


    def add_sig_args(self, signode, arglist, const):
        if not arglist:
            if self.sphinx_directive.objtype == 'function':
                # for functions, add an empty parameter list
                signode += addnodes.desc_parameterlist()
            if const:
                signode += addnodes.desc_addname(const, const)
            return 

        paramlist = addnodes.desc_parameterlist()
        arglist = arglist.replace('`', '').replace('\\ ', '') # remove markup
        # this messes up function pointer types, but not too badly ;)
        args = arglist.split(',')
        for arg in args:
            arg = arg.strip()
            param = addnodes.desc_parameter('', '', noemph=True)
            try:
                ctype, argname = arg.rsplit(' ', 1)
            except ValueError:
                # no argument name given, only the type
                self.add_cross_ref(param, arg)
            else:
                self.add_cross_ref(param, ctype)
                # separate by non-breaking space in the output
                param += nodes.emphasis(' '+argname, u'\xa0'+argname)
            paramlist += param
        signode += paramlist
        if const:
            signode += addnodes.desc_addname(const, const)


    def get_index_text(self, name):
        return _('%s (C %s)') % (name, self.objtype)



class CVariable(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CVariable, self).__init__(sphinx_directive,
                                        project_info,
                                        app,
                                        data_object)
        self.objtype = "variable"


    def get_index_text(self, name):
        return _('%s (C variable)') % name


    def add_return_type(self, *args):
        self.add_cross_ref(*args)



class CTypedef(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CTypedef, self).__init__(sphinx_directive,
                                        project_info,
                                        app,
                                        data_object)
        self.objtype = "typedef"


    def get_index_text(self, name):
        return _('%s (C type)') % name


    def add_return_type(self, node, ctype):
        # emphasize the 'return type'
        # used for macros, defines, structs, enums, typedefs and
        # so on where 'return type' is actually the type name
        parts = wsplit_re.split(ctype)
        node += nodes.emphasis(parts[0], parts[0])
        parts = parts[1:len(parts)]
        for part in parts:
            tnode = nodes.Text(part, part)
            node += tnode


 
class CFunction(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CFunction, self).__init__(sphinx_directive,
                                        project_info,
                                        app,
                                        data_object)
        self.objtype = "function"


    def get_index_text(self, name):
        return _('%s (C function)') % name


    def add_return_type(self, *args):
        # use add_cross_ref, no emphasis on return type
        self.add_cross_ref(*args)


 

class CEnum(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CEnum, self).__init__(sphinx_directive,
                                        project_info,
                                        app,
                                        data_object)
        self.objtype = "enum"


    def get_index_text(self, name):
        return _('%s (C enum)') % name



class CTypedefEnum(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CTypedefEnum, self).__init__(sphinx_directive,
                                           project_info,
                                           app,
                                           data_object)
        self.objtype = "enum"


    def get_index_text(self, name):
        return _('%s (C enum)') % name


class CTypedefFunctionPointer(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CTypedefFunctionPointer, self).__init__(sphinx_directive,
                                                      project_info,
                                                      app,
                                                      data_object)
        self.objtype = "typedef function pointer"


    def get_index_text(self, name):
        return _('%s (C function pointer)') % name

    def add_return_type(self, node, ctype):
        # emphasize the 'return type'
        # used for macros, defines, structs, enums, typedefs and
        # so on where 'return type' is actually the type name
        parts = wsplit_re.split(ctype)
        node += nodes.emphasis(parts[0], parts[0])
        parts = parts[1:len(parts)]
        for part in parts:
            tnode = nodes.Text(part, part)
            node += tnode

    def create_target(self, name):
        # clean up parentheses from canonical name
        m = c_funcptr_name_re.match(name)
        if m:
            name = m.group(1)
            
        return super(CTypedefFunctionPointer, self).create_target(name)

 
class CMacro(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CMacro, self).__init__(sphinx_directive,
                                     project_info,
                                     app,
                                     data_object)
        self.objtype = "macro"


    def get_index_text(self, name):
        return _('%s (C macro)') % name



class CDefine(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CDefine, self).__init__(sphinx_directive,
                                     project_info,
                                     app,
                                     data_object)
        self.objtype = "define"


    def get_index_text(self, name):
        return _('%s (C define)') % name


class CDefineHidden(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CDefineHidden, self).__init__(sphinx_directive,
                                            project_info,
                                            app,
                                            data_object)
        self.objtype = "define hidden"


    def get_index_text(self, name):
        return _('%s (C define)') % name


class CSpeectTypes(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CSpeectTypes, self).__init__(sphinx_directive,
                                            project_info,
                                            app,
                                            data_object)

    def add_return_type(self, *args):
        if hasattr(self.sphinx_directive, "typename_set"):
            if self.sphinx_directive.typename_set:
                self.add_cross_ref(*args)
            else:
                super(CSpeectTypes, self).add_return_type(*args)
        else:
            super(CSpeectTypes, self).add_return_type(*args)


    def set_name(self, name):
        self.name = name


    def get_name(self):
        return self.name


    def add_sig_name(self, signode, name):
        try:
            classname, funcname = name.split('::', 1)
            classname += '::'
            signode += addnodes.desc_addname(classname, classname)
            signode += addnodes.desc_name(funcname, funcname)
            # name (the full name) is still both parts
        except ValueError:
            signode += addnodes.desc_name(name, name)

        if "inheritance" in self.sphinx_directive.options:
            inheritance_node = addnodes.desc_content('')
            self.add_inheritance_diagram(inheritance_node)
            signode += inheritance_node

        # clean up parentheses from canonical name
        m = c_funcptr_name_re.match(name)
        if m:
            name = m.group(1)

        return self.get_full_name(name)


    def add_inheritance_diagram(self, signode):
        node = speect_inheritance_diagram()
        node.document = self.sphinx_directive.state.document
        env = self.sphinx_directive.state.document.settings.env

        class_role = env.get_domain('c').role('type')
        # Store the original content for use as a hash
        node['parts'] = self.sphinx_directive.options.get('parts', 0)
        node['content'] = self.sphinx_directive.options["inheritance"].strip()

        # Create a graph starting with the list of classes
        try:
            graph = SpeectInheritanceGraph(self.sphinx_directive.options["inheritance"].strip().split("."))
        except SpeectInheritanceException, err:
            return [node.document.reporter.warning(err.args[0],
                                                   line=self.lineno)]
            
        # Create xref nodes for each target of the graph's image map and
        # add them to the doc tree so that Sphinx can resolve the
        # references to real URLs later.  These nodes will eventually be
        # removed from the doctree after we're done with them.
        for name in graph.get_all_class_names():
            refnodes, x = class_role(
                'type', ':type:`%s`' % name, name, 0, self.sphinx_directive.state)
            node.extend(refnodes)
        # Store the graph object so we can use it to generate the
        # dot file later
        node['graph'] = graph
        signode += node


    
class CSpeectObject(CSpeectTypes):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CSpeectObject, self).__init__(sphinx_directive,
                                            project_info,
                                            app,
                                            data_object)
        self.objtype = "speectobject"


    def get_index_text(self, name):
        if name != self.data_object.name:
            if hasattr(self.sphinx_directive, "typemethod_set"):
                if self.sphinx_directive.typemethod_set:
                    return _('%s (C %s method)') % (name, self.data_object.name)

            return _('%s (C %s member)') % (name, self.data_object.name)
        else:
            return _('%s (C Speect object)') % name


    def before_content(self):
        # set the type for members that my be documented
        self.sphinx_directive.env.temp_data['c:type'] = self.get_name()
        self.sphinx_directive.typename_set = True
        self.no_section_titles = True


    def after_content(self):
        self.sphinx_directive.env.temp_data['c:type'] = None
        self.sphinx_directive.typename_set = False
        self.no_section_titles = False



class CSpeectClass(CSpeectTypes):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CSpeectClass, self).__init__(sphinx_directive,
                                           project_info,
                                           app,
                                           data_object)
        self.objtype = "speectclass"


    def get_index_text(self, name):
        if name != self.data_object.name:
            if hasattr(self.sphinx_directive, "typemethod_set"):
                if self.sphinx_directive.typemethod_set:
                    return _('%s (C %s method)') % (name, self.data_object.name)

            return _('%s (C %s member)') % (name, self.data_object.name)
        else:
            return _('%s (C Speect class)') % name




    def before_content(self):
        # set the type for members that my be documented
        self.sphinx_directive.env.temp_data['c:type'] = self.get_name()
        self.sphinx_directive.typename_set = True
        #self.no_section_titles = True


    def after_content(self):
        self.sphinx_directive.env.temp_data['c:type'] = None
        self.sphinx_directive.typename_set = False
        #self.no_section_titles = False



class CStruct(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CStruct, self).__init__(sphinx_directive,
                                      project_info,
                                      app,
                                      data_object)
        self.objtype = "struct"


    def get_index_text(self, name):
        if name != self.data_object.name:
            if hasattr(self.sphinx_directive, "typemethod_set"):
                if self.sphinx_directive.typemethod_set:
                    return _('%s (C %s method)') % (name, self.data_object.name)

            return _('%s (C %s member)') % (name, self.data_object.name)
        else:
            return _('%s (C struct)') % name


    def add_return_type(self, *args):
        if hasattr(self.sphinx_directive, "typename_set"):
            if self.sphinx_directive.typename_set:
                self.add_cross_ref(*args)
            else:
                super(CStruct, self).add_return_type(*args)
        else:
            super(CStruct, self).add_return_type(*args)


    def set_name(self, name):
        self.name = name


    def get_name(self):
        return self.name


    def before_content(self):
        # set the type for members that my be documented
        self.sphinx_directive.env.temp_data['c:type'] = self.get_name()
        self.sphinx_directive.typename_set = True


    def after_content(self):
        self.sphinx_directive.env.temp_data['c:type'] = None
        self.sphinx_directive.typename_set = False



class CTypedefStruct(CDomain):
    def __init__(self, sphinx_directive, project_info, app, data_object):
        super(CTypedefStruct, self).__init__(sphinx_directive,
                                             project_info,
                                             app,
                                             data_object)
        self.objtype = "struct"


    def get_index_text(self, name):
        if name != self.data_object.name:
            if hasattr(self.sphinx_directive, "typemethod_set"):
                if self.sphinx_directive.typemethod_set:
                    return _('%s (C %s method)') % (name, self.data_object.name)

            return _('%s (C %s member)') % (name, self.data_object.name)
        else:
            return _('%s (C struct)') % name


    def add_return_type(self, *args):
        if hasattr(self.sphinx_directive, "typename_set"):
            if self.sphinx_directive.typename_set:
                self.add_cross_ref(*args)
            else:
                super(CTypedefStruct, self).add_return_type(*args)
        else:
            super(CTypedefStruct, self).add_return_type(*args)


    def set_name(self, name):
        self.name = name


    def get_name(self):
        return self.name


    def before_content(self):
        # set the type for members that my be documented
        self.sphinx_directive.env.temp_data['c:type'] = self.get_name()
        self.sphinx_directive.typename_set = True


    def after_content(self):
        self.sphinx_directive.env.temp_data['c:type'] = None
        self.sphinx_directive.typename_set = False



C_OBJECT_TYPES = {
    "function" : CFunction,
    "typedef" : CTypedef,
    "struct" : CStruct,
    "typedef struct" : CTypedefStruct,
    "variable" : CVariable,
    "enum" : CEnum,
    "typedef enum" : CTypedefEnum,
    "typedef function pointer" : CTypedefFunctionPointer,
    "macro" : CMacro,
    "define hidden" : CDefineHidden,
    "define" : CDefine,
    "speect object" : CSpeectObject,
    "speect class" : CSpeectClass
    }
   
