CC = g++

CFLAGS := -I /usr/local/include/starpu/1.3  -I ./include

TARGET = ./a.out

SRCS = main.cpp frontend.cpp nodetypes.cpp

OBJS = $(SRCS:.cpp=.o)

INCDIR	= 

LIBDIR  = 

LIBS	=

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBDIR) $(LIBS)
	
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCDIR) -c $(SRCS)

all: clean $(OBJS) $(TARGET)

clean:
	-rm -f $(OBJS) $(TARGET) *.d