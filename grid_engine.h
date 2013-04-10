#include <vector>
#include <iostream>
#include <cassert>

namespace grid_engine {

class neighbours {
  public:
    virtual int dx(int i)     const { return -1; }
    virtual int dy(int i)     const { return -1; }
    virtual int begins(int i) const { return -1; }
    virtual int nlen()        const { return -1; }
    virtual int rlen()        const { return -1; }
};

class hex : public neighbours {
  private:
    static int begins0[],dx0[],dy0[],rlen0,nlen0;
  public:
    int begins(int i) const { return begins0[i]; }
    int dx    (int i) const { return dx0[i];     }
    int dy    (int i) const { return dy0[i];     }
    int rlen  ()      const { return rlen0;      }
    int nlen  ()      const { return nlen0;      }
};

class d8 : public neighbours {
  private:
    static int begins0[],dx0[],dy0[],rlen0,nlen0;
  public:
    int begins(int i) const { return begins0[i]; }
    int dx    (int i) const { return dx0[i];     }
    int dy    (int i) const { return dy0[i];     }
    int rlen  ()      const { return rlen0;      }
    int nlen  ()      const { return nlen0;      }
};

class d4 : public neighbours {
  private:
    static int begins0[],dx0[],dy0[],rlen0,nlen0;
  public:
    int begins(int i) const { return begins0[i]; }
    int dx    (int i) const { return dx0[i];     }
    int dy    (int i) const { return dy0[i];     }
    int rlen  ()      const { return rlen0;      }
    int nlen  ()      const { return nlen0;      }
};

}

#include "neighbours.h"

namespace grid_engine{

  d8 nd8;
  d4 nd4;
  hex nhex;

  template <class T>
  class grid_engine {
	  typedef std::vector< std::vector<T> > arr2d;
	  private:
		  arr2d data;
	  public:
		  typedef T		     value_type;
		  typedef T*       pointer;
		  typedef const T* const_pointer;
		  typedef T&       reference;
		  typedef const T& const_reference;
		  typedef int		   size_type;

		  void resize(int Gwidth, int Gheight){
			  data.resize(Gheight, std::vector<T> (Gwidth));
		  }

		  void resize(int Gwidth, int Gheight, reference default_item){
			  data.resize(Gheight, std::vector<T> (Gwidth, default_item));
		  }

      bool in_grid(int x, int y) const {
        return (x>=0 && y>=0 && x<width() && y<height());
      }

		  size_type width() const
			  {return data[0].size();}
		  size_type height() const
			  {return data.size();}
		  grid_engine() {};
		  grid_engine(int Gwidth, int Gheight){
			  resize(Gwidth,Gheight);
		  }
		  reference operator()(int x, int y)
			  {return data[y][x];}
		  const_reference operator()(int x, int y) const
			  {return data[y][x];}
		  void clear()
			  {data.clear();}

	    class nparser {
		    private:
			    grid_engine<T> &my_ge;
          neighbours *N;
          int i, x0, y0, outer_ring, current_ring;
          int curx, cury;
          bool toroid;

          bool valid(){
            if(toroid)
              return true;
            else
              return my_ge.in_grid(curx, cury);
          }

          void advance_until_valid(){
            assert(i!=-1);
            do {
              ++i;
              if(i==N->nlen() || i==N->begins(outer_ring+1)){
                i=-1;
                return;
              }
              curx=x0+N->dx(i);
              cury=y0+N->dy(i);
            } while(!valid());

            curx=curx%my_ge.width();
            cury=cury%my_ge.height();
          }
		    public:
			    nparser ( grid_engine<T> &ge, neighbours *N, int x0, int y0, int inner_ring, int outer_ring, bool toroid=false) : my_ge(ge), N(N), x0(x0), y0(y0), outer_ring(outer_ring), current_ring(inner_ring), toroid(toroid) {
            assert(ge.in_grid(x0,y0));
            assert(current_ring>0);
            assert(outer_ring>0);
            assert(outer_ring>=current_ring);

            i=N->begins(inner_ring);
            if(!valid())
              advance_until_valid();
          }
			    nparser& operator++(){   //Prefix ++
            advance_until_valid();
            return *this;
			    }
			    nparser operator++(int){ //Postfix ++
				    nparser tmp=*this;
				    ++*this;
				    return tmp;
			    }
			    value_type operator*() const {
				    return my_ge(curx,cury);
			    }
          bool good() const {
            return i!=-1;
          }
          int x() const {
            return curx;
          }
          int y() const {
            return cury;
          }
	    };

		  class parser {
			  private:
				  grid_engine<T> &my_ge;
				  int x0,y0;
			  public:
				  parser (grid_engine<T> &ge, int x0, int y0) : my_ge(ge), x0(x0), y0(y0) {}

				  reference operator*(){ return my_ge(x0,y0); }
				  parser& operator++(){   //Prefix ++
					  ++x0;
					  if(x0==my_ge.width()){
						  x0=0;
						  ++y0;
					  }
					  return *this;
				  }
          bool good() const {
            return x0<my_ge.width() && y0<my_ge.height();
          }
				  parser operator++(int){ //Postfix ++
					  parser tmp=*this;
					  ++*this;
					  return tmp;
				  }

          grid_engine<T>::nparser hexring(int inner_ring, int outer_ring=-1) const {
            if(outer_ring==-1) outer_ring=inner_ring;
            typename grid_engine<T>::nparser temp(my_ge, &nhex, x0, y0, inner_ring, outer_ring);
            return temp;
          }
          grid_engine<T>::nparser d8ring(int inner_ring, int outer_ring=-1) const {
            if(outer_ring==-1) outer_ring=inner_ring;
            typename grid_engine<T>::nparser temp(my_ge, &nd8, x0, y0, inner_ring, outer_ring);
            return temp;
          }
          grid_engine<T>::nparser d4ring(int inner_ring, int outer_ring=-1) const {
            if(outer_ring==-1) outer_ring=inner_ring;
            typename grid_engine<T>::nparser temp(my_ge, &nd4, x0, y0, inner_ring, outer_ring);
            return temp;
          }

          grid_engine<T>::nparser hextring(int inner_ring, int outer_ring=-1) const {
            if(outer_ring==-1) outer_ring=inner_ring;
            typename grid_engine<T>::nparser temp(my_ge, &nhex, x0, y0, inner_ring, outer_ring, true);
            return temp;
          }
          grid_engine<T>::nparser d8tring(int inner_ring, int outer_ring=-1) const {
            if(outer_ring==-1) outer_ring=inner_ring;
            typename grid_engine<T>::nparser temp(my_ge, &nd8, x0, y0, inner_ring, outer_ring, true);
            return temp;
          }
          grid_engine<T>::nparser d4tring(int inner_ring, int outer_ring=-1) const {
            if(outer_ring==-1) outer_ring=inner_ring;
            typename grid_engine<T>::nparser temp(my_ge, &nd4, x0, y0, inner_ring, outer_ring, true);
            return temp;
          }

          value_type operator*() const {
            return my_ge(x0,y0);
          }
          int x() const {
            return x0;
          }
          int y() const {
            return y0;
          }
		  };

		  parser begin() {return parser(*this, 0, 0);}
  };
}
