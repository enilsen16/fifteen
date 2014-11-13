

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
        // greet player
        greet();

        // ensure proper usage
        if (argc != 2)
        {
                printf("Usage: ./fifteen d\n");
                return 1;
        }

        // ensure valid dimensions
        d = atoi(argv[1]);
        if (d < MIN || d > MAX)
        {
                printf("Board must be between %i x %i and %i x %i, inclusive.\n",
                       MIN, MIN, MAX, MAX);
                return 2;
        }

        // initialize the board
        init();

        // accept moves until game is won
        while (true)
        {
                // clear the screen
                clear();

                // draw the current state of the board
                draw();

                // saves the current state of the board (for testing)
                save();

                // check for win
                if (won())
                {
                        printf("ftw!\n");
                        break;
                }

                // prompt for move
                printf("Tile to move: ");
                int tile = GetInt();

                // move if possible, else report illegality
                if (!move(tile))
                {
                        printf("\nIllegal move.\n");
                        usleep(500000);
                }

                // sleep for animation's sake
                usleep(500000);
        }

        // that's all folks
        return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
        printf("\033[2J");
        printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
        clear();
        printf("GAME OF FIFTEEN\n");
        usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
        int num = (d * d) - 1;
        for (int i = 0; i < d; i++) {
                for (int j = 0; j < d; j++) {
                        board[i][j] = num;
                        num--;
                }
        }
        if ((((d * d) - 1) % 2) != 0) {
                board[d-1][d-2] = board[d-1][d-2] + board[d-1][d-3];
                board[d-1][d-3] = board[d-1][d-2] - board[d-1][d-3];
                board[d-1][d-2] = board[d-1][d-2] - board[d-1][d-3];
        }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
        //For each row
        for (int i = 0; i < d; i++) {
                //For each value in row
                for (int j = 0; j < d; j++ ) {
                        //Print value and space
                        if (board[i][j] == 0) {
                                printf("__");
                        } else {
                                printf("%2d ", board[i][j]);
                        }

                }
                //Print newline
                printf("\n");
        }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
        //User inputs tile they'd like to move
        //Checks to see if that tile exists and stores its postion
        int tile_i;
        int tile_j;
        if (tile > ((d * d) - 1)) {
                return false;
        }
        for (int i = 0; i < d; i++) {
                for (int j = 0; j < d; j++) {
                        if (board[i][j] == tile) {
                                tile_i = i;
                                tile_j = j;
                        }
                }
        }
        //Checks to see if blank tile is above, below, left or right of input tile
        //If not return false
        int tmp = board[tile_i][tile_j];
        if (tile_i > 0 && board[tile_i-1][tile_j] == 0) { //up
                //Swap blank tile and input tile
                board[tile_i][tile_j] = board[tile_i-1][tile_j];
                board[tile_i-1][tile_j] = tmp;
                return true;
        } else if (tile_i <= d-2 && board[tile_i+1][tile_j] == 0) { //down
                board[tile_i][tile_j] = board[tile_i+1][tile_j];
                board[tile_i+1][tile_j] = tmp;
                return true;
        } else if (tile_j > 0 && board[tile_i][tile_j-1] == 0) { //left
                board[tile_i][tile_j] = board[tile_i][tile_j-1];
                board[tile_i][tile_j-1] = tmp;
                return true;
        } else if (tile_j <= d-2 && board[tile_i][tile_j+1] == 0) { //right
                board[tile_i][tile_j] = board[tile_i][tile_j+1];
                board[tile_i][tile_j + 1] = tmp;
                return true;
        } else {
                return false;
        }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
        for (int i = 0; i < d; i++) {
                for (int j = 0; j < d-1; j++) {
                        if ( board[d-1][d-2] == ((d * d) - 1) && board[d-1][d-1] == 0) {
                                return true;
                        }
                        else if (board[i][j+1] - board[i][j] == 1) {
                                continue;
                        } else {
                                return false;
                        }
                }
        }
        return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
        // log
        const string log = "log.txt";

        // delete existing log, if any, before first save
        static bool saved = false;
        if (!saved)
        {
                unlink(log);
                saved = true;
        }

        // open log
        FILE* p = fopen(log, "a");
        if (p == NULL)
        {
                return;
        }

        // log board
        fprintf(p, "{");
        for (int i = 0; i < d; i++)
        {
                fprintf(p, "{");
                for (int j = 0; j < d; j++)
                {
                        fprintf(p, "%i", board[i][j]);
                        if (j < d - 1)
                        {
                                fprintf(p, ",");
                        }
                }
                fprintf(p, "}");
                if (i < d - 1)
                {
                        fprintf(p, ",");
                }
        }
        fprintf(p, "}\n");

        // close log
        fclose(p);
}
