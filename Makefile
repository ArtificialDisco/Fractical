#CPP = g++ `wx-config --cppflags --cflags --libs`
#CPPO = g++ `wx-config --cppflags --cflags` -c -DDEBUG

CPP = g++
CPPO = g++

CFLAGS = `wx-config --cxxflags` -c
LIBS = `wx-config --libs`

INSTALL = install

HEADERS = main.h \
		frame.h \
		options.h \
		complex.h \
		parameters.h

SOURCES = main.cpp \
		frame.cpp \
		options.cpp \
		complex.cpp \
		parameters.cpp

OBJECTS = main.o \
		frame.o \
		options.o \
		complex.o \
		parameters.o

TARGET = fractical

.SUFFIXES: .c .cpp
.c.o:
	$(CPPO) -o $@ $< $(CFLAGS)
.cpp.o:
	$(CPPO) -o $@ $< $(CFLAGS)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	rm -rf $(OBJECTS)
	rm -rf $(TARGET)

install:
	$(INSTALL) $(TARGET) /usr/bin


main.o: frame.h main.cpp main.h
frame.o: frame.h complex.h frame.cpp main.h
complex.o: complex.cpp complex.h
options.o: options.h main.h frame.h options.cpp
parameters.o: parameters.h main.h parameters.cpp

