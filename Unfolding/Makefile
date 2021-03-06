#
#  This file is part of TUnfold.
#
#  TUnfold is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  TUnfold is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with TUnfold.  If not, see <http://www.gnu.org/licenses/>.
#
ROOTCONFIG   := $(ROOTSYS)/bin/root-config
ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS  := $(shell $(ROOTCONFIG) --ldflags)
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
ROOTGLIBS    := $(shell $(ROOTCONFIG) --glibs)
ROOTVERSION  := $(shell $(ROOTCONFIG) --version)
ROOTMAJORVERSION := $(word 1,$(subst ., ,$(ROOTVERSION)))
ROOTCINT=$(ROOTSYS)/bin/rootcint

CXXFLAGS=-isystem $(shell $(ROOTCONFIG) --incdir) -I$(ROOTSYS)/htmldoc -I. -O0 -g -Wall -Wshadow -W -Woverloaded-virtual -fPIC $(ROOTCFLAGS)
LDFLAGS=$(ROOTLDFLAGS) -L. -Wl,-rpath .
CXX=$(shell $(ROOTCONFIG) --cxx)

OSTYPE=$(shell uname)
ROOTLIBS     := -L$(shell $(ROOTCONFIG) --libdir) -lXMLParser $(ROOTLIBS)

## version number Vx.y (e.g. x=17 y=0)
## code version Vx (e.g. x=17)
## header files TUnfold*.h
## source files TUnfold*.cxx
## macro files testUnfold*.C
##
## classes have the code version in the class name
##  e.g. TUnfoldV17
##
## in the header file, #define statements are added
## such that the user can use the conventional class names
##  e.g. #define TUnfold TUnfoldV17
##
## when exported to root, the code version is removed from the class name

TUNFOLDVERSION :=$(shell grep TUnfold_VERSION TUnfold.h)
TUNFOLDVERSION :=$(filter-out \#define TUnfold_VERSION,$(TUNFOLDVERSION))
TUNFOLDVERSION :=$(shell echo $(TUNFOLDVERSION))
TUNFOLDCODEVER :=$(subst ., ,$(TUNFOLDVERSION))
TUNFOLDCODEVER :=$(word 1,$(TUNFOLDCODEVER))
HEADER=$(shell ls TUnfold*.h | grep -v Dict)
SRC=$(shell ls TUnfold*$(TUNFOLDCODEVER).cxx)

MY_MACRO=$(shell ls unfold_code*.C) # include my macros
OBJ_binning = binning.o
OBJ_hist = hist_filler.o
OBJ_do_unfolding = do_unfolding.o unfold.o
OBJ_Plotter = Plotter.o Plots.o normalise.o

## this is for changing the code version
TUNFOLDNEWVERSION:=V17.6
TUNFOLDNEWCODEVER :=$(subst ., ,$(TUNFOLDNEWVERSION))
TUNFOLDNEWCODEVER :=$(word 1,$(TUNFOLDNEWCODEVER))
DIRNEW:=../TUnfold_$(TUNFOLDNEWVERSION)
HEADERNEW:=$(subst TUnfold,$(DIRNEW)/TUnfold,$(HEADER))
SRCNEW:=$(subst $(TUNFOLDCODEVER),$(TUNFOLDNEWCODEVER),$(SRC))
SRCNEW:=$(subst TUnfold,$(DIRNEW)/TUnfold,$(SRCNEW))
##
## library for stand-alone test

LIB=unfold

ifeq ($(OSTYPE),Linux)
	MAKELIBRARY = $(CXX) $(CXXFLAGS) -shared -o $(2) $(1)
	LB=lib$(LIB).so
#	LB0=lib$(LIB)0.so
else
	MAKELIBRARY = rm $(2) ; ar rv $(2) $(1) ; ranlib $(2)
	LB=lib$(LIB).a
#	LB0=lib$(LIB)0.a
endif

COBJC=$(SRC:%.cxx=%.o)
DICT=TUnfold$(TUNFOLDCODEVER)Dict.C
DICTINPUT=$(if $(subst 5,,$(ROOTMAJORVERSION)),$(HEADER),$(SRC))

TUNFOLDSOURCE := TUnfoldV17.cxx TUnfoldSysV17.cxx TUnfoldDensityV17.cxx TUnfoldBinningV17.cxx TUnfoldBinningXMLV17.cxx

TUNFOLDDICT := $(subst .cxx,Dict.cxx,$(TUNFOLDSOURCE))

##
## my macros
MY_BIN:=$(MY_BINSRC:%.C=%)


##
## tar file including all files
##

VERSIONTAR:=TUnfold_$(TUNFOLDVERSION).tgz

##
## source tree for root
## (with version number from class names)
##
ROOTSOURCES=$(subst TUnfold,hist/unfold/src/TUnfold,$(SRC:%$(TUNFOLDCODEVER).cxx=%.cxx))
ROOTHEADERS= $(subst TUnfold,hist/unfold/inc/TUnfold,$(HEADER))

##
## tar file including the root source tree
##

ROOTTAR:=TUnfold_$(TUNFOLDVERSION)_for_root.tgz

## shortcuts
##
##  depend
##  clean
##  lib
##  tar
##  roottar
##  manual


%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.C
	$(CXX) $(CXXFLAGS) -c $<

%Dict.cxx: %.cxx
	rm -f $@ $(@:.cxx=.h)
	$(ROOTCINT) $@ -c $<

depend:  $(SRC) $(HEADER)
	makedepend -V -I. $(SRC) $(HEADER)

clean:
	rm -f $(LB) *.o *Dict.* $(BINSRC) $(BIN:%=%.ps) *\~ tunfold_manual.pdf tunfold_manual.dvi *.pcm testUnfold5binning.xml testUnfold*.root filter testUnfold5.ps
	rm -f $(OBJ)
	rm -rf hist tutorials doxygen

lib: $(LB)

tar: $(VERSIONTAR)

roottar: $(ROOTTAR)


topsub: binning hist_filler do_unfolding Plotter

binning: $(OBJ_binning)
	$(CXX) $(CXXFLAGS) -o binning $(OBJ_binning) $(DICT) $(LDFLAGS) -l$(LIB) $(ROOTLIBS)

hist_filler: $(OBJ_hist)
	$(CXX) $(CXXFLAGS) -o hist_filler $(OBJ_hist) $(DICT) $(LDFLAGS) -l$(LIB) $(ROOTLIBS)

do_unfolding: $(OBJ_do_unfolding)
	$(CXX) $(CXXFLAGS) -o do_unfolding $(OBJ_do_unfolding) $(DICT) $(LDFLAGS) -l$(LIB) $(ROOTLIBS)

Plotter: $(OBJ_Plotter)
	$(CXX) $(CXXFLAGS) -o Plotter $(OBJ_Plotter) $(DICT) $(LDFLAGS) -l$(LIB) $(ROOTLIBS)



# extract doxygen documentation

filter: doxygentest/filter.cxx
	g++ -g --std=c++11 $< -o$@

doxygen: clean filter
	mkdir -p doxygen
	export DOXYGEN_OUTPUT_DIRECTORY=doxygen export DOXYGEN_SOURCE_DIRECTORY=. ; doxygen doxygentest/Doxyfile

# create tar file including all files beloning to this version

$(VERSIONTAR): $(HEADER) $(SRC) $(MACRO) altercodeversion.sh Makefile README COPYING tunfold_manual.tex tunfold_manual.pdf $(FIGURES) testUnfold6binning.xml testUnfold7binning.xml tunfoldbinning.dtd
	tar cvfz $@ $+

# create root source tree

$(ROOTTAR): $(ROOTSOURCES) $(ROOTHEADERS) $(ROOTMACROS)
	tar cvfz $@ $+

$(ROOTHEADERS): hist/unfold/inc/%.h : %.h altercodeversion.sh
	mkdir -p hist/unfold/inc
	./altercodeversion.sh $< $(TUNFOLDCODEVER) > $@

$(ROOTSOURCES): hist/unfold/src/%.cxx : %$(TUNFOLDCODEVER).cxx altercodeversion.sh
	mkdir -p hist/unfold/src
	./altercodeversion.sh $< $(TUNFOLDCODEVER) > $@

$(ROOTMACROS): tutorials/unfold/%.C : %.C
	mkdir -p tutorials/unfold
	cp $< $@

# create new version
# (some manual changes are required in addition)

newversion: copyoldversion $(HEADERNEW) $(SRCNEW)

copyoldversion:
	make $(VERSIONTAR)
	mkdir -p $(DIRNEW)
	cp $(VERSIONTAR) $(DIRNEW)/$(VERSIONTAR)
	cd $(DIRNEW) ; tar xvfz $(VERSIONTAR)
	rm $(DIRNEW)/$(VERSIONTAR)
	mkdir -p $(DIRNEW)/tmpheader
	rm $(DIRNEW)/*.h
	rm $(DIRNEW)/*.cxx

$(HEADERNEW): $(HEADER)
	mkdir -p $(DIRNEW)
	./altercodeversion.sh $(subst $(DIRNEW)/,,$@) $(TUNFOLDCODEVER) $(TUNFOLDNEWCODEVER) > $@

$(SRCNEW): $(SRC)
	mkdir -p $(DIRNEW)
	./altercodeversion.sh $(subst $(TUNFOLDNEWCODEVER),$(TUNFOLDCODEVER),$(subst $(DIRNEW)/,,$@)) $(TUNFOLDCODEVER) $(TUNFOLDNEWCODEVER) > $@

# Root cint preprocessor

dict: $(DICT)

$(DICT): $(DICTINPUT)
	$(ROOTCINT) -f $@ -c -p $^

# library of TUnfold classes

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LB): $(COBJC)
	$(call MAKELIBRARY,$(COBJC),$(LB))

# create code to compile example macros stand-alone

# $(MY_BINSRC): Makefile
# 	rm -f $@
# 	echo "#include <TError.h>" > $@
# 	echo "/* This code is generated automatically, do not edit */" >> $@
# 	echo "void "$(patsubst %.C,%,$(subst unfold_app_,unfold_code_,$@))"(char*, char*, char*);" >> $@
# 	echo "int main(int argc, char* argv[]) {" >>$@
# 	echo "gErrorIgnoreLevel=kInfo+1;" >>$@
# 	echo "gErrorAbortLevel=kError;" >>$@
# 	echo $(patsubst %.C,%,$(subst unfold_app_,unfold_code_,$@))"(argv[1],argv[2],argv[3]);" >>$@
# 	echo "return 0;" >> $@
# 	echo "}" >> $@

# compile macros stand-alone

%.o: %.C
	$(CXX) $(CXXFLAGS) -c $< -o $@

# $(MY_BIN): unfold_%: unfold_code_%.C $(LB) $(DICT) unfold_code_%.o
# 	$(CXX) $(CXXFLAGS) $< -o  $@ $(DICT) unfold_code_$(*).o $(LDFLAGS) -l$(LIB) \
# 	$(ROOTLIBS)



# DO NOT DELETE

TUnfoldBinningV17.o: TUnfoldBinningXML.h TUnfoldBinning.h TUnfold.h
TUnfoldBinningXMLV17.o: TUnfold.h TUnfoldBinningXML.h TUnfoldBinning.h
TUnfoldDensityV17.o: TUnfoldDensity.h TUnfoldSys.h TUnfold.h TUnfoldBinning.h
TUnfoldSysV17.o: TUnfoldSys.h TUnfold.h
TUnfoldV17.o: TUnfold.h
TUnfoldBinning.o: TUnfold.h
TUnfoldBinningXML.o: TUnfoldBinning.h TUnfold.h
TUnfoldDensity.o: TUnfoldSys.h TUnfold.h TUnfoldBinning.h
TUnfoldSys.o: TUnfold.h
