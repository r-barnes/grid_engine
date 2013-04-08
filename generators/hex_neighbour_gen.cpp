/*
X-Coordinates
./a.out  | sed 's/^N [0-9]*://' | sed 's/,[^)]*\)/,/g' | sed 's/\(//g'
Y-Coordinates
./a.out  | sed 's/^N [0-9]*://' | sed 's/\([^,]*,//g' | sed 's/\)/,/g'

Each ring adds 6 neighbours, so the neighbours for a particular ring are at:
*/

#include <iostream>
#include <queue>
#include <string>
#include <cstdlib>
#include <set>

#define NF(y,column) (((column%2)==0)?(y):-(y))

using namespace std;

//Even columns. Multiply y-coordinate by -1 to make this odd columns
int dx_hex[6]={ 0, -1, 1, 0, -1,  1};
int dy_hex[6]={-1,  0, 0, 1, -1, -1};

class grid_cell{
  public:
    int x,y;
    grid_cell(int x, int y) : x(x), y(y) {}
    grid_cell(){}
    bool operator< (const grid_cell& a) const { return y<a.y || x<a.x; }
};

int main(int argc, char **argv){
  if(argc!=2){
    cout<<argv[0]<<" <NUMBER OF RINGS>"<<endl;
    return -1;
  }

  int maxrings=atoi(argv[1]);

	queue<grid_cell> hex_edge;
  vector<int> begins, xs, ys;
  set<grid_cell> used;
  int ring=0;

	hex_edge.push(grid_cell(-1,-1));
	hex_edge.push(grid_cell(0,0));

  begins.push_back(0);
  xs.push_back(0);
  ys.push_back(0);

	while(true){
		grid_cell c=hex_edge.front();
		hex_edge.pop();

		if(c.x==-1){
      ring++;
      if(ring>maxrings)
        break;
      begins.push_back(xs.size());
			hex_edge.push(grid_cell(-1,-1));
			continue;
		}

		for(int n=0;n<6;n++){	//Look through neighbours
			if (!used.count(grid_cell(c.x+dx_hex[n],c.y+NF(dy_hex[n],c.x)))) {
        used.insert(grid_cell(c.x+dx_hex[n],c.y+NF(dy_hex[n],c.x)));
        xs.push_back(c.x+dx_hex[n]);
        ys.push_back(c.y+NF(dy_hex[n],c.x));
				hex_edge.push(grid_cell(c.x+dx_hex[n],c.y+NF(dy_hex[n],c.x)));
			}
		}
	}

  cout<<"Begins:"<<endl;
  for(int i=0;i<begins.size();++i)
    cout<<begins[i]<<", ";
  cout<<endl<<endl;

  cout<<"X-offsets:"<<endl;
  for(int i=0;i<xs.size();++i)
    cout<<xs[i]<<", ";
  cout<<endl<<endl;

  cout<<"Y-offsets:"<<endl;
  for(int i=0;i<ys.size();++i)
    cout<<ys[i]<<", ";
  cout<<endl<<endl;
}
