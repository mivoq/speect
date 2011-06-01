import speect

# create a new utterance
utt = speect.SUtterance()

# set utterance feature
utt.features["name"] = "hrg example"

# create a new "Token" relation
token_rel = utt.relation_new("Token")

# only deletes token_rel
del token_rel

# to delete "Token" relation
# utt.relation_del("Token")

# get "Token" relation
token_rel = utt.relation_get("Token")

# append an item to "Token" relation
# Note that append/prepend can take a shared contents, more later
item_t = token_rel.append()

# set the name feature of the item
item_t["name"] = "21"

# create a "Word" relation
word_rel = utt.relation_new("Word")

# and explode "21" token in "Word" relation
item_w1 = word_rel.append()
item_w1["name"] = "twenty"
item_w2 = word_rel.append()
item_w2["name"] = "one"

# item_w1 and item_w2 must be daughters of item_t
# these daughters share content with item_w1 and item_w2
item_t.add_daughter(item_w1)
item_t.add_daughter(item_w2)


print(utt)

#save utterance
import speect.utt_ebml

utt.save_ebml("tmp.utt")

del utt

#load utterance
lutt = speect.SUtterance.load_ebml("tmp.utt")

print(lutt)
