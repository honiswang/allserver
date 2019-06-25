include ../../build.mak
include $(PJDIR)/build/common.mak
export LIBDIR := ../lib
export BINDIR := ../bin

RULES_MAK := $(PJDIR)/build/rules.mak


###############################################################################
# Gather all flags.
#
export _CFLAGS 	:= $(CC_CFLAGS) $(OS_CFLAGS) $(HOST_CFLAGS) $(M_CFLAGS) \
		   $(PJ_CFLAGS) $(CFLAGS) $(CC_INC)../../pjsip/include \
		   $(CC_INC)../../pjlib/include \
		   $(CC_INC)../../pjlib-util/include \
		   $(CC_INC)../../pjnath/include \
		   $(CC_INC)../../pjmedia/include
export _CXXFLAGS:= $(_CFLAGS) $(CC_CXXFLAGS) $(OS_CXXFLAGS) $(M_CXXFLAGS) \
		   $(HOST_CXXFLAGS) $(CXXFLAGS) 
export _LDFLAGS := $(CC_LDFLAGS) $(OS_LDFLAGS) $(M_LDFLAGS) $(HOST_LDFLAGS) \
		   $(APP_LDFLAGS) $(APP_LDLIBS) $(LDFLAGS) 

###############################################################################
# Defines for building ALLSERVER
#
export ALLSERVER_SRCDIR = ../src/allserver
export ALLSERVER_OBJS += main.o sync.o pstring.o singleton.o job.o testjob.o common.o serverjob.o
export ALLSERVER_CFLAGS += $(PJ_CFLAGS) $(CFLAGS)
export ALLSERVER_CXXFLAGS += $(PJ_CXXFLAGS) $(CFLAGS) -DDEBUG
export ALLSERVER_LDFLAGS += $(PJ_LDFLAGS) $(PJ_LDLIBS) $(LDFLAGS)
ifeq ($(EXCLUDE_APP),0)
export ALLSERVER_EXE:=allserver-$(TARGET_NAME)$(HOST_EXE)
endif

###############################################################################
# Defines for building pjsystest
#

export CC_OUT CC AR RANLIB HOST_MV HOST_RM HOST_RMDIR HOST_MKDIR OBJEXT LD LDOUT 

TARGETS := $(ALLSERVER_EXE) 

all: $(TARGETS)

swig:
	$(MAKE) -C ../src/swig
	
doc:

dep: depend
distclean: realclean

.PHONY: all dep depend clean realclean distclean
.PHONY: $(TARGETS)
.PHONY: $(ALLSERVER_EXE)

allserver: $(ALLSERVER_EXE)
$(ALLSERVER_EXE):
	$(MAKE) -f $(RULES_MAK) APP=ALLSERVER app=allserver $(subst /,$(HOST_PSEP),$(BINDIR)/$@)
	@if echo "$(TARGET_NAME)" | grep -q "apple-darwin_ios$$"; then \
	  for F in $(filter %$(TARGET_NAME).a,$(PJ_LIBXX_FILES)); do \
	    if test -f $$F; then \
	      tmp=`echo $${F##*/} | sed -e "s/\-$(TARGET_NAME)\.a/.a/"`; \
	      ln -sf $$F ../src/allserver/ios/$$tmp; \
	    fi; \
	  done; \
	fi;


samples:
	$(MAKE) -f Samples.mak

.PHONY: allserver.ko
allserver.ko:
	$(MAKE) -f $(RULES_MAK) APP=ALLSERVER app=allserver $(subst /,$(HOST_PSEP),$(LIBDIR)/$@)

clean depend realclean:
	$(MAKE) -f $(RULES_MAK) APP=ALLSERVER app=allserver $@
	$(MAKE) -f Samples.mak $@
	@if test "$@" = "depend"; then \
	  echo '$(BINDIR)/$(ALLSERVER_EXE): $(APP_LIB_FILES)' >> .allserver-$(TARGET_NAME).depend; \
	fi
	@if echo "$(TARGET_NAME)" | grep -q "apple-darwin_ios$$"; then \
	  for F in $(filter %$(TARGET_NAME).a,$(PJ_LIBXX_FILES)); do \
	    tmp=`echo $${F##*/} | sed -e "s/\-$(TARGET_NAME)\.a/.a/"`; \
	    rm -f ../src/allserver/ios/$$tmp; \
	  done; \
	fi;
