#define VIDEO_MEMORY 0xB8000
#define COLUMNS 80
#define ROWS 25
#include "screen.h"

void main() {


    clear_screen();
    char string[] = "a\nb\nc\n";
    print_string(string);
    print_string(string);
    while(1)
    {
        __asm__("hlt");
    // Halt the CPU (infinite loop)
    }
}

