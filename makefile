CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-Wall
LDFLAGS=
LDLIBS=-luv

SRCS=dapps.cc \
	utilities/ArgsProcessor.cc

OBJS=$(subst .cc,.o,$(SRCS))

all: dapps

dapps: $(OBJS)
	$(CXX) $(LDFLAGS) -o dapps $(OBJS) $(LDLIBS)
	
depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

-include .depend