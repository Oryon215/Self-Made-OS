#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

void clear_screen ();
void print_string ( char * message );
void set_char_at_video_memory(char character, int offset);
#endif // SCREEN_H_INCLUDED
