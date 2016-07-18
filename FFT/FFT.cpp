#include <iostream>
#include <fstream>
#include <sstream>
#include <complex>
#include <math.h>
#include <valarray>


using namespace std;

// PI constant
const double PI = 3.14192653589793238460;
//using type complex due to the nature of the FFT function. double for higher precision
typedef complex<double> Complex;
//using valarray because its easier to deal with in the recursive function (creating subsets)
typedef valarray<Complex> Complex_Array;

typedef int Color;

void loadImage(string file){
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
	vector<vector<Color>> res(altura);
	for(int i = 0; i < altura; i++){res[i] = vector<Color>(ancho);}
	//r * 0.2126
	//g * 0.7152
	//b * 0.0722
	int fil = 0;
	int col = 0;
	int total = 1;
	cout<<altura<<endl;
	cout<<ancho<<endl;
	while(archivo.getline(linea,300)){
		//cout<<"TOTAL1->"<<total<<endl;
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
	cout<<"termin"<<endl;
	archivo.close();
	string file3 = tt + ".pgm";
	ofstream archivoB(file3.c_str());
	archivoB<<"P2"<<endl;
	archivoB<<"# Reconstructed Image"<<endl;
	archivoB<<ancho<<" "<<altura<<endl;
	archivoB<<255<<endl;
	total = 1;

	for(int i = 0; i < res.size(); i++){
		for(int j = 0; j < res[i].size(); j++){
			
			total ++;
			archivoB<<res[i][j]<<endl;
		}
	}
	archivoB.close();
}

//recursive function implementing the FFT based on Cooley/Tukey algorithm. in-place. input array is overwritten.
void fft(Complex_Array& x)
{
	size_t N = x.size();
	if (N>1)
	{

		//split elements into even and odd
		Complex_Array even = x[slice(0, N / 2, 2)];
		Complex_Array odd = x[slice(1, N / 2, 2)];

		//apply the recursive function
		fft(even);
		fft(odd);

		//blend both array together
		for (size_t i = 0; i<N / 2; i++)
		{
			Complex t = polar(1.0, -2 * PI*i / N)*odd[i];
			x[i] = even[i] + t;
			x[i + N / 2] = even[i] - t;
		}
	}
}

//concealed recursive function implementing the IFFT based on Cooley/Tukey algorithm. in-place. input array is overwritten.
void _ifft(Complex_Array& x)
{
	size_t N = x.size();
	if (N>1)
	{
		//split elements into even and odd
		Complex_Array even = x[slice(0, N / 2, 2)];
		Complex_Array odd = x[slice(1, N / 2, 2)];

		//apply the recursive function
		_ifft(even);
		_ifft(odd);

		//blend both array together
		for (size_t i = 0; i<N / 2; i++)
		{
			//positive angle instead of negative angle
			Complex t = polar(1.0, 2 * PI*i / N)*odd[i];
			x[i] = even[i] + t;
			x[i + N / 2] = even[i] - t;
		}
	}
}

void ifft(Complex_Array& x)
{
	//call the concealed _ifft function
	_ifft(x);

	size_t N = x.size();
	//loop and divide all elements by N
	for (size_t i = 0; i<N; i++)
	{
		x[i] = x[i] / (double)N;
	}
}


int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		string file(argv[1]);
		string file2;
		for(auto iter = file.begin(); iter != file.end(); ++iter){
			if(*iter == '.') break;
			else file2.push_back(*iter);
		}
		file2 += ".pgm";
		loadImage(file);
		int row = 0, col = 0, numrows = 0, numcols = 0, max_val = 0;
		Complex *signal;
		Complex **all_image;
		stringstream ss;
		string inputLine = "";
		double max = 0;

		cout << "Status: Reading the image file." << endl;
		ifstream infile(file2.c_str());

		//read version line
		getline(infile, inputLine);

		//read the comment
		getline(infile, inputLine);

		//dump the contents into the stringstream
		ss << infile.rdbuf();
		infile.close();

		//get image dimensions
		ss >> numcols >> numrows;
		ss >> max_val;

		//initialze arrays

		signal = new Complex[numcols];
		all_image = new Complex*[numrows];
		for (int i = 0; i < numrows; i++)
			all_image[i] = new Complex[numcols];

		cout << "Status: Applying FFT to the image." << endl;
		// apply FFT on every row
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

		cout << "Finished FFT on the whole image in " << (clock() - start) << " clocks." << endl;

		cout << "Status: Writing the frequency spectrum image to the hard disk." << endl;
		string file3 = "Frequency" + file2;
		ofstream output_freq(file3.c_str());
		output_freq << "P2" << endl;
		output_freq << "# Frequency Image" << endl;
		output_freq << numcols << " " << numrows << endl;
		output_freq << 255 << endl;

		//calculate a scaling factor for the output values
		float scaling_factor = 255 / log(1 + max);

		//output fourth quadrant as the first quadrant and third quadrant as second
		for (int i = numrows / 2; i<numrows; i++)
		{
			for (int j = numcols / 2; j<numcols; j++)
				output_freq << int(scaling_factor*log(1 + abs(all_image[i][j]))) << endl;
			for (int j = 0; j<numcols / 2; j++)
				output_freq << int(scaling_factor*log(1 + abs(all_image[i][j]))) << endl;
		}

		//output second quadrant as the third quadrant and first quadrant as forth
		for (int i = 0; i<numrows / 2; i++)
		{
			for (int j = numcols / 2; j<numcols; j++)
				output_freq << int(scaling_factor*log(1 + abs(all_image[i][j]))) << endl;
			for (int j = 0; j<numcols / 2; j++)
				output_freq << int(scaling_factor*log(1 + abs(all_image[i][j]))) << endl;
		}

		output_freq.close();

		cout << "Status: Applying IFFT to reconstruct the image." << endl;
		//apply the IFFT to reconstruct the image
		start = clock();
		for (col = 0; col<numcols; col++)
		{
			Complex column[numrows];
			for (int i = 0; i<numrows; i++)
			{
				column[i] = all_image[i][col];
			}
			Complex_Array x(column, numrows);
			ifft(x);

			for (int i = 0; i<numrows; i++)
			{
				all_image[i][col] = x[i];
			}
		}

		for (row = 0; row < numrows; row++)
		{
			Complex single_row[numcols];
			for (int i = 0; i < numcols; i++)
			{
				single_row[i] = all_image[row][i];
			}

			Complex_Array x(single_row, numcols);
			ifft(x);

			for (int i = 0; i<numcols; i++)
			{
				all_image[row][i] = x[i];
			}
		}

		cout << "Finished IFFT on the whole image in " << (clock() - start) << " clocks." << endl;

		cout << "Status: Writing the reconstructed image to the hard disk." << endl;
		string file4 = "Reconstructed" + file2;
		ofstream output_recon(file4.c_str());
		output_recon << "P2" << endl;
		output_recon << "# Reconstructed Image" << endl;
		output_recon << numcols << " " << numrows << endl;
		output_recon << 255 << endl;

		//outut the real values only since they correspond to the actual pixel values
		for (int i = 0; i<numrows; i++){
			for (int j = 0; j<numcols; j++){
				int ttt = (int)real(all_image[i][j]);
				if(ttt >= 0) output_recon<<ttt<<endl;
				else output_recon<<0<<endl;
				

			}
		}
		output_recon.close();
	}

	else
	{
		cout << "ERROR->La sintaxis es la siguiente: ./run <image>" << endl;
		
	}
	return 0;
}