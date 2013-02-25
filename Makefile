CXXFLAGS =	-O2 -g -Wall -Werror -fmessage-length=0

OBJS =		main.o args.o config.o socketserver.o workerspool.o

LIBS =

BINDIR = 	bin
TARGET =	$(BINDIR)/server

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
