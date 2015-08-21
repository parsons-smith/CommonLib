CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS =	 main.o	ThreadPool.o Condition.o

LIBS = -lpthread

TARGET =	ThreadPool

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
