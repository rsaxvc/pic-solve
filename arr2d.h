#ifndef ARR2D_H
#define ARR2D_H

#include <cstddef>
#include <ostream>

template <typename T>
class arr2d
	{
	public:
		arr2d( const arr2d & orig );
		arr2d( size_t w, size_t h ){cons(w,h);}
		arr2d( size_t w, size_t h, const T & dflt );
		~arr2d(){delete[] dptr;}
		T & operator()(size_t x, size_t y);
		size_t getHeight()const{return height;}
		size_t getWidth()const{return width;}

		template<typename M>
		friend std::ostream & operator<<( std::ostream & os, const arr2d & a );
	private:
		void cons( size_t w, size_t h );
		inline size_t locate(size_t x, size_t y ){return y * width + x;}
		T * dptr;
		size_t width;
		size_t height;
	};

template <typename T>
std::ostream & operator<<( std::ostream & os, arr2d<T> & a )
{
for(size_t y = 0; y < a.getHeight(); ++y )
	{
	for(size_t x = 0; x < a.getWidth(); ++x )
		{
		if(x!=0)os<<',';
		os<<a(x,y)?'X':' ';
		}
	os<<std::endl;
	}
return os;
}

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
arr2d<T>::arr2d( const arr2d & orig )
{
cons( orig.width, orig.height );
for( size_t i = 0; i < width * height; ++i )
	{
	dptr[i]=orig.dptr[i];
	}
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

