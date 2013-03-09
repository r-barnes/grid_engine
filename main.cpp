#include "grid_engine.h"
#include <iostream>
using namespace std;

int main(){
	conn8_engine<int> hi(10,10);

	hi(0,0)=3;
	hi(2,3)=7;

	for(conn8_engine<int>::neighbours n=hi.nbegin(4,4,3);n!=hi.nend(4,4,3);n++)
		*n=5;

	for(int y=0;y<10;y++){
		for(int x=0;x<10;x++)
			cout<<hi(x,y);
		cout<<endl;
	}
}
