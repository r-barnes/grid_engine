#include "grid_engine.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int main(){
  //Define a general 40x40 grid of integers
  typedef grid_engine::grid_engine<int> gtype;
  gtype grid(40,40);

  //Set each cell in the grid to a random number
  for(gtype::parser i=grid.begin();i.good();++i)
    *i=rand();

  //Set each cell in the grid to the average value of itself and its neighbours
  for(gtype::parser i=grid.begin();i.good();++i)
    for(gtype::nparser n=i.d8ring(1);n.good();++n)
      *i+=*n;

  //Add to every cell the values of all neighbors within 5
  for(gtype::parser i=grid.begin();i.good();++i)
    for(gtype::nparser n=i.d8ring(1,5);n.good();++n)
      *i+=*n;

  //Add to every cell the values of all neighbors at distance 5
  for(gtype::parser i=grid.begin();i.good();++i)
    for(gtype::nparser n=i.d8ring(5);n.good();++n)
      *i+=*n;

  //Add to every cell the values of all neighbors at distances 3-5 (inclusive)
  for(gtype::parser i=grid.begin();i.good();++i)
    for(gtype::nparser n=i.d8ring(3,5);n.good();++n)
      *i+=*n;

  //Assuming D4 connectivity...
  //Add to every cell the values of all neighbors at distances 3-5 (inclusive)
  for(gtype::parser i=grid.begin();i.good();++i)
    for(gtype::nparser n=i.d4ring(3,5);n.good();++n)
      *i+=*n;

  //Assuming hexagonal connectivity...
  //Add to every cell the values of all neighbors at distances 3-5 (inclusive)
  for(gtype::parser i=grid.begin();i.good();++i)
    for(gtype::nparser n=i.hexring(3,5);n.good();++n)
      *i+=*n;
}
