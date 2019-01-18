function dibujar()
	f = @(x) sin(x);
	limXIzq = -5;
	limXDer = 5;
	limYArr = 1;
	limYAbj = -1;
	amplitud = 1000	;
	x = linspace(limXIzq,limXDer,amplitud);
	y = f(x);
	for i = 1:amplitud
		y = f(x(1:i));	
		plot(x(1:i),y(1:i))
		xlim([limXIzq limXDer]);
		ylim([limYAbj limYArr]);	
		pause(0.0000000001)
	end
end