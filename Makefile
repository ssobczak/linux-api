CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		main.o args.o config.o server.o listenner.o

LIBS =

BINDIR = 	bin
TARGET =	$(BINDIR)/server

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
