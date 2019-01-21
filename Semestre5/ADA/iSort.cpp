#include <iostream>  
#include <vector>

using namespace std;

void selectsort(vector<int> &vec){
	int n = vec.size();
	for(int i = 0; i < n - 1; i++){
		int menor = 1000000;
		int index = 0;
		for(int j = i; j < n; j++) 
			if(vec[j] < menor){
				menor = vec[j];
				index = j;
			}
		swap(vec[index],vec[i])
	}
}

void insertSort(vector<int> &vec){
	for(int j = 1; j < vec.size(); j++){
		int key = vec[j];
		int i = j - 1;
		while(i >= 0 and vec[i] < key){
			swap(vec[i + 1], vec[i]);
			i--;
		}
	}
}

vector<int> sumBin(vector<int> &A, vector<int> &B){
	int n = A.size();
	vector<int> C(n + 1);
	int l = 0;
	for(int i = n - 1; i >= 0; i--){
		if(!A[i] and !B[i]){
			C[i + 1]  = l;
			l = 0; 
		}
		else if(A[i] and B[i]){
			C[i + 1] = l;
			if(!l) l = !l;
		}
		else{
			if((A[i] or B[i]) and l){
				C[i + 1] = 0;
				l = 1;
			}
			else C[i + 1] = 1;
		}
	}
	C[0] = l;
	return C;
}

int main(){
	vector<int> vec = {31,45,23,1,10,55,100,2};
	selectsort(vec);
	for(int i : vec) cout<<i<<endl;
}