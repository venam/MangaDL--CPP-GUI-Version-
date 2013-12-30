CPPFLAGS=-g -fPIC -O2 -std=c++11 -Wall -DDEBUG `pkg-config gtk+-2.0 --cflags`
LDLIBS=-L. -Wl,-rpath,. \
	   -lbrowser -ldlder -lgui -ldl_facto -lconfig \
	   -lpthread -lcurl -lsqlite3 `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs`
SRC=testing6.cpp
OUT=test
OUT_O=test.o
RM=rm -f

all: browser.so dlder.so mgreader.o mgpark.o dl_facto.so  gui.so config.so
	g++ $(CPPFLAGS) $(SRC) -c -o $(OUT_O)
	g++ $(OUT_O) $(LDLIBS) -o $(OUT)

browser.so:
	g++ $(CPPFLAGS) manager/browser/browser.cpp -c -o browser.o
	g++ -shared -fPIC -o libbrowser.so browser.o

dlder.so:
	g++ $(CPPFLAGS) manager/dlder/dlder.cpp -c -o dlder.o
	g++ -shared -fPIC -o libdlder.so dlder.o

mgreader.o:
	g++ $(CPPFLAGS) manager/dlder/provider/mgreader.cpp -c -o mgreader.o

mgpark.o:
	g++ $(CPPFLAGS) manager/dlder/provider/mgpark.cpp -c -o mgpark.o

dl_facto.so:
	g++ $(CPPFLAGS)  manager/dlder/dl_facto.cpp -c -o dl_facto.o
	g++ -shared -fPIC -o libdl_facto.so dl_facto.o mgreader.o mgpark.o

gui.so:
	g++ $(CPPFLAGS)  manager/gui/gui.cpp -c -o gui.o
	g++ -shared -fPIC -o libgui.so gui.o

config.so:
	g++ $(CPPFLAGS) manager/cfg/config.cpp -c -o config.o
	g++ -shared -fPIC -o libconfig.so config.o

clean:
	$(RM) *.o
	$(RM) *.so


