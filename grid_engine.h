#include <boost/numeric/ublas/matrix.hpp>
template <class T>
class grid_engine : public boost::numeric::ublas::matrix<T>{
	public:
		T& operator()(int x, int y)
			{return boost::numeric::ublas::matrix<T>::operator()(x,y);}
		const T& operator()(int x, int y) const
			{return boost::numeric::ublas::matrix<T>::operator()(x,y);}
		void resize(int width, int height, bool preserve=false)
			{boost::numeric::ublas::matrix<T>::resize(width,height,preserve);}
};


template <class T>
class hex_engine : public grid_engine<T> {
	public:
		
};
