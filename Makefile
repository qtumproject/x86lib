#Copyright (c) 2007 - 2010 Jordan "Earlz/hckr83" Earls  <http://www.Earlz.biz.tm>
#All rights reserved.

#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions
#are met:

#1. Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#3. The name of the author may not be used to endorse or promote products
#   derived from this software without specific prior written permission.
#   
#THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
#INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
#AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
#THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#This file is part of the x86Lib project.


_OBJS=objs/x86Lib.o objs/ops/ect.o objs/ops/groups.o objs/ops/store.o objs/ops/flags.o objs/ops/strings.o objs/ops/flow.o objs/ops/maths.o \
      objs/device_manager.o objs/modrm.o objs/cpu_helpers.o


debug_CPPFLAGS=-Wall -g -fexceptions -I./include -DX86LIB_BUILD -fPIC
release_CPPFLAGS=-Os -I./include -DX86LIB_BUILD
profile_CPPFLAGS=-pg -O2 -fexceptions -I./include -DX86LIB_BUILD
#_CPPFLAGS=-O2 -march=athlon64 -I./include -DX86LIB_BUILD
#_CPPFLAGS=-O2 -save-temps -I./include -DX86LIB_BUILD

#NOTE: Full(-O3) optimization does not work...it causes problems with op_cache.
#but, every other optimization level works including for size(-Os)

VERSION=1.0


#target information:
#default: will build the library and test application with debug information, and statically link it.
#  default will not create the shared object, as it is assumed you only make this during development.
#test: will build only the test application and asm code and will statically link it.
#release: will build everything except asm source and will build using static linking.
# this will also build iwth optimizations.
#install_static: will install the static library into $(INSTALL_PREFIX)/usr/local/lib
#install_header: will install the x86lib header into $(INSTALL_PREFIX)/usr/local/include
#install_shared: will install the shared library into $(INSTALL_PREFIX)/usr/local/lib
#install: will build shared-release, install_header, and install_shared targets.
#install-static: will build release, install_header, and install_static targets.
#uninstall: will remove the header, shared object, and static library from $(INSTALL_PREFIX)/usr/local

default:
#build the library
	i386-elf-gcc -c testos.c -o testos.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fdata-sections -ffunction-sections
	i386-elf-gcc -dead_strip -T linker.ld -o testos.bin -ffreestanding -O2 -nostdlib testos.o -lgcc -Wl,--gc-sections
	yasm -o test_os.bin test_os.asm
	yasm -o bios.bin bios.asm
	g++ $(debug_CPPFLAGS) -c x86Lib.cpp -o objs/x86Lib.o
	g++ $(debug_CPPFLAGS) -c ops/ect.cpp -o objs/ops/ect.o
	g++ $(debug_CPPFLAGS) -c ops/maths.cpp -o objs/ops/maths.o
	g++ $(debug_CPPFLAGS) -c ops/flow.cpp -o objs/ops/flow.o
	g++ $(debug_CPPFLAGS) -c ops/strings.cpp -o objs/ops/strings.o
	g++ $(debug_CPPFLAGS) -c ops/flags.cpp -o objs/ops/flags.o
	g++ $(debug_CPPFLAGS) -c ops/store.cpp -o objs/ops/store.o
	g++ $(debug_CPPFLAGS) -c ops/groups.cpp -o objs/ops/groups.o
	g++ $(debug_CPPFLAGS) -c device_manager.cpp -o objs/device_manager.o
	g++ $(debug_CPPFLAGS) -c modrm.cpp -o objs/modrm.o
	g++ $(debug_CPPFLAGS) -c cpu_helpers.cpp -o objs/cpu_helpers.o
	ar crs libx86Lib.a $(_OBJS)
	g++ -fPIC -shared $(_OBJS) -o libx86Lib.so.$(VERSION)
#Build test client application
	g++ $(debug_CPPFLAGS) -c tester.cpp -o objs/tester.o
	g++ $(debug_CPPFLAGS) -static -o x86Lib_test objs/tester.o -lx86Lib -L.

install_static:
#This is only for unix based OSs, where /usr/local/lib is library dir, and /usr/local/include is include dircd /usr
#This will install the static library.	
	install libx86Lib.a $(INSTALL_PREFIX)/usr/local/lib

install_header:
	install include/x86Lib.h $(INSTALL_PREFIX)/usr/local/include

install_shared:
	install libx86Lib.so.$(VERSION) $(INSTALL_PREFIX)/usr/local/lib

#installs the shared release
install: shared-release install_shared install_header

install-static: release install_static install_header

uninstall:
	rm -f $(INSTALL_PREFIX)/usr/local/include/x86Lib.h
	rm -f $(INSTALL_PREFIX)/usr/local/lib/libx86Lib.so.$(VERSION)
	rm -f $(INSTALL_PREFIX)/usr/local/lib/libx86Lib.a

clean:
	rm -f $(_OBJS)
