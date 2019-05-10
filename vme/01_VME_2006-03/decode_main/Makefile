# @(#) 17 Mar 2006

ROOTSYS		= $(shell root-config --prefix)
include $(ROOTSYS)/etc/Makefile.arch
HdrSuf		= h
ifeq ($(MAKECMDGOALS),print_info)
CXXFLAGS	+= -DPRINT_INFO
endif

PROJECT		= HPTDC
NAMES		= THPTDCEvent
LNKDEF		= LinkDef

SRCS		= $(NAMES:=.$(SrcSuf))
HDRS		= $(SRCS:.$(SrcSuf)=.$(HdrSuf)) $(LNKDEF).$(HdrSuf)

DICT		= G__$(PROJECT).$(SrcSuf)
DICTH		= $(DICT:.$(SrcSuf)=.$(HdrSuf))
DICTO		= $(DICT:.$(SrcSuf)=.$(ObjSuf))

OBJS		= $(SRCS:.$(SrcSuf)=.$(ObjSuf)) $(DICTO)
PROJECTLIB	= lib$(PROJECT).$(DllSuf)

MAIN		= decode_main.$(SrcSuf)
MAINO		= $(MAIN:.$(SrcSuf)=.$(ObjSuf))
PROJECTPRG	= decode_main

ALLDICT		= $(DICT) $(DICTH)
ALLSRCS		= $(SRCS) $(HDRS) $(MAIN)
ALLOBJS		= $(OBJS) $(MAINO)

DISTNAME	= hptdc_$(shell date +%F).src
TMPDIR		= tmp
LIBDIR		= .
MAKEDEPEND	= rmkdepend

OBJS		:= $(patsubst %.$(ObjSuf),$(TMPDIR)/%.$(ObjSuf),$(OBJS))
PROJECTLIB	:= $(LIBDIR)/$(PROJECTLIB)

##### targets #####

all:		$(PROJECTPRG)

$(PROJECTLIB):	$(DICT) $(OBJS)
		@if [ ! -d $@ ]; then \
		    mkdir -p $(dir $@); fi
		@rm -f $(PROJECTLIB)
		@$(LD) $(SOFLAGS) $(LDFLAGS) $(OBJS) $(OutPutOpt) $(PROJECTLIB)
		@chmod 555 $(PROJECTLIB)
		@echo "==> $(PROJECTLIB) done"

$(DICT):	$(HDRS)
		@echo "Generating dictionary $@ ... "
		@rootcint -f $(DICT) -c $(HDRS)

$(PROJECTPRG):	$(PROJECTLIB) $(MAINO)
		@$(LD) $(LDFLAGS) $(MAINO) $(PROJECTLIB) $(OOOTHERRRR) \
		   $(LIBS) $(OutPutOpt) $(PROJECTPRG)
		@echo "program: $(PROJECTPRG) done"

print_info:	all

dist:
		@rm -f $(DISTNAME).tar.gz;
		@tar cvf $(DISTNAME).tar $(ALLSRCS) Makefile *.C
		@gzip $(DISTNAME).tar
		@echo -e "\n$(DISTNAME).tar.gz done"

clean:
		@echo "Delete all tmp files"
		@rm -f $(ALLOBJS) $(ALLDICT)

depend:		$(SRCS)
		@if [ ! -d $(TMPDIR)/Make-depend ]; then \
		    mkdir -p $(dir $(TMPDIR)/Make-depend); fi
		@if [ ! -e $(TMPDIR)/Make-depend ]; then \
		    > $(TMPDIR)/Make-depend; fi
		@$(MAKEDEPEND) -f $(TMPDIR)/Make-depend -p$(TMPDIR)/ \
		-- $(CXXFLAGS) -- $(SRCS) 2>/dev/null
		@echo "$(TMPDIR)/Make-depend done"

##### rules #####

.SUFFIXES:	.$(SrcSuf)

# loop over all %.cxx
.$(SrcSuf).$(ObjSuf):
		$(CXX) $(CXXFLAGS) -c $<

# loop over all TMPDIR/%.cxx
$(OBJS):	$(TMPDIR)/%.$(ObjSuf):%.$(SrcSuf)
		@if [ ! -d $@ ]; then \
		    mkdir -p $(dir $@); fi
		@$(CXX) $(CXXFLAGS) -c $< -o $@
		@echo -e "$@ done"

##### Make-depend #####
-include $(TMPDIR)/Make-depend
