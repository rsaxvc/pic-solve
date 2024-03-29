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

static int count_points_in_rule( const std::vector<int> & rule )
{
int retn = 0;
for( size_t i = 0; i < rule.size(); ++i )
	{
	retn += rule[i];
	}
return retn;
}

static int count_slots_used_by_rule( const std::vector<int> & rule )
{
if( rule.size() == 0 )
	return 0;
else
	return rule.size() - 1 + count_points_in_rule( rule );
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
	//this one is a little more complicated, if there's too many points, give up
	return count_points_in_rule( board_rules ) <= count_points_in_rule( rules );
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
bool * buf;
bool retn = true;

if( a.getWidth() > a.getHeight() )
	buf = new bool[ a.getWidth() ];
else
	buf = new bool[ a.getHeight() ];

for( size_t y = 0; y < a.getHeight(); ++y )
	{
	for( size_t x = 0; x < a.getWidth(); ++x )
		buf[x] = a(x,y);

	if( !line_valid( buf, a.getWidth(), s.hrules[y] ) )
		{
		retn = false;
		goto done;
		}
	}

for( size_t x = 0; x < a.getWidth(); ++ x )
	{
	for( size_t y = 0; y < a.getHeight(); ++y )
		buf[y] = a(x,y);

	if( !line_valid( buf, a.getHeight(), s.vrules[x] ) )
		{
		retn = false;
		goto done;
		}
	}

done:
delete[]( buf );
return retn;
}

/*does not violate any rules*/
static bool solution_plausible( arr2d<bool> & a, const serializer & s )
{
assert( s.getHeight() == a.getHeight() );
assert( s.getWidth()  == a.getWidth() );
bool * buf;
bool retn = true;

if( a.getWidth() > a.getHeight() )
	buf = new bool[ a.getWidth() ];
else
	buf = new bool[ a.getHeight() ];

for( size_t y = 0; y < a.getHeight(); ++y )
	{
	for( size_t x = 0; x < a.getWidth(); ++x )
		buf[x] = a(x,y);

	if( !line_plausible( buf, a.getWidth(), s.hrules[y] ) )
		{
		retn = false;
		goto done;
		}
	}

for( size_t x = 0; x < a.getWidth(); ++ x )
	{
	for( size_t y = 0; y < a.getHeight(); ++y )
		buf[y] = a(x,y);
	if( !line_plausible( buf, a.getHeight(), s.vrules[x] ) )
		{
		retn = false;
		goto done;
		}
	}

done:
delete[]( buf );
return retn;
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

static void apply_hrule( size_t row, std::vector<int> rule, arr2d<bool> & a )
{
size_t x = 0;
size_t count;

assert( count_slots_used_by_rule( rule ) == a.getWidth() );

for( size_t i = 0; i < rule.size(); ++i )
	{
	count = rule[i];
	for( size_t j = 0; j < rule[i]; ++j )
		{
		a(x++,row) = true;
		}
	x++;//skip a space
	}
}

static void apply_vrule( size_t col, std::vector<int> rule, arr2d<bool> & a )
{
size_t y = 0;
size_t count;

assert( count_slots_used_by_rule( rule ) == a.getWidth() );

for( size_t i = 0; i < rule.size(); ++i )
	{
	count = rule[i];
	for( size_t j = 0; j < rule[i]; ++j )
		{
		a(col,y++) = true;
		}
	y++;//skip a space
	}
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

for( size_t x = 0; x < s.getWidth(); ++x )
	if( count_slots_used_by_rule( s.vrules[x] ) == s.getHeight() )
		apply_vrule( x, s.vrules[x], array );

for( size_t y = 0; y < s.getHeight(); ++y )
	if( count_slots_used_by_rule( s.hrules[y] ) == s.getWidth() )
		apply_hrule( y, s.hrules[y], array );

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
