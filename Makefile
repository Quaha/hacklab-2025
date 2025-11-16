TARGET = app
CXX = g++
CXXFLAGS = -O3 -DNDEBUG -Wall -Wextra -std=c++11 -Ihackathon_internals -Isrc
LDFLAGS = -flto -O3

SRCS = $(wildcard hackathon_internals/*.cpp src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
ENTRY_POINT = interface.cpp

$(TARGET): $(OBJS) $(ENTRY_POINT)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

all: $(TARGET)

# Upload solution to OBS
upload:
	./upload_solution.sh
