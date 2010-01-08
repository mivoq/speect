from speect import *

if __name__ == "__main__":
    utt = SUtterance()
    rel = SRelation()
    utt.relations["Word"] = rel
    utt.features["name"] = SString("test utt")
    item = rel.append()
    item_daughter = item.add_daughter()
    item_daughter.features["score"] = SFloat(-3.14)
