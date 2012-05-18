#include <vector>
template <class T>
class grid_engine {
	private:
		std::vector< std::vector<T> > data;
	public:
		T& operator()(int x, int y)
			{return data[y][x];}
		const T& operator()(int x, int y) const
			{return data[y][x];}
		void resize(int Gwidth, int Gheight, bool preserve=false);
		grid_engine();
		grid_engine(int width, int height);
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
class hex_engine : public grid_engine<T> {
	public:
		
};
