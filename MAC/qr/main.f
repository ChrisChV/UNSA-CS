program qr
	real, dimension(3,2) :: matrix
	matrix(1,1) = 1
	matrix(1,2) = 2

	matrix(2,1) = 3
	matrix(2,2) = 4

	matrix(3,1) = 5
	matrix(3,2) = 6

	print *, NORM2(matrix)

end program qr