# If necessary, modify the paths to GSL.
GSL_INCLUDE_PATH=.
GSL_LIB_PATH=.

# The rest usually does not need to be modified.
CC=g++
CFLAGS=-O3 -Wall -pedantic -std=c++11 -fopenmp -ffast-math -flto -march=native

INCLUDE=-I $(GSL_INCLUDE_PATH)
LINK=-L $(GSL_LIB_PATH) -lgsl -lgslcblas

LINK_DEPENDENCIES=$(wildcard $(GSL_LIB_PATH)/libgsl.a)

LIBRARY=MultiDimGrid

ARCHIVE_NAME=multidimgrid
DOX_NAME=Doxyfile
MAKE_NAME=Makefile
README_NAME=readme
DOC_NAME=documentation

LIB_PATH=src
EXE_PATH=demo
DOC_PATH=doc

LIB_HEADERS=$(wildcard $(LIB_PATH)/*.h) $(wildcard *.h)
LIB_SOURCES=$(wildcard $(LIB_PATH)/*.cpp)
LIB_TEMPLATES=$(wildcard $(LIB_PATH)/*.tpp)
LIB_TEMPLATE_HEADERS=$(LIB_TEMPLATES:.tpp=.h)
LIB_OBJECTS=$(LIB_SOURCES:.cpp=.o)
LIB_DEPENDENCIES=$(LIB_OBJECTS:.o=.d)

ARCHIVE_FILE=lib$(ARCHIVE_NAME).a

EXE_SOURCES=$(wildcard $(EXE_PATH)/*.cpp)
EXECUTABLES=$(EXE_SOURCES:.cpp=.x)

CLEAN_FILES=$(LIB_OBJECTS) $(LIB_DEPENDENCIES) $(ARCHIVE_FILE) $(EXECUTABLES)
NECESSARY_FILES=$(DOX_NAME) $(MAKE_NAME) $(README_NAME) $(LIB_HEADERS) $(LIB_SOURCES) $(LIB_TEMPLATES) $(EXE_SOURCES)

all: $(LIB_OBJECTS) $(ARCHIVE_FILE) $(EXECUTABLES)

-include $(LIB_DEPENDENCIES)

%.o: %.cpp $(LINK_DEPENDENCIES)
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@ $(LINK)
	@$(CC) -MM $< > $*.d
	@\sed -i.bak "1s|^|$(LIB_PATH)/|" $*.d && \rm $*.d.bak

$(ARCHIVE_FILE): $(LIB_OBJECTS) $(LIB_TEMPLATES) $(LIB_TEMPLATE_HEADERS)
	\ar rcs $@ $^

%.x: %.cpp $(ARCHIVE_FILE)
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@ -L. -l$(ARCHIVE_NAME) $(LINK)

$(DOC_PATH)/$(DOC_NAME).html: $(LIB_HEADERS) $(LIB_SOURCES) $(LIB_TEMPLATES)
	\doxygen $(DOX_NAME)
	@\ln -sf html/index.html $(DOC_PATH)/$(DOC_NAME).html

doc: $(DOC_PATH)/$(DOC_NAME).html

clean:
	\rm -f $(CLEAN_FILES)

portable:
	@\cp $(MAKE_NAME) $(MAKE_NAME).tmp
	@\sed -i.bak "2c\GSL_INCLUDE_PATH=." $(MAKE_NAME) && \rm $(MAKE_NAME).bak
	@\sed -i.bak "3c\GSL_LIB_PATH=." $(MAKE_NAME) && \rm $(MAKE_NAME).bak
	\tar -czf $(LIBRARY).tar.gz $(NECESSARY_FILES)
	@\mv $(MAKE_NAME).tmp $(MAKE_NAME)