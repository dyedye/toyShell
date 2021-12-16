CXX=g++

SRCS=main.cpp pathResolver.cpp utils.cpp
OPTS=-std=c++17 #-DDEBUG
OBJS=$(SRCS:%.cpp=%.o)
TARGET=shell

all: $(TARGET)
%.o: %.cpp
	$(CXX) $< -c -o $@ $(OPTS)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(OPTS) -o $@

test: $(TARGET)
	./$(TARGET) 

clean:
	rm -f $(OBJS) $(TARGET)