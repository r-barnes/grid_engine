#ifndef _rbarnes_grid_engine_
#define _rbarnes_grid_engine_

#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <string>
#include <sstream>

namespace grid_engine {

///Base class facilitating polymorphism of Hex, D4, and D8
class neighbours {
  public:
    virtual int dx(int i)     const { return -1; }
    virtual int dy(int i)     const { return -1; }
    virtual int begins(int i) const { return -1; }
    virtual int nlen()        const { return -1; }
    virtual int rlen()        const { return -1; }
};

///Used to store hexagonal neighbours at varying distances
class Hex : public neighbours {
  private:
    static int begins0[],dx0[],dy0[],rlen0,nlen0;
  public:
    int begins(int i) const { return begins0[i]; }
    int dx    (int i) const { return dx0[i];     }
    int dy    (int i) const { return dy0[i];     }
    int rlen  ()      const { return rlen0;      }
    int nlen  ()      const { return nlen0;      }
};

///Used to store D8 neighbours at varying distances
class D8 : public neighbours {
  private:
    static int begins0[],dx0[],dy0[],rlen0,nlen0;
  public:
    int begins(int i) const { return begins0[i]; }
    int dx    (int i) const { return dx0[i];     }
    int dy    (int i) const { return dy0[i];     }
    int rlen  ()      const { return rlen0;      }
    int nlen  ()      const { return nlen0;      }
};

///Used to store D4 neighbours at varying distances
class D4 : public neighbours {
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

#include "grid_engine_neighbours.h"

namespace grid_engine{

  D8 nd8;   ///< Singleton instance of the D8 neighbourhood
  D4 nd4;   ///< Singleton instance of the D4 neighbourhood
  Hex nhex; ///< Singleton instance of the Hex neighbourhood

  ///Maps an integer i to the range [0,max)
  static inline int toroidabs(int i, int max){
    if(i>=0)
      return i%max;
    else
      return max-((-i)%max);
  }

  ///A grid of data
  template <class T>
  class grid_engine {
	  typedef std::vector< std::vector<T> > arr2d;
	  private:
		  arr2d data; ///< Storage variable for the grid
	  public:
		  typedef T		     value_type;
		  typedef T*       pointer;
		  typedef const T* const_pointer;
		  typedef T&       reference;
		  typedef const T& const_reference;
		  typedef int		   size_type;

      ///Used to examine neighbours at varying distances around a central cell
	    class nparser {
		    private:
			    grid_engine<T> &my_ge;
          neighbours *N;  ///< Connectivity class of the grid ( Hex, D4, D8 )
          int i;          ///< Neighbour currently being considered
          int x0;         ///< x-coordinate of center of neighbourhood
          int y0;         ///< y-coordinate of center of neighbourhood
          int outer_ring; ///< Outermost (inclusive) ring of neighbours to consider
          int curx;       ///< Current x-coordinate of the parser
          int cury;       ///< Current y-coordinate of the parser
          bool toroid;    ///< If true, the neighbourhood wraps around the grid's edges
          ///True if the parser's current coordinates are within the grid. Always true for a torus.
          bool valid() const;
          ///Increment the parser until it points at a coordinate within the grid
          void advance_until_valid();
		    public:
          /**
            @brief Makes a new neighbour parser
            @param[in] ge      Grid the neighbour parser refers to
            @param[in] N       Connectivity class of the grid ( Hex, D4, D8 )
            @param[in] x0      x-coordinate of center of neighbourhood
            @param[in] y0      y-coordinate of center of neighbourhood
            @param[in] inner_ring   Innermost ring of neighbours to consider
            @param[in] outer_ring   Outermost (inclusive) ring of neighbours to consider
            @param[in] toroid  If true, then the neighbourhoods wrap around grid's edges
          */
			    nparser ( grid_engine<T> &ge, neighbours *N, int x0, int y0, int inner_ring, int outer_ring, bool toroid=false);
          ///Increments the parser to the next neighbour
			    nparser& operator++();
          ///Increments the parser to the next neighbour
			    nparser operator++(int);
          ///Reference to the data at the coordinate the parser is currently pointing it
			    reference operator*() const;
          ///Returns true if the parser can be safely incremented again
          bool good() const;
          ///Curent x-coordinate of parser
          int x()  const;
          ///Curent y-coordinate of the parser
          int y()  const;
          ///Curent x-offset of parser
          int dx() const;
          ///Curent y-offset of parser
          int dy() const;
	    };

      ///Used to examine the cells of a grid
		  class parser {
			  private:
				  grid_engine<T> &my_ge;
				  int x0; ///< Current x-coordinate of the parser
          int y0; ///< Current y-coordinate of the parser
			  public:
				  parser (grid_engine<T> &ge, int x0, int y0);
          ///Reference to the data at the coordinate the parser is currently pointing it
				  reference operator*() const;
          ///Increments the parser to the next coordinate
				  parser& operator++();
          ///Increments the parser to the next coordinate
				  parser operator++(int);
          ///Returns true if the parser can be safely incremented again
          bool good() const;
          ///Returns a hexagonal neighbour parser
          grid_engine<T>::nparser hexring(int inner_ring, int outer_ring) const;
          ///Returns a D8 neighbour parser
          grid_engine<T>::nparser d8ring(int inner_ring, int outer_ring) const;
          ///Returns a D4 neighbour parser
          grid_engine<T>::nparser d4ring(int inner_ring, int outer_ring) const;
          ///Returns a hexagonal toroidal neighbour parser
          grid_engine<T>::nparser hextring(int inner_ring, int outer_ring) const;
          ///Returns a D8 toroidal neighbour parser
          grid_engine<T>::nparser d8tring(int inner_ring, int outer_ring) const;
          ///Returns a D4 toroidal neighbour parser
          grid_engine<T>::nparser d4tring(int inner_ring, int outer_ring) const;
          ///Parser's x-coordinate
          int x() const;
          ///Parser's y-coordinate
          int y() const;
		  };

      ///Resize grid. Non-destructive during growth.
		  void resize(int Gwidth, int Gheight);
      ///Resize grid. Non-destructive during growth, only new elements are set to default.
		  void resize(int Gwidth, int Gheight, reference default_item);
      ///Returns true if (x,y) is within the bounds of the grid
      bool in_grid(int x, int y) const;
      ///Width of the grid
		  size_type width() const;
      ///Height of the grid
		  size_type height() const;
      ///Construct a new grid
		  grid_engine();
      ///Construct a new grid of the given width and height
		  grid_engine(int Gwidth, int Gheight);
      ///Construct a new grid of the given width and height, all cells set to default
		  grid_engine(int Gwidth, int Gheight, reference default_item);
      ///Destory all data in the grid
		  void clear();
      ///Parser pointing to the first cell of the grid
		  parser begin();
      ///Reference to the data at (x,y)
		  reference operator()(int x, int y);
      ///Reference to the data at the grid cell the neighbour parser is currently pointing it
		  reference operator()(const nparser &n);
      ///Reference to the data at the grid cell the parser is currently pointing it
		  reference operator()(const parser &n);
      ///Constant Reference to the data at (x,y)
		  const_reference operator()(int x, int y) const;
      ///Set the entire grid to val
      void fill(const value_type &val);
      /**@brief Print the grid in a well-defined string format; can be read with from_str().

         The format is <WIDTH> <HEIGHT> (x0,y0) (x1,y0) ... (xN,y0) (x0,y1) (x1,y1) ... (xN,yN)

         For example:
         <pre>
           1 2 3
           4 5 6
           7 8 9
         </pre>
         becomes: 3 3 1 2 3 4 5 6 7 8 9
      */
      std::string to_str() const;
      ///Read a grid printed with to_str()
      void from_str(const std::string &str);
  };


////////////////////
//grid_engine
////////////////////

  template <class T>
  typename grid_engine<T>::parser
  grid_engine<T>::begin() {return parser(*this, 0, 0);}

  template <class T>
  typename grid_engine<T>::reference
  grid_engine<T>::operator()(int x, int y){
    assert(in_grid(x,y));
    return data[y][x];
  }

  template <class T>
  typename grid_engine<T>::reference
  grid_engine<T>::operator()(const nparser &n){
    assert(in_grid(n.x(),n.y()));
    return data[n.y()][n.x()];
  }

  template <class T>
  typename grid_engine<T>::reference
  grid_engine<T>::operator()(const parser &n){
    assert(in_grid(n.x(),n.y()));
    return data[n.y()][n.x()];
  }

  template <class T>
  typename grid_engine<T>::const_reference
  grid_engine<T>::operator()(int x, int y) const {
    assert(in_grid(x,y));
    return data[y][x];
  }

  template <class T>
  void grid_engine<T>::fill(const value_type &val) {
    for(typename std::vector< std::vector<T> >::iterator i=data.begin();i!=data.end();++i)
      std::fill((*i).begin(),(*i).end(),val);
  }

  template <class T>
  std::string grid_engine<T>::to_str() const {
    std::stringstream ss;
    ss<<width()<<" "<<height();
    for(int y=0;y<height();++y)
    for(int x=0;x< width();++x)
      ss<<" "<<data[y][x];
    return ss.str();
  }

  template <class T>
  void grid_engine<T>::from_str(const std::string &str){
    std::stringstream ss(str);
    int the_width,the_height;
    ss>>width>>height;
    resize(width,height);
    for(int y=0;y<height();++y)
    for(int x=0;x< width();++x)
      ss>>data[y][x];
  }

  template <class T>
  void grid_engine<T>::resize(int Gwidth, int Gheight){
	  data.resize(Gheight, std::vector<T> (Gwidth));
    for(typename std::vector< std::vector<T> >::iterator i=data.begin();i!=data.end();++i)
      (*i).resize(Gwidth);
  }

  template <class T>
  void grid_engine<T>::resize(int Gwidth, int Gheight, reference default_item){
	  data.resize(Gheight, std::vector<T> (Gwidth, default_item));
    for(typename std::vector< std::vector<T> >::iterator i=data.begin();i!=data.end();++i)
      (*i).resize(Gwidth, default_item);
  }

  template <class T>
  bool grid_engine<T>::in_grid(int x, int y) const {
    return (x>=0 && y>=0 && x<width() && y<height());
  }

  template <class T>
  typename grid_engine<T>::size_type
  grid_engine<T>::width() const {
    return data[0].size();
  }

  template <class T>
  typename grid_engine<T>::size_type
  grid_engine<T>::height() const {
    return data.size();
  }

  template <class T>
  grid_engine<T>::grid_engine() {};

  template <class T>
  grid_engine<T>::grid_engine(int Gwidth, int Gheight){
	  resize(Gwidth,Gheight);
  }

  template <class T>
  grid_engine<T>::grid_engine(int Gwidth, int Gheight, reference default_item){
	  resize(Gheight, Gwidth, default_item);
  }

  template <class T>
  void grid_engine<T>::clear() {
    data.clear();
  }

///////////////////////
//NPARSER
///////////////////////

  template <class T>
  bool grid_engine<T>::nparser::valid() const {
    if(toroid)
      return true;
    else
      return my_ge.in_grid(curx, cury);
  }

  template <class T>
  void grid_engine<T>::nparser::advance_until_valid(){
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

    if(toroid){
      curx=toroidabs(curx,my_ge.width() );
      cury=toroidabs(cury,my_ge.height());
    }
  }

  template <class T>
  grid_engine<T>::nparser::nparser ( grid_engine<T> &ge, neighbours *N, int x0, int y0, int inner_ring, int outer_ring, bool toroid) : my_ge(ge), N(N), x0(x0), y0(y0), outer_ring(outer_ring), toroid(toroid) {
    assert(ge.in_grid(x0,y0));
    assert(inner_ring>=0);
    assert(outer_ring>=0);
    assert(outer_ring>=inner_ring);

    i=N->begins(inner_ring);
    curx=x0+N->dx(i);
    cury=y0+N->dy(i);

    if(toroid){
      curx=toroidabs(curx,my_ge.width() );
      cury=toroidabs(cury,my_ge.height());
    }

    if(!valid())
      advance_until_valid();
  }

  template <class T>
  typename grid_engine<T>::nparser&
  grid_engine<T>::nparser::operator++(){   //Prefix ++
    advance_until_valid();
    return *this;
  }

  template <class T>
  typename grid_engine<T>::nparser
  grid_engine<T>::nparser::operator++(int){ //Postfix ++
    nparser tmp=*this;
    ++*this;
    return tmp;
  }

  template <class T>
  typename grid_engine<T>::reference
  grid_engine<T>::nparser::operator*() const {
    return my_ge(curx,cury);
  }

  template <class T>
  bool grid_engine<T>::nparser::good() const { return i!=-1; }

  template <class T>
  int grid_engine<T>::nparser::x()  const { return curx; }

  template <class T>
  int grid_engine<T>::nparser::y()  const { return cury; }

  template <class T>
  int grid_engine<T>::nparser::dx() const { return N->dx(i); }

  template <class T>
  int grid_engine<T>::nparser::dy() const { return N->dy(i); }

////////////////////////////
//PARSER
////////////////////////////

  template <class T>
  grid_engine<T>::parser::parser (grid_engine<T> &ge, int x0, int y0) : my_ge(ge), x0(x0), y0(y0) {}

  template <class T>
  typename grid_engine<T>::reference
  grid_engine<T>::parser::operator*() const {
    return my_ge(x0,y0);
  }

  template <class T>
  typename grid_engine<T>::parser&
  grid_engine<T>::parser::operator++(){   //Prefix ++
	  ++x0;
	  if(x0==my_ge.width()){
		  x0=0;
		  ++y0;
	  }
	  return *this;
  }

  template <class T>
  typename grid_engine<T>::parser
  grid_engine<T>::parser::operator++(int){ //Postfix ++
	  parser tmp=*this;
	  ++*this;
	  return tmp;
  }

  template <class T>
  bool grid_engine<T>::parser::good() const {
    return x0<my_ge.width() && y0<my_ge.height();
  }

  template <class T>
  typename grid_engine<T>::nparser
  grid_engine<T>::parser::hexring(int inner_ring, int outer_ring=-1) const {
    if(outer_ring==-1) outer_ring=inner_ring;
    return grid_engine<T>::nparser(my_ge, &nhex, x0, y0, inner_ring, outer_ring);
  }

  template <class T>
  typename grid_engine<T>::nparser
  grid_engine<T>::parser::d8ring(int inner_ring, int outer_ring=-1) const {
    if(outer_ring==-1) outer_ring=inner_ring;
    return grid_engine<T>::nparser(my_ge, &nd8, x0, y0, inner_ring, outer_ring);
  }

  template <class T>
  typename grid_engine<T>::nparser
  grid_engine<T>::parser::d4ring(int inner_ring, int outer_ring=-1) const {
    if(outer_ring==-1) outer_ring=inner_ring;
    return grid_engine<T>::nparser(my_ge, &nd4, x0, y0, inner_ring, outer_ring);
  }

  template <class T>
  typename grid_engine<T>::nparser
  grid_engine<T>::parser::hextring(int inner_ring, int outer_ring=-1) const {
    if(outer_ring==-1) outer_ring=inner_ring;
    return grid_engine<T>::nparser(my_ge, &nhex, x0, y0, inner_ring, outer_ring, true);
  }

  template <class T>
  typename grid_engine<T>::nparser
  grid_engine<T>::parser::d8tring(int inner_ring, int outer_ring=-1) const {
    if(outer_ring==-1) outer_ring=inner_ring;
    return grid_engine<T>::nparser(my_ge, &nd8, x0, y0, inner_ring, outer_ring, true);
  }

  template <class T>
  typename grid_engine<T>::nparser
  grid_engine<T>::parser::d4tring(int inner_ring, int outer_ring=-1) const {
    if(outer_ring==-1) outer_ring=inner_ring;
    return grid_engine<T>::nparser(my_ge, &nd4, x0, y0, inner_ring, outer_ring, true);
  }

  template <class T>
  int grid_engine<T>::parser::x() const {
    return x0;
  }

  template <class T>
  int grid_engine<T>::parser::y() const {
    return y0;
  }

}
#endif
