CXXFLAGS =	-O2 -g -Wall -Werror -std=c++11

OBJS =		main.o args.o config.o echoserver.o eventqueue.o socketserver.o workerspool.o

HDRS = 		listenner.h

LIBS =		-pthread

BINDIR = 	bin
TARGET =	$(BINDIR)/server

$(TARGET):	$(OBJS) $(HDRS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
