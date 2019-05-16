# Makefile for Strela project
# support architectures: linux, linuxx8664gcc
# 06 Feb 2005

ARCH		:= $(shell root-config --arch)

CXX		=
ObjSuf		= o
SrcSuf		= cxx
HdrSuf		= h
ExeSuf		=
DllSuf		= so
OutPutOpt	= -o # keep whitespace after "-o"

ROOTCFLAGS	:= $(shell root-config --cflags)
ROOTLIBS	:= $(shell root-config --libs)
ROOTGLIBS	:= $(shell root-config --glibs)

ifeq ($(ARCH),linux)
# Linux with egcs, gcc 2.9x, gcc 3.x
CXX		= g++
CXXFLAGS	= -O -Wall -fPIC
LD		= g++
LDFLAGS		= -O
SOFLAGS		= -shared
endif

ifeq ($(ARCH),linuxx8664gcc)
# AMD Opteron (64 bit mode) Linux with gcc 3.x
CXX		= g++
CXXFLAGS	= -O2 -Wall -fPIC
LD		= g++
LDFLAGS		= -O2
SOFLAGS		= -shared
endif

CXXFLAGS	+= $(ROOTCFLAGS)
ifeq ($(MAKECMDGOALS),info)
CXXFLAGS	+= -DINFO
endif
ifeq ($(MAKECMDGOALS),debug)
CXXFLAGS	+= -DDEBUG -DINFO
endif
LIBS		= $(ROOTLIBS) $(SYSLIBS)
GLIBS		= $(ROOTGLIBS) $(SYSLIBS)

###### macros #####

# change only this
PROJECT		= MyEvent
NAMES		= TStrawTrack TMyEvent
LNKDEF		= TrackLinkDef
# end changes

SRCS		= $(NAMES:=.$(SrcSuf))
HDRS		= $(SRCS:.$(SrcSuf)=.$(HdrSuf)) $(LNKDEF).$(HdrSuf)

DICT		= $(PROJECT)Dict.$(SrcSuf)
DICTH		= $(DICT:.$(SrcSuf)=.$(HdrSuf))
DICTO		= $(DICT:.$(SrcSuf)=.$(ObjSuf))

OBJS		= $(SRCS:.$(SrcSuf)=.$(ObjSuf)) $(DICTO)
PROJECTLIB	= lib$(PROJECT).$(DllSuf)

#MAIN		= main.cxx
#MAINO		= $(MAIN:.$(SrcSuf)=.$(ObjSuf))
#PROJECTPRG	= strela

ALLDICT		= $(DICT) $(DICTH) $(DICTG) $(DICTGH)
ALLSRCS		= $(SRCS) $(HDRS) $(SRCSG) $(HDRSG) $(MAIN)
ALLOBJS		= $(OBJS) $(OBJSG) $(MAINO)

DISTNAME	= dist_$(shell date +%F)
TGTDIR		= tgt
LIBDIR		= lib
MAKEDEPEND	= rmkdepend

OBJS		:= $(patsubst %.$(ObjSuf),$(TGTDIR)/%.$(ObjSuf),$(OBJS))
PROJECTLIB	:= $(LIBDIR)/$(PROJECTLIB)

##### targets #####

all:		$(PROJECTLIB)

$(PROJECTLIB):	$(DICT) $(OBJS)
		@[ -d $(dir $@) ] || mkdir -p $(dir $@)
		@rm -f $(PROJECTLIB)
		$(LD) $(SOFLAGS) $(LDFLAGS) $(OBJS) $(OutPutOpt) $(PROJECTLIB)
		@chmod 555 $(PROJECTLIB)
		@echo "$(PROJECTLIB) done"

$(DICT):	$(HDRS)
		@echo "Generating dictionary $@ ..."
		rootcint -f $(DICT) -c $(HDRS)

$(PROJECTPRG):	$(PROJECTLIB) $(MAINO)
		$(LD) $(LDFLAGS) $(MAINO) $(PROJECTLIB) $(OOOTHERRRR) \
		$(LIBS) $(OutPutOpt) $(PROJECTPRG)
		@echo "$(PROJECTPRG) done"

debug:		all

info:		all

dist:
		rm -f $(DISTNAME).tar.gz; \
		tar cvf $(DISTNAME).tar $(ALLSRCS) *.C Makefile; \
		gzip $(DISTNAME).tar
		@echo "$(DISTNAME).tar.gz done"

clean:
		@rm -f $(ALLOBJS) $(ALLDICT)

depend:		$(SRCS)
		@[ -d $(dir $(TGTDIR)/Make-depend) ] || \
		mkdir -p $(dir $(TGTDIR)/Make-depend)
		@( test -e $(TGTDIR)/Make-depend ) || \
		>$(TGTDIR)/Make-depend
		$(MAKEDEPEND) -f$(TGTDIR)/Make-depend -p$(TGTDIR)/ \
		-- $(CXXFLAGS) -- $(SRCS)

##### rules #####

.SUFFIXES:	.$(SrcSuf)

# loop over all %.cxx
.$(SrcSuf).$(ObjSuf):
		$(CXX) $(CXXFLAGS) -c $<

# loop over all TGTDIR/%.cxxz
$(OBJS):	$(TGTDIR)/%.$(ObjSuf):%.$(SrcSuf)
		@[ -d $(dir $@) ] || mkdir -p $(dir $@)
		$(CXX) $(CXXFLAGS) -c $< -o $@

##### Make-depend #####
-include $(TGTDIR)/Make-depend
