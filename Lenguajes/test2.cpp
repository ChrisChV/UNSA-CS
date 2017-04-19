#include <iostream>
#include <string>
#include <time.h>

using namespace std;

int main()
{
  long sum = 0;
  clock_t ini = clock();
  for(long i = 0; i < 1000000000; i++){
      int key = i % 5;
      switch(key){
          case 0: sum += 100; break;
          case 1: sum += 101; break;
          case 2: sum += 102; break;
          case 3: sum += 103; break;
          case 4: sum += 104; break;
      }

  }
  clock_t end = clock();
  double seconds = double(end - ini) / CLOCKS_PER_SEC;
  cout<<sum<<endl;
  cout<<seconds<<endl;
}
