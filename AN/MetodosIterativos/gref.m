function gref()
	%%fun = @(x) x - x.^(x-cos(x));
	%%fun = @(x) x - cos(sin(x));
	fun = @(x) x.^5 - 3 * x.^3 - 2 * x.^2 + 2 - x;
	x = linspace(-2,2);
	y = fun(x);
	plot(x,y);
	grid on
end