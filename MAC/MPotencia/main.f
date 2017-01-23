program potencias

	integer :: i = 0
	integer :: j = 0
	integer :: m = 2
	integer :: n = 2
	real(16), dimension(3,3) :: matrix
	real(16), dimension(3) :: ini

	matrix(1,1) = 3
	matrix(1,2) = -1
	matrix(1,3) = 0

	matrix(2,1) = -1
	matrix(2,2) = 2
	matrix(2,3) = -1

	matrix(3,1) = 0
	matrix(3,2) = -1
	matrix(3,3) = 3

	ini(1) = 1
	ini(2) = 1
	ini(3) = 1

	call Mpotencias(matrix,ini,3,10)

	

end program	potencias

subroutine Mpotencias(M,ini,N,iter)
	integer :: i = 0
	real(16), dimension(N,N) :: M
	real(16), dimension(N) :: v
	real(16), dimension(N) :: u
	real(16), dimension(N) :: ini
	real(16) :: dominante,temp
	v = ini
	do i = 1,iter
		call mul(M,v,N,u)
		call mayor(u,N,dominante)
		temp = 1.0 / dominante
		call mulnum(u,temp,N,v)
		call printMatriz(u,N,1)
		write(*,*)
		call printMatriz(v,N,1)
		write(*,*)
	end do

return
end subroutine Mpotencias

subroutine mayor(M,N,res)
	real(16), dimension(N) :: M
	real(16) :: res,temp
	integer :: i
	res = -1
	do i = 1,N
		temp = abs(M(i))
		if((res == -1) .or. (res < temp)) then
			res = temp
		end if
	end do
return
end subroutine mayor

subroutine printMatriz(M,f,c)
	integer :: i,j
	integer :: f,c
	real(16), dimension(f,c) :: M
	do i=1,f
    	do j=1,c
        	write(*,'(F10.6,$)') M(i,j)
    	end do
    	write(*,*)
	end do
return
end subroutine printMatriz


subroutine mul(A,B,N,res)
	integer :: N,i,j,k
	real(16), dimension(N,N) :: A
	real(16), dimension(N) :: B
	real(16), dimension(N) :: res
	real(16) :: sum

	do i=1,N
		sum = 0
		do j=1,N
			sum = sum + B(j) * A(i,j)
		end do
		res(i) = sum
	end do
return
end subroutine mul

subroutine mulnum(A,x,N,res)
	integer :: N,i
	real(16) :: x
	real(16), dimension(N) :: A,res
	do i=1,N
		res(i) = A(i) * x
	end do
return
end subroutine mulnum
