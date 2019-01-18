function f = f3(x)
	f = 0;
	f1 = @(x) x.*x.*x + 1;
	f2 = @(x) x./(x.^2+1);
	if (x < 0)
		f = f1(x);
	else
		f = f2(x);
	endif
endfunction

function f4()
	x = linspace(-10,10,21);
	y = f3(x);
	plot(x,y,'r');
	grid on
endfunction
