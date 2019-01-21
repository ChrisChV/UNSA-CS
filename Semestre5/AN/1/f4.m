

function f4()
	f1 = @(x) x.*x.*x + 1;
	f2 = @(x) x./(x.^2+1);
	x = linspace(-10,10,21);
	
endfunction
