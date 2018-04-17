all: bbox svg glyph
	g++ -c main1.cpp
	g++ -c main2.cpp
	g++ -o main1 main1.o glyph.o svg.o bbox.o -lboost_system -lboost_filesystem
	g++ -o main2 main2.o glyph.o svg.o bbox.o -lboost_system -lboost_filesystem

bbox:
	g++ -c bbox.cpp bbox.hpp

svg:
	g++ -c svg.cpp svg.hpp

glyph:
	g++ -c glyph.cpp glyph.hpp

clean:
	rm main1 main2 *.o *.gch
	rm -r out


