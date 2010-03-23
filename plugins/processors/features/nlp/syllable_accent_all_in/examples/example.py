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
## Example of the using a Python callback with the SFeatProcessrCB class.           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

from __future__ import print_function
import speect
import featproc_callback


def runme():

    # define a callback function. 
    def get_item_name(item):
        """
        A simple callback function to get the name of
        the item, mangle and return it as a new C Speect SObject
        instance.
        @param item: The item object.
        @type item: object (Python object)
        @return: Extracted name feature.
        @rtype: C Speect SObject
        """

        # get the item as a SItem object
        pyitem = speect.SItem(object=item, owner=False)

        # get the item's name as a Python unicode string
        name = pyitem.features["name"].get()

        # do something with the name
        name += "->from_python_callback"

        # create a new SString with the name
        new_string = speect.SString(name)

        # disown it so that it is not
        # deleted by Python
        new_string._disown()

        # return the C Speect SObject
        return new_string._get_speect_object()


    # create a new instance of the callback feature
    # processor with the above defined function as
    # the callback function.
    fcb = speect.SFeatProcessorCB(get_item_name)

    # create a new relation
    rel = speect.SRelation()

    # create a new item that is appended to the relation
    item = rel.append()

    # set the item's name
    item.features["name"] = speect.SString("iy")

    # run the feature processor on the item
    extracted_feature = fcb.run(item)

    print("extracted item feature: \'",extracted_feature.get(),"\'")



# main function
if __name__ == '__main__':
    runme()
    speect.py_speect_quit()
