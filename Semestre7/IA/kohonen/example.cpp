#include "CImg.h"
#include "Kohonen2d.h"

using namespace cimg_library;

void displayImg(int tam, vector<vector<int>> matrix){
	CImg<unsigned char> visu(tam,tam,1,3,0);
  	CImgDisplay  draw_disp(visu,"Neuoras");
  	const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 },amarillo[]={255,255,0},blanco[]={255,255,255},celeste[]={0,255,205},morado[]={222,76,138},rosado[]={124,0,255};
	for(int i=0;i<tam;i++){
  		for(int j=0;j<tam;j++) {
			switch(matrix[i][j]){
				case 0: visu.draw_point(i,j,blanco); break;
				case 1: visu.draw_point(i,j,blue); break;
				case 2: visu.draw_point(i,j,red); break;
				case 3: visu.draw_point(i,j,green); break;
				case 4: visu.draw_point(i,j,amarillo); break;
				case 5: visu.draw_point(i,j,rosado); break;
				case 6: visu.draw_point(i,j,celeste); break;
				case 7: visu.draw_point(i,j,morado); break;
			}	
		}	
	}
  	while (!draw_disp.is_closed()) {
  		visu.display(draw_disp);
    }
}

int main() {
  	NeuroMap nm("res1","res2","TotalLearningN.dat","TotalTestN.dat");
  	displayImg(nm.NMsize,nm.clases);
}
