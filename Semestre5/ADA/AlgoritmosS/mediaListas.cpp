#include <iostream>
#include <cstdlib>
#include <time.h>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

int media(vector<int> &a, vector<int> &b, int a0, int af, int b0, int bf){
	int aM = (a0 + af) / 2;
	int bM = (b0 + bf) / 2;
	if((af - a0) + (bf - b0) == 1){
		cout<<"aaaaa"<<endl;
		vector<int> rr;
		for(int i = a0; i <= af; i++){
			rr.push_back(a[i]);
		}
		for(int i = b0; i <= bf; i++){
			rr.push_back(b[i]);
		}
		sort(rr.begin(), rr.end());
		if((a.size() + b.size())%2 == 0) return rr[1];
		return rr[2];
	}
	if(af == a0 and bf == b0){
		/*if(af - a0 == 1){
			cout<<"xxxxx"<<endl;
			if(a[af] > b[bf]) return a[af];
			return b[bf];
		}*/
		cout<<"1->"<<a[aM]<<endl;
		cout<<"2->"<<b[bM]<<endl;
		if(a[aM] > b[bM]) return a[aM];
		return b[bM];
	}
	if(a[aM] <= b[bM]){
	//cout<<"A"<<endl;
	 return media(a,b,aM,af,b0,bM);
	}
	//cout<<"B"<<endl;
	return media(a,b,a0,aM,bM,bf);
}

tuple<vector<int>,vector<int>> generarlistas(int n){
	int a = rand()%n+1;
	int b = rand()%n+1;
	vector<int> res1;
	vector<int> res2;
	int actual = 0;
	for(int i = 0; i < a; i++){
		actual += rand()%6;
		res1.push_back(actual);
	}
	actual = 0;
	for(int i = 0; i < b; i++){
		actual += rand()%11;
		res2.push_back(actual);
	}
	return make_tuple(res1,res2);
}

void imprimir(vector<int> &a, vector<int> &b){
	for(int i = 0; i < a.size();i++){
		cout<<a[i]<<" ";
	}
	cout<<endl;
	for(int i = 0; i < b.size();i++){
		cout<<b[i]<<" ";
	}
	cout<<endl;
}

int main(){
	srand(time(NULL));
	int n;
	cout<<"Ingrese el tamaño mayor n->";
	cin>>n;
	auto tt = generarlistas(n);
	vector<int> aa = {5,10,12,16,17};
	vector<int> bb = {3,11};
	auto a = get<0>(tt);
	auto b = get<1>(tt);
	
	auto c = a;

	c.insert(c.end(),b.begin(),b.end());
	sort(c.begin(), c.end());
	imprimir(a,b);
	
	for(int i = 0; i < c.size(); i++){
		cout<<c[i]<<" ";
	}
	cout<<endl;
	cout<<"Media->"<<media(a,b,0,a.size()-1,0,b.size()-1)<<endl;;
}