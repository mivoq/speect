import speect
import uttproc_callback


def do_it(utt):
    rel = utt.relation_new("Token")
    item = rel.append()
    item["name"] = "first"
    item = rel.append()
    item["name"] = "second"


utt_processor = speect.SUttProcessor.callback(do_it)

v = speect.SVoice("/home/aby/Data/Voices/English/eng-ZA/Lwazi/voice.txt")
v.uttProcessor_set("Tokenize", utt_processor)


utt = v.synth("text-to-segments", "hello world")



print utt
