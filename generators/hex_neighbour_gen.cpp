/*
X-Coordinates
./a.out  | sed 's/^N [0-9]*://' | sed 's/,[^)]*\)/,/g' | sed 's/\(//g'
Y-Coordinates
./a.out  | sed 's/^N [0-9]*://' | sed 's/\([^,]*,//g' | sed 's/\)/,/g'

Each ring adds 6 neighbours, so the neighbours for a particular ring are at:
*/

#include <cstdio>
#include <queue>
#include <string>
#include <cstdlib>

#define MAX_DIM  100
#define OX MAX_DIM/2
#define OY MAX_DIM/2
#define IN_GRID(x,y) (x>=0 && y>=0 && x<MAX_DIM && y<MAX_DIM)
#define NF(y,column) (((column%2)==0)?(y):-(y))

using namespace std;

//Even columns. Multiply y-coordinate by -1 to make this odd columns
int dx_hex[6]={0,-1,1,0,-1,1};
int dy_hex[6]={-1,0,0,1,-1,-1};

typedef struct grid_cell_type {
	int x;
	int y;
	grid_cell_type(int x0, int y0):x(x0),y(y0){}
	grid_cell_type(){}
} grid_cell;

int main(){
	int stat[MAX_DIM][MAX_DIM];
	int ncount;
	setbuf(stderr,NULL);
	queue<grid_cell> hex_edge;

	//Reset stat
	for(int x=0;x<MAX_DIM;x++)
	for(int y=0;y<MAX_DIM;y++)
		stat[x][y]=0;

	hex_edge.push(grid_cell(-1,-1));
	hex_edge.push(grid_cell(OX,OY));
	stat[OX][OY]=ncount=1;

	while(true){
		grid_cell c=hex_edge.front();
		hex_edge.pop();

		if(c.x==-1){
			ncount++;
			if(ncount==OX)
				return 0;
			hex_edge.push(grid_cell(-1,-1));
			printf("\nN %d:",ncount-1);
			continue;
		}

		for(int n=0;n<6;n++){	//Look through neighbours
			if(!IN_GRID(c.x+dx_hex[n],c.y+NF(dy_hex[n],c.x))){
				printf(" OUT OF BOUNDS!\n");
				return 0;
			} else if (stat[c.x+dx_hex[n]][c.y+NF(dy_hex[n],c.x)]==0) {
				printf(" (%d,%d)",(c.x+dx_hex[n])-OX,(c.y+NF(dy_hex[n],c.x))-OY);
				stat[c.x+dx_hex[n]][c.y+NF(dy_hex[n],c.x)]=ncount;
				hex_edge.push(grid_cell(c.x+dx_hex[n],c.y+NF(dy_hex[n],c.x)));
			}
		}
	}
}
