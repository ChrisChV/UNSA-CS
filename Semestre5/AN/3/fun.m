function f()
	x= linspace(1-2*10^-8,1+2*10^-8,450);
	%x = linspace(-1,1,100);
	y =  x.^ 7 - 7 * x.^ 6 + 21 * x.^ 5 - 35 * x.^ 4 + 35 * x.^ 3 - 21 * x.^ 2 + 7 * x - 1;
	plot(y);
	grid on
end