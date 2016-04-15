function f1(i,f)
	n = i:f;
	x = 10 .^ (n);
	y = (1 + (1 ./ n)) .^ n;
	format long g
	[x' y']
end

function f2(i,f)
	n = i:f;
	x = 10 .^ (n);
	y = ((2 .* (n.^3)) + (3.*n) - 1)./((3 .* (n.^3)) + (2.*(n.^2)) - 1);
	format long g
	[x' y']
end

function s = f3(n)
	s = 0;
	for i = 1:n
		s = s + (1/i);
	end
	%16.69531136;
	%18.99789641;
end

function s = f4(n)
	s = 0;
	for i = 0:n
		s = s + (1/factorial(i));
	end
	%
end

function dtob(d)
	b = [];
	index = 1;
	while(d != 0)
		b(index) = mod(d,2);
		index++;
		d = floor(d/2);
	end
	b = fliplr(b);
	c = columns(b);
	for i = 1:c
		printf('%i',b(i));
	end
	printf('\n');
end

function b = dtobf(d)
	b = 0;
	index = -1;
	if(d > 1 || d < 0)
		printf('Numero Incorrecto')
	else
		while(d != 0)
			temp = d * 2;
			pEntera = floor(temp); 
			pFrac = temp - pEntera;
			b = b + (pEntera *( 10 ^ index));
			index--;
			d = pFrac;
		end
	end
end