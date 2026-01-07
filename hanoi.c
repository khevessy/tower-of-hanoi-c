/*******************************************************************************
 *
 * Hanoi puzzle - recursive solution.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>     /* atoi() */
#include <string.h>


#define LEVELS_MAX      100U    /* Maximum number of levels */
#define ROWS            3U      /* Number of horizontal rows */

#define MIN_HEIGHT      2U      /* Minimum height */
#define MAX_HEIGHT      LEVELS_MAX  /* Maximum height */

//#define SUPPRESS_OUTPUT 1

/**
 * Current state
 */
typedef struct
{
    uint8_t Array[LEVELS_MAX];  /* The discs (from bottom to top) */
    uint8_t Top;                /* Where we would be saving the next level */

} LIFO_t;

/**
 * State variable - per row (vertical)
 * Columns[0].Array[0] -> Left bottom disc
 */
LIFO_t Columns[ROWS];

/**
 * How many times was function moveDisc() called
 */
static int g_MoveDiscCount = 1;


/**
 * Main solution function to be called. Solves the puzzle.
 * @param  pState State of the puzzle
 * @param  height Height of the first row (initial state)
 * @retval None
 */
void solve(LIFO_t* pState, int height);

/**
 * Find an appropriate row index that is not equal to neither of the parameters.
 * @param  deny1 First forbidden value
 * @param  deny2 Second forbidden value
 * @retval Valid index that is not equal to the forbidden values
 */
uint8_t findRow(uint8_t deny1, uint8_t deny2);

/**
 * Recursive worker function.
 * @param  pState Current state of the puzzle
 * @param  src Source index
 * @param  dst Destination index
 * @param  elements How many elements to move
 * @retval None
 */
static void move(LIFO_t* pState, uint8_t src, uint8_t dst, uint8_t elements);

/**
 * Move a disc to the specified position.
 * @param  pState State variable
 * @param  src Source index
 * @retval dst Destination index
 */
static void moveDisc(LIFO_t *pState, uint8_t src, uint8_t dst);

/**
 * Find height of the puzzle.
 * @param  pState State variable
 * @retval Height of the highest row
 */
static uint8_t findHeight(const LIFO_t *pState);

/**
 * Print the state of the puzzle.
 * @param  pState State variable
 * @retval None
 */
static void printState(const LIFO_t *pState);


/**
 * main()
 */
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        (void) fprintf(stderr, "Usage: ./hanoi ROWS\n");
    }
    else
    {
        int height = atoi(argv[1]);
        if (height < MIN_HEIGHT)
        {
            (void) fprintf(stderr, "Error: too small height. Must be <%d; %d>.\n",
                           MIN_HEIGHT, MAX_HEIGHT);
        }
        else if (height > MAX_HEIGHT)
        {
            (void) fprintf(stderr, "Error: too small height. Must be <%d; %d>.\n",
                           MIN_HEIGHT, MAX_HEIGHT);
        }
        else
        {
            (void) memset((void*) Columns, 0, sizeof(Columns));
            for (int i = 0; i < height; i++)
            {
                Columns[0].Array[i] = (height - i);
            }
            Columns[0].Top = height;
            printf("Initial state:");
            printState(Columns);
            g_MoveDiscCount = 0;
            solve(Columns, height);
            printf("Final state in %d moves:", g_MoveDiscCount);
            printState(Columns);
        }
    }
    return 0;
}

void solve(LIFO_t* pState, int height)
{
    move(pState, 0, ROWS - 1U, height);
}

uint8_t findRow(uint8_t deny1, uint8_t deny2)
{
    uint8_t ret = deny1;
    do
    {
        ret = (ret + 1U) % ROWS;
    } while ((ret == deny1) || (ret == deny2));
    return ret;
}

void move(LIFO_t* pState, uint8_t src, uint8_t dst, uint8_t elements)
{
#ifndef SUPPRESS_OUTPUT
    static int call = 0;
    printf("move(%d, %d, %d) call #%d\n", src, dst, elements, call + 1);
    call++;
#endif /* SUPPRESS_OUTPUT */

    if (1U == elements)
    {
        moveDisc(pState, src, dst);
#ifndef SUPPRESS_OUTPUT
        printf("Trivial case");
        printState(pState);
#endif /* SUPPRESS_OUTPUT */
    }
    else
    {
        uint8_t dstNew = findRow(src, dst);
        // Move ones over me not to dest
        move(pState, src, dstNew, elements - 1U);

        // Move me to dest
        moveDisc(pState, src, dst);

#ifndef SUPPRESS_OUTPUT
        printf("Bottom disc moved (trivial case)");
        printState(pState);
#endif /* SUPPRESS_OUTPUT */

        // Move others to me
        move(pState, dstNew, dst, elements - 1U);
    }
}

void moveDisc(LIFO_t *pState, uint8_t src, uint8_t dst)
{
    uint8_t srcTop = pState[src].Top;
    uint8_t dstTop = pState[dst].Top;

    if (srcTop > 0)
    {
        uint8_t disc = pState[src].Array[srcTop - 1U];
        pState[src].Array[srcTop - 1U] = 0;
        pState[src].Top--;

        pState[dst].Array[dstTop] = disc;
        pState[dst].Top++;
    }
    g_MoveDiscCount++;
#ifndef SUPPRESS_OUTPUT
    (void) printf("moveDisc() #%d\n", g_MoveDiscCount);
#endif /* SUPPRESS_OUTPUT */
}

uint8_t findHeight(const LIFO_t *pState)
{
    uint8_t ret = 0;
    for (uint8_t i = 0; i < ROWS; i++)
    {
        for (uint8_t j = 0; j < LEVELS_MAX; j++)
        {
            if (pState[i].Array[j] > ret)
            {
                ret = pState[i].Array[j];
            }
        }
    }
    return ret;
}

void printState(const LIFO_t *pState)
{
    uint8_t height = findHeight(pState);
    printf("\n");
    for (uint8_t i = 0; i < height; i++)
    {
        for (uint8_t j = 0; j < ROWS; j++)
        {
            // Print from the top
            printf("    %3d", pState[j].Array[height - 1 - i]);
        }
        printf("\n");
    }
    for (uint8_t j = 0; j < ROWS; j++)
    {
        printf("    ===");
    }
    printf("\n\n");
}
