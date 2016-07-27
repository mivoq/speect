"""
    doxybridge.domains.inheritance_diagram
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :license: MIT, see LICENSE for details.
"""

from docutils import nodes

class speect_inheritance_diagram(nodes.General, nodes.Element):
    """
    A docutils node to use as a placeholder for the Speect objects inheritance diagram.
    """
    pass


class SpeectInheritanceException(Exception):
    pass


class SpeectInheritanceGraph(object):
    # These are the default attrs for graphviz
    default_graph_attrs = {
        'rankdir': 'LR',
        'size': '"8.0, 12.0"',
    }
    default_node_attrs = {
        'shape': 'box',
        'fontsize': 10,
        'height': 0.25,
        'fontname': '"Vera Sans, DejaVu Sans, Liberation Sans, '
                    'Arial, Helvetica, sans"',
        'style': '"setlinewidth(0.5)"',
    }
    default_edge_attrs = {
        'arrowsize': 0.5,
        'style': '"setlinewidth(0.5)"',
    }
    
    def __init__(self, class_names):
        """
        *class_names* is a list of child classes to show bases from.
        """
        self.class_info = self._class_info(class_names)
        if not self.class_info:
            raise SpeectInheritanceException('No classes found for '
                                             'inheritance diagram')
 

    def _class_info(self, class_names):
        class_info = []
        num_inherit = len(class_names)
        inherit_cnt = 0
                
        for cls in class_names:
            if inherit_cnt == 0:
                class_info.append((cls, []))
            else:
                class_info.append((cls, [class_names[inherit_cnt-1]]))
            inherit_cnt += 1

        return class_info


    def get_all_class_names(self):
        """Get all of the class names involved in the graph."""
        return [name for (name, _) in self.class_info]


    def _format_node_attrs(self, attrs):
        return ','.join(['%s=%s' % x for x in attrs.items()])


    def _format_graph_attrs(self, attrs):
        return ''.join(['%s=%s;\n' % x for x in attrs.items()])


    def generate_dot(self, name, parts, urls={}, env=None):
#                     graph_attrs={}, node_attrs={}, edge_attrs={}):
        """Generate a graphviz dot graph from the classes that were passed in
        to __init__.

        *name* is the name of the graph.

        *urls* is a dictionary mapping class names to HTTP URLs.

        *graph_attrs*, *node_attrs*, *edge_attrs* are dictionaries containing
        key/value pairs to pass on as graphviz properties.
        """
        g_attrs = self.default_graph_attrs.copy()
        n_attrs = self.default_node_attrs.copy()
        e_attrs = self.default_edge_attrs.copy()
#        g_attrs.update(graph_attrs)
#        n_attrs.update(node_attrs)
#        e_attrs.update(edge_attrs)
        if env:
            if hasattr(env, "config"):
                if hasattr(env.config, "inheritance_graph_attrs"):
                    g_attrs.update(env.config.inheritance_graph_attrs)
                if hasattr(env.config, "inheritance_node_attrs"):
                    n_attrs.update(env.config.inheritance_node_attrs)
                if hasattr(env.config, "inheritance_edge_attrs"):
                    e_attrs.update(env.config.inheritance_edge_attrs)
        res = []
        res.append('digraph %s {\n' % name)
        res.append(self._format_graph_attrs(g_attrs))

        for name, bases in self.class_info:
            # Write the node
            this_node_attrs = n_attrs.copy()
            url = urls.get(name)
            if url is not None:
                this_node_attrs['URL'] = '"%s"' % url
            res.append('  "%s" [%s];\n' %
                       (name, self._format_node_attrs(this_node_attrs)))

            # Write the edges
            for base_name in bases:
                res.append('  "%s" -> "%s" [%s];\n' %
                           (base_name, name,
                            self._format_node_attrs(e_attrs)))
        res.append('}\n')
        return ''.join(res)


