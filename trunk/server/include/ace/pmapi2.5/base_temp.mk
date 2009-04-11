#for cpp
CC=g++  -g 

CP=/bin/cp -f
RM=/bin/rm -f

CFLAGS_DEF= -fPIC -DUNIX -D_GNU_SOURCE -D__LINUX_OS__ 
CDFLAGS_DEF=-g $(CFLAGS_DEF)
INCLUDE_DEF=${AB_INC} -I$(PM_PUB_ROOT)/include
SRCS_DEF=$(wildcard $(SRCDIR)/*.cpp)
OBJS_DEF_TMP=$(SRCS_DEF:.cpp=.o)
OBJS_DEF=$(patsubst $(SRCDIR)/%,%,$(OBJS_DEF_TMP))

#for c 
cc=gcc -g -Wall 
cflags_def=-DUNIX -fPIC -D_GNU_SOURCE -D__LINUX_OS__ ${AB_INC}
cdflags_def=-g $(cflags_def)
include_def=${AB_INC} -I$(PM_PUB_ROOT)/include
srcs_def=$(wildcard $(SRCDIR)/*.c)
objs_def_tmp=$(srcs_def:.c=.o)
objs_def=$(patsubst $(SRCDIR)/%,%,$(objs_def_tmp))

LINKFLAGS_DEF=${AB_LIB} -L$(PM_PUB_ROOT)/lib 
LIB_DEF=
LIBS_DEF=

all:$(OBJS_DEF) $(objs_def)
	$(CC)  -o $(TARGET) $(objs_def) $(OBJS_DEF) $(LIB_DEF) $(LIBS_DEF) $(LIB) $(LIBS) $(LINKFLAGS_DEF) $(LINKFLAGS)
clean:
	$(RM) $(TARGET) $(OBJS_DEF) $(objs_def)

%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS_DEF) $(CFLAGS) $(INCLUDE) $(INCLUDE_DEF) -c -o $@ $<

%.o: $(SRCDIR)/%.c
	$(cc) $(cflags_def) $(cflags) $(include) $(include_def) -c -o $@ $<

