LIBRARY := SUHH2TopSubstructure

LHAPDFINC=$(shell scram tool tag lhapdf INCLUDE)
LHAPDFLIB=$(shell scram tool tag LHAPDF LIBDIR)

FJINC=$(shell scram tool tag FASTJET INCLUDE)
FJLIB=$(shell scram tool tag FASTJET LIBDIR)

USERCXXFLAGS := -I${LHAPDFINC}\
                -I${FJINC}
USERLDFLAGS := -lSUHH2core -lSUHH2common -lGenVector -lSUHH2JetMETObjects -L${LHAPDFLIB} -lLHAPDF\
                -Wl,-rpath,${FJLIB} -lm -L${FJLIB} -lfastjettools -lfastjet -lHOTVR -lNsubjettiness -lRecursiveTools

# enable par creation; this is necessary for all packages containing AnalysisModules
# to be loaded from by AnalysisModuleRunner.
PAR := 1
include ../Makefile.common
