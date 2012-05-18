#include <vector>
template <class T> class grid_engine_iterator;

template <class T>
class grid_engine {
	friend class grid_engine_iterator<T>;
	private:
		std::vector< std::vector<T> > data;
	public:
		typedef T		value_type;
		typedef T		*pointer;
		typedef const T	*const_pointer;
		typedef T		&reference;
		typedef const T	&const_reference;
		typedef int		size_type;
		typedef grid_engine_iterator<T> iterator;
		reference operator()(int x, int y)
			{return data[y][x];}
		const_reference operator()(int x, int y) const
			{return data[y][x];}
		void resize(int Gwidth, int Gheight, bool preserve=false);
		size_type width() const
			{return data[0].size();}
		size_type height() const
			{return data.size();}
		grid_engine();
		grid_engine(int width, int height);
		void clear()
			{data.clear();}
		reference front()
			{return data[0][0];}
		reference back()
			{return data[height()-1][width()-1];}
		const_reference front() const
			{return data[0][0];}
		const_reference back() const
			{return data[height()-1][width()-1];}
		iterator begin() { return iterator(*this, 0, 0); }
		iterator end() { return iterator(*this, width()-1, height()-1); }
};

template <class T>
grid_engine<T>::grid_engine() {}

template <class T>
grid_engine<T>::grid_engine(int Gwidth, int Gheight){
	resize(Gwidth,Gheight);
}

template <class T>
void grid_engine<T>::resize(int Gwidth, int Gheight, bool preserve){
	data.resize(Gheight);
	for(int i=0;i<Gheight;i++)
		data[i].resize(Gwidth);
}

template <class T>
class grid_engine_iterator{
	private:
		grid_engine<T>	&my_ge;
		int x,y;
	public:
		grid_engine_iterator( grid_engine<T> &ge, int x0, int y0 ){
			my_ge=ge;
			x=x0;
			y=y0;
		}
		bool operator==(grid_engine<T>::iterator &other){
			return (other.my_ge==my_ge && other.x==x && other.y==y);
		}
		bool operator!=(grid_engine<T>::iterator &other){
			return !(operator==(other));
		}
};



template <class T>
class hex_engine : public grid_engine<T> {
	public:
		
};
