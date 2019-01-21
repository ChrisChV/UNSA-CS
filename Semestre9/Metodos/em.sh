spin -a emDekker
cc -o pan pan.c
./pan

#spin -a faultyEM
#cc -o pan pan.c
#./pan

#spin -t faultyEM

#cc -DREACH -o pan pan.c
#./pan -i -m73

#cc -DBFS -o pan pan.c
#./pan
#spin -p -t mutex_flaw

