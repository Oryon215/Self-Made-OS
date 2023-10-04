#include "lowlevel.h"
#include "screen.h"

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

void scroll();

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

int get_cursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}


void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

void set_char_at_video_memory(char character, int offset) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

void print_string(char string[]) {
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0)
    {
        set_cursor(offset);
        if (offset > get_offset(79, 24))
        {
            scroll();
            offset = get_cursor();
        }
        if (string[i] == '\n')
        {
            int rows = offset / (MAX_COLS*2);
            offset = get_offset(0, rows + 1);
            set_cursor(offset);
        }
        else
        {
            set_char_at_video_memory(string[i], offset);
            offset+=2;

        }
        i+=1;
    }
    set_cursor(offset);
}

void clear_screen()
{
    int mul = 2;
    int len = MAX_COLS*MAX_COLS;
    for (int i = 0; i<len; i++)
    {
        set_char_at_video_memory(' ',i*mul);
    }
    set_cursor(0);
}

void scroll()
{
    for (int i = 1; i<MAX_ROWS; i++)
    {
        for (int j = 0; j<MAX_COLS; j++)
        {
            int offset1 = get_offset(i - 1, j);
            int offset2 = get_offset(i, j);
            *(char *)(VIDEO_ADDRESS + offset1) = *(char *)(VIDEO_ADDRESS + offset2);
        }
    }
    for( int i = 0; i<MAX_COLS; i++)
    {
        int offset = get_offset(i, 24);
        *(char *)(VIDEO_ADDRESS + offset) = *(char *)" ";
    }
    set_cursor(get_offset(0, 24));
}

