#############################################################################
# Makefile for building: NacServer
# Generated by qmake (2.01a) (Qt 4.8.1) on: Sun Sep 14 23:23:54 2014
# Project:  ../NacServer/NacServer.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile ../NacServer/NacServer.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I../NacServer -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I../NacServer -I../NacServer/common -I../NacServer/server -I. -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -lQtGui -lQtCore -lpthread -lnetfilter_queue -ltokyocabinet 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = ../NacServer/dialog.cpp \
		../NacServer/main.cpp \
		../NacServer/common/des.cpp \
		../NacServer/common/KWhiteList.cpp \
		../NacServer/common/rsa.cpp \
		../NacServer/server/KServer.cpp \
		../NacServer/server/KServerSocket.cpp \
		../NacServer/server/KUsrInfo.cpp \
		../NacServer/server/mylog.cpp moc_dialog.cpp
OBJECTS       = dialog.o \
		main.o \
		des.o \
		KWhiteList.o \
		rsa.o \
		KServer.o \
		KServerSocket.o \
		KUsrInfo.o \
		mylog.o \
		moc_dialog.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		../NacServer/NacServer.pro
QMAKE_TARGET  = NacServer
DESTDIR       = 
TARGET        = NacServer

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	{ test -n "$(DESTDIR)" && DESTDIR="$(DESTDIR)" || DESTDIR=.; } && test $$(gdb --version | sed -e 's,[^0-9]\+\([0-9]\)\.\([0-9]\).*,\1\2,;q') -gt 72 && gdb --nx --batch --quiet -ex 'set confirm off' -ex "save gdb-index $$DESTDIR" -ex quit '$(TARGET)' && test -f $(TARGET).gdb-index && objcopy --add-section '.gdb_index=$(TARGET).gdb-index' --set-section-flags '.gdb_index=readonly' '$(TARGET)' '$(TARGET)' && rm -f $(TARGET).gdb-index || true

Makefile: ../NacServer/NacServer.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile ../NacServer/NacServer.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile ../NacServer/NacServer.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/NacServer1.0.0 || $(MKDIR) .tmp/NacServer1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/NacServer1.0.0/ && $(COPY_FILE) --parents ../NacServer/dialog.h ../NacServer/common/constants.h ../NacServer/common/DES.h ../NacServer/common/KMac.hpp ../NacServer/common/KProtocol.hpp ../NacServer/common/KWhiteList.h ../NacServer/common/rsa.h ../NacServer/server/KServer.h ../NacServer/server/KServerSocket.h ../NacServer/server/KUsrInfo.h ../NacServer/server/mylog.h .tmp/NacServer1.0.0/ && $(COPY_FILE) --parents ../NacServer/dialog.cpp ../NacServer/main.cpp ../NacServer/common/des.cpp ../NacServer/common/KWhiteList.cpp ../NacServer/common/rsa.cpp ../NacServer/server/KServer.cpp ../NacServer/server/KServerSocket.cpp ../NacServer/server/KUsrInfo.cpp ../NacServer/server/mylog.cpp .tmp/NacServer1.0.0/ && (cd `dirname .tmp/NacServer1.0.0` && $(TAR) NacServer1.0.0.tar NacServer1.0.0 && $(COMPRESS) NacServer1.0.0.tar) && $(MOVE) `dirname .tmp/NacServer1.0.0`/NacServer1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/NacServer1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_dialog.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_dialog.cpp
moc_dialog.cpp: ../NacServer/common/constants.h \
		../NacServer/server/KUsrInfo.h \
		../NacServer/server/mylog.h \
		../NacServer/common/KMac.hpp \
		../NacServer/common/rsa.h \
		../NacServer/dialog.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ../NacServer/dialog.h -o moc_dialog.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

dialog.o: ../NacServer/dialog.cpp ../NacServer/dialog.h \
		../NacServer/common/constants.h \
		../NacServer/server/KUsrInfo.h \
		../NacServer/server/mylog.h \
		../NacServer/common/KMac.hpp \
		../NacServer/common/rsa.h \
		../NacServer/server/KServer.h \
		../NacServer/server/KServerSocket.h \
		../NacServer/common/KProtocol.hpp \
		../NacServer/common/KWhiteList.h \
		../NacServer/common/DES.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dialog.o ../NacServer/dialog.cpp

main.o: ../NacServer/main.cpp ../NacServer/dialog.h \
		../NacServer/common/constants.h \
		../NacServer/server/KUsrInfo.h \
		../NacServer/server/mylog.h \
		../NacServer/common/KMac.hpp \
		../NacServer/common/rsa.h \
		../NacServer/server/KServer.h \
		../NacServer/server/KServerSocket.h \
		../NacServer/common/KProtocol.hpp \
		../NacServer/common/KWhiteList.h \
		../NacServer/common/DES.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o ../NacServer/main.cpp

des.o: ../NacServer/common/des.cpp ../NacServer/common/DES.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o des.o ../NacServer/common/des.cpp

KWhiteList.o: ../NacServer/common/KWhiteList.cpp ../NacServer/common/KWhiteList.h \
		../NacServer/common/constants.h \
		../NacServer/common/KMac.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o KWhiteList.o ../NacServer/common/KWhiteList.cpp

rsa.o: ../NacServer/common/rsa.cpp ../NacServer/common/rsa.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o rsa.o ../NacServer/common/rsa.cpp

KServer.o: ../NacServer/server/KServer.cpp ../NacServer/server/KServer.h \
		../NacServer/server/KServerSocket.h \
		../NacServer/common/constants.h \
		../NacServer/dialog.h \
		../NacServer/server/KUsrInfo.h \
		../NacServer/server/mylog.h \
		../NacServer/common/KMac.hpp \
		../NacServer/common/rsa.h \
		../NacServer/common/KProtocol.hpp \
		../NacServer/common/KWhiteList.h \
		../NacServer/common/DES.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o KServer.o ../NacServer/server/KServer.cpp

KServerSocket.o: ../NacServer/server/KServerSocket.cpp ../NacServer/server/KServerSocket.h \
		../NacServer/common/constants.h \
		../NacServer/dialog.h \
		../NacServer/server/KUsrInfo.h \
		../NacServer/server/mylog.h \
		../NacServer/common/KMac.hpp \
		../NacServer/common/rsa.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o KServerSocket.o ../NacServer/server/KServerSocket.cpp

KUsrInfo.o: ../NacServer/server/KUsrInfo.cpp ../NacServer/server/KUsrInfo.h \
		../NacServer/common/constants.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o KUsrInfo.o ../NacServer/server/KUsrInfo.cpp

mylog.o: ../NacServer/server/mylog.cpp ../NacServer/server/mylog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mylog.o ../NacServer/server/mylog.cpp

moc_dialog.o: moc_dialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_dialog.o moc_dialog.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

