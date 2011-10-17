"""
    doxybridge.directive
    ~~~~~~~~~~~~~~~~~~~~
    Directive factory

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :license: MIT, see LICENSE for details.
"""

class DirectiveFactory(object):
    def __init__(self, domains, summaries):
        self.domains = domains
        self.summaries = summaries


    def create_summary_directive(self, sphinx_directive, project_info, names):
        try:
            summaryClass = self.summaries[project_info.domain()]
        except KeyError:
            this_env = sphinx_directive.state.document.settings.env
            error_string = "Unable to create a doxygen summary for '%s' domain" \
                           % project_info.domain()
            raise RuntimeError(error_string)

        try:
            summaryHandler = summaryClass.get_handler(sphinx_directive, project_info, names)
        except ValueError:
            raise ValueError
            
        return summaryHandler   

    
    def create_domain_directive(self, sphinx_directive, project_info):
        try:
            domainClass = self.domains[project_info.domain()]
        except KeyError:
            this_env = sphinx_directive.state.document.settings.env
            error_string = "Unable to create a doxygen directive for '%s' domain" \
                           % project_info.domain()
            raise RuntimeError(error_string)

        try:
            domainHandler = domainClass.get_handler(sphinx_directive, project_info)
        except ValueError:
            raise ValueError
            
        return domainHandler
