	#include <iostream>
#include <fstream>

using namespace std;

void generarClues(string fileAcroos, string fileDown, string fileOut){
	ifstream archivoAcross(fileAcroos.c_str());
	ifstream archivoDown(fileDown.c_str());
	ofstream archivoOut("tempOut");
	int estado = 0;
	char linea[255];
	string ans;
	archivoOut<<"<table id=\"Clues\" style=\"display: none;\">";
	archivoOut<<"<tbody>"<<endl<<"<tr>"<<endl<<"<td>"<<endl<<endl;
	archivoOut<<"<table class=\"ClueList\">"<<endl;
	archivoOut<<"<tbody id=\"CluesAcross\">"<<endl;
	archivoOut<<"<tr><td colspan=\"2\"><h3 class=\"ExerciseSubtitle\" id=\"CluesAcrossLabel\">Horizontal</h3></td></tr>"<<endl;
	while(archivoAcross.getline(linea,255)){
		if(estado == 0){
			archivoOut<<"<tr>";
			archivoOut<<"<td class=\"ClueNum\">"<<linea<<"</td>";
			string temp(linea);
			ans = temp;
			estado = 1;
		}
		else if(estado == 1){
			archivoOut<<"<td id=\"Clue_A_"<<ans<<"\" class=\"clue\">"<<linea<<"</td></tr>"<<endl;
			estado = 0;
		}
	}
	archivoOut<<"</tbody>"<<endl<<"</table>"<<endl<<"</td>"<<endl<<"<td>"<<endl<<endl;
	archivoOut<<"<table class=\"ClueList\">"<<endl;
	archivoOut<<"<tbody id=\"CluesDown\">"<<endl;
	archivoOut<<"<tr><td colspan=\"2\"><h3 class=\"ExerciseSubtitle\" id=\"CluesDownLabel\">Vertical</h3></td></tr>"<<endl;
	archivoAcross.close();
	estado = 0;
	while(archivoDown.getline(linea,255)){
		if(estado == 0){
			archivoOut<<"<tr>";
			archivoOut<<"<td class=\"ClueNum\">"<<linea<<"</td>";
			string temp(linea);
			ans = temp;
			estado = 1;	
		}
		else if(estado == 1){
			archivoOut<<"<td id=\"Clue_D_"<<ans<<"\" class=\"clue\">"<<linea<<"</td></tr>"<<endl;
			estado = 0;	
		}
	}
	archivoOut<<"</tbody>"<<endl<<"</table>"<<endl<<"</td>"<<endl<<"</tr>"<<endl;
	archivoOut<<"</tbody>"<<endl<<"</table>"<<endl;
	archivoOut.close();
	archivoDown.close();
	string command = "cat tempOut >>"  + fileOut;
	system(command.c_str());
	system("rm tempOut");
}

void generarCrucigrama(string fileInA, string fileInB, string fileOut){
	string cA;
	string cB;
	const char TAB = 9;
	int row = 0;
	int col = 0;
	ifstream archivoInA(fileInA.c_str());
	ifstream archivoInB(fileInB.c_str());
	ofstream archivoOut("tempO");
	ofstream archivoOutA("tempA");
	ofstream archivoOutB("tempB");
	ofstream archivoOutC("tempC");
	archivoOut<<"L = new Array();"<<endl;
	archivoOutA<<"CL = new Array();"<<endl;
	archivoOutB<<"<table class=\"CrosswordGrid\">"<<endl;
	archivoOutB<<"<tbody>"<<endl;
	archivoOutC<<"G = new Array();"<<endl;
	while(archivoInA>>cA and archivoInB>>cB){
		if(col == 0){
			archivoOut<<"L["<<to_string(row)<<"] = new Array(";
			archivoOutA<<"CL["<<to_string(row)<<"] = new Array(";
			archivoOutB<<"<tr id=\"Row_"<<to_string(row)<<"\">"<<endl;
			archivoOutC<<"G["<<to_string(row)<<"] = new Array(";
		}
		if(cA == "|" and cB == "|"){
			archivoOut<<");"<<endl;
			archivoOutA<<");"<<endl;
			archivoOutC<<");"<<endl;
			col = 0;
			row++;
		}
		else if(cA == "&"){
			if(col == 0) {
				archivoOut<<"''";
				archivoOutA<<"'"<<cB<<"'";
				archivoOutC<<"''";
			}
			else {
				archivoOut<<",''";
				archivoOutA<<",'"<<cB<<"'";	
				archivoOutC<<",''";
			}
			archivoOutB<<TAB<<"<td class=\"BlankCell\">&nbsp;</td>"<<endl;
			col++;
		}
		else{
			if(col == 0){
				archivoOut<<"'"<<cA<<"'";
				archivoOutA<<"'"<<cB<<"'";
				archivoOutC<<"''";
			}
			else{
				archivoOut<<",'"<<cA<<"'";
				archivoOutA<<",'"<<cB<<"'";	
				archivoOutC<<",''";
			}
			if(cB == "0"){
				archivoOutB<<TAB<<"<td class=\"LetterOnlyCell\"><span id=\"L_";
				archivoOutB<<to_string(row)<<"_"<<to_string(col)<<"\">&nbsp;</span></td>"<<endl;	
			}
			else{
				archivoOutB<<TAB<<"<td class=\"NumLetterCell\">"<<endl;
		 		archivoOutB<<TAB<<TAB<<"<a href=\"javascript:void(0);\" ";
		 		archivoOutB<<"class=\"GridNum\" onclick=\"ShowClue("<<cB<<","<<to_string(row);
		 		archivoOutB<<","<<to_string(col)<<")\">"<<cB<<"</a>"<<endl;
		 		archivoOutB<<TAB<<TAB<<"<span class=\"NumLetterCellText\" id=\"L_";
		 		archivoOutB<<to_string(row)<<"_"<<to_string(col)<<"\" onclick=\"ShowClue(";
		 		archivoOutB<<cB<<","<<to_string(row)<<","<<to_string(col)<<")\">&nbsp;&nbsp;&nbsp;</span>"<<endl;
				archivoOutB<<TAB<<"</td>"<<endl;	
			}			
			col++;
		}
	}
	archivoOutB<<"</tbody>"<<endl<<"</table>"<<endl;
	archivoOut<<endl;
	archivoOutA<<endl;
	archivoOutB<<endl;
	archivoOutC<<endl;
	archivoOutA.close();
	archivoOutB.close();
	archivoOutC.close();
	archivoOut.close();
	string command = "cat tempO tempA tempC tempB  > " + fileOut;
	system(command.c_str());
	system("rm tempA tempB tempC tempO");
}

void generarCruciGrid(string fileI, string fileO){
	ifstream archivoIn(fileI.c_str());
	ofstream archivoOut(fileO.c_str());
	string c;
	const char TAB = 9;
	int row = 0;
	int col = 0;
	archivoOut<<"<table class=\"CrosswordGrid\">"<<endl;
	archivoOut<<"<tbody>"<<endl;
	while(archivoIn>>c){
		if(col == 0){
			string temp = "<tr id=\"Row_" + to_string(row) + "\">";
			archivoOut<<"<tr id=\"Row_"<<to_string(row)<<"\">"<<endl;
		}
		if(c == "|"){
			col = 0;
			row++;
		}
		else if(c == "b"){
			archivoOut<<TAB<<"<td class=\"BlankCell\">&nbsp;</td>"<<endl;
			col++;
		}
		else if(c == "l"){
			archivoOut<<TAB<<"<td class=\"LetterOnlyCell\"><span id=\"L_";
			archivoOut<<to_string(row)<<"_"<<to_string(col)<<"\">&nbsp;</span></td>"<<endl;
			col++;
		}
		else{
	 		archivoOut<<TAB<<"<td class=\"NumLetterCell\">"<<endl;
	 		archivoOut<<TAB<<TAB<<"<a href=\"javascript:void(0);\" ";
	 		archivoOut<<"class=\"GridNum\" onclick=\"ShowClue("<<c<<","<<to_string(row);
	 		archivoOut<<","<<to_string(col)<<")\">"<<c<<"</a>"<<endl;
	 		archivoOut<<TAB<<TAB<<"<span class=\"NumLetterCellText\" id=\"L_";
	 		archivoOut<<to_string(row)<<"_"<<to_string(col)<<"\" onclick=\"ShowClue(";
	 		archivoOut<<c<<","<<to_string(row)<<","<<to_string(col)<<")\">&nbsp;&nbsp;&nbsp;</span>"<<endl;
			archivoOut<<TAB<<"</td>"<<endl;
			col++;
		}
	}
	archivoOut<<"</tbody>"<<endl<<"</table>"<<endl;
	archivoOut.close();
	archivoIn.close();
}

int main(int argc, char * argv[]){
	/*
	if(argc == 1){
		cout<<"Faltan argumentos: <flag:1->Cruci,2->Clues>  <archivosEntrada> <archivoSalida>"<<endl;
		return 0;
	}
	string flag(argv[1]);
	if(flag == "1"){
		string fileI(argv[2]);
		string fileO(argv[3]);
		generarCruciGrid(fileI, fileO);	
	} 
	else {
		string fileA(argv[2]);
		string fileD(argv[3]);
		string fileO(argv[4]);
		generarClues(fileA, fileD, fileO);
	}*/
	if(argc != 6){
		cout<<"Faltan argumentos: <archivoInWords> <archivoInNumbers> <archivoCluesH> <archivoCluesV> <archivoSalida>"<<endl;
		return 0;
	}
	string fileIA(argv[1]);
	string fileIB(argv[2]);
	string fileH(argv[3]);
	string fileV(argv[4]);
	string fileO(argv[5]);
	generarCrucigrama(fileIA,fileIB,fileO);
	generarClues(fileH,fileV,fileO);

}