#include "tty.h"
#include "vga.h"
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <psf.h>
#include <string.h>


__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// ask for the modules
__attribute__((used, section(".limine_requests")))
static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0,
    
};


__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;
// requests are placed here
__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;


// Halt and catch fire function.
static void hcf(void) {
    asm("cli");
    for (;;) {
        asm ("hlt");
    }
}



struct limine_file* get_file(const char* path) {
    struct limine_module_response *module_response = module_request.response;


    for(size_t i = 0; i < module_response->module_count; i++) {
        if(strcmp(path, module_response->modules[i]->path) == 0) {
            return module_response->modules[i];
        }
    }

    return NULL;
}


/**
 * @brief Entry point of the os.
 * 
 * @return int 
 */
int kmain() {


    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }


    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Ensure we have modules
    if (module_request.response == NULL
        || module_request.response->module_count < 1) {
        hcf();
    }
    


    struct limine_file *font_file = get_file("boot/assets/fonts/zap-vga16.psf");

    // check if we got the font
    if (font_file == NULL) {
        font_file = module_request.response->modules[0];
    }

    // get the psf header
    struct psf1_header *font_header = (struct psf1_header*) font_file->address;
    
    
    // the magic for all psf1 files is 0x3604
    if(font_header->magic[0] != 0x36 || font_header->magic[1] != 0x04 ) {
        // font isnt psf1
        // halt for now in the future check if its psf2
        hcf();
    }

    struct psf1_font font = {
        font_header,
        font_file->address + sizeof(struct psf1_header),
        (font_file->address + sizeof(struct psf1_header)) + font_header->glyph_size * 256
    };




    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    
    
    char* helloworld = "Hello, World!\nWelcome to StealthyOS\0";

    terminal_init(&font, framebuffer, 100, 80);

    terminal_writestring(helloworld);

    hcf();
    return 0;
}
