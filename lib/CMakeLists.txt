project(klib)

add_library(klib src/string.c include/string.h)

target_include_directories(klib PUBLIC include/)

target_compile_options(
    klib
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
    PRIVATE "-ffreestanding"
    PRIVATE "-fno-stack-protector"
	PRIVATE "-fno-stack-check"
	PRIVATE "-fno-lto"
	PRIVATE "-fPIE"
	PRIVATE "-m64"
	PRIVATE "-mcmodel=large"
	PRIVATE "-march=x86-64"
	PRIVATE "-mno-80387"
	PRIVATE "-mno-mmx"
	PRIVATE "-mno-sse"
	PRIVATE "-mno-sse2"
	PRIVATE "-mno-red-zone"
)
