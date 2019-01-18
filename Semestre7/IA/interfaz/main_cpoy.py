from Tkinter import *
from Image import *
from ImageTk import *
from threading import *
from decimal import *
from copy import *
import numpy as np
import regresion
import commands

POINT_RADIO = 5
POINT_POLI_RADIO = 2
WIDTH = 800
HEIGHT = 600

DESPL_X = 31.7
DESPL_Y = 18.7
ORIGEN = (17,521)
TOP = 64

red_points = []
blue_points = []

canvas = 0
plano = 0
master = 0

ans = 'help'
mem = 'help'

def track_to_list_left(lts):
    def left_mouse_click(event):
    	if(event.y >= TOP and event.y <= ORIGEN[1] and event.x >= ORIGEN[0]):
        	canvas = event.widget
        	canvas.create_oval(event.x-POINT_RADIO,event.y-POINT_RADIO,event.x+POINT_RADIO,event.y+POINT_RADIO,fill='red',width=1)
        	pointX = float(event.x - ORIGEN[0]) / DESPL_X
        	pointY = float(ORIGEN[1] - event.y) / DESPL_Y
        	lts.append((pointX, pointY))
    return left_mouse_click

def track_to_list_right(lts):
    def right_mouse_click(event):
    	if(event.y >= TOP and event.y <= ORIGEN[1] and event.x >= ORIGEN[0]):
        	canvas = event.widget
        	canvas.create_oval(event.x-POINT_RADIO,event.y-POINT_RADIO,event.x+POINT_RADIO,event.y+POINT_RADIO,fill='blue',width=1)
        	pointX = float(event.x - ORIGEN[0]) / DESPL_X
        	pointY = float(ORIGEN[1] - event.y) / DESPL_Y
        	lts.append((pointX, pointY))
    return right_mouse_click


def Interfaz():
	img = Image.open("plano.png")
	img.thumbnail((WIDTH,HEIGHT),Image.ANTIALIAS)

	global master
	global plano
	global canvas
	master = Tk()
	plano = PhotoImage(img)
	
	canvas = Canvas(master, width=WIDTH, height=HEIGHT)
	canvas.create_image(WIDTH/2,HEIGHT/2,anchor=CENTER,image=plano,tags="img")
	canvas.pack()

	tracking_function_left = track_to_list_left(red_points)
	tracking_function_right = track_to_list_right(blue_points)
	canvas.bind('<Button-1>', lambda event: tracking_function_left(event))
	canvas.bind('<Button-3>', lambda event: tracking_function_right(event))


	mainloop()

def resetCanvas():
	global canvas;
	global master;
	global plano;
	global red_points;
	global blue_points;
	canvas.delete("all")
	red_points = []
	blue_points = []

	tracking_function_left = track_to_list_left(red_points)
	tracking_function_right = track_to_list_right(blue_points)
	canvas.bind('<Button-1>', lambda event: tracking_function_left(event))
	canvas.bind('<Button-3>', lambda event: tracking_function_right(event))
	canvas.create_image(WIDTH/2,HEIGHT/2,anchor=CENTER,image=plano,tags="img")
	canvas.pack()	

def plotPoint(x,y,color,radio):
	global canvas;
	global ORIGEN
	global DESPL_X
	global DESPL_Y
	pointX = Decimal(x) * Decimal(DESPL_X) + Decimal(ORIGEN[0])
	pointY = Decimal(ORIGEN[1]) -  (Decimal(y) * Decimal(DESPL_Y))
	canvas.create_oval(pointX-radio,pointY-radio,pointX+radio,pointY+radio,fill=color,width=1)

def plotPoints():
	global red_points
	global blue_points
	for e in red_points:
		plotPoint(e[0],e[1],'red',POINT_RADIO)
	for e in blue_points:
		plotPoint(e[0],e[1],'blue',POINT_RADIO)

def plotPolinomio(thetaFile):
	O = regresion.readTheta(thetaFile)
	tx = np.arange(Decimal(0.0),Decimal(24.5),Decimal(0.1))	
	ty = tx
	res = regresion.getCurvaNivel(tx,ty,O)
	for i in range(0,len(res[0])):
		plotPoint(res[0][i],res[1][i],'yellow',POINT_POLI_RADIO)

interfaz_thread = Thread(target=Interfaz)
interfaz_thread.start()
flag = 0
while(True):
	try:
		comando = 0
		if(flag == 0):
			comando = input()
			if(comando != 'ans' and comando != 'sc' and comando != 'p ans'):
				ans = deepcopy(comando)
		elif(flag == 1):
			comando = deepcopy(ans)
			flag = 0
		elif(flag == 2):
			comando = deepcopy(mem)
			ans = deepcopy(mem)
			flag = 0

	except:
		print "No se reconoce el comando"
	else:
		comandos = comando.split()
#		try:
		if(comandos[0] == 'p'):
			if(comandos[1] == 'blue'):
				print blue_points
			elif(comandos[1] == 'red'):
				print red_points
			elif(comandos[1] == 'ans'):
				print ans
			elif(comandos[1] == 'mem'):
				print mem
		elif(comandos[0] == 'rt'):
			file = comandos[1]
			n = int(comandos[2])
			regresion.generarRandom(n,file)
			print "Thetas generadas correctamente"
		elif(comandos[0] == 'go'):
			file = comandos[1]
			n = int(comandos[2])
			O = regresion.readTheta(file)
			regresion.graficar(O,red_points,blue_points,n)
		elif(comandos[0] == 'run'):
			file = comandos[1]
			n = int(comandos[2])
			regresion.alfa = float(comandos[3])
			regresion.half_life = float(comandos[4])
			points = regresion.unirPuntos(red_points,blue_points,regresion.getThetaSize(file))
			regresion.regresionFile(file,points[0],points[1],n)
			print "Iteraciones terminadas con exito"
		elif(comandos[0] == 'runr'):
			file = comandos[1]
			n = int(comandos[2])
			iteraciones = int(comandos[3])
			regresion.generarRandom(n,file)
			regresion.alfa = float(comandos[4])
			regresion.half_life = float(comandos[5])
			points = regresion.unirPuntos(red_points,blue_points,regresion.getThetaSize(file))
			regresion.regresionFile(file,points[0],points[1],iteraciones)
			print "Iteraciones terminadas con exito"

		elif(comandos[0] == 'save'):
			file = comandos[1]
			regresion.savePoints(file,red_points,blue_points)
			print "Archivo guardado correctamente"
		elif(comandos[0] == 'load'):
			file = comandos[1]
			res = regresion.loadPoints(file)
			red_points = res[0]
			blue_points = res[1]
			plotPoints()
			print "Archivo cargado correctamente"
		elif(comandos[0] == 'g'):
			file = comandos[1]
			plotPolinomio(file)
		elif(comandos[0] == 'clear'):
			if(comandos[1] == 'all'):
				resetCanvas()
			elif(comandos[1] == 'poli'):
				tempRed = deepcopy(red_points)
				tempBlue = deepcopy(blue_points)
				resetCanvas()
				red_points = deepcopy(tempRed)
				blue_points = deepcopy(tempBlue)
				plotPoints()
		elif(comandos[0] == 'ans'):
			flag = 1
		elif(comandos[0] == 'sc'):
			mem = deepcopy(ans)
			temp = '(' + ans + ') Copiado'
			print temp
		elif(comandos[0] == 'mem'):
			flag = 2
		elif(comandos[0] == 'test'):
			n = int(comandos[1])
			points = regresion.unirPuntos(red_points,blue_points,n)
			print points

		elif(comandos[0] == 'cp'):
			commands.getoutput(comando)
		elif(comandos[0] == 'ls'):
			print commands.getoutput(comando)
		elif(comandos[0] == 'cat'):
			print commands.getoutput(comando)
		elif(comandos[0] == 'exit'):
			break
		elif(comandos[0] == 'help'):
			print commands.getoutput('cat help')
		else:
			print "NO se reconoce el comando"
#		except OverflowError:
#			print "Math range error"
#		except IOError:
#			print "No se encuentra el archivo"
#		except:
#			print "NO se reconoce el comando"

print "EXIT"
master.quit()

