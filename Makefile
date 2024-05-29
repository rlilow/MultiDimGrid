CC=g++
CFLAGS=-O3 -Wall -pedantic -std=c++11 -fopenmp -ffast-math -flto -march=native

LIBRARY=MultiDimGrid

ARCHIVE_NAME=multidimgrid
DOX_NAME=Doxyfile
MAKE_NAME=Makefile
README_NAME=README.md
DOC_NAME=documentation

LIB_PATH=src
EXE_PATH=demo
DOC_PATH=doc

LIB_HEADERS=$(wildcard $(LIB_PATH)/*.hpp) $(wildcard *.hpp)
LIB_SOURCES=$(wildcard $(LIB_PATH)/*.cpp)
LIB_TEMPLATES=$(wildcard $(LIB_PATH)/*.tpp)
LIB_TEMPLATE_HEADERS=$(LIB_TEMPLATES:.tpp=.hpp)
LIB_OBJECTS=$(LIB_SOURCES:.cpp=.o)
LIB_DEPENDENCIES=$(LIB_OBJECTS:.o=.d)

ARCHIVE_FILE=lib$(ARCHIVE_NAME).a

EXE_SOURCES=$(wildcard $(EXE_PATH)/*.cpp)
EXECUTABLES=$(EXE_SOURCES:.cpp=.x)

CLEAN_FILES=$(LIB_OBJECTS) $(LIB_DEPENDENCIES) $(ARCHIVE_FILE) $(EXECUTABLES)
NECESSARY_FILES=$(DOX_NAME) $(MAKE_NAME) $(README_NAME) $(LIB_HEADERS) $(LIB_SOURCES) $(LIB_TEMPLATES) $(EXE_SOURCES)

all: $(LIB_OBJECTS) $(ARCHIVE_FILE) $(EXECUTABLES)

-include $(LIB_DEPENDENCIES)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	@$(CC) -MM $< > $*.d
	@\sed -i.bak "1s|^|$(LIB_PATH)/|" $*.d && \rm $*.d.bak

$(ARCHIVE_FILE): $(LIB_OBJECTS) $(LIB_TEMPLATES) $(LIB_TEMPLATE_HEADERS)
	\ar rcs $@ $^

%.x: %.cpp $(ARCHIVE_FILE)
	$(CC) $(CFLAGS) $< -o $@ -L. -l$(ARCHIVE_NAME)

$(DOC_PATH)/$(DOC_NAME).html: $(LIB_HEADERS) $(LIB_SOURCES) $(LIB_TEMPLATES)
	\doxygen $(DOX_NAME)
	@\ln -sf html/index.html $(DOC_PATH)/$(DOC_NAME).html

doc: $(DOC_PATH)/$(DOC_NAME).html

clean:
	\rm -f $(CLEAN_FILES)

portable:
	\tar -czf $(LIBRARY).tar.gz $(NECESSARY_FILES)