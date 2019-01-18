'''
XNPIO
v0.0.2
'''

import mechanize
import commands
import MySQLdb

tipo = int(input("NOTAS(1) - LIBRETA(2)->"))
apPa = raw_input("APELLIDO PATERNO->")
apMa = raw_input("APELLIDO MATERNO->")
nombre = raw_input("NOMBRE->")
escuela = raw_input("ESCUELA->")

datos = ["localhost","root","root","universidad"]

query = "call search(\"" + apPa + "\",\"" + apMa + "\",\"" + nombre + "\",\"" + escuela + "\");"

print query

connection = MySQLdb.connect(*datos)
cursor = connection.cursor()
cursor.execute(query);
data = cursor.fetchall()
cursor.close()
connection.close()

flag = False

if(tipo == 1):
	for registro in data:
		flag = True
		print registro
		cui = registro[4]
		dni = registro[0]
		res = open("res.html","w")
		br = mechanize.Browser()
		br.open("http://extranet.unsa.edu.pe/sisacad/parcialesv16/")
		br.select_form(nr=0)
		br.form['cui'] = str(cui)
		br.form['dni'] = str(dni)
		br.submit()
		res.write(br.response().read())
		res.close()
		commands.getoutput("firefox res.html")
		raw_input("PRESION ENTER PARA CONTINUAR")

else:
	for registro in data:
		flag = True
		print registro
		cui = registro[4]
		dni = registro[0]
		escuela = registro[7]
		com = "firefox http://extranet.unsa.edu.pe/sisacad/libretas/descarga.php?file=/tmp/Libreta_De_Notas_" + str(cui) + ".pdf\&codal=" + str(cui)
		print com
		commands.getoutput(com)
		'''
		res = open("res.html","w")
		br = mechanize.Browser()
		br.open("http://extranet.unsa.edu.pe/sisacad/libretas/")
		br.select_form(nr=0)
		br.form['cui'] = str(cui)
		br.form['dni'] = str(dni)
		br.form['escuela'] = ["450"]
		control = br.form.find_control("escuela")
		control.set_value_by_label((escuela,))
		br.submit()
		res.write(br.response().read())
		res.close()
		commands.getoutput("firefox res.html")
		'''
		raw_input("PRESION ENTER PARA CONTINUAR")

if(flag == False):
	print "NO HUBO COINCIDENCIAS"

