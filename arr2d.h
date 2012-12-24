#ifndef ARR2D_H
#define ARR2D_H

#include <cstddef>

template <typename T>
class arr2d
	{
	public:
		arr2d( size_t w, size_t h ){cons(w,h);}
		arr2d( size_t w, size_t h, const T & dflt );
		~arr2d(){delete[] dptr;}
		T & operator()(size_t x, size_t y);
	private:
		void cons( size_t w, size_t h );
		inline size_t locate(size_t x, size_t y ){return y * width + x;}
		T * dptr;
		size_t width;
		size_t height;
	};

template <typename T>
T & arr2d<T>::operator()( size_t x, size_t y )
{
return dptr[locate(x,y)];
}

template <typename T>
void arr2d<T>::cons( size_t w, size_t h )
{
width = w;
height = h;
dptr = new T[width*height];
}

template <typename T>
arr2d<T>::arr2d( size_t w, size_t h, const T & dflt )
{
cons(w,h);

for( size_t y = 0; y < height; ++y )
	for( size_t x = 0; x < width; ++x )
		dptr[locate(x,y)]=dflt;
}

#endif

