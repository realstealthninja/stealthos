# stealthos
StealthOs is meant to be a study of operating systems, Beware I am not a very competent C or assembly
developer. This code may contain bugs and errors and use at your own risk. If you do continue 
Please try it in a virtual machine.

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
export TARGET=x86_64 # specifing that our target platform is an x86_64 machine using the elf format
export PREFIX="/usr/local/cross" # where to install the cross compiler
```
> ![CAUTION]
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
copy over the release you got into the `build-binutils` directory and extract it. You should have the source code inside this folder.

Next we must configure this source code to fit our job. thats via the configure executable inside this directory

```bash
./configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
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








## resources used
- [the little os book](https://littleosbook.github.io)
- [OS dev wiki](https://wiki.osdev.org)
