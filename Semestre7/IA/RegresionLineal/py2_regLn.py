from random import randint
import matplotlib.pyplot as plt
import numpy as np
import math
from decimal import *

TAM_FIL_BD = 14

halflife = 1400
alfa = 0.1

def normalize(bd,outFile):
	res = bd
	for i in range(0,len(bd[0])):
		temp = [bd[j][i] for j in range(0,len(bd))]
		numMin = min(temp)
		numMax = max(temp)
		norm = [(e - numMin) / (numMax - numMin) for e in temp]
		for j in range(0,len(bd)):
			res[j][i] = norm[j]
	file = open(outFile, "w")
	for lista in res:
		for i in range(0,len(lista)):
			if(i != len(lista) - 1):
				 temp = str(lista[i]) + ','
				 file.write(temp)
			else:
				file.write(str(lista[i]))
		file.write('\n')
	file.close()

def readTheta(thetaFile):
	file = open(thetaFile,"r")
	linea = file.readline()
	linea = linea.split(",")
	file.close()
	return [float(e) for e in linea]

def readBd(fileName):
	file = open(fileName, "r")
	res = []
	for line in file:
		atributos = line.split(',')
		#atributos[TAM_FIL_BD - 1] = atributos[TAM_FIL_BD - 1].rstrip()
		atributos = [float(e) for e in atributos]
		res.append(atributos)
	return res
	file.close()

def getX(bd):
	res = [[1] + fil for fil in bd]
	[fil.pop(TAM_FIL_BD) for fil in res]
	return res

def getY(bd):
	return [bd[i][TAM_FIL_BD-1] for i in range (0,len(bd))]

def llenarRandom(O):
	for i in range(0,len(O)):
		O[i] = randint(1,25);

def graficar(O, X, Y, num):
	tx = np.arange(0.0,5.0,0.1)
	ty = tx * O[1] + O[0]
	fig = plt.figure()
	Xtemp = [X[i][1] for i in range (0,len(X))]
	plt.plot(Xtemp,Y,'ro',tx,ty)
	file = ''
	if num < 10:
		file = 'imagenes/img00' + str(num) + '.png'
	elif num < 100:
		file = 'imagenes/img0' + str(num) + '.png'
	else:
		file = 'imagenes/img' + str(num) + '.png'
	fig.savefig(file, dpi=fig.dpi)

def graficarTest(H, Y):
	iden = np.arange(0.0,1.1,0.1)
	plt.plot(Y, H, 'ro', iden, iden)
	plt.show()

def prod(V1,V2):
	return sum([e[0] * e[1] for e in zip(V1,V2)])
	#return V1[0] * Decimal(V2[0]) + V1[1] * Decimal(V2[1]) + V1[2] * Decimal(math.pow(V2[1],2)) + V1[3] * Decimal(math.pow(V2[1],3))
	#suma = 0
	#for i in range(0,len(V1)):


def h(theta,X):
	return [prod(theta,e) for e in X]

def calcularError(X,Y,theta):
	H = h(theta,X)
	#thsum = sum([e**2 for e in theta if e != theta[0]])
	#thsum = thsum * halflife
	return sum([(e[0]-e[1])**2  for e in zip(Y,H)])  / (2.0 * len(H))


def change_theta(theta,X,Y):
	H = h(theta,X)
	var = [0] * len(theta)
	for j in range(0,len(X[0])):
		var[j]= sum([alfa * (Y[i]-H[i])*-1 * X[i][j] for i in range(0,len(Y))])  / len(H)
	return [theta[i] - var[i] for i in range (0,len(theta))]


def regresion(X,Y,umbral):
	O = [0] * len(X[0])
	#llenarRandom(O)
	O = [5,1]
	error = calcularError(X, Y, O)
	tempError = error + 1
	numGraf = 0;
	graficar(O, X, Y, numGraf);
	numGraf += 1
	while error > umbral and tempError != error:
		O = change_theta(O, X, Y)
		tempError = error
		error = calcularError(X, Y, O)
		print error
		input()
		graficar(O, X, Y, numGraf)
		numGraf += 1
		print error
	print O
	graficar(O, X, Y, 1)

def regresionFile(thetaFile,X,Y,iteraciones):
	O = readTheta(thetaFile)
	error = calcularError(X, Y, O)
	print error
	for i in range(0,iteraciones):
		O = change_theta(O, X, Y)
		error = calcularError(X, Y, O)
		print error
	file = open(thetaFile,"w")
	for i in range(0,len(O)):
		if(i != len(O) - 1):
			temp = str(O[i]) + ','
			file.write(temp)
		else:
			file.write(str(O[i]))
	file.close()

def verifyTest(thetaFile,X,Y):
	O = readTheta(thetaFile)
	H = h(O, X)
	graficarTest(H, Y)



# x[i][j] - min x[i][j] / max(x[i][j]) - min(x[i][j])
# h(x,theta)

X = [[1,3],[1,4],[1,5]]
Y = [3,4,5]
regresion(X,Y,0.00001)

#bd = readBd("wineApN.csv")
#X = getX(bd)
#Y = getY(bd)
#regresionFile("theta", X, Y, 1000)

#bd = readBd("wineTestN.csv")
#X = getX(bd)
#Y = getY(bd)
#verifyTest("theta",X,Y)


#normalize(bd,"wineN.csv")


#O = [0] * TAM_FIL_BD
#llenarRandom(O)
#print O

#getcontext().prec = 20
#X = [[1,1],[1,2],[1,3],[1,4],[1,4],[1,5],[1,6],[1,7],[1,7],[1,8],[1,9],[1,9],[1,10]]
#Y = [1,3,5,4,6,6,5,6,4,5,4,3,2]
#regresion(X, Y, 0.1)


#bd = readBd("bdhouse.csv")

#X = getX(bd)
#Y = getY(bd)
#regresionFile("theta",X,Y,5000)

#verifyTest("theta",X,Y)



