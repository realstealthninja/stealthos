cmake_minimum_required(VERSION 3.30)

project(StealthOS VERSION 0.1.0 LANGUAGES C ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(lib/)
add_subdirectory(kernel/)

set(CMAKE_BUILD_TYPE Debug)

# command to generate stealthos.iso
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/stealthos.iso
    COMMAND ${CMAKE_COMMAND} -E chdir ../extern/limine/ make
    COMMAND ${CMAKE_COMMAND} -E make_directory "iso/boot/limine"
    COMMAND ${CMAKE_COMMAND} -E make_directory "iso/boot/assets"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/kernel/kernel.elf ${CMAKE_BINARY_DIR}/iso/boot/kernel.elf
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/assets/ ${CMAKE_BINARY_DIR}/iso/boot/assets/

    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/iso/limine.conf ${CMAKE_BINARY_DIR}/iso/boot/limine/limine.conf
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ../extern/limine/limine-bios.sys ${CMAKE_BINARY_DIR}/iso/boot/limine/limine-bios.sys
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ../extern/limine/limine-bios-cd.bin ${CMAKE_BINARY_DIR}/iso/boot/limine/limine-bios-cd.bin
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ../extern/limine/limine-uefi-cd.bin ${CMAKE_BINARY_DIR}/iso/boot/limine/limine-uefi-cd.bin


    COMMAND ${CMAKE_COMMAND} -E make_directory "iso/EFI/BOOT"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ../extern/limine/BOOTX64.EFI ${CMAKE_BINARY_DIR}/iso/EFI/BOOT/BOOTX64.EFI
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ../extern/limine/BOOTIA32.EFI ${CMAKE_BINARY_DIR}/iso/EFI/BOOT/BOOTIA32.EFI

    



    COMMAND xorriso ARGS -as mkisofs -R
                        -r -J -b boot/limine/limine-bios-cd.bin
                        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus
                        -apm-block-size 2048 --efi-boot /boot/limine/limine-uefi-cd.bin
                        -efi-boot-part --efi-boot-image --protective-msdos-label ${CMAKE_BINARY_DIR}/iso
                        -o stealthos.iso
    COMMAND ../extern/limine/limine bios-install stealthos.iso
    DEPENDS ${CMAKE_BINARY_DIR}/kernel/kernel.elf
)

# you call build this target via
# cmake --build . --target iso
add_custom_target(iso
    DEPENDS ${CMAKE_BINARY_DIR}/stealthos.iso
)


#   run bochs -f bochsrc.txt -q
