TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    metropolis.cpp \
    random.cpp \
    actions/action.cpp \
    actions/impaction.cpp

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
    metropolis.h \
    random.h \
    actions/action.h \
    actions/impaction.h

