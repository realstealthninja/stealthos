# stealthos
StealthOs is meant to be a study of operating systems, Beware I am not a very competent C or assembly
developer. This code may contain bugs and errors and use at your own risk. If you do continue 
Please try it in a virtual machine.


# building
The build process requires a [cross compiler](https://wiki.osdev.org/GCC_Cross-Compiler)

## Cross compilers
Cross compilers are compiler which can be used to compile code written in the host system to another system of same or different architecture, size, any attributes really.

I'll be storing my cross compiler in `/usr/local/cross` since im the administrator of my system.


### prequisites
download the following for compiling a cross compiler.
- binutils [release](https://sourceware.org/pub/binutils/releases/) preferably the latest.
- gcc [release](https://ftp.gnu.org/gnu/gcc/) preferably the latest


Its also helpful to have the following enviornment variables
```bash
export TARGET=x86_64-elf # specifing that our target platform is an x86_64 machine using the elf format
export PREFIX="/usr/local/cross" # where to install the cross compiler
```
> [!CAUTION]
> DO NOT MINDLESSLY TYPE COMMANDS
> You may mess up your local gcc/libgcc
> these above variables get reset per shell
> thus it is import to confirm that these are
> properly set before typing commands

make a new directory somewhere inside your projects directory (if you have one)
```bash
mkdir cross
mkdir cross/build-binutils
mkdir cross/build-gcc
```


### binutils
extract the release you got then cd into the`build-binutils` directory.

Next we must configure this source code to fit our job. thats via the configure executable inside this directory

```bash
../binutils.x.x.x/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
```

explanation of the above command
- target: the platform we are targeting
- prefix: where binutils is to be installed
- disable-werror: disables treating erros as warnings in the build
- with-sysroot: (help needed)
- diable-nls: stop using native language support

we must then compile and install to our prefix
```bash
make -j $(nproc)
make install
```
the `-j$(nproc)` just makes the compiler use all the available cores. i really didnt want to wait for this build

And there you have it you have bin utils up

### GCC
similarily we will be compiling GCC.
begin by switching to the gcc directory.

Then you'll have to create a file named `t-x86_64-elf` inside`config/i386` make the directory if need be.

```bash
# Add libgcc multilib variant without red zone requirement

MULTILIB_OPTION += mno-red-zone
MULTILIB_DIRNAMES += no-red-zone
```

what we are doing here is disabling red zone. Which is explained very well in the [os dev wiki](https://wiki.osdev.org/Libgcc_without_red_zone)

edit gcc/config.gcc
```bash
 x86_64-*-elf*)
	tmake_file="${tmake_file} i386/t-x86_64-elf" # include the new multilib configuration
	tm_file="${tm_file} i386/unix.h i386/att.h elfos.h newlib-stdint.h i386/i386elf.h i386/x86-64.h"
	;;
```

then build this;
```bash
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH

mkdir build-gcc
cd build-gcc
../gcc-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
make all-gcc
make all-target-libgcc
make all-target-libstdc++-v3
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3
```
these commands were copied from the gcc cross compiler article
of [osdev.org](https://wiki.osdev.org/GCC_Cross-Compiler)

## building the os
get the source code; git clone <link>.
edit the TC-stealthos.cmake toolchain file
put the path of the cross compiler in there

```bash
mkdir build
cd build
cmake .. --DCMAKE_TOOLCHAIN_FILE=TC-stealthos.cmake
cmake --build . --target iso
```

and you'll have a stealthos.iso file


## resources used
- [the little os book](https://littleosbook.github.io)
- [OS dev wiki](https://wiki.osdev.org)
