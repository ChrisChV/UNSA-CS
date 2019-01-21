#ifndef MYTIME_H
#define MYTIME_H

#include <iostream>
#include <ctime>

class MyTime{
	public:
		MyTime(){}
		void init(){begin_t = clock();}	
		void end(){end_t = clock();}
		double getTime(){return double(end_t - begin_t) / CLOCKS_PER_SEC;}
		void print(){
			double res = double(end_t - begin_t) / CLOCKS_PER_SEC;
			cout<<res<<"s"<<endl;
		}
	private:
		clock_t begin_t;
		clock_t end_t;
};

#endif