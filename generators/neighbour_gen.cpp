//Each ring adds 6 neighbours

#include <iostream>
#include <queue>
#include <string>
#include <cstdlib>
#include <set>
using namespace std;

#define NF(y,column) (((column%2)==0)?(y):-(y)) //Used for hex

class grid_cell{
  public:
    int x,y;
    grid_cell(int x, int y) : x(x), y(y) {}
    grid_cell(){}
		bool operator< (const grid_cell& a) const { return x<a.x || (x==a.x && y<a.y); }
};

int main(int argc, char **argv){
  if(argc!=3){
    cout<<argv[0]<<" <hex/d8/d4> <NUMBER OF RINGS>"<<endl;
    return -1;
  }

  int maxrings=atoi(argv[2]);

	vector<int> dx,dy;
	if(argv[1][0]=='H'){
    //Even columns. Multiply y-coordinate by -1 to make this odd columns
    //Truth: The hexagonal data is stored in a 2D rectangular grid which is internally mapped to a hexagonal space. This mapping shifts columns with odd x-coordinates down a half-cell with respect to columns with even x-coordinates.
		//int dx_hex[6]={ 0, -1, 1, 0, -1,  1};
		//int dy_hex[6]={-1,  0, 0, 1, -1, -1};
		dx.push_back( 0); 		dy.push_back(-1);
		dx.push_back(-1); 		dy.push_back( 0);
		dx.push_back( 1); 		dy.push_back( 0);
		dx.push_back( 0); 		dy.push_back( 1);
		dx.push_back(-1); 		dy.push_back(-1);
		dx.push_back( 1); 		dy.push_back(-1);
	} else if (argv[1][1]=='8') {
		dx.push_back(-1);			dy.push_back(-1);
		dx.push_back( 0);			dy.push_back(-1);
		dx.push_back( 1);			dy.push_back(-1);
		dx.push_back(-1);			dy.push_back( 0);
		dx.push_back( 1);			dy.push_back( 0);
		dx.push_back(-1);			dy.push_back( 1);
		dx.push_back( 0);			dy.push_back( 1);
		dx.push_back( 1);			dy.push_back( 1);
	} else if (argv[1][1]=='4') {
		dx.push_back( 0);			dy.push_back(-1);
		dx.push_back(-1);			dy.push_back( 0);
		dx.push_back( 1);			dy.push_back( 0);
		dx.push_back( 0);			dy.push_back( 1);
	}

	queue<grid_cell> edge;
  vector<int> begins, xs, ys;
  set<grid_cell> used;
  int ring=0;

	edge.push(grid_cell(999999,999999));
	edge.push(grid_cell(0,0));
  used.insert(grid_cell(0,0));

  begins.push_back(0);
  xs.push_back(0);
  ys.push_back(0);

	while(true){
		grid_cell c=edge.front();
		edge.pop();

		if(c.x==999999){
      ring++;
      if(ring>=maxrings)
        break;
      begins.push_back(xs.size());
			edge.push(grid_cell(999999,999999));
			continue;
		}

		if(argv[1][0]=='h'){
			for(int n=0;n<dx.size();n++){	//Look through neighbours
				if (!used.count(grid_cell(c.x+dx[n],c.y+NF(dy[n],c.x)))) {
		      used.insert(grid_cell(c.x+dx[n],c.y+NF(dy[n],c.x)));
		      xs.push_back(c.x+dx[n]);
		      ys.push_back(c.y+NF(dy[n],c.x));
					edge.push(grid_cell(c.x+dx[n],c.y+NF(dy[n],c.x)));
				}
			}
		} else {
			for(int n=0;n<dx.size();n++){	//Look through neighbours
				if (!used.count(grid_cell(c.x+dx[n],c.y+dy[n]))) {
		      used.insert(grid_cell(c.x+dx[n],c.y+dy[n]));
		      xs.push_back(c.x+dx[n]);
		      ys.push_back(c.y+dy[n]);
					edge.push(grid_cell(c.x+dx[n],c.y+dy[n]));
				}
			}
		}
	}

  cout<<"int "<<argv[1]<<"::rlen0="<<begins.size()<<";"<<endl;
  cout<<"int "<<argv[1]<<"::nlen0="<<xs.size()<<";"<<endl;
  cout<<"int "<<argv[1]<<"::begins0[]={"<<begins[0];
  for(int i=1;i<begins.size();++i)
    cout<<", "<<begins[i];
  cout<<"};"<<endl<<endl;

  cout<<"int "<<argv[1]<<"::dx0[]={"<<xs[0];
  for(int i=1;i<xs.size();++i)
    cout<<", "<<xs[i];
  cout<<"};"<<endl<<endl;

  cout<<"int "<<argv[1]<<"::dy0[]={"<<ys[0];
  for(int i=1;i<ys.size();++i)
    cout<<", "<<ys[i];
  cout<<"};"<<endl<<endl;
}
