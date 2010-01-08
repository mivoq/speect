######################################################################################
## Copyright (c) 2009 The Department of Arts and Culture,                           ##
## The Government of the Republic of South Africa.                                  ##
##                                                                                  ##
## Contributors:  Meraka Institute, CSIR, South Africa.                             ##
##                                                                                  ##
## Permission is hereby granted, free of charge, to any person obtaining a copy     ##
## of this software and associated documentation files (the "Software"), to deal    ##
## in the Software without restriction, including without limitation the rights     ##
## to use, copy, modify, merge, publish, distribute, sublicense, and#or sell        ##
## copies of the Software, and to permit persons to whom the Software is            ##
## furnished to do so, subject to the following conditions:                         ##
## The above copyright notice and this permission notice shall be included in       ##
## all copies or substantial portions of the Software.                              ##
##                                                                                  ##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       ##
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         ##
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      ##
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           ##
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    ##
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        ##
## THE SOFTWARE.                                                                    ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : December 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Python functions for loading plug-ins.                                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{
def load_plugin(path):
    """
    Load a plug-in from the given path. If the plug-in at the given
    path has already been loaded, then the plug-in's reference counter
    is increased. This reduces the need for multiple redundant calls.
    If the given path does not include any path separators (just a file name)
    then the path is concatenated with the default plug-in path.
    @param path: The full path and name of the plug-in to load.
    @type path: str
    @return: The plug-in object as a L{SObject}.
    @rtype: L{SObject}
    @note: If the retured plug-in L{SObject} goes out of scope, then the
    plug-in will be unloaded. Call the L{SObject._disown} method to keep
    the plug-in loaded.
    """

    if not isinstance(path, str):
        raise RuntimeError('Argument \'path\' must be of type \'str\'')

    plugin_object = py_load_plugin(path)
    if not plugin_object:
        string = 'Speect failed to load plug-in at path \'' +\
                 path + '\''
        raise RuntimeError(string)

    return SObject(object=plugin_object, owner=True)
%}
