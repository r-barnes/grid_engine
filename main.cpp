#include "grid_engine.h"
#include <iostream>
using namespace std;

int main(){
  typedef grid_engine::grid_engine<int> hex;
  hex hi(3,3);

  for(hex::parser i=hi.begin();i.good();i++)
    for(hex::nparser n=i.hexring(1,4);n.good();n++)
      cout<<*n<<endl;  

//  for(grid_engine::grid_engine<int>::niterator n=hi.niterator(1,1,1,1);n!=hi.end();n++)
//    cout<<*n<<endl;

/*
	hi(0,0)=3;
	hi(2,3)=7;

	for(conn8_engine<int>::neighbours n=hi.nbegin(4,4,3);n!=hi.nend(4,4,3);n++)
		*n=5;

	for(int y=0;y<10;y++){
		for(int x=0;x<10;x++)
			cout<<hi(x,y);
		cout<<endl;
	}
*/
}
