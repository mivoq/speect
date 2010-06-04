import speect
import speect.uttproc_callback


def do_it(utt):
    rel = utt.relation_new("Token")
    item = rel.append()
    item["name"] = "first"
    item = rel.append()
    item["name"] = "second"


utt_processor = speect.SUttProcessor.callback(do_it)

v = speect.SVoice("/home/aby/Development/testing_voices/eng-ZA/voice.txt")
v.uttProcessor_set("Tokenize", utt_processor)


utt = v.synth("hello world", "text-to-segments" )



print utt
