CXX = g++
CXXFLAGS := -Wall -std=c++17

SRC = gl-abstractions/
LIBS = libs/
OBJECT_FILES = $(SRC)ebo.o $(SRC)shader.o $(SRC)texture.o $(SRC)vao.o $(SRC)vbo.o

OBJECT_EXTERNAL = $(LIBS)file-management/file_management.o
.PHONY: $(OBJECT_EXTERNAL)

gl_abstractions.o: $(OBJECT_EXTERNAL) $(OBJECT_FILES)
	ld -r $(OBJECT_FILES) -o gl_abstractions.o

#src
$(SRC)ebo.o: $(SRC)ebo.h $(SRC)ebo.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)ebo.cpp -o $(SRC)ebo.o
$(SRC)shader.o: $(SRC)shader.h $(SRC)shader.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)shader.cpp -o $(SRC)shader.o
$(SRC)texture.o: $(SRC)texture.h $(SRC)texture.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)texture.cpp -o $(SRC)texture.o
$(SRC)vao.o: $(SRC)vao.h $(SRC)vao.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)vao.cpp -o $(SRC)vao.o
$(SRC)vbo.o: $(SRC)vbo.h $(SRC)vbo.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)vbo.cpp -o $(SRC)vbo.o

$(LIBS)file-management/file_management.o:
	cd $(LIBS)file-management && make

clean:
	rm $(OBJECT_FILES) gl_abstractions.o