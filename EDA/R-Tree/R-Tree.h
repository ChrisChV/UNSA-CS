#include <iostream>
#include <tuple>
#include <algorithm>

using namespace std;

typedef float Coordenada;
typedef tuple<Coordenada,Coordenada> Point
typedef float Distancia;
typedef float Area;

enum LIMIT_ENUM{TOP,LOWER};
enum COR_ENUM{X,Y};
enum FLAG_REGION_ENUM{LEAF,NO_LEAF,OBJECT};

bool funComp(Region * a, Region * b){
	return get<X>(a->limites[LOWER]) < get<X>(b->limites[LOWER]);
}

class RTree{
	public:
		void insert(Point top, Point lower);
	private:
		class Region{
			private:
				
			public:	
				Region(){padre = nullptr;}
				Region(Point top, Point lower, int flag){
					this->flag = flag;
					limites[TOP] = top;
					limites[LOWER] = lower;	
					padre = nullptr;
				}
				void cpy(Region * reg){
					flag = reg->flag;
					limites = reg->limites;
					regions = reg->regions;
					padre = reg->padre;
				}
				bool isLeaf(){return flag == LEAF;}
				bool isObject(){return flag == OBJECT;}
				void insert(Region * reg){regions.push_back(reg);}
				int flag;
				Point limites[2];
				vector<Region *> regions;
				Region * padre;
		};
		bool overflow(Region * reg){return reg->regions.size() > M;}
		void nueva_area_rectangulos(Region * reg, Region * obj);
		vector<Point> _generatePoints(vector<Region*> vec);
		Region * _getMinArea(Region * reg, Region * obj);
		Area area_rectangulos(Region * reg, Region * obj);
		Region * generateRegion(vector<Punto> v);
		Region * root;
		int dimensions;
		int M;
};


vector<Point> _generatePoints(vector<Region*> vec){
	vector<Point> res;
	for(auto iter = vec.begin(); iter != vec.end(); ++iter){
		res.push_back((*iter)->limites[TOP]);
		res.push_back((*iter)->limites[LOWER]);
	}
	return res;
}

typename RTree::Region * RTree::generateRegion(vector<Punto> v){
    Coordenada minx=get<0>(v[0]);
    Coordenada maxx=get<0>(v[0]);
    Coordenada minx=get<1>(v[0]);
    Coordenada maxx=get<1>(v[0]);
    for(int i=1; i<v.size();i++){
        if (get<0>(v[i])>maxx){maxx=get<0>(v[i]);}
        if (get<0>(v[i])<minx){minx=get<0>(v[i]);}
        if (get<1>(v[i])>maxy){maxy=get<1>(v[i]);}
        if (get<1>(v[i])<miny){miny=get<1>(v[i]);}
    }
    Punto c1 = make_tuple(minx,maxy);
    Punto c2 = make_tuple(maxx,miny);
    return new Region(c1, c2, NO_LEAF);
}


void RTree::nueva_area_rectangulos(Region * reg, Region * obj){
	Punto a1 = reg->limites[TOP];
	Punto a2 = reg->limites[LOWER];
	Punto b1 = obj->limites[TOP];
	Punto b2 = obj->limites[LOWER];
    Punto c1 = make_tuple(min(get<0>(a1),get<0>(b1)),max(get<1>(a1),get<1>(b1)));
    Punto c2 = make_tuple(max(get<0>(a2),get<0>(b2)),min(get<1>(a2),get<1>(b2)));
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
    return dx*dy-(get<0>(a2)-get<0>(a1))*(get<1>(a1)-get<1>(a2))
}


typename RTree::Region * _getMinArea(Region * reg, Region * obj){
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
	Region * nuevo = new Region(top,lower, OBJECT);
	Region * iter = root;
	if(root == nullptr){
		root = new Region(top,lower,LEAF);
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
  			cout<<tam/2<<endl;
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
				root->insert(r1,r2);
				r1->padre = root;
				r2->padre = root;
				break;
			}
			else{
				Region * tempP = iter->padre;
				iter->cpy(r1);
				tempP->insert(r2);
				iter = tempP; 
			}
		}	
		else{
			nueva_area_rectangulos(iter,r2);
			iter = iter->padre;
		} 
	}
}
