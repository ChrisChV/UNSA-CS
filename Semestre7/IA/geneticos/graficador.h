#ifndef GRAFICADOR_H
#define GRAFICADOR_H

#include <iostream>

using namespace std;

void graficarGauss(string img, string points, string angle1, string angle2){

	string view = "set view " + angle1 + "," + angle2;

	string xRange = "set xrange [0:8192]";
	string yRange = "set yrange [0:8192]";

	string A = "10000";
	string centerX = "4096";
	string centerY = "4096";
	string oX = "1500";
	string oY = "1500";


	string imagePath = "imagenes/" + img;

	string pointSize = "1";
	string pointType = "7";

	points = "'" + points + "' using 1:2:3 with points pointsize " + pointSize + " pointtype " + pointType;

	string plot = "splot " + A + "*exp(-(((x-" + centerX + ")**2)/(2*(" + oX + "**2))+((y-" + centerY + ")**2)/(2*(" + oY + "**2))))," + points;

	string command = "gnuplot -e \"set term png;" + view + ";set output '" + imagePath + "';" + xRange + ";" + yRange + ";" + plot + ";\"";

	system(command.c_str());
	
}


#endif