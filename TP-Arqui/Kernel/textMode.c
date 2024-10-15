// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <textMode.h>


static data_char screen_state[STD_CHAR_AMOUNT_X * STD_CHAR_AMOUNT_Y];

static uint8_t scale = 1;
static uint64_t last_pos = 0;
static uint64_t current_pos = 0;

static uint64_t x_pos = 0;
static uint64_t y_pos = 0;

void put_char(uint8_t c, uint32_t fore_color, uint32_t back_color) {
    screen_state[current_pos].c = c;
    screen_state[current_pos].fore_color = fore_color;
    screen_state[current_pos].back_color = back_color;
    current_pos++;
}

uint64_t put_string(uint8_t * string, uint64_t size, uint32_t fore_color, uint32_t back_color) {
    int i;
    
    for(i = 0; i < size; i++) {
        
        if( y_pos + ((x_pos + (size * get_w_char() * scale)) / get_width()) >= get_height()){
            clear_screen_and_buffer();
            current_pos=last_pos=0;
        }
        put_rectangle(x_pos, y_pos, x_pos + get_w_char() * scale / 3, y_pos + get_y_char() * scale - 1, BLACK);
        
        if(string[i] == '\b') {
            if(current_pos > 0){
            
                if(x_pos >= get_w_char() * scale){
                    x_pos -= (get_w_char() * scale);
                }else{
                    y_pos -= (get_y_char() * scale);
                    x_pos = (get_width() - get_w_char() * scale);
                }
                 
                put_char_scale(' ', BLACK, BLACK, &x_pos, &y_pos, scale);
                current_pos--;
                last_pos--;
                screen_state[current_pos].c = '\0';
                if(x_pos >= get_w_char() * scale){
                    x_pos -= (get_w_char() * scale);
                }else{
                    y_pos -= (get_y_char() * scale);
                    x_pos = (get_width() - get_w_char() * scale);
                }
            } 
        }  
        else {
            put_char(string[i], fore_color, back_color);
        }  
    }
    print_state();
    
    return i;
}

void put_string_nt(uint8_t * string, uint32_t fore_color, uint32_t back_color) {
    put_string(string, strlen(string), fore_color, back_color);
}

void set_scale_text_mode(uint64_t new_scale) {
    clear_screen();
    scale = new_scale;
    last_pos = 0;
    x_pos = 0;
    y_pos = 0;
    print_state();
}

void print_state() {    

    for(int i = last_pos; i < current_pos; i++) {
        if(x_pos >= get_width()) {
            x_pos = 0;
            y_pos +=  get_y_char() * scale;
        }
        if(y_pos >= get_height()) { 
            clear_screen_and_buffer();
            x_pos = 0;
            y_pos = 0;
        }
        if(screen_state[i].c == '\n') {
            x_pos = 0;
            y_pos += get_y_char() * scale;
        }
        else if(screen_state[i].c == '\t') {
            x_pos += 4 * get_w_char() * scale;
        }
        else if(screen_state[i].c != '\0') {
            put_char_scale(screen_state[i].c, screen_state[i].fore_color, screen_state[i].back_color, &x_pos, &y_pos, scale);
        }
    }
    
    put_rectangle(x_pos, y_pos, x_pos + get_w_char() * scale / 3, y_pos + get_y_char() * scale - 1, WHITE);

    last_pos = current_pos;
}

void clear_screen_and_buffer() {
    clear_screen();
    for(int i = 0; i <= current_pos; i++) {
        screen_state[i].c = '\0';
    }
    current_pos = last_pos = 0;
    x_pos = y_pos = 0;
}

void clear_screen() {
    put_rectangle(0, 0, get_width(), get_height(), 0x000000);
}