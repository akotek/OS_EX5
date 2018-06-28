CC=g++
CXX=g++
RANLIB=ranlib

LIBSRC=VirtualMemory.h VirtualMemory.cpp MemoryConstants.h
LIBOBJ= VirtualMemory.o
TARSRCS = VirtualMemory.cpp Makefile README

INCS=-I.
CFLAGS = -Wall -std=c++11 -g $(INCS)
CXXFLAGS = -Wall -std=c++11 -g $(INCS)

OSMLIB = libVirtualMemory.a
TARGETS = $(OSMLIB)

TAR=tar
VAL=valgrind
TARFLAGS=-cvf
TARNAME=ex5.tar

.PHONY: all, clean, tar

all: $(TARGETS)

$(TARGETS): $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) $(LIBOBJ) $(OSMLIB) *~ *core


depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
