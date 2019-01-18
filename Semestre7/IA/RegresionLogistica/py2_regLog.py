from random import randint
import matplotlib.pyplot as plt
import numpy as np
import math
from decimal import *

TAM_FIL_BD = 14

alfa = 0.001

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
	return [Decimal(e) for e in linea]

def readBd(fileName):
	file = open(fileName, "r")
	res = []
	for line in file:
		atributos = line.split(',')
		atributos[TAM_FIL_BD - 1] = atributos[TAM_FIL_BD - 1].rstrip()
		atributos = [Decimal(e) for e in atributos]
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
		O[i] = Decimal(randint(0,10))

def graficar(O, X, Y, num):
	tx = np.arange(Decimal(0.0),Decimal(10.0),Decimal(0.1))
	ty = (tx * O[1] + O[0]) / - O[2]
	fig = plt.figure()
	Xtemp = [X[i][1] for i in range (0,len(X))]
	Ytemp = [X[i][2] for i in range (0,len(X))]
	plt.plot(Xtemp,Ytemp,'ro',tx,ty)
	file = ''
	if num < 10:
		file = 'imagenes/img00' + str(num) + '.png'
	elif num < 100:
		file = 'imagenes/img0' + str(num) + '.png'
	else:
		file = 'imagenes/img' + str(num) + '.png'
	fig.savefig(file, dpi=fig.dpi)

def graficarTest(H, Y):
	iden = np.arange(-1.0,2.0,0.1)
	plt.plot(Y, H, 'ro', iden, iden)
	plt.show()

def prod(V1,V2):
	return sum([e[0] * e[1] for e in zip(V1,V2)])

def h(theta,X):
	return [prod(theta,e) for e in X]

def signoide(H):
	return [Decimal(1.0) / Decimal(Deciaml(1.0) + Decimal(math.exp(Decimal(-1.0) *e))) for e in H]

def calcularError(X,Y,theta):
	H = h(theta,X)
	S = signoide(H)
	suma = 0
	for i in range(1,len(Y)):
		temp1 = 0
		temp2 = 0
		if(S[i] != 0):
			temp1 = Decimal(math.log(S[i]))
		if(1 - S[i] != 0):
			temp2 = Decimal(math.log(1 - S[i]))
		suma += Decimal(Y[i] * temp1 + (1 - Y[i]) * temp2)
	return Decimal(suma) * Decimal((-1.0 / len(H)))


	#return sum([e[0] * Decimal(math.log(e[1])) + (1 - e[0]) * Decimal(math.log(1 - e[1])) for e in zip(Y,S)]) / Decimal((-1 / len(S)))


def change_theta(theta,X,Y):
	H = h(theta,X)
	S = signoide(H)
	var = [0] * len(theta)
	#for j in range(0, len(theta)):
	#	suma = 0
	#	for i in range(0, len(Y)):
	#		suma += (Y[i]-S[i]) * (X[i][j])
	#	var[j] = theta[j] -Decimal(0.1) * Decimal(suma / len(H))
	#return var

	for j in range(0,len(theta)):
		var[j] = Decimal(sum([Decimal(alfa) * (S[i] - Y[i]) * X[i][j] for i in range(0,len(Y))])) / Decimal(len(H))
	return [theta[i] - var[i] for i in range (0,len(theta))]


def regresion(X,Y,umbral):
	O = [0] * len(X[0])
	llenarRandom(O)
	#O = [1,1]
	error = calcularError(X, Y, O)
	tempError = error
	numGraf = 0
	#graficar(O, X, Y, numGraf);
	numGraf += 1
	print error
	while error > umbral and tempError >= error:
		tempError = error
		O = change_theta(O, X, Y)
		error = calcularError(X, Y, O)
		#graficar(O, X, Y, numGraf)
		numGraf += 1
		print error
	return O

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

def verifyTest(O,X,Y):
	#O = readTheta(thetaFile)
	H = h(O, X)
	S = signoide(H)
	print S

def SS(S):
	return [e > 0.5 for e in S]


def verifyTestbd(thetaFile,X,Y):
	O = readTheta(thetaFile)
	H = h(O, X)
	S = signoide(H)
	print S
	R = SS(S)
	unoB = 0;
	unoM = 0;
	ceroB = 0;
	ceroM = 0;
	for i in range(0,len(S)):
		if(Y[i] == Decimal(1.0)):
			if(R[i] == True):
				unoB += 1
			else: 
				unoM += 1
		elif(Y[i] == Decimal(0.0)):
			if(R[i] == False):
				ceroB += 1
			else:
				ceroM += 1
	print unoB
	print unoM
	print ceroB
	print ceroM



def generarRandom(size,thetaFile):
	file = open(thetaFile,'w')
	for i in range(0,size):
		if(i != size - 1):
			temp = str(Decimal(randint(0,10))) + ','
			file.write(temp)
		else:
			file.write(str(Decimal(randint(0,10))))
	file.close()

getcontext().prec = 20
#X = [[1,2],[1,3],[1,13],[1,50],[1,70],[1,69]]
#Y = [0,0,0,1,1,1]
#O = regresion(X,Y,0.1)
#print O
#XX = [[1,4],[1,10],[1,68],[1,71]]
#YY = [0,0,1,1]
#verifyTest(O, XX, YY)

#[Decimal('-5.1186457733092453315'), Decimal('0.18486358534335473531')]


#X = [[1,1,1],[1,2,3],[1,3,4],[1,5,3],[1,6,1],[1,7,5]];
#Y = [0,0,0,1,1,1]

X = [[1,0,0],[1,2,4],[1,4,6],[1,7,6],[1,8,4],[1,8,1],[1,3,1],[1,4,1],[1,4,3],[1,5,4],[1,6,1],[1,6,3]]
Y = [0,0,0,0,0,0,1,1,1,1,1,1]

#O = regresion(X,Y,1)
#print O
YY = [e[2] for e in X]

O = [Decimal('7.0205389449338242972'), Decimal('-1.4480109347076861973'), Decimal('0.28255538005316165002')]

graficar(O,X,YY,1)



#generarRandom(TAM_FIL_BD,"theta")

#bd = readBd("wineN.csv")

#normalize(bd,"wineN.csv")

#X = getX(bd)
#Y = getY(bd)
#regresionFile("theta",X,Y,500)

#verifyTestbd("theta",X,Y)



