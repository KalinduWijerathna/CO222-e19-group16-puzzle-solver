
/* ~######## ###    ##~    ~#######~    ~########   ~~#########        ~########  .######.   #*        #*     #*     ~#######    ~########
  ###    ### ###    ###   ###    ###   ###    ###   ###    ###        ###    ### ###    ### ###       ###    ###   ###    ###   ###    ###
  ###    #*  ###    ###   ###    ###   ###    #*    ###    ###        ###    #*  ###    ### ###       ###    ###   ###    #*    ###    ###
  ###        ###    ###   ###    ###  ~###___      ~###____##*        ###        ###    ### ###       ###    ###  ~###___      ~###____##*
  *########* ###    ### *#########*  **###***     **###*****         *########## ###    ### ###       ###    ### **###***     **#####*****
         ### ###    ###   ###          ###    #~  *###########               ### ###    ### ###   ### ###    ###    #~  *###########
  #*     ### ###    ###   ###          ###    ###   ###    ###        ~#*    ### ###    ### ####    # ###    ###   ###    ###   ###    ###
 ~#########* *########*  ~####*        ##########   ###    ###       ~#########*  *######*  #####~~##  *######*    ##########   ###    ###
                                                    ###    ###                              *                                   ###    ###   V 2.0 */
/* Authers      : E/19/309 RAMBUKWELLA H.M.W.K.G. MR  e19309@eng.pdn.ac.lk
                  E/19/446 WIJERATHNA I.M.K.D.I. MR   e19446@eng.pdn.ac.lk
   Group No.    : 16
   Project      : CO222-2022 Crossword Puzzle Solver
   File Name    : puzzle-dynamic.c     // SUPER_SOLVER_Version_2.0
   Last Updated : Tue 17/01/08/2023 13:00:59
*/

// Including the standerd I/O and string libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining the Const. value N for static array sizes for each words in the word list and each spaces in the grid
#define N 50

// Declaring and initializing global variables
char **mtrxPuzzle;    // pointer to dynamic 2D array to store spaces of the grid
char **mtrxWords;     // pointer to dynamic 2D array to store words of the word list
int **wLenMtrx;       // pointer to dynamic 2D array to store word lengths of each word
int puzzleSize = 0;   // Number of lines of puzzle
int puzzleWidth = 0;  // Width of puzzle
int wordsGiven = 0;   // Number of given words
int maxWidthWord = 0; // Width of the Maximum width word out of the word list
int wordsCanFill = 0; // Number of spaces in the grid
//FILE *logFile;        // log file to check for errors

// Main function is declared and defined at the end of the program

// Function get the input of the grid into a dynamic 2D array
char **getMatrix(int *size, int *width)
{
    char **temp = (char **)calloc(1, sizeof(char *));

    int i = 0;
    int max_len = 0;

    while (1)
    {
        char text[1000];
        fgets(text, 1000, stdin);
        if (text[0] == '\n')
        {
            break;
        }

        if (max_len < strlen(text))
        {
            max_len = strlen(text);
        }

        temp = (char **)realloc(temp, sizeof(char *) * (i + 1));
        temp[i] = (char *)calloc(strlen(text) + 1, sizeof(char));
        strcpy(temp[i], text);
        i++;
    }

    char **mtrx = (char **)calloc(i, sizeof(char *));
    for (int x = 0; x < i; x++)
    {
        mtrx[x] = (char *)calloc(max_len, sizeof(char));
        strcpy(mtrx[x], temp[x]);
        free(temp[x]);
    }
    *size = i;
    *width = max_len;
    free(temp);
    return mtrx;
}

/*Function to check if the Grid input is invalid(return 1 if invalid)
  invalid conditions considerd=>
        if the grid consists chars other than '#','*','A'-'Z' > return 1
        if each line of the grid is not line separated        > return 1 */
int checkPuzzle(char **mtrx, int size, int width)
{
    if (size < 2 && width < 2)
    {
        return 1;
    }

    for (int i = 0; i < size; i++)
    {
        if (mtrx[i][0] == 0)
            break;

        for (int j = 0; j < width; j++)
        {
            if (mtrx[i][j] == '\n')
                break;

            if (mtrx[i][j] != '*' && mtrx[i][j] != '#' && !(mtrx[i][j] <= 'z' && mtrx[i][j] >= 'a') && !(mtrx[i][j] <= 'Z' && mtrx[i][j] >= 'A'))
            {
                return 1;
            }
        }

        if (i > 0 && strlen(mtrx[i]) != strlen(mtrx[0]))
        {
            return 1;
        }
    }

    return 0;
}

/*Function to check if the Word list input is invalid(return 1 if invalid)
  invalid conditions considerd=>
        if the word list consists chars other than 'A'-'Z','a'-'z' > return 1
        if each word of the word list is not line separated        > return 1 */
int checkWord(char **mtrx, int size, int width)
{
    if (size < 1)
    {
        return 1;
    }

    for (int x = 0; x < size; x++)
    {
        if (mtrx[x][0] == 0)
        {
            mtrx[x][0] = 0;
            break;
        }

        if (strlen(mtrx[x]) - 1 <= 1)
        {
            return 1;
        }

        for (int y = 0; y < width; y++)
        {
            if (mtrx[x][y] == '\n' || mtrx[x][y] == 0)
                break;
            if (mtrx[x][y] > 'z' || mtrx[x][y] < 'A')
            {
                return 1;
            }
            if (mtrx[x][y] > 'Z' && mtrx[x][y] < 'a')
            {
                return 1;
            }
        }
    }

    return 0;
}

// Function to print the correctly filled completed matrix
void printmtrx(char **mtrx, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (*(mtrx[i]) == 0)
            return;
        printf("%s", mtrx[i]);
    }
}

// Function to generate the log file of the puzzle filled so far
/*void printFilemtrx(char mtrx[N][N])
{
    for (int i = 0; i < N; i++)
    {
        if (mtrx[i][0] == 0)
        {
            fprintf(logFile, "\n");
            return;
        }
        fprintf(logFile, "%s", mtrx[i]);
    }
}*/

// Function to copy the current(partiaaly completed) puzzle
char **copyMatrix(char **mtrx1, int size, int width)
{
    char **mtrxCopy = (char **)calloc(size, sizeof(char *));
    for (int x = 0; x < size; x++)
    {
        if (strlen(mtrx1[x]) < 1)
            return mtrxCopy;

        mtrxCopy[x] = (char *)calloc(width, sizeof(char));
        strcpy(mtrxCopy[x], mtrx1[x]);
    }

    return mtrxCopy;
}

// Function to find the length of a horizontal space in the word list(eg: ####)
int hText(int i, int j, int width, char **mtrx)
{
    if (j < width && mtrx[i][j] != '*')
    {
        return 1 + hText(i, j + 1, width, mtrx);
    }
    else
    {
        return 0;
    }
}

/*Function to find the length of a vertical space in the grid (eg: #
                                                                   #
                                                                   #  ) */
int vText(int i, int j, int size, char **mtrx)
{
    if (i < size && mtrx[i][j] != '*')
    {
        return 1 + vText(i + 1, j, size, mtrx);
    }
    else
    {
        return 0;
    }
}

// function to check if the provided words in the word list is suffient to fill all the spaces in the grid
int **findWordsPuzzle(char **mtrx, int size, int width)
{
    int count = 0;
    int **lenMtrx = (int **)calloc(1, sizeof(int *));
    lenMtrx[0] = (int *)calloc(5, sizeof(int));

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (mtrx[i][j] != '*' && (j == 0 || (j > 0 && mtrx[i][j - 1] == '*')))
            {
                int hWordLen = hText(i, j, width - 1, mtrx);
                if (hWordLen > 1)
                {
                    lenMtrx = (int **)realloc(lenMtrx, sizeof(int *) * (count + 1));
                    lenMtrx[count] = (int *)calloc(5, sizeof(int));

                    lenMtrx[count][0] = 0;
                    lenMtrx[count][1] = i;
                    lenMtrx[count][2] = j;
                    lenMtrx[count][3] = hWordLen;

                    count++;
                }
            }
            if (mtrx[i][j] != '*' && (i == 0 || (i > 0 && mtrx[i - 1][j] == '*')))
            {
                int vWordLen = vText(i, j, size, mtrx);
                if (vWordLen > 1)
                {
                    lenMtrx = (int **)realloc(lenMtrx, sizeof(int *) * (count + 1));
                    lenMtrx[count] = (int *)calloc(5, sizeof(int));

                    lenMtrx[count][0] = 1;
                    lenMtrx[count][1] = i;
                    lenMtrx[count][2] = j;
                    lenMtrx[count][3] = vWordLen;

                    count++;
                }
            }
        }
    }
    wordsCanFill = count;
    return lenMtrx;
}

// Function to fill the vertical spaces of the grid
int fillvert(int x, int y, int size, char **mtrx, char *mtrx2)
{
    for (int i = 0; i < size; i++)
    {
        if (mtrx[x + i][y] == 35)
        {
            mtrx[x + i][y] = mtrx2[i];
        }
        else if (mtrx[x + i][y] == mtrx2[i])
        {
            continue;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

// Function to fill the horizontal spaces of the grid
int fillhor(int x, int y, int size, char **mtrx, char *mtrx2)
{
    for (int i = 0; i < size; i++)
    {
        if (mtrx[x][y + i] == 35)
        {
            mtrx[x][y + i] = mtrx2[i];
        }
        else if (mtrx[x][y + i] == mtrx2[i])
        {
            continue;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

void free2dArray(char **mtrx, int size){
    for(int i=0; i<size; i++)
        free(mtrx[i]);
    free(mtrx);
}

/*Main filling Function which uses the basics of Backtracking Algorithm to fill and print the completed Puzzle
 */
int fillpuzzle(int pos, char **mtrxP, char **mtrxW, int **mtrxWLen)
{
    int w_index = pos;

    for (int l_index = 0; l_index < wordsCanFill; l_index++)
    {
        char **mtrxPfill;
        mtrxPfill = copyMatrix(mtrxP, puzzleSize, puzzleWidth);

        int *wlen = mtrxWLen[l_index];

        if (wlen[3] == 0)
            break;

        if (wlen[4] == 1)
            continue;

        if (strlen(mtrxW[w_index]) - 1 == wlen[3])
        {
            if (wlen[0])
            {
                if (fillvert(wlen[1], wlen[2], wlen[3], mtrxPfill, mtrxW[w_index]))
                {
                    // printFilemtrx(mtrxPfill);
                    // fprintf(logFile, "%s\n", mtrxW[w_index]);
                    free2dArray(mtrxPfill, puzzleSize);
                    continue;
                }
            }
            else
            {
                if (fillhor(wlen[1], wlen[2], wlen[3], mtrxPfill, mtrxW[w_index]))
                {
                    // printFilemtrx(mtrxPfill);
                    // fprintf(logFile, "%s\n", mtrxW[w_index]);
                    free2dArray(mtrxPfill, puzzleSize);
                    continue;
                }
            }

            wlen[4] = 1;
            if (pos + 1 == wordsGiven)
            {
                mtrxPuzzle = copyMatrix(mtrxPfill, puzzleSize, puzzleWidth);
                free2dArray(mtrxPfill, puzzleSize);
                return 1;
            }
            if (fillpuzzle(pos + 1, mtrxPfill, mtrxW, mtrxWLen)){
                free2dArray(mtrxPfill, puzzleSize);
                return 1;
            }
            else
                wlen[4] = 0;
        }
    }

    if (pos)
        return 0;
    return 0;
}

int main() // Main Function
{
    //logFile = fopen("log.txt", "w"); // opening the log file

    mtrxPuzzle = getMatrix(&puzzleSize, &puzzleWidth); // calling this function to get the input of the grid into a 2D array
    mtrxWords = getMatrix(&wordsGiven, &maxWidthWord); // calling this function to get the input of the word list into a 2D array

    /*If (The input of the grid is invalid) OR (The input of the word list is invalid)
        =>   print "INVALID INPUT"  */
    if (checkWord(mtrxWords, wordsGiven, maxWidthWord) || checkPuzzle(mtrxPuzzle, puzzleSize, puzzleWidth))
    {
        printf("INVALID INPUT\n");
        return 0;
    }

    // // calling this function to check if the provided words in the word list is suffient to fill all the spaces in the grid
    wLenMtrx = findWordsPuzzle(mtrxPuzzle, puzzleSize, puzzleWidth);

    // If Words given in the list > spaces in the grid => Output "IMPOSSIBLE"
    if (wordsGiven > wordsCanFill)
    {
        printf("IMPOSSIBLE\n");
        return 0;
    }

    /*If all the words cannot be filled with given words considering the lengths of words and empty spaces
                => print "IMPOSSIBLE"*/
    if (!fillpuzzle(0, mtrxPuzzle, mtrxWords, wLenMtrx))
    {
        printf("IMPOSSIBLE\n");
        return 0;
    }

    // Print the completely filled matrix
    printmtrx(mtrxPuzzle, puzzleSize);

    //free dynamically allocated arrays
    free2dArray(mtrxPuzzle, puzzleSize);
    free2dArray(mtrxWords, wordsGiven);



    return 0;
}
