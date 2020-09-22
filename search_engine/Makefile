all: search-shell

search-shell: search-shell.cpp md_parser.o util.o webpage.o cmdhandler.o combiners.o handler.o searchui.o searcheng.o txt_parser.o 
	g++ -g -Wall --std=c++11 search-shell.cpp md_parser.o util.o webpage.o cmdhandler.o combiners.o handler.o searcheng.o searchui.o txt_parser.o -o search-shell

crawl: crawl.cpp md_parser.o txt_parser.o webpage.o  util.o webpage.o
	g++ -g -Wall --std=c++11 crawl.cpp md_parser.o txt_parser.o util.o webpage.o -o crawl

md_parser.o: md_parser.cpp md_parser.h util.h pageparser.h webpage.h
	g++ -g -Wall --std=c++11 -c md_parser.cpp -o md_parser.o

util.o: util.cpp util.h webpage.h
	g++ -g -Wall --std=c++11 -c util.cpp -o util.o

webpage.o: webpage.cpp webpage.h util.h
	g++ -g -Wall --std=c++11 -c webpage.cpp -o webpage.o

cmdhandler.o: util.h cmdhandler.h cmdhandler.cpp searcheng.h handler.h msort.h
	g++ -g -Wall --std=c++11 -c cmdhandler.cpp -o cmdhandler.o

combiners.o: combiners.cpp util.h wpscombiner.h combiners.h
	g++ -g -Wall --std=c++11 -c combiners.cpp -o combiners.o

handler.o: handler.h handler.cpp util.h
	g++ -g -Wall --std=c++11 -c handler.cpp -o handler.o

searchui.o: searchui.cpp searchui.h searcheng.h handler.h util.h
	g++ -g -Wall --std=c++11 -c searchui.cpp -o searchui.o

txt_parser.o: pageparser.h util.h txt_parser.cpp txt_parser.h
	g++ -g -Wall --std=c++11 -c txt_parser.cpp -o txt_parser.o

searcheng.o: searcheng.h searcheng.cpp pageparser.h webpage.h util.h 
	g++ -g -Wall --std=c++11 -c searcheng.cpp -o searcheng.o

clean:
	rm -f *.o search-shell crawl

*PHONY: clean

