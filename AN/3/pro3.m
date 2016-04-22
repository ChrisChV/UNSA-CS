function terminos(n)
	res = [];
	err = [];
	for i = 2:n
		if(i == 2)
		 res(i - 1) = 2;
		else
			res(i -1) = sum(res) +  (2^(i-1/2)) * sqrt(1- sqrt(1- (4^(1-i)) * (res(i-2)^2)));
		end
		err(i - 1) = abs((pi - res(i-1)) / res(i-1));
	end
	res
	%%[res' err']
end