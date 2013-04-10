#ifndef _rbarnes_grid_engine_
#define _rbarnes_grid_engine_

#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <string>
#include <sstream>

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

	    class nparser {
		    private:
			    grid_engine<T> &my_ge;
          neighbours *N;
          int i, x0, y0, outer_ring, current_ring;
          int curx, cury;
          bool toroid;
          bool valid() const;
          void advance_until_valid();
		    public:
			    nparser ( grid_engine<T> &ge, neighbours *N, int x0, int y0, int inner_ring, int outer_ring, bool toroid=false);
			    nparser& operator++();
			    nparser operator++(int);
			    value_type operator*() const;
          bool good() const;
          int x()  const;
          int y()  const;
          int dx() const;
          int dy() const;
	    };

		  class parser {
			  private:
				  grid_engine<T> &my_ge;
				  int x0,y0;
			  public:
				  parser (grid_engine<T> &ge, int x0, int y0);
				  reference operator*() const;
				  parser& operator++();
				  parser operator++(int);
          bool good() const;
          grid_engine<T>::nparser hexring(int inner_ring, int outer_ring) const;
          grid_engine<T>::nparser d8ring(int inner_ring, int outer_ring) const;
          grid_engine<T>::nparser d4ring(int inner_ring, int outer_ring) const;

          grid_engine<T>::nparser hextring(int inner_ring, int outer_ring) const;
          grid_engine<T>::nparser d8tring(int inner_ring, int outer_ring=-1) const;
          grid_engine<T>::nparser d4tring(int inner_ring, int outer_ring=-1) const;
          int x() const;
          int y() const;
		  };

		  void resize(int Gwidth, int Gheight);
		  void resize(int Gwidth, int Gheight, reference default_item);
      bool in_grid(int x, int y) const;
		  size_type width() const;
		  size_type height() const;
		  grid_engine();
		  grid_engine(int Gwidth, int Gheight);
		  void clear();
		  parser begin();
		  reference operator()(int x, int y);
		  reference operator()(const nparser &n);
		  const_reference operator()(int x, int y) const;
      void fill(const value_type &val);
      std::string to_str() const;
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
  }

  template <class T>
  void grid_engine<T>::resize(int Gwidth, int Gheight, reference default_item){
	  data.resize(Gheight, std::vector<T> (Gwidth, default_item));
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

    curx%=my_ge.width();
    cury%=my_ge.height();
  }

  template <class T>
  grid_engine<T>::nparser::nparser ( grid_engine<T> &ge, neighbours *N, int x0, int y0, int inner_ring, int outer_ring, bool toroid) : my_ge(ge), N(N), x0(x0), y0(y0), outer_ring(outer_ring), current_ring(inner_ring), toroid(toroid) {
    assert(ge.in_grid(x0,y0));
    assert(current_ring>=0);
    assert(outer_ring>=0);
    assert(outer_ring>=current_ring);

    i=N->begins(inner_ring);
    curx=x0+N->dx(i);
    cury=y0+N->dy(i);

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
  typename grid_engine<T>::value_type
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
  grid_engine<T>::parser::hextring(int inner_ring, int outer_ring) const {
    if(outer_ring==-1) outer_ring=inner_ring;
    return grid_engine<T>::nparser(my_ge, &nhex, x0, y0, inner_ring, outer_ring, true);
  }

  template <class T>
  typename grid_engine<T>::nparser
  grid_engine<T>::parser::d8tring(int inner_ring, int outer_ring) const {
    if(outer_ring==-1) outer_ring=inner_ring;
    return grid_engine<T>::nparser(my_ge, &nd8, x0, y0, inner_ring, outer_ring, true);
  }

  template <class T>
  typename grid_engine<T>::nparser
  grid_engine<T>::parser::d4tring(int inner_ring, int outer_ring) const {
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
