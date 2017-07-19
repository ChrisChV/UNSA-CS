import cv2
import numpy as np
import os

def resize(file, h, w):
	tam = str(w) + "x" + str(h) + "!"
	os.system("convert -resize " + tam + " "  + " " + file + " " + file)

def resizeDir(direc, names, h, w):
	archivo = open(names,'r')
	for line in archivo:
		line = line.strip('\n')
		resize(direc + "/" + line, h, w)


def imgToArray(file):
	img = cv2.imread(file)
	res = np.ndarray(shape = (img.shape[0],img.shape[1]), dtype = int)
	for i in range(img.shape[0]):
		temp = [0] * img.shape[1]
		for j in range(img.shape[1]):
			temp[j] = img[i,j,0]
		res[i] = np.array(temp)
	return res

def dirToArray(direc, names, n, h, w):
	archivo = open(names, 'r')
	res = np.ndarray(shape = (n,h,w), dtype = int)
	i = 0;
	for line in archivo:
		line = line.strip('\n')
		res[i] = imgToArray(direc + "/" + line)
		i += 1
		print "Img->" + str(i)

	return res

def getClassFromImgs(classMap, names, fileOut): #Agarra la primera letra del nombre para ver en que clase esta
	arClassMap = open(classMap, 'r')
	clases = {}
	for line in arClassMap:
		temp = line.split(" ")
		clases[temp[1].strip('\n')] = int(temp[0])
	arClassMap.close()
	arNames = open(names, 'r')
	arFileOut = open(fileOut, 'w')
	for line in arNames:
		arFileOut.write(str(clases[line[0]]) + '\n')
	arNames.close()
	arFileOut.close()
	
def getClassFromFile(clases):
	lista = []
	archivo = open(clases, 'r')
	for line in archivo:
		lista.append(int(line.strip('\n')))
	return np.array(lista)
