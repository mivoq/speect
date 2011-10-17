"""
    doxybridge.project
    ~~~~~~~~~~~~~~~~~~

    :copyright: Copyright (c) 2011 The Department of Arts and Culture, The Government
                of the Republic of South Africa.
    :copyright: Copyright (c) 2009, Michael Jones
    :license: MIT, see LICENSE for details.
"""

import sys

class ProjectInfo(object):

    def __init__(self, name, path, reference, domain, match):

        self._name = name
        self._path = path
        self._reference = reference
        self._domain = domain
        self._match = match

    def name(self):
        return self._name

    def path(self):
        return self._path

    def reference(self):
        return self._reference

    def domain(self):
        return self._domain


class ProjectInfoFactory(object):

    def __init__(self, match):

        self.match = match
        self.projects = {}
        self.default_project = None
        self.project_count = 0
        self.project_info_store = {}
        

    def update(
            self,
            projects,
            default_project,
            ):

        self.projects = projects
        self.default_project = default_project


    def default_path(self):

        return self.projects[self.default_project][0]

    def default_domain(self):

        return self.projects[self.default_project][1]

    def create_project_info(self, options):

        name = ""
        path = self.default_path()
        domain = self.default_domain()

        if options.has_key("project"):
            try:
                path = self.projects[ options["project"] ][0]
                name = options["project"]
                domain = self.projects[ options["project"] ][1]
            except KeyError, e:
                sys.stderr.write(
                    "Unable to find project '%s' in doxygen_projects dictionary" \
                    % options["project"]
                    )

        if options.has_key("path"):
            path = options["path"]

        if options.has_key("domain"):
            domain = options["domain"]


        try:
            return self.project_info_store[path]
        except KeyError:

            reference = name

            if not name:
                name = "project%s" % self.project_count
                reference = path
                self.project_count += 1

            project_info = ProjectInfo(
                name,
                path,
                reference,
                domain,
                self.match
                )

            self.project_info_store[path] = project_info

            return project_info
