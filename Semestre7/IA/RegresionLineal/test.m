#!/usr/bin/octave -qf
x = [1.000000,2.000000,3.000000,4.000000,5.000000];
y = [1.000000,2.000000,3.000000,4.000000,5.000000];
plot(x,y,'*')
fun = @(x) 0.0327544+ x *0.990928
tempX = linspace(0,5,100);
tempY = fun(tempX);
hold on;
plot(tempX,tempY)
pause();
