#include <iostream>
#include <cstdlib>
#include <fstream>

#include "serializer.h"
#include "arr2d.h"

int main( int n, const char * args[] )
{
int width;
int height;

if( n != 2 )
	{
	std::cerr<<"Usage:"<<args[0]<<" inputfile"<<std::endl;
	return 2;
	}

std::ifstream ifstr(args[1]);
serializer s;
ifstr>>s;
ifstr.close();

std::cout<<s;

arr2d<bool> array(s.getWidth(),s.getHeight(),false);
}
