#include "serializer.h"
#include "arr2d.h"

#include <iostream>
#include <cstdlib>


int main( int n, const char * args[] )
{
int width;
int height;

if( n != 3 )
	{
	std::cerr<<"Usage:"<<args[0]<<" width height"<<std::endl;
	return 2;
	}

width = atoi( args[1] );
height = atoi( args[2] );

arr2d<bool> array(width,height,false);
srand(time(NULL));

//arbitrarily place tokens in 1/3 of squares
for( size_t count = width * height / 3; count > 0; --count )
	{
	array(rand()%width,rand()%height)=true;
	}

serializer s( width, height );

//print rules for horizontal lines
for( size_t y = 0; y < height; ++y )
	{
	std::vector<int> hrule;
	size_t count = 0;
	for( size_t x = 0; x < width; ++x )
		{
		if( array(x,y) )
			{
			count++;
			}
		else
			{
			if( count )
				{
				hrule.push_back(count);
				}
			count = 0;
			}
		}
	if( count )
		{
		hrule.push_back(count);
		}
	s.insert_hrule( y, hrule );
	}

//print rules for vertical lines
for( size_t x = 0; x < width; ++x )
	{
	std::vector<int> vrule;
	size_t count = 0;
	for( size_t y = 0; y < height; ++y )
		{
		if( array(x,y) )
			{
			count++;
			}
		else
			{
			if( count )
				{
				vrule.push_back(count);
				}
			count = 0;
			}
		}
	if( count )
		{
		vrule.push_back(count);
		}
	s.insert_vrule( x, vrule );
	}

std::cout<<s;

//print the solved map
for( size_t y = 0; y < height; ++y )
	{
	for( size_t x = 0; x < width; ++x )
		std::cout<< (array(x,y) ? 'X' : ' ');
	std::cout<<std::endl;
	}
}
