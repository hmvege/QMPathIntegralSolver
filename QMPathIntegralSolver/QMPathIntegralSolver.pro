TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    lib/random.cpp \
    actions/action.cpp \
    actions/impaction.cpp \
    observables/observable.cpp \
    potentials/potential.cpp \
    potentials/harmonicoscillator.cpp \
    observables/correlatorx1.cpp \
    observables/correlatorx3.cpp \
    system.cpp

## MPI Settings, uncommented for mac
#QMAKE_CXX = mpicxx
#QMAKE_CXX_RELEASE = $$QMAKE_CXX
#QMAKE_CXX_DEBUG = $$QMAKE_CXX
#QMAKE_LINK = $$QMAKE_CXX
#QMAKE_CC = mpicc

#QMAKE_CFLAGS += $$system(mpicc --showme:compile)
#QMAKE_LFLAGS += $$system(mpicxx --showme:link)
#QMAKE_CXXFLAGS += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
#QMAKE_CXXFLAGS_RELEASE += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK

HEADERS += \
    lib/random.h \
    actions/action.h \
    actions/impaction.h \
    observables/observable.h \
    potentials/potential.h \
    potentials/harmonicoscillator.h \
    observables/correlatorx1.h \
    observables/correlatorx3.h \
    system.h

# Adds processor specific optimizations
QMAKE_CFLAGS += -march=native
QMAKE_CXXFLAGS += -march=native
QMAKE_CXXFLAGS_RELEASE += -march=native

# Adds O3 optimizations
QMAKE_CFLAGS += -O3
QMAKE_CXXFLAGS += -O3
QMAKE_CXXFLAGS_RELEASE += -O3

# Forces C++11
QMAKE_CFLAGS += -std=c++11
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE += -std=c++11

# Removes flags
QMAKE_CFLAGS -= -O2
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2
