import speect

utt = speect.SUtterance()

# array, (float and int)
import array_int
import array_float

tmp = [10 , 2 , 5]
ai = array_int.SArrayInt(tmp)
utt.features["ai"] = ai

tmp = [10.2 , 2.5 , 5.7]
af = array_float.SArrayFloat(tmp)
utt.features["af"] = af

# matrices, (float and int)
import matrix_int
import matrix_float

tmp = [[10 , 2 , 5], [4 , 8 , 3]]
mi = matrix_int.SMatrixInt(tmp)
utt.features["mi"] = mi

tmp = [[10.3 , 2.1 , 5.8], [4.5 , 8.7 , 3.2]]
mf = matrix_float.SMatrixFloat(tmp)
utt.features["mf"] = mf

# tracks, (float and int)
import track_int
import track_float

times = [0.1, 0.2, 0.3]

tmp = [[10 , 2 , 5], [4 , 8 , 3], [14 , 83 , 31]]
ti = track_int.STrackInt(times, tmp)
utt.features["ti"] = ti

tmp = [[10.3 , 2.1 , 5.8], [4.5 , 8.7 , 3.2], [14.0 , 83.11 , 31.453]]
tf = track_float.STrackFloat(times, tmp)
utt.features["tf"] = tf

# save utterance (we need the array, matrices and track ebml plugins)
p1 = speect.SPlugin("array-int-ebml.spi")
p2 = speect.SPlugin("array-float-ebml.spi")
p3 = speect.SPlugin("matrix-int-ebml.spi")
p4 = speect.SPlugin("matrix-float-ebml.spi")
p5 = speect.SPlugin("track-int-ebml.spi")
p6 = speect.SPlugin("track-float-ebml.spi")

# and the utterance ebml interface
import utt_ebml

utt.save_ebml("tmp.utt")

# and load it again

utt2 = speect.SUtterance.load_ebml("tmp.utt")

# get the float track
tf2 = utt2.features["tf"]

# get the python data
track = tf2.get()

