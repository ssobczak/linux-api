CXXFLAGS =	-O2 -g -Wall -Werror -fmessage-length=0

OBJS =		main.o args.o config.o echoserver.o socketserver.o workerspool.o

HDRS = 		listenner.h

LIBS =		-pthread

BINDIR = 	bin
TARGET =	$(BINDIR)/server

$(TARGET):	$(OBJS) $(HDRS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
