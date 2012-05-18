#include "grid_engine.h"
#include <iostream>
using namespace std;

int main(){
	grid_engine<int> hi(4,5);

	cout<<hi(3,4)<<endl;
}
