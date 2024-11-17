cmake_minimum_required(VERSION 3.30)

project(StealthOS VERSION 0.1.0 LANGUAGES C ASM)


add_subdirectory(kernel/)

# command to generate stealthos.iso
add_custom_command(
    OUTPUT stealthos.iso
    COMMAND genisoimage
    ARGS -R
    -b boot/grub/stage2_eltorito    
    -no-emul-boot                   
    -boot-load-size 4               
    -A stealthos                    
    -input-charset utf8             
    -quiet                          
    -boot-info-table                
    -o stealthos.iso                
    iso
    DEPENDS kernel.elf
)
