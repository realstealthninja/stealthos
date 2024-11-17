cmake_minimum_required(VERSION 3.30)

project(StealthOS VERSION 0.1.0 LANGUAGES C ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(kernel/)

# command to generate stealthos.iso
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/stealthos.iso
    COMMAND ${CMAKE_COMMAND} -E make_directory "iso/boot/grub"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/kernel/kernel.elf ${CMAKE_BINARY_DIR}/iso/boot/kernel.elf
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/iso/grub.cfg ${CMAKE_BINARY_DIR}/iso/boot/grub/grub.cfg
    COMMAND grub-mkrescue ARGS -o stealthos.iso ${CMAKE_BINARY_DIR}/iso
    DEPENDS ${CMAKE_BINARY_DIR}/kernel/kernel.elf
)

# you call build this target via
# cmake --build . --target iso
add_custom_target(iso
    DEPENDS ${CMAKE_BINARY_DIR}/stealthos.iso
)

#   run bochs -f bochsrc.txt -q
