#include "graphics/graphics.h"
#include "keyboard/keyboard.h"
#include "builtins/builtins.h"
#include <stdbool.h>

#define VGA_ADDRESS 0xB8000   /* video memory begins here. */

/* VGA provides support for 16 colors */
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE_COLOR 15
#define GRAPHICAL_CAPABILITY 1

bool GRAPHICS_MODE = true;

unsigned short *terminal_buffer;
unsigned int overall_vga_index;

void clear_screen(void)
{
    int index = 0;
    /* there are 25 lines each of 80 columns;
       each element takes 2 bytes */
    while (index < 80 * 25 * 2) {
            terminal_buffer[index] = ' ';
            index += 1;
    }
}

void set_index_to_next_line() {
    float mid = overall_vga_index / 80;

    if (mid == 0) {
        return;
    } else {
        overall_vga_index += mid;
    }
}

void print_string(char *str, unsigned char color, unsigned int vga_index)
{
    if (vga_index == -1) {
        vga_index = overall_vga_index;
        overall_vga_index += 80;
    }
    int index = 0;
    while (str[index]) {
            terminal_buffer[vga_index] = (unsigned short)str[index]|(unsigned short)color << 8;
            index++;
            vga_index++;
    }
}

void LoadSystem() {
    if (GRAPHICS_MODE) {
        #if GRAPHICAL_CAPABILITY == 1
            
        #endif
    } else {

    }
}

void CleanupSystem() {

}

char *get_input() {
    print_string(get_keyboard_input(), WHITE_COLOR, -1);
}

void main(void)
{
    terminal_buffer = (unsigned short *)VGA_ADDRESS;
    overall_vga_index = 0;

    clear_screen();
    print_string("Loading...", WHITE_COLOR, -1);

    LoadSystem();
    
    if (!GRAPHICS_MODE) {
        print_string("Loaded", WHITE_COLOR, -1);

        print_string("Exiting...", WHITE_COLOR, -1);

        print_string(get_input(), WHITE_COLOR, -1);
    } else {
        #if GRAPHICAL_CAPABILITY == 1
            clear_screen();

        #endif
    }
    CleanupSystem();

    return;
}
