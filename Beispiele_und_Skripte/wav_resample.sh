#!/bin/bash
#in bash eingetippt resamplet alle .wav nach 44100 im working directory und benennt sie, wie die ursprungsdatei; kann auch leerzeichen
#find . -type f -print0 | xargs -0 -n 1 ../../../Beispiele_und_Skripte/wav2mp3.sh

export TERM=$1
export OUT=`echo $TERM | sed 's/.wav$/2.wav/'`

#export OUT=`echo $TERM | sed 's/wav$/mp3/'`
sox "$TERM" "$OUT" rate 44100

mv "$OUT" "$TERM"


