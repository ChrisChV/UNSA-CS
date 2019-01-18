fileA = open("iris.dat",'r')
fileB = open("irisA.dat",'w')
for line in fileA:
	n = line.split(',')
	for i in range(0,4):
		fileB.write(str(n[i]))
		if(i != 3):
			fileB.write(' ')
		
			
fileA.close()
fileB.close()