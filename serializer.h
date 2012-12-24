#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <vector>
#include <istream>
#include <ostream>
#include <string>

class serializer
	{
	public:
		serializer(){width=0;height=0;}
		serializer(int w, int h){width=w;height=h;vrules.reserve(width);hrules.reserve(height);}
		void insert_hrule( int pos, std::vector<int> rules ){hrules.push_back(rules);}
		void insert_vrule( int pos, std::vector<int> rules ){vrules.push_back(rules);}
		int getWidth( void ){return width;}
		int getHeight( void ){return height;}
		friend std::ostream & operator<<(std::ostream & out,const serializer & );
		friend std::istream & operator>>(std::istream & in,serializer & );
	private:
		int width;
		int height;
		std::vector< std::vector<int> > hrules;
		std::vector< std::vector<int> > vrules;
		friend std::istream & operator>>( std::istream & is, std::string & linestring );
	};

#endif
