from random import randint
import matplotlib.pyplot as plt
import numpy as np
import math
import mpmath
from decimal import *

TAM_FIL_BD = 14

alfa = 0.001
half_life = 0.0
centro = []

def copyFile(thetaFile):
	fileR = open(thetaFile,'r')
	fileW = open(thetaFile + ".copy",'w')
	linea = fileR.readline()
	fileW.write(linea)
	fileR.close()
	fileW.close()

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

def getThetaSize(thetaFile):
	file = open(thetaFile,"r")
	linea = file.readline()
	linea = linea.split(",")
	file.close()
	return len([Decimal(e) for e in linea])

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
		O[i] = Decimal(randint(1,10))

def getCurvaNivel(X,Y,theta):
	res = [0] * 2;
	resX = []
	resY = []
	for i in range(0, len(X)):
		print i
		for j in range(0, len(Y)):
			#temp = float(theta[0]) + float(theta[1]) * float(X[i]) + float(theta[2]) * float(Y[j]) + float(theta[3]) * float(X[i])**2 + float(theta[4]) * float(Y[j]) **2
			exp1 = 2
			exp2 = 0
			temp = 0
			for k in range(0,len(theta)):
				if(k == 0):
					temp += float(theta[0])
				elif(k == 1):
					temp += float(theta[1]) * float(X[i])
				elif(k == 2):
					temp += float(theta[2]) * float(Y[j])
				else:
					temp += float(theta[k]) * float(X[i]) ** exp1 * float(X[j]) ** exp2
					exp2 += 1
					if(exp2 > exp1):
						exp2 = 0
						exp1 += 1
			if(int(temp) == 0):
				resX.append(X[i])
				resY.append(Y[j])
	res[0] = resX
	res[1] = resY
	return res

def getAngles(X,Y):
	res = [0] * len(X)
	for i in range (1,len(X)):
		if(X[i] - X[0] != 0):
			res[i] = math.atan((Y[i] - Y[0]) / (X[i] - X[0]))
	points = zip(X,Y,res)
	points.sort(key=lambda tub:tub[2])
	resX = [e[0] for e in points]
	resY = [e[1] for e in points]
	resres = [0] * 2
	resres[0] = resX
	resres[1] = resY
	return resres



def graficar(O, red, blue, num):
	tx = np.arange(Decimal(0.0),Decimal(20.0),Decimal(0.1))

	ty = tx
	res = getCurvaNivel(tx,ty,O)

	fig = plt.figure()
	redXtemp = [red[i][0] for i in range (0,len(red))]
	redYtemp = [red[i][1] for i in range (0,len(red))]
	blueXtemp = [blue[i][0] for i in range (0,len(blue))]
	blueYtemp = [blue[i][1] for i in range (0,len(blue))]

	plt.plot(res[0],res[1],'y.',redXtemp,redYtemp,'ro',blueXtemp,blueYtemp,'bo')

	if(num > -1):
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
	#return sum([V1[i] * Decimal(math.pow(V2[i],i)) for i in range(0,len(V1))])
	#return V1[0] * V2[0] + V1[1] * V2[1] * V2[1] + V1[2] * V2[2]
	#return V1[1] * V2[1] * V2[1] + V1[2] * V2[2] * V2[2] - V1[0]
	#return V1[0] - Decimal(pow(V1[1] * V2[1] + V1[2],2)) - Decimal(pow(V1[3] * V2[2] + V1[4]))
	#return V1[1] * (V2[1] - centro[0])**2 + V1[2] * (V2[2] - centro[1])**2- V1[0]
	#return V1[0] + V1[1] * V2[1] + V1[2] * V[2] + V1[3] * V2[1]**2 + V1[4] + V2[2]**2



def h(theta,X):
	return [prod(theta,e) for e in X]

def signoide(H):
	return [Decimal(1.0) / (Decimal(1.0) + Decimal(mpmath.exp(Decimal(-1.0) *Decimal(e)))) for e in H]

def calcularError(X,Y,theta):
	H = h(theta,X)
	S = signoide(H)
	#print S
	suma = 0
	for i in range(1,len(Y)):
		temp1 = 0
		temp2 = 0
		if(S[i] != 0):
			temp1 = Decimal(math.log(S[i]))
		if(1 - S[i] != 0):
			temp2 = Decimal(math.log(1 - S[i]))
		suma += Decimal(Y[i] * temp1 + (1 - Y[i]) * temp2)
	return Decimal(suma) * Decimal(-1.0) / Decimal(len(H))  + sum([Decimal(e ** 2) for e in theta if e != theta[0]]) * Decimal(half_life) / (Decimal(2.0) * Decimal(len(H)))

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
		var[j] = Decimal(sum([Decimal(alfa) * Decimal(S[i] - Y[i]) * Decimal(X[i][j]) for i in range(0,len(Y))])) / Decimal(len(H))
	#return [theta[i] - var[i] + (Decimal(alfa) * Decimal(half_life) / Decimal(len(H))) * Decimal(theta[i]) for i in range (0,len(theta))]
	theta[0] = theta[0] - var[0]
	for i in range(0,len(theta)):
		theta[i] = theta[i] - var[i] + (Decimal(alfa) * Decimal(half_life) / Decimal(len(H))) * Decimal(theta[i])
	return theta


def regresion(X,Y,umbral):
	O = [0] * len(X[0])
	llenarRandom(O)
	#O = [Decimal('4.26142207147552718926726902021'), Decimal('0.108757829329852110247291447472'), Decimal('-1.36800995809979309388658939651')]
	error = calcularError(X, Y, O)
	tempError = error
	numGraf = 0
	#graficar(O, X, Y, numGraf);
	numGraf += 1
	print O
	print error
	while error > umbral: #  and tempError >= error:
		tempError = error
		O = change_theta(O, X, Y)
		error = calcularError(X, Y, O)
		#graficar(O, X, Y, numGraf)
		numGraf += 1
		print error
	return O

def regresionFile(thetaFile,X,Y,iteraciones):
	copyFile(thetaFile)
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

def hallarCentro(X):
	sumX = sum([e[1] for e in X])
	sumY = sum([e[2] for e in X])
	return [Decimal(sumX)/Decimal(len(X)),Decimal(sumY) / Decimal(len(X))]

def generarRandom(size,thetaFile):
	file = open(thetaFile,'w')
	for i in range(0,size):
		if(i != size - 1):
			temp = str(Decimal(randint(0,10))) + ','
			file.write(temp)
		else:
			file.write(str(Decimal(randint(0,10))))
	file.close()


def aumentarCuadrados(X):
	res = [0] * (len(X))
	for i in range(0,len(X)):
		res[i] = [0] * 5
	for i in range(0,len(X)):
		for j in range(0,len(X[0])):
			res[i][j] = X[i][j]
			if(j != 0):
				res[i][j + 2] = X[i][j]**2
	return res

def generarPoli(X,n):
	res = [0] * (len(X))
	for i in range(0,len(X)):
		res[i] = [0] * n
	exp1 = 2
	exp2 = 0
	for i in range(0,len(X)):
		exp1 = 2
		exp2 = 0
		for j in range(0,n):
			if(j < 3):
				res[i][j] = X[i][j]
			else:
				res[i][j] = X[i][1]**exp1 * X[i][2] ** exp2
				exp2 += 1
				if(exp2 > exp1):
					exp2 = 0
					exp1 += 1
	return res




def unirPuntos(red,blue,n):
	res = [0] * 2
	resX = []
	resY = []
	for i in range(0,len(red)):
		resX.append([Decimal(1.0),Decimal(red[i][0]),Decimal(red[i][1])])
		resY.append(0)
	for i in range(0,len(blue)):
		resX.append([Decimal(1.0),Decimal(blue[i][0]),Decimal(blue[i][1])])
		resY.append(1)
	resX = generarPoli(resX,n)
	res[0] = resX
	res[1] = resY
	return res


def savePoints(saveFile,red,blue):
	file = open(saveFile,'w')
	for i in range(0,len(red)):
		if(i != len(red) - 1):
			temp = str(red[i][0]) + ','
			file.write(temp)
		else:
			temp = str(red[i][0]) + '\n'
			file.write(temp)
	for i in range(0,len(red)):
		if(i != len(red) - 1):
			temp = str(red[i][1]) + ','
			file.write(temp)
		else:
			temp = str(red[i][1]) + '\n'
			file.write(temp)
	for i in range(0,len(blue)):
		if(i != len(blue) - 1):
			temp = str(blue[i][0]) + ','
			file.write(temp)
		else:
			temp = str(blue[i][0]) + '\n'
			file.write(temp)
	for i in range(0,len(blue)):
		if(i != len(blue) - 1):
			temp = str(blue[i][1]) + ','
			file.write(temp)
		else:
			temp = str(blue[i][1]) + '\n'
			file.write(temp)
	file.close()

def loadPoints(loadFile):
	res = [0] * 2
	file = open(loadFile,'r')
	redX = file.readline().split(',')
	redY = file.readline().split(',')
	blueX = file.readline().split(',')
	blueY = file.readline().split(',')
	resRed = []
	resBlue = []
	for i in range(0,len(redX)):
		if(i == len(redX) - 1):
			redX[i] = redX[i].rstrip()
			redY[i] = redY[i].rstrip()
		resRed.append((float(redX[i]),float(redY[i])))
	for i in range(0,len(blueX)):
		if(i == len(blueX) - 1):
			blueX[i] = blueX[i].rstrip()
			blueY[i] = blueY[i].rstrip()
		resBlue.append((float(blueX[i]),float(blueY[i])))
	res[0] = resRed
	res[1] = resBlue
	return res

getcontext().prec = 15

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

#X = [[1,0,0],[1,2,4],[1,4,6],[1,7,6],[1,8,4],[1,8,1],[1,3,1],[1,4,1],[1,4,3],[1,5,4],[1,6,1],[1,6,3]]
#Y = [0,0,0,0,0,0,1,1,1,1,1,1]

#X = [[1,2,4],[1,3,8],[1,10,13],    [1,6,1],[1,7,6],[1,8,4]]
#Y = [0,0,0,1,1,1]


#X = [[1,2,2],[1,4,5],[1,3,9],[1,4,15],[1,9,2],[1,13,2],[1,11,16],[1,14,16],[1,15,4],[1,18,2],[1,20,6],[1,18,9],[1,18,13],       [1,5,10],[1,7,6],[1,10,4],[1,9,9],[1,8,13],[1,11,14],[1,12,6],[1,12,9],[1,14,12],[1,15,9]]
#Y = [0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1]
#X = aumentarCuadrados(X)

#generarRandom(5,"theta")


#regresionFile("theta",X,Y,3000)




#O = [Decimal('3.82697014101882716474514909095'), Decimal('0.193071176573820887684151064551'), Decimal('6.21202479023867507775558820628')]
#O = [-50,1,1,1,1]


#YY = [e[2] for e in X]
#O = readTheta("theta")
#graficar(O,X,YY,2)




#generarRandom(TAM_FIL_BD,"theta")

#bd = readBd("wineN.csv")

#normalize(bd,"wineN.csv")

#X = getX(bd)
#Y = getY(bd)
#regresionFile("theta",X,Y,500)

#verifyTestbd("theta",X,Y)



