import enchant
import threading

def filtro(i):
	d = enchant.Dict("en_US")	
	archivo = open("Resultados2/000" + str(i),"r")
	res = open("Resultados3/000" + str(i),"w")
	total = 0
	total2 = 0;
	for linea in archivo:
		print(total2)
		primera = ""
		segunda = ""
		flag = False
		total2 += 1
		for c in linea:
			if(c == " " and flag == False):
				flag = True
			elif(c == "	" and flag == True):
				break
			elif (flag == False):
				primera = primera + c
			elif (flag == True):
				segunda = segunda + c
		if(d.check(primera) and d.check(segunda)):
			res.write(linea)
			total += 1
	print(total)
	print(total2)
	tt = open("Resultados/total000" + str(i),"w")
	tt.write(str(total) + "\n")
	tt.close()
	archivo.close()
	res.close()	
	return 

for i in range(0,32):
	filtro(i)

