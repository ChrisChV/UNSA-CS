def normalize(bd,outFile):
	res = bd
	for i in range(0,len(bd[0]) - 1):
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
				 temp = str(lista[i]) + ' '
				 file.write(temp)
			else:
				file.write(str(lista[i]))
		file.write('\n')
	file.close()


def readBd(fileName):
	file = open(fileName, "r")
	res = []
	for line in file:
		atributos = line.split(' ')
		#atributos[TAM_FIL_BD - 1] = atributos[TAM_FIL_BD - 1].rstrip()
		atributos = [float(e) for e in atributos]
		res.append(atributos)
	return res
	file.close()

bd = readBd("TotalTest.dat")
normalize(bd,"TotalTestN.dat")

