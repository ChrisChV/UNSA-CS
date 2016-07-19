#include "CImg.h"
#include <cmath>
#include <string.h>
#include <iostream>
#include <complex>
//#include <windows>
#include "stdlib.h"


using namespace std;
using namespace cimg_library;



void computeFourier()
{

	CImg<float> img("1.pgm");
	CImgList<float> myfft = img.get_FFT();
    cimglist_apply(myfft,shift)(img.width()/2,img.height()/2,0,0,2);
    const CImg<unsigned char> mag = ((myfft[0].get_pow(2) + myfft[1].get_pow(2)).sqrt() + 1).log().normalize(0,255);
	mag.save("spectrum.png");
	CImgDisplay main_disp(mag,"Spectrum");
	while (!main_disp.is_closed()) {
   	 main_disp.wait();
   	} 
	
}

int tol = 20;

void myFilter()
{
	CImg<float> img("1.pgm");
	CImgList<float> myfft = img.get_FFT();
    cimglist_apply(myfft,shift)(img.width()/2,img.height()/2,0,0,2);
    
    CImg<unsigned char> spec("spectrum.png");
    
    for (int ii=0;ii<img.height();ii++)
    {
        for (int jj=0;jj<img.width();jj++)
        {
            int color = spec(jj,ii);
            if (color<=tol)
            {
            	//cout<<"pollo"<<endl;
            	myfft[0](jj,ii) = 0;
            	myfft[1](jj,ii) = 0;
            }
        }
   	}
   	
   	cimglist_apply(myfft,shift)(img.width()/2,img.height()/2,0,0,2);
    img.FFT(myfft[0],myfft[1],true);
    const CImg<unsigned char> mag = ((myfft[0].get_pow(2) + myfft[1].get_pow(2)).sqrt() + 1).log().normalize(0,255);
	CImgDisplay main_disp(mag,"Nueva imagen");
  	while (!main_disp.is_closed()) {
   	 main_disp.wait();
   	} 
}

int main(){

	computeFourier();
	myFilter();
	
	return 0;
}
