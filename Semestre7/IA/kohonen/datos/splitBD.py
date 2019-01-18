from random import randint

def splitBD(bdFile, porcentaje1, porcentaje2, res1, res2):
	count = 0;
	file = open(bdFile, "r")
	for line in file:
		count += 1;
	file.close();
	file = open(bdFile, "r")
	resfile1 = open(res1, "w")
	resfile2 = open(res2, "w")
	num1 = porcentaje1 * count / 100
	num2 = porcentaje2 * count / 100
	count1 = 0
	count2 = 0
	temp = 0
	for line in file:
		temp = randint(0,1)
		if(temp == 0 and count1 != num1):
			count1 += 1
			resfile1.write(line)
		elif(temp == 1 and count2 != num2):
			count2 += 1
			resfile2.write(line)
		elif(temp == 0):
			count2 += 1
			resfile2.write(line)
		else:
			count1 += 1
			resfile1.write(line)
	file.close()
	resfile1.close()
	resfile2.close()


splitBD("NE2.dat",60,40,"NE2Learning.dat","NE2Test.dat")

