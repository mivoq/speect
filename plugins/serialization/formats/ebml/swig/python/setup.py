#!/usr/bin/env python
######################################################################################
##                                                                                  ##
## Setup SWIG/Python interface.                                                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

"""
setup.py file for SWIG Speect
"""

from distutils.core import setup, Extension

featproc_py_module = Extension('_featproc_py',
                               library_dirs=['/home/aby/Development/speect/engine/lib/'],
                               libraries=['spct'],
                               sources=['feat_processor_py_wrap.c'],
                               include_dirs=['.', '../../src', '/home/aby/Development/speect/engine/src']
                           )

setup (name = 'featproc_py',
       version = '0.1',
       author      = "HLT",
       description = """Swig Python interface for speect""",
       ext_modules = [featproc_py_module],
       py_modules = ["featproc_py"],
       )

