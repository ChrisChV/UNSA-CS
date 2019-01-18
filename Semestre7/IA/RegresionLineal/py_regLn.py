from random import randint
from math import *
from decimal import *
import matplotlib.pyplot as plt
import numpy as np

def multiply(A,B):
	res = [0] * len(B[0])
	for i in range(0,1):
		for j in range(0,len(B[0])):
			for k in range(0,len(A)):
				res[j] += A[k] * B[k][j];
	return res

def llenarRandom(O):
	for i in range(0,len(O)):
		O[i] = randint(0,10);

def calcularError(O, X, Y):
	h = multiply(O,X)
	suma = 0
	for i in range(0,len(h)):
		suma += pow(Y[i] - h[i],2)
	return Decimal(suma) / Decimal(2 * len(h))

def derivada(O, X, Y, index):
	h = multiply(O,X)
	suma = 0
	for i in range(0,len(h)):
		if index == 0:
			suma += (Y[i] - h[i]) * -1
		else:
			suma += (Y[i] - h[i]) * (X[index][i] * -1)
	return Decimal(suma) / Decimal(len(h))


def graficar(O, X, Y, num):
	tx = np.arange(Decimal(0.0),Decimal(5.0),Decimal(0.1))
	ty = tx * O[1] + O[0]
	fig = plt.figure()
	plt.plot(X[1],Y,'ro',tx,ty)
	file = ''
	if num < 10:
		file = 'imagenes/img00' + str(num) + '.png'
	elif num < 100:
		file = 'imagenes/img0' + str(num) + '.png'
	else:
		file = 'imagenes/img' + str(num) + '.png'
	fig.savefig(file, dpi=fig.dpi)


def regresion(X, Y, umbral):
	O = [0] * len(X)
	llenarRandom(O)
	error = calcularError(O, X, Y)
	numGraf = 0;
	graficar(O, X, Y,numGraf)
	numGraf += 1
	while Decimal(error) > Decimal(umbral):
		temp = [0] * len(O)
		temp[0] = O[0] - Decimal(0.1) * derivada(O, X, Y, 0)
		for i in range(0,len(O)):
			temp[i] = O[i] - Decimal(0.1) * derivada(O, X, Y, i)
		O = temp
		error = calcularError(O, X, Y)
		graficar(O, X, Y,numGraf)
		numGraf += 1
		print numGraf



getcontext().prec = 20
X = [[1,1,1,1,1],[1,2,3,4,5]]
Y = [1,2,3,4,5]
regresion(X,Y,Decimal(0.00001))







