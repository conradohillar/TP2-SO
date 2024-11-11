#include <colors.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STD_FORE_COLOR WHITE
#define STD_BACK_COLOR BLACK
#define MAX_PARAMS 5
#define MAX_PARAM_LENGTH 50

typedef void (*one_param_fn)(uint8_t param);

uint64_t printcolor(uint8_t *str, uint64_t fore_color, uint64_t back_color);

uint64_t print(uint8_t *str);

uint64_t printerr(uint8_t *str);

void print_at(uint8_t *buffer, uint64_t size, uint32_t fore_color,
              uint32_t back_color, uint32_t x, uint32_t y, uint64_t scale);

uint64_t scan(uint8_t *buffer, uint64_t max_length);

uint8_t getchar(void);

uint64_t putcharcolor(uint8_t c, uint32_t fore_color, uint32_t back_color);

uint64_t putcharerr(uint8_t c);

uint64_t putchar(uint8_t c);

void print_spaces(uint64_t spaces);

uint64_t strlen(uint8_t *string);

int64_t strcmp(uint8_t *str1, uint8_t *str2);

int8_t split_string(const uint8_t *str,
                    uint8_t words[MAX_PARAMS][MAX_PARAM_LENGTH]);

uint32_t itoa(uint32_t num, uint8_t *str);

void itoh(uint64_t num, uint8_t *str);

uint64_t get_ticks();

void check_keyboard(uint8_t *buffer);

void make_sound(uint64_t millis, uint32_t freq);

void sleep(uint64_t secs, uint64_t millis);

int64_t loop_fn(uint64_t argc, uint8_t *argv[]);

int64_t ps_fn(uint64_t argc, uint8_t *argv[]);

int64_t mem_fn(uint64_t argc, uint8_t *argv[]);

int64_t help_fn(uint64_t argc, uint8_t *argv[]);

int64_t test_semaphores_fn(uint64_t argc, uint8_t *argv[]);

int64_t cat_fn(uint64_t argc, uint8_t *argv[]);

int64_t wc_fn(uint64_t argc, uint8_t *argv[]);

int64_t filter_fn(uint64_t argc, uint8_t *argv[]);

uint8_t is_number(uint8_t *str);

int64_t philos_fn(uint64_t argc, uint8_t *argv[]);