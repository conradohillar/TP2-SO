// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <eliminator.h>
#include <libc.h>
#include <shell.h>
#include <syscaller.h>
#include <song_book.h>


#define HEIGHT 768
#define WIDTH 1024
#define BLOCK_SIZE 12

// Have to add Sound feature and phrases when every player dies

void put_rectangle(uint64_t TopLeftX, uint64_t TopLeftY, uint64_t BottomRightX, uint64_t BottomRightY, uint32_t color)
{
    // extern function from an assembly file (syscaller.asm)
    sys_put_rectangle_asm(TopLeftX, TopLeftY, BottomRightX, BottomRightY, color);
}


// the game is being played in this matrix, you must know that the scale for the screen
// is 1 : BLOCK_SIZE, that means that one position here is equivalent to a BLOCK_SIZExBLOCK_SIZE pixel space in the screen matrix

typedef struct playerType
{
    uint64_t pos_x;
    uint64_t pos_y;
    int64_t velocity_x;
    int64_t velocity_y;
    uint64_t is_alive;
    uint64_t number;
} playerType;

uint64_t speed = 0;
uint64_t players = 0;
uint64_t score = 0;



uint8_t matrix[HEIGHT / BLOCK_SIZE][WIDTH / BLOCK_SIZE] = {{0}};

void clear_matrix(){
    for(int i = 0; i < (HEIGHT / BLOCK_SIZE); i++){
        for(int j = 0; j < (WIDTH / BLOCK_SIZE); j++){
            matrix[i][j]=0;
        }   
    }
}


// Return states: 0 player moved, 1 player dead, -1 internal error
int move_player(playerType *player)
{
    // Checking the new position for the player
    int64_t new_position_x = player->pos_x + player->velocity_x;
    int64_t new_position_y = player->pos_y + player->velocity_y;

    // Hitting the border case
    if (new_position_x >= (WIDTH / BLOCK_SIZE) - 1 || new_position_x < 1 || new_position_y >= (HEIGHT / BLOCK_SIZE) - 1 || new_position_y < 2 || matrix[new_position_y][new_position_x] != 0)
    {
        player->is_alive = 0;
        return 1;
    }
    // Moved into not available space

    // Setting the older position with an available number
    matrix[player->pos_y][player->pos_x] = player->number;

    // Update the player's position
    player->pos_x = new_position_x;
    player->pos_y = new_position_y;

    // Only in this turn the place is occupied
    matrix[new_position_y][new_position_x] = 3;

    return 0;
}

int change_velocity(playerType *player, int64_t new_vel_x, int64_t new_vel_y)
{
    if(new_vel_x != -player->velocity_x) {
        player->velocity_x = new_vel_x;
    }
    if(new_vel_y != -player->velocity_y) {
        player->velocity_y = new_vel_y;
    }
    return 0;
}

int is_dead(playerType *player)
{
    return (player->is_alive == 0);
}

void print_matrix(playerType *player1, playerType *player2, uint64_t number)
{
    uint8_t buffer[7] = {0};
    itoa(score, buffer);
    print_at( (uint8_t *) "Score: ", 7, BLACK, RED, 456, 5, 1);
    print_at(buffer, 7, BLACK, RED, 512, 5, 1);
    
    for (uint64_t i = 2; i < HEIGHT / BLOCK_SIZE - 1; i++)
    {
        for (uint64_t j = 1; j < WIDTH / BLOCK_SIZE - 1; j++)
        {
            if (i == player1->pos_y && j == player1->pos_x)
            {
                put_rectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, i * BLOCK_SIZE + BLOCK_SIZE, 0x0000FF);
            }
            else if (number == 2 && i == player2->pos_y && j == player2->pos_x)
            {
                put_rectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, i * BLOCK_SIZE + BLOCK_SIZE, 0x00FF00);
            }
            else if (matrix[i][j] == 1)
            {
                put_rectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, i * BLOCK_SIZE + BLOCK_SIZE, 0x000088);
            }
            else if (number == 2 && matrix[i][j] == 2)
            {
                put_rectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, i * BLOCK_SIZE + BLOCK_SIZE, 0x008800);
            }
            else
            {
                put_rectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, i * BLOCK_SIZE + BLOCK_SIZE, 0x000000);
            }
        }
    }
}

void eliminator_menu()
{
    sys_clear_screen_asm();
    while(1) {

        put_rectangle(0, 0, WIDTH, HEIGHT, BLACK);
        uint64_t x_write = 0;
        uint64_t y_write = 0;
        print_at( (uint8_t *) "Select number of players (1 or 2) or 'q' to quit: ", 50, RED, BLACK, x_write, y_write, 2);
        // play_eliminator is universal, you must pass it the amount of players (1 or 2)

        uint8_t c;
        while((c = getchar())){
            if(c == '1' || c == '2' || c == 'q'){
                break;
            }
        }
        switch (c)
        {
        case '1' :
        case '2' :
            players = c - '0';
            print_at((uint8_t *)&c, 1, WHITE, BLACK, x_write += 50 * 2 * 8, y_write, 2);
            x_write = 0;    
            y_write += 32;
            print_at( (uint8_t *) "Select speed (1 - 5): ", 22, RED, BLACK, x_write, y_write, 2);
            speed = '\0';
            while((speed = getchar())) {
                if(speed >= '1' && speed <= '5')
                    break;
            }
            print_at((uint8_t *)&speed, 1, WHITE, BLACK, x_write += 22 * 2 * 8, y_write, 2);
            speed -= '0';
            x_write = 0;
            y_write += 64;
            print_at( (uint8_t *) "Player 1 plays with: ", 21, BLUE, BLACK, x_write, y_write, 2);
            print_at( (uint8_t *) "W-A-S-D", 7, WHITE, BLACK, x_write += 21 * 2 * 8, y_write, 2);
            if(players == 2){
                x_write = 0;
                y_write += 64;
                print_at( (uint8_t *) "Player 2 plays with: ", 21, GREEN, BLACK, x_write, y_write, 2);
                print_at( (uint8_t *) "I-J-K-L", 7, WHITE, BLACK, x_write += 21 *2 * 8, y_write, 2);
            }
            x_write = 0;
            y_write += 64;
            print_at( (uint8_t *) "Press ENTER to start", 20, RED, BLACK, x_write, y_write, 2);
            while(getchar() != '\n');

            score = 0;

            play_eliminator(players);
            break;

        case 'q':
            put_rectangle(0, 0, WIDTH, HEIGHT, BLACK);
            return;

        default:
            eliminator_menu();
            break;
        }
    }
    return;
}   

void refresh_directions(uint8_t keyboard_buffer[3], playerType* player1, playerType* player2, uint64_t number){
        int64_t new_vel_y = 0;
        int64_t new_vel_x = 0;
    switch (keyboard_buffer[0])
        {

        case '\0':
            break;

        case 'w':
            if (keyboard_buffer[0] != keyboard_buffer[1])
            {
                new_vel_x = 0;
                new_vel_y = -1;
                change_velocity(player1, new_vel_x, new_vel_y);
                keyboard_buffer[0] = keyboard_buffer[1];
            }
            break;
        case 'a':
            if (keyboard_buffer[0] != keyboard_buffer[1])
            {
                new_vel_x = -1;
                new_vel_y = 0;
                change_velocity(player1, new_vel_x, new_vel_y);
                keyboard_buffer[0] = keyboard_buffer[1];
            }
            break;

        case 's':
            if (keyboard_buffer[0] != keyboard_buffer[1])
            {
                new_vel_x = 0;
                new_vel_y = 1;
                change_velocity(player1, new_vel_x, new_vel_y);
                keyboard_buffer[0] = keyboard_buffer[1];
            }
            break;

        case 'd':
            if (keyboard_buffer[0] != keyboard_buffer[1])
            {
                new_vel_x = 1;
                new_vel_y = 0;
                change_velocity(player1, new_vel_x, new_vel_y);
                keyboard_buffer[0] = keyboard_buffer[1];
            }
            break;

        case 'i':
            if (number == 2)
            {
                if (keyboard_buffer[0] != keyboard_buffer[2])
                {
                    new_vel_x = 0;
                    new_vel_y = -1;
                    change_velocity(player2, new_vel_x, new_vel_y);
                    keyboard_buffer[0] = keyboard_buffer[2];
                }
            }
            break;
        case 'j':
            if (number == 2)
            {
                if (keyboard_buffer[0] != keyboard_buffer[2])
                {
                    new_vel_x = -1;
                    new_vel_y = 0;
                    change_velocity(player2, new_vel_x, new_vel_y);
                    keyboard_buffer[0] = keyboard_buffer[2];
                }
            }
            break;

        case 'k':
            if (number == 2)
            {
                if (keyboard_buffer[0] != keyboard_buffer[2])
                {
                    new_vel_x = 0;
                    new_vel_y = 1;
                    change_velocity(player2, new_vel_x, new_vel_y);
                    keyboard_buffer[0] = keyboard_buffer[2];
                }
            }
            break;

        case 'l':
            if (number == 2)
            {
                if (keyboard_buffer[0] != keyboard_buffer[2])
                {
                    new_vel_x = 1;
                    new_vel_y = 0;
                    change_velocity(player2, new_vel_x, new_vel_y);
                    keyboard_buffer[0] = keyboard_buffer[2];
                }
            }
            break;

        default:
            break;
        }

}

uint64_t play_eliminator(uint64_t number)
{
    
    clear_matrix();
    // Initialize the player structs with values
    playerType player1 = {(WIDTH / BLOCK_SIZE) / 2, (HEIGHT / BLOCK_SIZE) / 4, 0, 1, 1, 1};
    playerType player2 = {(WIDTH / BLOCK_SIZE) / 2, (3 * HEIGHT / BLOCK_SIZE) / 4, 0, -1, 1, 2};

    // pos 1 is for vel_player_1, pos 2 for player 2 and pos 0 is an aux
    uint8_t keyboard_buffer[3] = {'s', 'i', '\0'};
    score = 0;

    //prints the border around the screen
    put_rectangle(0, 0, WIDTH, BLOCK_SIZE * 2, RED);
    put_rectangle(0, HEIGHT - BLOCK_SIZE - 1, WIDTH, HEIGHT, RED);
    put_rectangle(0, 0, BLOCK_SIZE, HEIGHT, RED);
    put_rectangle(WIDTH - BLOCK_SIZE - 1, 0, WIDTH, HEIGHT, RED);

    uint64_t actual_time = get_ticks();
    uint64_t last_time = actual_time;

    // this module processes the movement only when delta time
    while (1)
    {
        if ((actual_time = get_ticks()) > last_time + (5 - speed))
        {
            last_time = actual_time;
            // both players are always initialized
            int state = move_player(&player1);
            if (state != 0)
            {
                if (is_dead(&player1))
                {
                    state = 1;
                    // rutina de perdedor jugador
                    break;
                }
            }
            // player 2 only moves if the amount of players is 2
            if (number == 2)
            {
                state = move_player(&player2);
                if (state != 0)
                {
                    if (is_dead(&player2))
                    {
                        state = 1;
                        // rutina de ganador jugador
                        break;
                    }
                }
            }
            score += speed;
            print_matrix(&player1, &player2, number);
        }

        // when thats not the case
        check_keyboard(keyboard_buffer);
        refresh_directions(keyboard_buffer, &player1, &player2, number);
    }

    print_matrix(&player1, &player2, number);

    uint64_t x_write = 224;
    uint64_t y_write = 300;
    print_at( (uint8_t *) " Game Over! ", 12, BLACK, RED, x_write, y_write, 6);
    x_write = 384;
    y_write = 430;
    if(players == 2) {
        if(player1.is_alive){ 
            print_at( (uint8_t *) " Player 1 wins! ", 16, BLACK, BLUE, x_write, y_write, 2);
        } else
            print_at( (uint8_t *) " Player 2 wins! ", 16, BLACK, GREEN, x_write, y_write, 2);
    }
    else {
        print_at( (uint8_t *) " Score:         ", 16, BLACK, RED, x_write, y_write, 2);
        uint8_t buff[7] = {'\0'};
        itoa(score, buff);
        x_write += 128;
        print_at(buff, 7, BLACK, RED, x_write, y_write, 2);
    }
    play_gameover();
    x_write = 232;
    y_write = 500;
    print_at( (uint8_t *) " Press ENTER to return to the menu ", 35, BLACK, RED, x_write, y_write, 2);
    while(getchar() != '\n');
    put_rectangle(0, 0, WIDTH, HEIGHT, BLACK);

    return 0;
}