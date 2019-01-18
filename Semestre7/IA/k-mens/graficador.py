import numpy as np
import sys
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def graficar(file,k):
	archivo = open(file,'r')
	X = []
	for i in range(k):
		X.append([])
	Y = []
	for i in range(k):
		Y.append([])
	colores = ['r','b','g','y']
	for line in archivo:
		punto = line.split(' ')
		X[int(punto[2])].append(float(punto[0]))
		Y[int(punto[2])].append(float(punto[1]))
	archivo.close()
	for i in range(0,k):
		plt.plot(X[i],Y[i],colores[i] + 'o')
	plt.show()

def graficar3D(file , k):
	archivo = open(file,'r')
	X = []
	for i in range(k):
		X.append([])
	Y = []
	for i in range(k):
		Y.append([])
	Z = []
	for i in range(k):
		Z.append([])
	colores = ['r','b','g','y']
	for line in archivo:
		punto = line.split(' ')
		X[int(punto[3])].append(float(punto[0]))
		Y[int(punto[3])].append(float(punto[1]))
		Z[int(punto[3])].append(float(punto[2]))
	archivo.close()
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	for i in range(k):
		ax.scatter(X[i], Y[i], Z[i], c=colores[i], marker='o')
	ax.set_xlabel('X Label')
	ax.set_ylabel('Y Label')
	ax.set_zlabel('Z Label')
	plt.show()

if len(sys.argv) < 2:
	print "Faltan Argumentos <k>"
else:
	graficar3D("points.dat",int(sys.argv[1]))

