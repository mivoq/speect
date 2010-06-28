import speect
import speect.featproc_callback
import speect.pyobject
import cPickle as pickle


# Note that a voice that wants to use this feature processor with the 
# MorphDecomp utt processor needs to define the utt processor in the voice.txt
# file, under the list of utterance-processors:
#
#   "MorphDecomp" :
#   {
#	"class" : "SMorphDecompUttProc",
#	"plug-in" : "morphdecomp_uttproc.spi"
#   }
#



# item = word from Word relation
def morphdecomp(item):

    morphdict = item.voice().features["morphdict"]
    word = item["name"]
    morphlist = morphdict[word]

    morphnameList = []
    morphtypeList = []

    for morpheme in morphlist:
        morphname, morphtype = morpheme
        morphnameList.append(morphname)
        morphtypeList.append(morphtype)
        
    sMap = speect.SMap()
    sMap["morphemes"] = morphnameList
    sMap["classes"] = morphtypeList

    return sMap


morhpdecomp_feat_processor = speect.SFeatProcessor.callback(morphdecomp)

def add_to_voice(voice):
    with open("morphdict.pickle") as infh:
        d = pickle.load(infh)
        
    voice.features["morphdict"] = d

    voice.featProcessor_set("morphdecomp", morhpdecomp_feat_processor)
 
