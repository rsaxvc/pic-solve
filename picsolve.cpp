#include <cassert>
#include <iostream>
#include <cstdlib>
#include <fstream>

#include "serializer.h"
#include "arr2d.h"

std::vector<int> build_rules( bool * buf, size_t bufsz )
{
std::vector<int> rule;
size_t count = 0;
for( size_t i = 0; i < bufsz; ++i )
	{
	if( buf[i] )
		{
		count++;
		}
	else
		{
		if( count )
			{
			rule.push_back(count);
			}
		count = 0;
		}
	}
if( count )
	{
	rule.push_back(count);
	}
return rule;
}

static bool line_plausible( bool * buf, size_t bufsz, const std::vector<int> & rules )
{
std::vector<int> board_rules = build_rules( buf, bufsz );
if( rules.size() == board_rules.size() )
	{
	for( size_t i = 0; i < rules.size(); ++i )
		{
		if( board_rules[i] > rules[i] )
			{
			//if any of the board rules have a longer chain than game rules, we cannot win
			return false;
			}
		}
	return true;
	}
else if( board_rules.size() > rules.size() )
	{
	//if the board has more rules than the rulebook, we cannot win
	return false;
	}
else
	{
	//this one is a little more complicated, assume true for now
	return true;
	}
return true;
}

static bool line_valid( bool * buf, size_t bufsz, const std::vector<int> & rules )
{
return rules == build_rules( buf, bufsz );
}

/*does not violate any rules*/
static bool solution_valid( arr2d<bool> & a, const serializer & s )
{
assert( s.getHeight() == a.getHeight() );
assert( s.getWidth()  == a.getWidth() );

bool * hbuf = new bool[ a.getWidth() ];
for( size_t y = 0; y < a.getHeight(); ++y )
	{
	for( size_t x = 0; x < a.getWidth(); ++x )
		{
		hbuf[x] = a(x,y);
		}

	if( !line_valid( hbuf, a.getWidth(), s.hrules[y] ) )
		{
		return false;
		}
	}
delete[]( hbuf );

bool * vbuf = new bool[ a.getHeight() ];
for( size_t x = 0; x < a.getWidth(); ++ x )
	{
	for( size_t y = 0; y < a.getHeight(); ++y )
		{
		vbuf[y] = a(x,y);
		}
	if( !line_valid( vbuf, a.getHeight(), s.vrules[x] ) )
		{
		return false;
		}
	}
delete[]( vbuf );

return true;
}

/*does not violate any rules*/
static bool solution_plausible( arr2d<bool> & a, const serializer & s )
{
assert( s.getHeight() == a.getHeight() );
assert( s.getWidth()  == a.getWidth() );

bool * hbuf = new bool[ a.getWidth() ];
for( size_t y = 0; y < a.getHeight(); ++y )
	{
	for( size_t x = 0; x < a.getWidth(); ++x )
		hbuf[x] = a(x,y);

	if( !line_plausible( hbuf, a.getWidth(), s.hrules[y] ) )
		return false;
	}
delete[]( hbuf );

bool * vbuf = new bool[ a.getHeight() ];
for( size_t x = 0; x < a.getWidth(); ++ x )
	{
	for( size_t y = 0; y < a.getHeight(); ++y )
		vbuf[y] = a(x,y);
	if( !line_plausible( vbuf, a.getHeight(), s.vrules[x] ) )
		return false;
	}
delete[]( vbuf );

return true;
}

bool recurse( arr2d<bool> & start, const serializer & s )
{
for( size_t x = 0; x < start.getWidth(); ++x )
	{
	for( size_t y = 0; y < start.getHeight(); ++y )
		{
		if( start(x,y) )
			{
			//point already in use
			}
		else
			{
			start(x,y) = true;
			if( solution_valid( start, s ) )
				{
				return true;
				}
			else if( solution_plausible( start, s ) )
				{
				if( recurse( start, s ) )
					{
					return true;
					}
				}
			start(x,y) = false;
			}
		}
	}
return false;
}

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

arr2d<bool> array(s.getWidth(),s.getHeight(),false);

bool ret = recurse( array, s );
if( ret )
	{
	std::cout<<"One possible solution follows"<<std::endl<<array;
	}
else
	{
	std::cout<<"no solution found"<<std::endl;
	}
}
