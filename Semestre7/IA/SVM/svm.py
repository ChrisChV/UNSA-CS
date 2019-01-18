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
	tx = np.arange(0.0,24.5	,0.1)
	ty = (-w[0] * tx - b) / w[1]
	tx1 = tx
	ty1 = (-w[0]/w[1]) * (tx - S[0][0]) + S[0][1]
	tx2 = tx
	ty2 = (-w[0]/w[1]) * (tx - S[len(S) - 1][0]) + S[len(S) - 1][1]
	plt.plot(x,y,'ro',tx,ty,tx1,ty1,tx2,ty2)
	plt.show()

def fit(x, y): 
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

#x = np.array([[1,2,3,4,5],[5,4,3,2,1]]);
#y = np.array([2,2]);

#alfa = Variable(2)


#obj = Minimize(sum_entries(x - y * alfa))
#cons = [alfa < 0, sum_entries(y * alfa) == 0]
#p = Problem(obj,cons)

#print p.solve()
#print alfa.value




x = np.array([[]])
y = np.array([-1., -1., -1. ,-1. ,-1., -1.,  1. , 1. , 1. , 1. , 1. , 1. , 1. , 1. , 1.])

#x = np.array([[1.0,2.0],[3.0,4.0]])
#y = np.array([-1.0,1.0])


alfa = fit(x,y)

w = getW(alfa,y,x)
b = getB(alfa,w,x,y)
S = getS(alfa,x)
graficar(x,w,b,S)


#alfa = Variable()
#print dotProductSVM(x)
#print productSVM(y)

#obj = Maximize(alfa * alfa)
#cons = [alfa >= 0]
#p = Problem(obj,cons)

#p.solve()

