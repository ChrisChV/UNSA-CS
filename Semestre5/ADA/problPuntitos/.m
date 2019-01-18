#!/usr/bin/octave -qf
x = [0,1,2,2,2,3,3,4,4,5,6,7,7,8,8,9,9,9,9,9];
y = [2,5,0,7,4,3,2,7,3,9,3,9,0,6,3,9,2,3,4,7];
plot(x,y,'*')
x = [2,0];
y = [0,2];
hold on
plot(x,y,'r')
x = [0,1];
y = [2,5];
hold on
plot(x,y,'r')
x = [1,2];
y = [5,7];
hold on
plot(x,y,'r')
x = [2,5];
y = [7,9];
hold on
plot(x,y,'r')
x = [5,7];
y = [9,9];
hold on
plot(x,y,'r')
x = [7,9];
y = [9,7];
hold on
plot(x,y,'r')
x = [9,9];
y = [7,2];
hold on
plot(x,y,'r')
x = [9,7];
y = [2,0];
hold on
plot(x,y,'r')
x = [7,2];
y = [0,0];
hold on
plot(x,y,'r')
pause()