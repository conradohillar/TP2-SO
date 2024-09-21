#include <shell.h>


#define MAX_TEXT_SIZE 2
#define MIN_TEXT_SIZE 1
#define COMM_BUFF_SIZE 128

static uint64_t scale = 1;

void help() {
    uint8_t * supertab = (uint8_t *)"\t\t\t\t";
    printcolor( (uint8_t *) "help      ", ORANGE, BLACK); printcolor( (uint8_t *) " - prints a list of all possible commands\n", GRAY, BLACK);
    printcolor( (uint8_t *) "divzero   ", ORANGE, BLACK); printcolor( (uint8_t *) " - tests the divide by zero exception\n", GRAY, BLACK);
    printcolor( (uint8_t *) "inopcode  ", ORANGE, BLACK); printcolor( (uint8_t *) " - tests the invalid opcode exception\n", GRAY, BLACK);
    printcolor( (uint8_t *) "time      ", ORANGE, BLACK); printcolor( (uint8_t *) " - returns the current system time in HH:MM:SS format\n", GRAY, BLACK);
    printcolor( (uint8_t *) "regstatus ", ORANGE, BLACK); printcolor( (uint8_t *) " - prints the values of the CPU registers\n", GRAY, BLACK);
    print( (uint8_t *) "\t\t\t "); printcolor( (uint8_t *) "press Ctrl + r to save the register states\n",GRAY,BLACK);
    printcolor( (uint8_t *) "eliminator", ORANGE, BLACK); printcolor( (uint8_t *) " - play the \"Eliminator\" game\n", GRAY, BLACK);
    printcolor( (uint8_t *) "inctext   ", ORANGE, BLACK); printcolor( (uint8_t *) " - increases the size of the text, (max: 2)\n", GRAY, BLACK);
    printcolor( (uint8_t *) "dectext   ", ORANGE, BLACK); printcolor( (uint8_t *) " - decreases the size of the text, (min: 1)\n", GRAY, BLACK);
    printcolor( (uint8_t *) "clear     ", ORANGE, BLACK); printcolor( (uint8_t *) " - clears the screen\n", GRAY, BLACK);
    printcolor( (uint8_t *) "playsong N", ORANGE, BLACK); printcolor( (uint8_t *) " - plays the N-th tune, being N:\n",GRAY,BLACK);
    print(supertab); printcolor( (uint8_t *) "1: Starting beep sound\n", GRAY, BLACK);
    print(supertab); printcolor( (uint8_t *) "2: Happy birthday\n", GRAY, BLACK);
    print(supertab); printcolor( (uint8_t *) "3: La Cucaracha\n", GRAY, BLACK);
    print(supertab); printcolor( (uint8_t *) "4: The Imperial March\n", GRAY, BLACK);
    print(supertab); printcolor( (uint8_t *) "5: Dale Dale Bo'\n", GRAY, BLACK);
    print(supertab); printcolor( (uint8_t *) "6: Game Over! sound\n", GRAY, BLACK);
    print(supertab); printcolor( (uint8_t *) "7: Terraria - Day\n", GRAY, BLACK);
    print(supertab); printcolor( (uint8_t *) "8: Himno Nacional Argentino\n", GRAY, BLACK);
}


void check_div_by_zero() {
    divzero_excep_asm();
}

void check_invalid_opcode() {
    inopcode_excep_asm();
}

void get_registers() {
    sys_get_registers_asm();
}

void run_eliminator() {
    eliminator_menu();
    sys_clear_screen_asm();
}

void increase_text_size() {
    if(scale < MAX_TEXT_SIZE) {
        sys_set_text_size_asm(++scale);
    }
    else { 
        print( (uint8_t *) "Text at maximum size (2)\n");
    }
}

void decrease_text_size() {
    if(scale > MIN_TEXT_SIZE) {
        sys_set_text_size_asm(--scale);
    }
    else {
        print( (uint8_t *) "Text at minimum size (1)\n");
    }
}

void get_time() {
    uint8_t buffer[9];
    sys_get_time_asm(buffer);
    printcolor(buffer, ORANGE, BLACK);
    putchar('\n');
}

void clear() {
    sys_clear_screen_asm();
} 

void play_song(uint8_t idx){
    song_dispatcher(idx);
}

static uint8_t * commands[] = {(uint8_t *) "help", (uint8_t *) "divzero", (uint8_t *) "inopcode", (uint8_t *) "time", (uint8_t *) "regstatus", (uint8_t *) "eliminator", (uint8_t *) "inctext", (uint8_t *) "dectext", (uint8_t *) "clear"};

static void (* functions[])(void) = {help, check_div_by_zero, check_invalid_opcode, get_time, get_registers, run_eliminator, increase_text_size, decrease_text_size, clear};

uint64_t get_command(uint8_t * str) {
    for(int i = 0; i < 9; i++) {
        if(strcmp(commands[i], str) == 0) {
            functions[i]();
            return 1;
        }
    }
    uint8_t * command10 = (uint8_t *) "playsong";
    for(int j = 0; j < strlen(command10); j++)
        if(str[j] != command10[j])
            return 0;
    uint8_t idx = str[9] - '0';
    if(idx >= 1 && idx <= 8){
        play_song(idx);
        return 1;
    }
    return 0;
}

void run_shell() {
    
    uint8_t command_buffer[COMM_BUFF_SIZE];

    play_startsound();
    
    sys_set_text_size_asm(1);
    scale = 1;
    printcolor( (uint8_t *) "Type \"help\" to see a list of commands\n", WHITE, BLUE);

    while(1) {
        uint64_t buff_pos = 0;
        printcolor( (uint8_t *) ">: ", GREEN, BLACK);
        uint8_t c;

        while(((c = getchar()) != '\n' || buff_pos == 0) && buff_pos < COMM_BUFF_SIZE) {

            if(c == '\b') {
                if(buff_pos > 0) {
                    putchar('\b');
                    buff_pos--;
                } else 
                    make_sound(120, 70);
            }   
            else if(c != '\n') {
                command_buffer[buff_pos++] = c;
                putchar(c);
            }
        }

        putchar('\n');
        command_buffer[buff_pos] = '\0';

        if(get_command(command_buffer) == 0) {
            putcharcolor('\"', RED, BLACK);
            printcolor(command_buffer, RED, BLACK);
            putcharcolor('\"', RED, BLACK);
            printcolor( (uint8_t *) " is not a command\n", RED, BLACK);
        }
    }
}