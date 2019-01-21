function i = integral(f,a,b,n)
	ancho = abs(a-b)/n;
	i = 0;
	contador = a + ancho/2;
	while(contador<b)
		i = i + (ancho*f(contador));
		contador+=ancho;
	end
end

function prob(f,u,n,a,b)
	A = [];
	F = [];
	for i=1:n
		for j=1:n
			temp = @(x) f(x,i) .* f(x,j);
			A(i,j) = integral(temp,0,1,1000);
		end
	end

	for i=1:n
		temp = @(x) f(x,i) .* u(x);
		F(i,1) = integral(temp,0,1,1000);
	end

	C = inv(A) * F;
	C
	U = 0;
	x = linspace(a,b);
	yu = u(x);
	for i=1:n
		if(i == 1)
			U = @(x) C(i) .* f(x,i);
		else
			U = @(x) U(x) + (C(i) .* f(x,i));
			plot(x,yu,'r')
			hold on
			yU = U(x);
			plot(x,yU,'g')
			legend('Solución Exacta','Solución Aproximada')
			disp('Gráfica Número')
			disp(i)
			continuar =input('PRESIONE CUALQUIER TECLA PARA CONTINUAR');
			hold off
		endif
	end
end

function main()
	n = input('Ingrese el valor de n');
	a = input('Ingrese el limite inferior');
	b = input('Ingrese el limite superior');
	f = @(x,n) x.^n .* (1-x);
	u = @(x) sin(x)/sin(1) - x;
	prob(f,u,n,a,b);
end