PLATFORM=x86
CROSS=
OBJSDIR=$(PLATFORM)/objs
SRCDIR=./src
TARGETDIR=$(PLATFORM)/bin
TARGETNAME=test
TARGET=$(TARGETDIR)/$(TARGETNAME)
MARCODEFINE=

#search *.c *.cpp
$(info "----------SRCDIR:$(SRCDIR)--------")
SRCS=$(shell find $(SRCDIR)  \( -name ".git" -prune \) -o \( -name "*.cpp" -o -name "*.c" \) -print)
$(info "----------SRCS:$(SRCS)--------------------")


# cpp compiler
CXX=$(CROSS)g++

# c compiler
CC=$(CROSS)gcc

LD = $(CROSS)ld
AR = $(CROSS)ar
STRIP = $(CROSS)strip
DUMP = $(CROSS)objdump

# compiler options
CPP_STD=-std=c++11 
C_STD=#-std=c99 
WARN=-Wall
DEBUG=-g -ggdb
OPT=-O0

ifdef STATIC_LINK
    LDLIBS += -lcurl #./$(PLATFORM)/lib/libcurl.a
    LDLIBS += -ljsoncpp
    LDLIBS += ./$(PLATFORM)/lib/libcrypto.a
    LDLIBS += ./$(PLATFORM)/lib/libssl.a
    LDLIBS += -ljpeg
    LDLIBS += -ldl
else
    LDLIBS += -lrt
endif


ifdef LINUX_THREAD
	LDLIBS += -lpthread
endif

LIBPATH+= -L./$(PLATFORM)/lib

# preprocesser arguments
INCLUDE+= -I./include/aip-cpp-sdk-0.5.0
INCLUDE+= -I./$(PLATFORM)/include
INCLUDE+= -I./include
INCLUDE+= -I./$(PLATFORM)/include/jpeg

# compiler arguments
CXXFLAGS=$(CPP_STD) $(WARN) $(DEBUG) $(OPT) $(INCLUDE) $(MACRODEFINE)

CFLAGS=$(C_STD) $(WARN) $(DEBUG) $(OPT) $(INCLUDE) $(MACRODEFINE)  

LDFLAGS+= -Wl,-rpath=./$(PLATFORM)/lib

LINK=$(CXX)

# link
#LINK=$(CXX) $(LDLIBS) $(LIBPATH)

TMPOBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJSDIR)/%.o, $(SRCS))
$(info "----------TMPOBJS:$(TMPOBJS)-------------------")
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJSDIR)/%.o, $(TMPOBJS))
$(info "----------OBJS:$(OBJS)-------------------")
OBJD := $(patsubst %.o, %.d, $(OBJS))
$(info "----------OBJD:$(OBJD)-------------------")

DEPS=$(OBJS:.o=.d)

all : $(TARGET)

$(info "----------1--DEPS:$(DEPS)-----------------")
#-include $(DEPS)
ifeq ($(filter $(MAKECMDGOALS),clean install show),)
	sinclude $(DEPS)
endif

# make dependences
$(OBJSDIR)/%.d: $(SRCDIR)/%.c
	@set -e; \
    rm -f $@;	\
    echo "make deps $@ $<";   \
	$(CC) -MM $(INCLUDE) $< >$@.$$$$; \
    sed 's/.*\.o/$(subst /,\/, $(dir $@))& $(subst /,\/,$@)/g' <$@.$$$$  >$@; \
	rm -f $@.$$$$; 

$(OBJSDIR)/%.d: $(SRCDIR)/%.cpp
	@set -e;  \
    rm -f $@;	\
    echo "make deps $@ $<";   \
	$(CC) -MM $(INCLUDE) $< >$@.$$$$; \
    sed 's/.*\.o/$(subst /,\/, $(dir $@))& $(subst /,\/,$@)/g' <$@.$$$$  >$@; \
	rm -f $@.$$$$; 

# link
$(TARGET): $(OBJS)
	@echo "dir:$(dir $@),$@,$<"
	@mkdir -p $(dir $@)
	$(LINK) -o $@ $^ $(LDLIBS) $(LIBPATH) $(LDFLAGS)

# compile c
$(OBJSDIR)/%.o: $(SRCDIR)/%.c
	@echo "dir:$(dir $@),$@,$<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ 

# compile cpp
$(OBJSDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "dir:$(dir $@),$@,$<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean:
	$(RM) $(TARGET)  $(OBJS)  $(DEPS)
install:
	mv $(TARGET) $(BINPATH)
show:
	make -v
	@echo "***************Information***************"
	@echo  "TARGET:             $(TARGET)"
	@echo  "FINAL_COMPILE:      $(FINAL_COMPILE)"
	@echo  "BINPATH:            $(BINPATH)"
	@echo  "OBJSDIR:            $(OBJSDIR)"
	@echo  "OBJS:               $(OBJS)"
	@echo  "INCLUDE:            $(INCLUDE)"
	@echo  "DEPS:               $(DEPS)"
