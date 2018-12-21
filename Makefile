# It is mandatory to set GLAD_PATH to the glad root directory
# Do it this way `make GLAD_PATH=`
# For example `make GLAD_PATH=C://opengl/glad`

ifeq ($(GLAD_PATH),)
 $(warning Using default glad path `./`. Ignore this if cleaning. Look at first three lines in Makefile to solve.)
endif

SRC = src/
INC = include/
LIBS_PATH = libs/

CXX = g++
CXXFLAGS := -Wall -std=c++17 -I$(GLAD_PATH)/include -I$(LIBS_PATH)file-management/include

OBJECT_FILES = $(SRC)ebo.o $(SRC)shader.o $(SRC)texture.o $(SRC)vao.o $(SRC)vbo.o
.PHONY: $(OBJECT_FILES_GENERATED_BY_LIBS)

gl_abstractions.o: $(OBJECT_FILES)
	ld -r $(OBJECT_FILES) -o gl_abstractions.o

#src
$(SRC)ebo.o: $(INC)stypox/gl/ebo.h $(SRC)ebo.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)ebo.cpp -o $(SRC)ebo.o
$(SRC)shader.o: $(INC)stypox/gl/shader.h $(SRC)shader.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)shader.cpp -o $(SRC)shader.o
$(SRC)texture.o: $(INC)stypox/gl/texture.h $(SRC)texture.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)texture.cpp -o $(SRC)texture.o
$(SRC)vao.o: $(INC)stypox/gl/vao.h $(SRC)vao.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)vao.cpp -o $(SRC)vao.o
$(SRC)vbo.o: $(INC)stypox/gl/vbo.h $(SRC)vbo.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC)vbo.cpp -o $(SRC)vbo.o

clean:
	rm $(OBJECT_FILES) gl_abstractions.o
	cd $(LIBS_PATH)file-management && make clean