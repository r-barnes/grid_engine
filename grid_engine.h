#include <vector>
#include <iostream>
#include <cassert>

#include "neighbours.h"

namespace grid_engine {

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
          neighbours N;
          int i, x0, y0, outer_ring, current_ring;

          void advance_until_valid(){
            while(!my_ge.in_grid(x0+N.dx[i],y0+N.dy[i])){
              ++i;
              if(i==N.nlen || i==N.begins[outer_ring+1]){
                i=-1;
                return;
              }
            }
          }
		    public:
			    nparser ( grid_engine<T> &ge, neighbours N, int x, int y, int inner_ring, int outer_ring) : my_ge(ge), N(N), x0(x), y0(y), outer_ring(outer_ring), current_ring(inner_ring) {
            assert(ge.in_grid(x,y));
            assert(current_ring>0);
            assert(outer_ring>0);
            assert(outer_ring>=current_ring);

            i=0;
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
				    return my_ge(x0+N.dx[i],y0+N.dy[i]);
			    }
          bool good() const {
            return i!=-1;
          }
          int x() const {
            return x0+N.dx[i];
          }
          int y() const {
            return y0+N.dy[i];
          }
	    };

		  class parser {
			  private:
				  grid_engine<T> &my_ge;
				  int x0,y0;
			  public:
				  parser (grid_engine<T> &ge, int x0, int y0) : my_ge(ge), x0(x0), y0(y0) {}

				  reference       operator*(){ return my_ge(x,y); }
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
          grid_engine<T>::nparser hexring(int inner_ring, int outer_ring) const {
            typename grid_engine<T>::nparser temp(my_ge, hex(), x0, y0, inner_ring, outer_ring);
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

  template<class T>
  class d8_engine : public grid_engine<T>{
    public:
		  d8_engine (int width, int height) : grid_engine<T>(width,height) {}

	  typename grid_engine<T>::nparser nring(int x, int y, int inner_ring, int outer_ring) const {
      return grid_engine<T>::nparser(*this, x, y, inner_ring, outer_ring);
	  }
  };

}
