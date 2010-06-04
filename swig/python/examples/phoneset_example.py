import speect
import speect.phoneset

# load voice
voice = speect.SVoice("/home/aby/Development/testing_voices/eng-ZA/voice.txt")

# create a new utterance with the voice
utt = speect.SUtterance(voice)

# set utterance feature
utt.features["name"] = "hrg example"

# create a new "Segment" relation
segment_rel = utt.relation_new("Segment")

# add an item (pause = "#") for this voice
item = segment_rel.append()
item["name"] = "#"

item = segment_rel.append()
item["name"] = "a"


# now we can get the voice from the item
v = item.voice()

# get the phoneset from the voice
phoneset = v.data_get("phoneset")

# get the vowel feature of item from phoneset
print phoneset.phone_has_feature("a", "vowel")


