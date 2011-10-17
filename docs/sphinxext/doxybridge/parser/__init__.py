"""
    doxybridge.parser
    ~~~~~~~~~~~~~~~~~

    :copyright: Copyright 2011, Aby Louw
    :license: BSD, see LICENSE for details.

    :copyright: Copyright (c) 2009, Michael Jones
    :license: BSD, see LICENSE for details.
"""

import doxybridge.parser.doxygen.index
import doxybridge.parser.doxygen.compound
import os

class ParserError(Exception):
    pass

class Parser(object):

    pass


class DoxygenIndexParser(Parser):

    def parse(self, project_info):

        filename = os.path.join(project_info.path(), "index.xml")
        try:
            return doxybridge.parser.doxygen.index.parse(filename)
        except doxybridge.parser.doxygen.index.ParseError:
            raise ParserError(filename)


class DoxygenCompoundParser(Parser):

    def __init__(self, project_info):

        self.project_info = project_info

    def parse(self, refid):

        filename = os.path.join(self.project_info.path(), "%s.xml" % refid)

        try:
            return doxybridge.parser.doxygen.compound.parse(filename)
        except doxybridge.parser.doxygen.compound.ParseError:
            raise ParserError(filename)

class DoxygenParserFactory(object):

    def __init__(self):

        pass

    def create_compound_parser(self, project_info):

        return DoxygenCompoundParser(project_info)



