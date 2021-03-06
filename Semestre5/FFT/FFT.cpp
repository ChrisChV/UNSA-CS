#include <iostream>
#include <fstream>
#include <sstream>
#include <complex>
#include <cmath>
#include <valarray>
#include <tuple>


using namespace std;

typedef complex<double> Complex;
typedef valarray<Complex> Complex_Array;

typedef int Color;

tuple<int,int> loadImage(string file){
	string file2;
	for(auto iter = file.begin(); iter != file.end(); ++iter){
		if(*iter == '.') break;
		else file2.push_back(*iter);
	}
	string tt = file2;
	file2 += ".txt";
	string comand = "convert " + file + " " + file2;
	system(comand.c_str());
	ifstream archivo(file2.c_str());
	string temp;
	for(int i = 0; i < 4; i++){
		archivo>>temp;
		cout<<temp<<endl;	
	} 
	int altura;
	int ancho;
	string heder;
	archivo>>heder;
	cout<<"H->"<<heder<<endl;
	bool f = false;
	string a;
	string b;
	for(auto iter = heder.begin(); iter != heder.end(); ++iter){
		if(*iter == ',' and !f) f = true;
		else if(*iter == ',' and f) break;
		else if(f) b.push_back(*iter);
		else a.push_back(*iter);
	}
	char linea[300];
	archivo.getline(linea,300);
	ancho = stoi(a);
	altura = stoi(b);
	int two = 1;
	while(pow(2,two) < ancho or pow(2,two) < altura) two++;
	int tam = pow(2,two);
	vector<vector<Color>> res(tam);
	for(int i = 0; i < altura; i++){res[i] = vector<Color>(tam);}
	//r * 0.2126
	//g * 0.7152
	//b * 0.0722
	int fil = 0;
	int col = 0;
	int total = 1;
	cout<<altura<<endl;
	cout<<ancho<<endl;
	cout<<tam<<endl;
	while(archivo.getline(linea,300)){
		string rgb(linea);
		string r;
		string g;
		string b;
		int estado = 0;
		int sum = 0;
		for(auto iter = rgb.begin(); iter != rgb.end(); ++iter){
			if(estado != 0){
				if(estado == 1){
					if(*iter != ','){
						if(*iter != ' ') r.push_back(*iter);
					} 
					else estado = 2;
				}
				else if(estado == 2){
					if(*iter != ',') {
						if(*iter != ' ') g.push_back(*iter);
					}
					else estado = 3;
				}
				else if(estado == 3){
					if(*iter != ',' and *iter != ')') {
						if(*iter != ' ') b.push_back(*iter);
					}
					else break;
				}
			}
			else if(*iter == '(') estado = 1;
		}
		res[fil][col] = stoi(r) * 0.2126 + stoi(g) * 0.7152 + stoi(b) * 0.0722;		
		col++;
		if(col == ancho){
			col = 0;
			fil++;
		}
		total++;
	}
	
	archivo.close();
	string file3 = tt + ".pgm";
	ofstream archivoB(file3.c_str());
	archivoB<<"P2"<<endl;
	archivoB<<"# BalckWhite Image"<<endl;
	archivoB<<ancho<<" "<<altura<<endl;
	archivoB<<255<<endl;
	total = 1;

	for(int i = 0; i < altura; i++){
		for(int j = 0; j < ancho; j++){
			total ++;
			archivoB<<res[i][j]<<endl;
		}
	}
	archivoB.close();


	string file4 = tt + "(2).pgm";
	ofstream resres(file4.c_str());
	resres<<"P2"<<endl;
	resres<<"# Total Image"<<endl;
	resres<<tam<<" "<<tam<<endl;
	resres<<255<<endl;

	for(int i = 0; i < res.size(); i++){
		for(int j = 0; j < res[i].size(); j++){
			resres<<res[i][j]<<endl;
		}
	}

	cout<<"termin"<<endl; 

	resres.close();

	return make_tuple(altura,ancho);
}

void fft(Complex_Array& x)
{
	size_t N = x.size();
	if (N>1)
	{
		Complex_Array even = x[slice(0, N / 2, 2)];
		Complex_Array odd = x[slice(1, N / 2, 2)];


		fft(even);
		fft(odd);

		for (size_t i = 0; i<N / 2; i++)
		{
			Complex t = polar(1.0, -2 * M_PI*i / N)*odd[i];
			x[i] = even[i] + t;
			x[i + N / 2] = even[i] - t;
		}
	}
}

void _ifft(Complex_Array& x)
{
	size_t N = x.size();
	if (N>1)
	{
		Complex_Array even = x[slice(0, N / 2, 2)];
		Complex_Array odd = x[slice(1, N / 2, 2)];

		_ifft(even);
		_ifft(odd);

		for (size_t i = 0; i<N / 2; i++)
		{
			Complex t = polar(1.0, 2 * M_PI*i / N)*odd[i];
			x[i] = even[i] + t;
			x[i + N / 2] = even[i] - t;
		}
	}
}

void ifft(Complex_Array& x)
{
	_ifft(x);

	size_t N = x.size();
	for (size_t i = 0; i<N; i++)
	{
		x[i] = x[i] / (double)N;
	}
}


void generarTransformada(string file){
	cout<<"Status: Generando Transformada"<<endl;
		string file2;
		for(auto iter = file.begin(); iter != file.end(); ++iter){
			if(*iter == '.') break;
			else file2.push_back(*iter);
		}
		string lon = file2 + ".pgm";
		file2 += "(2).pgm";

		auto tt = loadImage(file);
		int row = 0, col = 0, numrows = 0, numcols = 0, max_val = 0;
		Complex *signal;
		Complex **all_image;
		stringstream ss;
		string inputLine = "";
		double max = 0;
		cout << "Status: Reading the image file." << endl;
		ifstream infile(file2.c_str());

		getline(infile, inputLine);

		getline(infile, inputLine);

		ss << infile.rdbuf();
		infile.close();

		ss >> numcols >> numrows;
		ss >> max_val;


		signal = new Complex[numcols];
		all_image = new Complex*[numrows];
		for (int i = 0; i < numrows; i++)
			all_image[i] = new Complex[numcols];

		cout << "Status: Applying FFT to the image." << endl;

		size_t start = clock();
		for (row = 0; row < numrows; row++)
		{
			for (col = 0; col < numcols; col++)
			{
				ss >> signal[col];
			}

			Complex_Array x(signal, numcols);
			fft(x);

			for (int i = 0; i<numcols; i++)
			{
				all_image[row][i] = x[i];
			}
		}

		for (col = 0; col<numcols; col++)
		{
			Complex column[numrows];
			for (int i = 0; i<numrows; i++)
			{
				column[i] = all_image[i][col];
			}
			Complex_Array x(column, numrows);
			fft(x);

			for (int i = 0; i<numrows; i++)
			{
				all_image[i][col] = x[i];
				if (abs(x[i])>max)
					max = abs(x[i]);
			}
		}


		string file5 = "Real" + lon;
		ofstream out_real(file5.c_str());
		out_real<<"P2"<<endl;
		out_real<<"#	Real Image"<<endl;
		out_real<<numcols<<" "<<numrows<<endl;
		out_real << 255 << endl;

		for (int i = 0; i<numrows; i++){
			for (int j = 0; j<numcols; j++){
				int ttt = (int)real(all_image[i][j]);
				out_real<<ttt<<endl;
				//if(ttt >= 0) 
				//else output_recon<<0<<endl;
			}
		}

		out_real.close();

		string file6 = "Imaginario" + lon;
		ofstream out_imag(file6.c_str());
		out_imag<<"P2"<<endl;
		out_imag<<"#	Imaginario Image"<<endl;
		out_imag<<numcols<<" "<<numrows<<endl;
		out_imag << 255 << endl;

		for (int i = 0; i<numrows; i++){
			for (int j = 0; j<numcols; j++){
				int ttt = (int)imag(all_image[i][j]);
				out_imag<<ttt<<endl;
				//if(ttt >= 0) 
				//else output_recon<<0<<endl;
			}
		}

		out_imag.close();

		cout << "Finished FFT on the whole image in " << (clock() - start) << " clocks." << endl;

		cout << "Status: Writing the frequency spectrum image to the hard disk." << endl;
		string file3 = "Frequency" + lon;
		ofstream output_freq(file3.c_str());
		output_freq << "P2" << endl;
		output_freq << "# Frequency Image" << endl;
		output_freq << numcols << " " << numrows << endl;
		output_freq << 255 << endl;

		float scaling_factor = 255 / log(1 + max);


		//output fourth quadrant as the first quadrant and third quadrant as second
		/*
		for(int i = 0; i < numrows; i++){
			for(int j = 0; j < numcols; j++){
				output_freq << int(scaling_factor*log(1 + abs(all_image[i][j]))) << endl;
			}
		}
		*/

		
		for (int i = numrows / 2; i<numrows; i++)
		{
			for (int j = numcols / 2; j<numcols; j++)
				output_freq << int(scaling_factor*log(1 + sqrt(pow(real(all_image[i][j]),2) + pow(imag(all_image[i][j]),2)))) << endl;
			for (int j = 0; j<numcols / 2; j++)
				output_freq << int(scaling_factor*log(1 + sqrt(pow(real(all_image[i][j]),2) + pow(imag(all_image[i][j]),2)))) << endl;
		}

		for (int i = 0; i<numrows / 2; i++)
		{
			for (int j = numcols / 2; j<numcols; j++)
				output_freq << int(scaling_factor*log(1 + sqrt(pow(real(all_image[i][j]),2) + pow(imag(all_image[i][j]),2)))) << endl;
			for (int j = 0; j<numcols / 2; j++)
				output_freq << int(scaling_factor*log(1 + sqrt(pow(real(all_image[i][j]),2) + pow(imag(all_image[i][j]),2)))) << endl;
		}
		
		output_freq.close();
}


void reconstruir(string fileI, string fileR, string spectre){
	cout<<"Status: Reconstruyendo"<<endl;
	string file2;
	for(auto iter = fileI.begin(); iter != fileI.end(); ++iter){
			if(*iter == '.') break;
			else file2.push_back(*iter);
	}
		string lon = file2 + ".pgm";

		auto tt = loadImage(spectre);
		int row = 0, col = 0, numrows = 0, numcols = 0, max_val = 0;
		Complex *signal;
		Complex **all_image;
		stringstream img;
		stringstream sReal;
		stringstream sSpectre;
		string inputLine = "";
		double max = 0;
		cout << "Status: Reading the image file." << endl;

		ifstream infile(fileI.c_str());
		ifstream infile2(fileR.c_str());
		ifstream infile3(spectre.c_str());

		getline(infile, inputLine);

		getline(infile, inputLine);

		getline(infile2, inputLine);

		getline(infile2, inputLine);

		getline(infile3, inputLine);

		getline(infile3, inputLine);

		img << infile.rdbuf();
		sReal << infile2.rdbuf();
		sSpectre << infile3.rdbuf();

		infile.close();
		infile2.close();
		infile3.close();

		img >> numcols >> numrows;
		img >> max_val;

		sReal >> numcols >> numrows;
		sReal >>max_val;

		sSpectre >> numcols >> numrows;
		sSpectre >>max_val;		

		signal = new Complex[numcols];
		all_image = new Complex*[numrows];
		for (int i = 0; i < numrows; i++)
			all_image[i] = new Complex[numcols];

		double a;
		double b;
		for(int i = 0; i < numrows; i++){
			for(int j = 0; j < numcols; j++){
				img >> a;
				sReal >> b;		
				all_image[i][j] = complex<double>(b,a);
			}
		}

		int total = 20;
		int color;
		for(int i = 0; i < numrows; i++){
			for(int j = 0; j < numcols; j++){
				sSpectre >> color;
				if(color<=total){
					all_image[i][j] = complex<double>(0,0);
				}
			}
		}

		cout << "Status: Applying IFFT to reconstruct the image." << endl;

		max = 0;
		

		for (row = 0; row < numrows; row++)
		{
			Complex single_row[numcols];
			for (int i = 0; i < numcols; i++)
			{
				single_row[i] = all_image[row][i];
			}

			Complex_Array x(single_row, numcols);

			ifft(x);

			for (int i = 0; i < numcols; i++)
			{
				all_image[row][i] = x[i];
			}
		}
		for (col = 0; col < numcols; col++)
		{
			Complex column[numrows];
			for (int i = 0; i < numrows; i++)
			{
				column[i] = all_image[i][col];
			}
			Complex_Array x(column, numrows);

			ifft(x);

			for (int i = 0; i < numrows; i++)
			{
				all_image[i][col] = x[i];
			}
		}

		cout << "Status: Writing the reconstructed image to the hard disk." << endl;
		string file4 = "Reconstructed" + lon;
		ofstream output_recon(file4.c_str());
		output_recon << "P2" << endl;
		output_recon << "# Reconstructed Image" << endl;
		output_recon << numcols << " " << numrows << endl;
		output_recon << 255 << endl;

		
		for (int i = 0; i<numrows; i++){
			for (int j = 0; j<numcols; j++){
				int ttt = (int)real(all_image[i][j]);
				output_recon<<ttt<<endl;
				//if(ttt >= 0) 
				//else output_recon<<0<<endl;
			}
		}
		output_recon.close();
}

int main(int argc, char* argv[])
{
	if (argc == 3 or argc == 5)
	{
		string tipo(argv[1]);
		string file(argv[2]);
		if(argc == 5){
			string file2(argv[3]);
			string file3(argv[4]);
			if(tipo == "-r") reconstruir(file,file2,file3);
		}
		if(tipo == "-f") generarTransformada(file);
	}

	else
	{
		cout << "ERROR->La sintaxis es la siguiente: ./run <tipo> <image>" << endl;
		
	}
	return 0;
}