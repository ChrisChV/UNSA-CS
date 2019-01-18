#!/usr/bin/octave -qf
x = [2,4,19,21,22,30,33,38,39,44,53,58,65,69,70,71,74,74,81,92];
y = [88,56,27,50,13,47,0,53,68,83,74,23,93,39,86,47,32,45,66,68];
plot(x,y,'*')
x = [33,22];
y = [0,13];
hold on
plot(x,y,'r')
x = [22,4];
y = [13,56];
hold on
plot(x,y,'r')
x = [4,19];
y = [56,27];
hold on
plot(x,y,'r')
x = [19,2];
y = [27,88];
hold on
plot(x,y,'r')
x = [2,65];
y = [88,93];
hold on
plot(x,y,'r')
x = [65,92];
y = [93,68];
hold on
plot(x,y,'r')
x = [92,74];
y = [68,32];
hold on
plot(x,y,'r')
x = [74,33];
y = [32,0];
hold on
plot(x,y,'r')
pause()