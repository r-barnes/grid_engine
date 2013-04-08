#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

int main(int argc, char **argv){
  vector<int> xs, ys, begins;
  if(argc!=2){
    cerr<<argv[0]<<" <NUMBER OF RINGS>"<<endl;
    return -1;
  }

  int rings=atoi(argv[1]);
  xs.push_back(0);
  ys.push_back(0);
  begins.push_back(0);
  for(int r=1;r<rings;++r){
    begins.push_back(xs.size());
    for(int dx=-r;dx<=r;++dx){
      xs.push_back(dx);
      ys.push_back(-r);
    }
    for(int dy=-r+1;dy<=r;++dy){
      xs.push_back(r);
      ys.push_back(dy);
    }
    for(int dx=r-1;dx>=-r;--dx){
      xs.push_back(dx);
      ys.push_back(r);
    }
    for(int dy=r-1;dy>=-r+1;--dy){
      xs.push_back(-r);
      ys.push_back(dy);
    }
  }

  cout<<"d8_begins[]={"<<begins[0];
  for(int i=1;i<begins.size();++i)
    cout<<", "<<begins[i];
  cout<<"};"<<endl<<endl;

  cout<<"d8_dx[]={"<<xs[0];
  for(int i=1;i<xs.size();++i)
    cout<<", "<<xs[i];
  cout<<"};"<<endl<<endl;

  cout<<"d8_dy[]={"<<ys[0];
  for(int i=1;i<ys.size();++i)
    cout<<", "<<ys[i];
  cout<<"};"<<endl<<endl;
}
