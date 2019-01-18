#include <iostream>
#include <tuple>
#include <algorithm>
#include <fstream>
#include <string.h>
#include <queue>
#include <cmath>
#include <vector>

using namespace std;

typedef float Coordenada;
typedef tuple<Coordenada,Coordenada> Point;
typedef float Distancia;
typedef float Area;

enum LIMIT_ENUM{TOP,LOWER};
enum COR_ENUM{X,Y};
enum FLAG_REGION_ENUM{LEAF,NO_LEAF,OBJECT};

bool cruce(Coordenada a1,Coordenada a2,Coordenada b1,Coordenada b2){
    return a1 <= b2 and b1 <= a2;
}

Distancia distancia_rectangulos(Point a1,Point a2,Point b1,Point b2){
    if(cruce(get<0>(a1),get<0>(a2),get<0>(b1),get<0>(b2))){
        if(cruce(get<1>(a2),get<1>(a1),get<1>(b2),get<1>(b1))){
            return 0;
        }
        return min(abs(get<1>(a1)-get<1>(b2)),abs(get<1>(a2)-get<1>(b1)));
    }
    if(cruce(get<1>(a2),get<1>(a1),get<1>(b2),get<1>(b1))){
        return min(abs(get<0>(a1)-get<0>(b2)),abs(get<0>(a2)-get<0>(b1)));
    }
    Distancia dx=min(abs(get<0>(a1)-get<0>(b2)),abs(get<0>(a2)-get<0>(b1)));
    Distancia dy=min(abs(get<1>(a1)-get<1>(b2)),abs(get<1>(a2)-get<1>(b1)));
    return pow(pow(dx,2)+pow(dy,2),0.5);
}


class RTree{
	public:
		RTree(){
			root = nullptr;
			M = 2;
			dimensions = 2;
			actualId = 0;
		}
		RTree(int M){
			root = nullptr;
			this->M = M;
			dimensions = 2;
			actualId = 0;
		}
		void insert(Point top, Point lower);
		void print(string file);
		class Region{
			private:
				int id;
			public:	
				Region(){padre = nullptr;}
				Region(Point top, Point lower, int flag, int id){
					this->flag = flag;
					limites[TOP] = top;
					limites[LOWER] = lower;	
					padre = nullptr;
					this->id = id;
				}
				void cpy(Region * reg){
					flag = reg->flag;
					limites[TOP] = reg->limites[TOP];
					limites[LOWER] = reg->limites[LOWER];
					regions = reg->regions;
					padre = reg->padre;
					id = reg->id;
				}
				bool isLeaf(){return flag == LEAF;}
				bool isObject(){return flag == OBJECT;}
				void insert(Region * reg){regions.push_back(reg);}
				void print(){
					cout<<get<X>(limites[TOP])<<"->"<<get<Y>(limites[TOP])<<"->"<<get<X>(limites[LOWER])<<"->"<<get<Y>(limites[LOWER])<<endl;
				}
				int getId(){return id;}
				int flag;
				Point limites[2];
				vector<Region *> regions;
				Region * padre;
		};
		vector<Region *> searchNeighbors(Point query, int n);
        vector<Region *> searchRange(Point query, Distancia n);
	private:
		typedef tuple<Region *, Distancia> Distance;
        class minDistance{
                bool reverse;
            public:
                minDistance(const bool& revparam = false){ reverse = revparam; }
                bool operator() (const Distance &lhs, const Distance &rhs) const{
                    if (reverse) return (get<1>(lhs) > get<1>(rhs));
                    else return (get<1>(lhs) < get<1>(rhs));
                }
        };
        void test();
		bool overflow(Region * reg){return reg->regions.size() > M;}
		void nueva_area_rectangulos(Region * reg, Region * obj);
		void changePadre(Region * reg);
		vector<Point> _generatePoints(vector<Region*> vec);
		Region * _getMinArea(Region * reg, Region * obj);
		Area area_rectangulos(Region * reg, Region * obj);
		Region * generateRegion(vector<Point> v);
		tuple<Point,Point> getNewPoints(vector<Point> v);
		vector<Region *> getRegions();
		void resizeRegion(Region * reg);
		Region * root;
		int dimensions;
		int actualId;
		int M;
		typedef priority_queue<Distance, vector<Distance>, minDistance> minHeap;
};


vector<typename RTree::Region *> RTree::getRegions(){
	vector<Region *> res;
	vector<Region *> vec; 
	if(root == nullptr) return res;
	vec.push_back(root);
	while(vec.size() != 0){
		Region * iter = vec.front();
		vec.erase(vec.begin());
		res.push_back(root);
		if(!iter->isLeaf()){
			for(int i = 0; i < iter->regions.size(); i++){
				vec.push_back(iter->regions[i]);
			}
		}
	}
	return res;
}

void RTree::changePadre(Region * reg){
	for(int i = 0; i < reg->regions.size(); i++){
		reg->regions[i]->padre = reg;
	}
}

bool funComp(RTree::Region * a, RTree::Region * b){
	return get<X>(a->limites[LOWER]) < get<X>(b->limites[LOWER]);
}

void RTree::print(string file){
	string fi = file + ".dot";
	ofstream archivo(fi.c_str());
	archivo<<"digraph{graph [];node [fontsize = \"16\"shape = \"record\"];edge [];"<<endl;
	vector<Region *> vec;
	if(root != nullptr) vec.push_back(root);
	int regId = 1;
	while(vec.size() != 0){
		Region * iter = vec.front();
		vec.erase(vec.begin());
		string temp = "nodo" + to_string(iter->getId());
		archivo<<temp<<" [label = \"";
		temp.clear();
		for(int i = 0; i < iter->regions.size(); i++){
			string cc = "<c" + to_string(i) + "> ";
			if(iter->isLeaf()){
				string tempC = "(" + to_string(get<X>(iter->regions[i]->limites[TOP])) + "," + to_string(get<Y>(iter->regions[i]->limites[TOP])) + ";" + to_string(get<X>(iter->regions[i]->limites[LOWER])) + "," + to_string(get<Y>(iter->regions[i]->limites[LOWER])) + ") ";
				cc += tempC;
				cc += "|";
			}
			else{
				string tempC = "R" + to_string(regId);
				cc += tempC;
				cc += " |";
				regId++;
			}
			temp += cc;
		}
		temp.pop_back();
		temp += "\"];";
		archivo<<temp<<endl;
		if(!iter->isLeaf()){
			for(int i = 0; i < iter->regions.size(); i++){
				vec.push_back(iter->regions[i]);
				string tempC = "nodo" + to_string(iter->getId()) + ":c" + to_string(i) + " -> nodo" + to_string(iter->regions[i]->getId()) + ";";
				archivo<<tempC<<endl;
			}
		}
	}
	archivo<<"}";
	archivo.close();
	string command = "dot -Tpdf " + fi + " -o " + file + ".pdf";
	system(command.c_str());
}


vector<Point> RTree::_generatePoints(vector<Region*> vec){
	vector<Point> res;
	for(auto iter = vec.begin(); iter != vec.end(); ++iter){
		res.push_back((*iter)->limites[TOP]);
		res.push_back((*iter)->limites[LOWER]);
	}
	return res;
}

tuple<Point,Point> RTree::getNewPoints(vector<Point> v){
	Coordenada minx=get<0>(v[0]);
    Coordenada maxx=get<0>(v[0]);
    Coordenada miny=get<1>(v[0]);
    Coordenada maxy=get<1>(v[0]);
    for(int i=1; i<v.size();i++){
        if (get<0>(v[i])>maxx){maxx=get<0>(v[i]);}
        if (get<0>(v[i])<minx){minx=get<0>(v[i]);}
        if (get<1>(v[i])>maxy){maxy=get<1>(v[i]);}
        if (get<1>(v[i])<miny){miny=get<1>(v[i]);}
    }
    Point c1 = make_tuple(minx,maxy);
    Point c2 = make_tuple(maxx,miny);
    return make_tuple(c1,c2);
}

void RTree::resizeRegion(Region * reg){
	auto tt = getNewPoints(_generatePoints(reg->regions));
	reg->limites[TOP] = get<0>(tt);
	reg->limites[LOWER] = get<1>(tt);
}

typename RTree::Region * RTree::generateRegion(vector<Point> v){
    auto tt = getNewPoints(v);
    Point c1 = get<0>(tt);
    Point c2 = get<1>(tt);
    actualId++;
    return new Region(c1, c2, NO_LEAF,actualId-1);
}


void RTree::nueva_area_rectangulos(Region * reg, Region * obj){
	Point a1 = reg->limites[TOP];
	Point a2 = reg->limites[LOWER];
	Point b1 = obj->limites[TOP];
	Point b2 = obj->limites[LOWER];
    Point c1 = make_tuple(min(get<0>(a1),get<0>(b1)),max(get<1>(a1),get<1>(b1)));
    Point c2 = make_tuple(max(get<0>(a2),get<0>(b2)),min(get<1>(a2),get<1>(b2)));
    reg->limites[TOP] = c1;
    reg->limites[LOWER] = c2;
}


Area RTree::area_rectangulos(Region * reg, Region * obj){
	Point a1 = reg->limites[TOP];
	Point a2 = reg->limites[LOWER];
	Point b1 = obj->limites[TOP];
	Point b2 = obj->limites[LOWER];
    Area dx = max(get<0>(a2),get<0>(b2))-min(get<0>(a1),get<0>(b1));
    Area dy = max(get<1>(a1),get<1>(b1))-min(get<1>(a2),get<1>(b2));
    return dx*dy-(get<0>(a2)-get<0>(a1))*(get<1>(a1)-get<1>(a2));
}


typename RTree::Region * RTree::_getMinArea(Region * reg, Region * obj){
	Area minArea = -1;
	Region * actual = nullptr;
	for(auto iter = reg->regions.begin(); iter != reg->regions.end(); ++iter){
		Area temp = area_rectangulos(*iter, obj);
		if(minArea == -1 or temp < minArea){
			minArea = temp;
			actual = *iter;
		}
		else if(temp == 0){
			if(actual->regions.size() > (*iter)->regions.size()) actual = *iter;
		}
	}
	return actual;
}

void RTree::insert(Point top, Point lower){
	Region * nuevo = new Region(top,lower, OBJECT,actualId);
	actualId++;
	Region * iter = root;
	if(root == nullptr){
		root = new Region(top,lower,LEAF,actualId);
		actualId++;
		root->insert(nuevo);
		return;
	}
	while(true){
		if(iter->isLeaf()) break;
		iter = _getMinArea(iter, nuevo);
	}
	iter->insert(nuevo);
	Region * r2 = nuevo;
	while(iter != nullptr){
		if(overflow(iter)){
			sort(iter->regions.begin(),iter->regions.end(),funComp);
			vector<Region *> v1;
			vector<Region *> v2;
			int tam = iter->regions.size();
  			v1.resize(tam/2);
  			v2.resize(tam-tam/2);
  			memcpy(&(v1[0]),&(iter->regions[0]),sizeof(Region *)*tam/2);
  			memcpy(v2.data(),&iter->regions[tam/2], sizeof(Region *)*(tam-tam/2));
			Region * r1 = generateRegion(_generatePoints(v1));
			r2 = generateRegion(_generatePoints(v2));
			r1->regions = v1;
			r2->regions = v2;
			if(iter->isLeaf()){
				r1->flag = LEAF;
				r2->flag = LEAF;
			}
			if(iter->padre == nullptr){
				vector<Region *> temp = {r1,r2};
				root = generateRegion(_generatePoints(temp));
				root->insert(r1);
				root->insert(r2);
				r1->padre = root;
				r2->padre = root;
				changePadre(r1);
				changePadre(r2);
				break;
			}
			else{
				Region * tempP = iter->padre;
				cout<<tempP->getId()<<endl;
				iter->cpy(r1);
				tempP->insert(r2);
				r2->padre = tempP;
				iter->padre = tempP;
				changePadre(iter);
				changePadre(r2);
				iter = tempP;
				resizeRegion(iter);
			}
		}	
		else{
			resizeRegion(iter);
			iter = iter->padre;
		}
	}
}

vector<RTree::Region *> RTree :: searchNeighbors(Point query, int n){
    vector<Region *> results;
    Region *tmp = root;
    minHeap distances(minDistance(true));
    distances.push(make_tuple(tmp, distancia_rectangulos(tmp->limites[TOP], tmp->limites[LOWER], query, query))); // ?
    while(n){
        tmp = get<0>(distances.top());
        distances.pop();
        if(!tmp->isObject()){
            for(int i = 0; i < tmp->regions.size(); i++){
                distances.push(make_tuple(tmp->regions[i], distancia_rectangulos(tmp->regions[i]->limites[TOP], tmp->regions[i]->limites[LOWER], query, query)));
            }
        }else{
            results.push_back(tmp);
            n--;
        }
    }
    return results;
}

vector<RTree::Region *> RTree :: searchRange(Point query, Distancia n){
    vector<Region *> results;
    Region *tmp = root;
    minHeap distances(minDistance(true));
    distances.push(make_tuple(tmp, distancia_rectangulos(tmp->limites[TOP], tmp->limites[LOWER], query, query)));
    Distancia d;
    while(true){
        tmp = get<0>(distances.top());
        d = get<1>(distances.top());
        distances.pop();
        if(d <= n){
            if(!tmp->isObject()){
                for(int i = 0; i < tmp->regions.size(); i++){
                    distances.push(make_tuple(tmp->regions[i], distancia_rectangulos(tmp->regions[i]->limites[TOP], tmp->regions[i]->limites[LOWER], query, query)));
                }
            }else{
                results.push_back(tmp);
            }
        }else{
            break;
        }
    }
    return results;   
}

