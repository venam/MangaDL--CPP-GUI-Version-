CPPFLAGS=-g -O2 -std=c++11 -Wall -DDEBUG `pkg-config gtk+-2.0 --cflags`
LDLIBS=-lpthread -lcurl -lsqlite3 `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs`
SRC=testing6.cpp
OUT=test browser.o dl_facto.o dlder.o mgreader.o mgpark.o gui.o config.o
RM=rm -f

all: browser.o dlder.o mgreader.o mgpark.o dl_facto.o  gui.o config.o
	g++ $(SRC) $(LDLIBS) -o $(OUT)

browser.o:
	g++ $(CPPFLAGS) manager/browser/browser.cpp -c -o browser.o

dlder.o:
	g++ $(CPPFLAGS) manager/dlder/dlder.cpp -c -o dlder.o

mgreader.o:
	g++ $(CPPFLAGS) manager/dlder/provider/mgreader.cpp -c -o mgreader.o

mgpark.o:
	g++ $(CPPFLAGS) manager/dlder/provider/mgpark.cpp -c -o mgpark.o

dl_facto.o:
	g++ $(CPPFLAGS)  manager/dlder/dl_facto.cpp -c -o dl_facto.o

gui.o:
	g++ $(CPPFLAGS)  manager/gui/gui.cpp -c -o gui.o

config.o:
	g++ $(CPPFLAGS) manager/cfg/config.cpp -c -o config.o

clean:
	$(RM) *.o


