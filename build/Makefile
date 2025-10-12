CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I../include
LDFLAGS =
TARGET = ../diarybetes
SRCDIR = ../src
OBJDIR = ../obj

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
HEADERS = $(wildcard ../include/*.hpp)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)