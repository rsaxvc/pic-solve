#include <iostream>
#include <cstdlib>

#include "arr2d.h"

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

std::cout<<"W:"<<width<<std::endl;
std::cout<<"H:"<<height<<std::endl;

arr2d<bool> array(width,height,false);
srand(time(NULL));

//arbitrarily place tokens in 1/3 of squares
for( size_t count = width * height / 3; count > 0; --count )
	{
	array(rand()%width,rand()%height)=true;
	}

//print rules for horizontal lines
for( size_t y = 0; y < height; ++y )
	{
	std::cout<<"Y"<<y<<":";
	bool once = false;
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
				if( once )std::cout<<",";
				std::cout<<count;
				once = true;
				}
			count = 0;
			}
		}
	if( count )
		{
		if( once )std::cout<<",";
		std::cout<<count;
		}
	std::cout<<std::endl;
	}

//print rules for vertical lines
for( size_t x = 0; x < width; ++x )
	{
	std::cout<<"X"<<x<<":";
	bool once = false;
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
				if( once )std::cout<<",";
				std::cout<<count;
				once = true;
				}
			count = 0;
			}
		}
	if( count )
		{
		if( once )std::cout<<",";
		std::cout<<count;
		}
	std::cout<<std::endl;
	}


//print the solved map
for( size_t y = 0; y < height; ++y )
	{
	for( size_t x = 0; x < width; ++x )
		std::cout<< (array(x,y) ? 'X' : ' ');
	std::cout<<std::endl;
	}
}
