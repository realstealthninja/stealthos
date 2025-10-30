project(kernel)

add_executable(
    kernel.elf
    src/kernel.c
    src/vga.c
    src/psf.c
    src/tty.c
    src/io/serial.c
    src/memory/gdt.c
    src/memory/gdt.s
    src/interrupts.c
	src/interrupts.s
	src/io/keyboard.c
    src/pic.c
	src/io/ps2.c
)
set(CMAKE_C_STANDARD 23)
target_link_libraries(kernel.elf PRIVATE klib)
target_include_directories(kernel.elf PRIVATE include/ ../extern/limine)

target_compile_options(
    kernel.elf
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
	PRIVATE "-save-temps"
)

target_link_options(
	kernel.elf
	PUBLIC "-ffreestanding"
	PUBLIC "-nostdlib"
	PUBLIC "-mcmodel=kernel"
	PUBLIC "-static"
	PUBLIC "-pie"
	PUBLIC "-T${CMAKE_SOURCE_DIR}/kernel/boot/link.ld"
)

