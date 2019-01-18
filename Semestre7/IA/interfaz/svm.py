from cvxopt import matrix, solvers
import numpy as np
import matplotlib.pyplot as plt
import pickle, sys


def dotProduct(V1,V2):
	return sum([V1[i] * V2[i] for i in range(0,len(V1))])


def dotProductSVM(X):
	res = []
	for i in range(0,len(X)):
		for j in range(0,len(X)):
			res.append(dotProduct(X[i],X[j]))
	return np.asarray(res)

def productSVM(V,n):
	res = []
	for i in range(0,n):
		for j in range(0,n):
			res.append(V[i] * V[j])
	return np.asarray(res)

def graficar(points,w,b,S):
	x = [points[i][0] for i in range(0,len(points))]
	y = [points[i][1] for i in range(0,len(points))]
	tx = np.arange(0.0,24.5,0.1)
	ty = (-w[0] * tx - b) / w[1]
	tx1 = tx
	ty1 = (-w[0]/w[1]) * (tx - S[0][0]) + S[0][1]
	tx2 = tx
	ty2 = (-w[0]/w[1]) * (tx - S[len(S) - 1][0]) + S[len(S) - 1][1]
	plt.plot(x,y,'ro',tx,ty,tx1,ty1,tx2,ty2)
	plt.show()



def getAlfas(x, y): 
	NUM = x.shape[0]
	DIM = x.shape[1]

	K = y[:, None] * x #Inversa por x
	K = np.dot(K, K.T) #Producto Punto de las X
	P = matrix(K) 
	q = matrix(-np.ones((NUM, 1))) # matriz de -1 para hacer la sumatoria
	G = matrix(-np.eye(NUM)) # matriz identidad con -1 para volver negativos los alfa para voltear la inecuacion
	h = matrix(np.zeros(NUM)) # matriz de ceros para cumplir la inecuacion
	A = matrix(y.reshape(1, -1)) #inversa de y para la sumatoria
	b = matrix(np.zeros(1)) #matriz b con un cero par ala igualdad

	solvers.options['show_progress'] = False
	sol = solvers.qp(P, q, G, h, A, b)
	alphas = np.array(sol['x'])
	return alphas



def getW(alfa,y,x):
	return sum(np.array([alfa[i] * y[i] * x[i] for i in range (0,len(alfa))]))

def getS(alfa,x):
	return [x[i] for i in range(0,len(x)) if alfa[i] > 0.0001]

def getSY(alfa,x,y):
	return [y[i] for i in range(0,len(x)) if alfa[i] > 0.0001]

def getB(alfa,w,x,y):
	S = getS(alfa,x)
	Sy = getSY(alfa,x,y)
	b =  Sy - np.dot(S, w) 
	return b[0]

def getS(alfa,x):
	return [x[i] for i in range(0,len(x)) if alfa[i] > 0.0004]

def verifyPoint(x,y,alfa,b,point):
	return sum([alfa[i][0] * y[i] * dotProduct(point,x[i]) for i in range(0,len(x))])  + b

def concatPoints(red,blue):
	resX = []
	resY = []
	for e in red:
		resX.append([e[0],e[1]])
		resY.append(-1.0)
	for e in blue:
		resX.append([e[0],e[1]])
		resY.append(1.0)
	return np.array(resX),np.array(resY)

def SVM(x,y):
	alfa = getAlfas(x,y)
	w = getW(alfa,y,x)
	b = getB(alfa,w,x,y)
	S = getS(alfa,x)
	return alfa,w,b,S



