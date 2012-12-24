#include "serializer.h"

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <utility>
#include <vector>
#include <istream>
#include <ostream>
#include <iostream>

std::istream & operator>>( std::istream & is, std::string & linestring )
{
char byte;
std::string retn="";
do
	{
	byte = '\n';
	is.read( &byte, 1 );
	if( byte == '\n' )
		goto done;
	retn.push_back(byte);
	}while( !is.eof() );

done:
linestring = retn;
return is;
}

static std::vector<int> parse_string( std::string linebuf )
{
std::vector<int> retn;
const char * nextptr = strchr( linebuf.c_str(), ':' );

while( nextptr != NULL && nextptr[1] != '\0' )
	{
	nextptr++;
	retn.push_back( atoi( nextptr ) );
	nextptr = strchr( nextptr, ',' );
	}

return retn;
}

std::istream & operator>>( std::istream & is, serializer & s )
{
std::string wstring;
std::string hstring;
std::string linebuf;

is>>wstring;
if( wstring[0] == 'W' && wstring[1] == ':' )
	{
	s.width = atoi( wstring.c_str() + 2 );
	s.vrules.reserve(s.width);
	}
else
	goto fail;

is>>hstring;
if( hstring[0] == 'H' && hstring[1] == ':' )
	{
	s.height = atoi( hstring.c_str() + 2 );
	s.hrules.reserve(s.height);
	}
else
	goto fail;

for( size_t y = 0; y < s.height; ++y )
	{
	is>>linebuf;
	s.hrules.push_back(parse_string( linebuf ));
	}

for( size_t x = 0; x < s.width; ++x )
	{
	is>>linebuf;
	s.vrules.push_back(parse_string( linebuf ));
	}

return is;

fail:
	std::cerr<<"BAD FILE"<<std::endl;
	exit(4);
	return is;
}


std::ostream & operator<<( std::ostream & out, const serializer & s )
{
out<<"W:"<<s.width<<std::endl;
out<<"H:"<<s.height<<std::endl;

assert( s.hrules.size() == s.height );
assert( s.vrules.size() == s.width );

for( size_t y = 0; y < s.height; ++y )
	{
	out<<'Y'<<y<<':';
	for( size_t x = 0; x < s.hrules[y].size(); ++x )
		{
		out<<s.hrules[y][x];
		if( x + 1 != s.hrules[y].size() ) out << ',';
		}
	out<<std::endl;
	}

for( size_t x = 0; x < s.width; ++x )
	{
	out<<'X'<<x<<':';
	for( size_t y = 0; y < s.vrules[x].size(); ++y )
		{
		out<<s.vrules[x][y];
		if( y + 1 != s.vrules[ x ].size() ) out << ',';
		}
	out<<std::endl;
	}

return out;
}
