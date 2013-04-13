#include "grid_engine.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
using namespace std;

typedef grid_engine::grid_engine<int> gtype;

void dispgrid(gtype &grid){
  for(int y=0;y<grid.height();++y){
    for(int x=0;x<grid.width();++x)
      cout<<setw(4)<<grid(x,y)<<" ";
    cout<<endl;
  }
}

int main(){
  bool passed=true;

  //Define a general 5x5 grid of integers
  gtype grid(5,5), grid2(5,5);

  //Set each cell in the grid to a random number
  int j=0;
  for(gtype::parser i=grid.begin();i.good();++i)
    *i=j++;

  //Assuming D8 connectivity...
  //Set each cell in the grid to the sum of its neighbours at distance 1
  grid2.fill(0);
  for(gtype::parser i=grid2.begin();i.good();++i)
    for(gtype::nparser n=i.d8ring(1);n.good();++n)
      *i+=grid(n);

  cout<<"D8 Ring 1 Sum Test: ";
  if(grid2.to_str()=="5 5 12 20 25 30 20 28 48 56 64 42 53 88 96 104 67 78 128 136 144 92 52 90 95 100 60")
    cout<<"Passed."<<endl;
  else {
    cout<<"Failed."<<endl;
    passed=false;
  }

  //Assuming D8 connectivity...
  //Set each cell in the grid to the sum of its neighbours at distance 2
  grid2.fill(0);
  for(gtype::parser i=grid2.begin();i.good();++i)
    for(gtype::nparser n=i.d8ring(2);n.good();++n)
      *i+=grid(n);

  cout<<"D8 Ring 2 Sum Test: ";
  if(grid2.to_str()=="5 5 42 57 78 57 48 69 90 127 88 75 102 131 192 133 114 69 80 137 78 75 72 87 138 87 78")
    cout<<"Passed."<<endl;
  else {
    cout<<"Failed."<<endl;
    passed=false;
  }

  //Assuming D8 connectivity...
  //Add to every cell the values of all neighbors at distances 2-3 (inclusive)
  grid2.fill(0);
  for(gtype::parser i=grid2.begin();i.good();++i)
    for(gtype::nparser n=i.d8ring(2,3);n.good();++n)
      *i+=grid(n);

  cout<<"D8 Ring 2 + Ring 3 Sum Test: ";
  if(grid2.to_str()=="5 5 132 169 163 157 136 197 246 237 228 199 167 201 192 183 169 137 156 147 138 139 152 179 173 167 156")
    cout<<"Passed."<<endl;
  else {
    cout<<"Failed."<<endl;
    passed=false;
  }

  //Assuming D8 connectivity...
  //Add to every cell the values of all neighbors at distance 1
  //Assume a toroidal grid
  grid2.fill(0);
  for(gtype::parser i=grid2.begin();i.good();++i)
    for(gtype::nparser n=i.d8tring(1);n.good();++n)
      *i+=grid(n);

  cout<<"D8 Ring 1 Toroid Sum Test: ";
  if(grid2.to_str()=="5 5 90 83 91 99 92 55 48 56 64 57 95 88 96 104 97 135 128 136 144 137 100 93 101 109 102")
    cout<<"Passed."<<endl;
  else {
    cout<<"Failed."<<endl;
    passed=false;
  }

  //Assuming D4 connectivity...
  //Add to every cell the values of all neighbors at distance 1
  grid2.fill(0);
  for(gtype::parser i=grid2.begin();i.good();++i)
    for(gtype::nparser n=i.d4ring(1);n.good();++n)
      *i+=grid(n);

  cout<<"D4 Ring 1 Sum Test: ";
  if(grid2.to_str()=="5 5 6 8 11 14 12 16 24 28 32 26 31 44 48 52 41 46 64 68 72 56 36 58 61 64 42")
    cout<<"Passed."<<endl;
  else {
    cout<<"Failed."<<endl;
    passed=false;
  }

  //Assuming hexagonal connectivity...
  //Add to every cell the values of all neighbors at distance 1
  grid2.fill(0);
  for(gtype::parser i=grid2.begin();i.good();++i)
    for(gtype::nparser n=i.hexring(1);n.good();++n)
      *i+=grid(n);

  cout<<"Hex Ring 1 Sum Test: ";
  if(grid2.to_str()=="5 5 6 20 11 30 12 17 46 32 58 29 37 76 62 88 49 57 106 92 118 69 52 58 95 64 60")
    cout<<"Passed."<<endl;
  else {
    cout<<"Failed."<<endl;
    passed=false;
  }

  if(!passed){
    cout<<"###################"<<endl;
    cout<<"###################"<<endl;
    cout<<"    FAILED TESTS   "<<endl;
    cout<<"###################"<<endl;
    cout<<"###################"<<endl;
  }
}
