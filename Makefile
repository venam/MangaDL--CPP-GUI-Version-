CPPFLAGS=-g -std=c++11 -Wall
LDFLAGS=-g
LDLIBS=-lpthread -lcurl
SRC=testing3.cpp
OUT=test browser.o dl_facto.o dlder.o mgreader.o mgpark.o
RM=rm -f

all: browser.o dlder.o mgreader.o mgpark.o dl_facto.o 
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

clean:
	$(RM) *.o


