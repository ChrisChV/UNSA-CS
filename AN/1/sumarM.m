function s = sumarM(A)
	s = 0;
	f = rows(A);
	c = columns(A);
	for i = 1:f
		for j = 1:c
			l = A(i,j);
			s = s + l;
		end
	end
endfunction
