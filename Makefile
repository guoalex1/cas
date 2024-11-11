BUILD_DIR := build

CXX = g++ -std=c++20
CXXFLAGS = -Wall -g -O -MMD
SOURCES = $(wildcard *.cpp)
OBJFILES = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
DEPENDS = $(OBJFILES:%.o=%.d)
EXEC = cas

$(EXEC): $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(OBJFILES) -o $(EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Include the dependency files
-include $(DEPENDS)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(EXEC)
