#include <vector>
#include <iostream>
#include <cassert>

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

		  class iterator : public std::iterator < std::forward_iterator_tag, T, std::ptrdiff_t > {
			  private:
				  grid_engine<T> &my_ge;
				  int x,y;
			  public:
				  iterator (grid_engine<T> &ge, int x0, int y0) : my_ge(ge), x(x0), y(y0) {}

				  reference operator*(){ return my_ge(x,y); }
				  bool operator==(const iterator &other){
					  return (x==other.x && y==other.y); //TODO: my_ge==other.my_ge ???
				  }
				  bool operator!=(const iterator &other){ return !operator==(other); }
				  iterator& operator++(){
					  ++x;
					  if(x==my_ge.width()){
						  x=0;
						  ++y;
					  }
					  return *this;
				  }
				  iterator operator++(int){ //TODO: WTF?
					  iterator tmp=*this;
					  ++*this;
					  return tmp;
				  }
		  };

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
		  iterator begin()
        {return iterator(*this, 0, 0);}
		  iterator end()
        {return iterator(*this, 0, height()-1);}
  };




  template <class T>
  class hex_engine : public grid_engine<T> {
	  public:
		
  };




  template <class T>
  class conn8_engine : public grid_engine<T>{
	  typedef typename grid_engine<T>::reference reference;
	  public:
		  conn8_engine (int width, int height) : grid_engine<T>(width,height) {}
		  class niterator {
			  private:
				  conn8_engine<T> &my_ge;
				  int dx,dy;
          int x0,y0;
          int outer_ring;
				  int current_ring;
          bool started;
          void advance_until_vaid(){
            while(!in_grid(x,y))
              next_neighbour();
          }
          void next_neighbour(){
            if(started && dy==-current_ring){
              ++current_ring;
              dx=-current_ring;
              dy=-current_ring;
              if(current_ring>outer_ring)
                x0=-1;
            } else if(dy==-current_ring){ //On the top row
              ++dx;
              if(dx>current_ring){ //Past the right of the top row
                dx=current_ring;
                ++dy;
              }
            } else if (dx==current_ring){ //On the right row
              --dy;
              if(dy>current_ring){ //Past the bottom of the right row
                dy=current_ring;
                --dx;
              }
            } else if (dy==current_ring){ //On the bottom row
              --dx;
              if(dx<-current_ring){ //Past the left of the bottom row
                dx=-current_ring;
                --dy;
                started=true;
              }
            } else if (dx==-current_ring){ //On the left row
              --dy;
            }
          }
			  public:
				  niterator ( conn8_engine<T> &ge, int x, int y, int inner_ring, int outer_ring) : my_ge(ge), current_ring(inner_ring), outer_ring(outer_ring), x0(x), y0(y) {
            assert(in_grid(x,y));
            assert(current_ring>0);
            assert(outer_ring>0);
            dy=-inner_ring;
            dx=-inner_ring;
            advance_until_valid();
            started=false;
          }

				  bool operator==(const neighbours &other){
					  return (x0==other.x0 && y0==other.y0 && outer_ring==other.outer_ring && current_ring==other.current_ring && dx==other.dx && dy==other.dy); //TODO: my_ge==other.my_ge ???
				  }
				  bool operator!=(const neighbours &other){
					  return !(operator==(other));
				  }
				  niterator& operator++(){
            do {
              next_neighbour();
            } while (!in_grid(x,y));
				  }
				  niterator operator++(int){
					  neighbours tmp=*this;
					  ++*this;
					  return tmp;
				  }
				  niterator operator*() const {
					  return my_ge(x+dx,y+dy);
				  }
          void make_end(){
            x0=-1;
            current_ring=outer_ring+1;
            dx=-current_ring;
            dy=-current_ring;
          }
		  };

		  neighbours nbegin(int x, int y, int inner_ring, int outer_ring) const {
        return neighbours(*this, x, y, inner_ring, outer_ring);
		  }

		  neighbours nend() const {
        
			  return neighbours(;
		  }




  /*




		  class ring {
			  private:
				  conn8_engine<T> &my_ge;
				  int x,y;
				  int ring;
				  int dx,dy;
			  public:
				  ring ( conn8_engine<T> &ge, int x0, int y0, int ring0, int dx0, int dy0) : my_ge(ge), x(x0), y(y0), ring(ring0), dx(dx0), dy(dy0) {}

				  bool operator==(const neighbours &other){
					  return (x==other.x && y==other.y && ring==other.ring && dx==other.dx && dy==other.dy); //TODO: my_ge==other.my_ge ???
				  }
				  bool operator!=(const neighbours &other){
					  return !(operator==(other));
				  }
				  ring& operator++(){ //TODO
					  if(dy==-ring){
						  ++dx;
						  if(dy>ring){
							

					  return *this;
				  }
				  neighbours operator++(int){
					  neighbours tmp=*this;
					  ++*this;
					  return tmp;
				  }
				  reference operator*(){
					  std::cerr<<"Dereferencing"<<x<<" "<<y<<std::endl;
					  return my_ge(x+dx,y+dy);
				  }
		  };

		  ring rbegin(int x, int y, int ring){
			  if(within<1)
				  throw "Neighbours must be >=-1";
			  else
				  return ring(*this, x, y, ring, -ring, -ring);
		  }

		  ring rend(int x, int y, int ring){
			  return ring(*this, x, y, -1, ring, ring);
		  }*/
  };
}
