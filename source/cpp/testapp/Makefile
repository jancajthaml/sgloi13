
INCLUDE = -I/usr/include -I/usr/X11R6/include -I. -I../
LIBDIR = -L. -L/usr/X11R6/lib -L/usr/lib -L/usr/local/lib

COMPILERFLAGS = -Wall -g -O2 -DUSE_GUI=0
#-g 
CC = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE) 

LIBRARIES = -framework GLUT -framework OpenGL #-lXext -lX11 -lm
#LIBRARIES = -lglut -lGL -lGLU  #-lXext -lX11 -lm
#LIBRARIES = -lm
OBJS = testapp.o ../sgl.o nffread.o timer.o hdrloader.o
OBJS2 = raytracer.o sgl/sgl.o nffread.o timer.o hdrloader.o
TEST = ../tests.cpp
#-lXmu -lXi 
all: testapp
testapp: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(LIBDIR) $(OBJS) $(LIBRARIES)  

.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp -o $*.o

clean:
	rm -f testapp $(OBJS) test
test:
	$(CC) $(CFLAGS)  $(TEST) -o test
