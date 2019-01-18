#include <iostream>
#include <string>
#include <time.h>

using namespace std;

enum Colores{RED,BLUE,YELLOW,BLACK,GREEN}; 
enum Num{NUM1 = 100, NUM2, NUM3, NUM4, NUM5};

int main()
{
  long sum = 0;
  clock_t ini = clock();
  for(long i = 0; i < 1000000000; i++){
      int key = i % 5;
      switch(key){
          case RED: sum += NUM1; break;
          case BLUE: sum += NUM2; break;
          case YELLOW: sum += NUM3; break;
          case BLACK: sum += NUM4; break;
          case GREEN: sum += NUM5; break;
      }
  }
  clock_t end = clock();
  double seconds = double(end - ini) / CLOCKS_PER_SEC;
  cout<<sum<<endl;
  cout<<seconds<<endl;
}
