TARGET=SyncAny
LINKFLAGS=-lpmmail -lpmkernel -lpmscl  -lACE 
SRCDIR=src
PREH=SyncAny


#for cpp
CC=g++  -g 

CP=/bin/cp -f
RM=/bin/rm -f


CFLAGS_DEF= -fPIC -DUNIX -D_GNU_SOURCE -D__LINUX_OS__ 
CDFLAGS_DEF=-g $(CFLAGS_DEF)
INCLUDE_DEF=-I./include
SRCS_DEF=$(wildcard $(SRCDIR)/*.cpp)
OBJS_DEF_TMP=$(SRCS_DEF:.cpp=.o)
OBJS_DEF=$(patsubst $(SRCDIR)/%,%,$(OBJS_DEF_TMP))

LINKFLAGS_DEF=-L./lib 
LIB_DEF=
LIBS_DEF=

all:$(SRCDIR)/$(PREH).h.gch $(OBJS_DEF) 
	$(CC)  -o $(TARGET)  $(OBJS_DEF) $(LIB_DEF) $(LIBS_DEF) $(LIB) $(LIBS) $(LINKFLAGS_DEF) $(LINKFLAGS)
clean:
	$(RM) $(TARGET) $(OBJS_DEF)  $(SRCDIR)/$(PREH).h.gch

$(SRCDIR)/$(PREH).h.gch: $(SRCDIR)/$(PREH).h
	$(CC) $(CFLAGS_DEF) $(CFLAGS) $(INCLUDE) $(INCLUDE_DEF) $(SRCDIR)/$(PREH).h

%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS_DEF) $(CFLAGS) $(INCLUDE) $(INCLUDE_DEF) -c -o $@ $<



install:
	mkdir -p $(PM_PUB_ROOT)/bin
	cp $(TARGET) $(PM_PUB_ROOT)/bin
