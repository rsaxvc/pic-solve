CXX=g++
CXXFLAGS= -c -pg
CC=$(CXX)
BUILDS=picgen picsolve
SOURCES=`ls *.cpp`
all: .depend $(BUILDS)

.depend:
	fastdep $(SOURCES) > .depend

-include .depend

picgen: picgen.o serializer.o

picsolve: picsolve.o serializer.o

clean:
	rm -f *.o .depend $(BUILDS)
