function gref()
	%%fun = @(x) x - x.^(x-cos(x));
	%%fun = @(x) x - cos(sin(x));
	%%fun = @(x) x.^5 - 3 * x.^3 - 2 * x.^2 + 2 - x;
	fun = @(x) x.^2 - sin(x);
	%%fun = @(x) x.^3 - 1.265 .* x + 1;
	%%fun = @(x) x.^3 - 9 .* x.^2 + 27 .* x - 27;
	x = linspace(-3,3);
	y = fun(x);
	plot(x,y);
	grid on
end