CXX=g++

SRCS=main.cpp
OBJS=$(SRCS:%.cpp=%.o)
TARGET=shell
OPTS=-DDEBUG

all: $(TARGET)
%.o: %.cpp
	$(CXX) $< -c -o $@ $(OPTS)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(OPTS) -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)