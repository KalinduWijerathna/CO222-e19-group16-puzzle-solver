
/* ~######## ###    ##~    ~#######~    ~########   ~~#########        ~########  .######.   #*        #*     #*     ~#######    ~########
  ###    ### ###    ###   ###    ###   ###    ###   ###    ###        ###    ### ###    ### ###       ###    ###   ###    ###   ###    ###
  ###    #*  ###    ###   ###    ###   ###    #*    ###    ###        ###    #*  ###    ### ###       ###    ###   ###    #*    ###    ###
  ###        ###    ###   ###    ###  ~###___      ~###____##*        ###        ###    ### ###       ###    ###  ~###___      ~###____##*
  *########* ###    ### *#########*  **###***     **###*****         *########## ###    ### ###       ###    ### **###***     **#####*****
         ### ###    ###   ###          ###    #~  *###########               ### ###    ### ###   ### ###    ###    #~  *###########
  #*     ### ###    ###   ###          ###    ###   ###    ###        ~#*    ### ###    ### ####    # ###    ###   ###    ###   ###    ###
 ~#########* *########*  ~####*        ##########   ###    ###       ~#########*  *######*  #####~~##  *######*    ##########   ###    ###
                                                    ###    ###                              *                                   ###    ###   V 1.0 */
/* Authers      : E/19/309 RAMBUKWELLA H.M.W.K.G. MR  e19309@eng.pdn.ac.lk
                  E/19/446 WIJERATHNA I.M.K.D.I. MR   e19446@eng.pdn.ac.lk
   Group No.    : 16
   Project      : CO222-2022 Crossword Puzzle Solver
   File Name    : group_16_puzzle_static.c     // SUPER_SOLVER_Version_1.0
   Last Updated : Sun 01/08/2023 17:31:59
*/

//Including the standerd I/O and string libraries
#include <stdio.h>
#include <string.h>

//Defining the Const. value N for static array sizes for each words in the word list and each spaces in the grid
#define N 50

//Declaring and initializing global variables
int rows = 0;                //Number of Rows in the grid
int cols = 0;                //Number of Columns in the grid
char mtrxPuzzle[N][N] = {0}; //static 2D array to store spaces of the grid
char mtrxWords[N][N] = {0};  //static 2D array to store words of the word list
int wLenMtrx[N][5] = {0};    //static 2D array to store word lengths of each word
int wordsGiven = 0;          //Number of given words
int wordsCanFill = 0;        //Number of spaces in the grid
//FILE *logFile;               //log file to check for errors

//Declaring all the functions used in the program
void getMatrix(char mtrx[N][N]);
int checkPuzzle(char mtrx[N][N]);
int checkWord(char mtrx[N][N]);
void printmtrx(char mtrx[N][N]);
//void printFilemtrx(char mtrx[N][N]);
void copyMatrix(char mtrx1[N][N], char mtrx2[N][N]);
int hText(int i, int j, char mtrx[N][N]);
int vText(int i, int j, char mtrx[N][N]);
void findWordsPuzzle(char mtrx[N][N]);
int fillvert(int x, int y, int size, char mtrx[N][N], char mtrx2[N]);
int fillhor(int x, int y, int size, char mtrx[N][N], char mtrx2[N]);
int fillpuzzle(int pos, char mtrxP[N][N], char mtrxW[N][N], int mtrxWLen[N][5]);

int main() //Main Function
{
    //logFile = fopen("log.txt", "w"); //opening the log file

    getMatrix(mtrxPuzzle); //calling this function to get the input of the grid into a 2D array
    getMatrix(mtrxWords);  //calling this function to get the input of the word list into a 2D array

    /*If (The input of the grid is invalid) OR (The input of the word list is invalid)
        =>   print "INVALID INPUT"  */
    if (checkPuzzle(mtrxPuzzle) || checkWord(mtrxWords))
    {
        printf("INVALID INPUT");
        return 0;
    }

    // calling this function to check if the provided words in the word list is suffient to fill all the spaces in the grid
    findWordsPuzzle(mtrxPuzzle);

    //If Words given in the list > spaces in the grid => Output "IMPOSSIBLE"
    if (wordsGiven > wordsCanFill)
    {
        printf("IMPOSSIBLE");
        return 0;
    }

    /*If all the words cannot be filled with given words considering the lengths of words and empty spaces
                => print "IMPOSSIBLE"*/
    if (!fillpuzzle(0, mtrxPuzzle, mtrxWords, wLenMtrx))
    {
        printf("IMPOSSIBLE");
        return 0;
    }

    //Print the completely filled matrix
    printmtrx(mtrxPuzzle);

    return 0;
}

//Function get the input of the grid into a 2D array
void getMatrix(char mtrx[N][N])
{
    for (int i = 0; i < N; i++)
    {
        char text[N];
        fgets(text, N, stdin);
        if (text[0] == '\n')
        {
            strcpy(mtrx[i], "\0");
            break;
        }
        strcpy(mtrx[i], text);
    }
}

/*Function to check if the Grid input is invalid(return 1 if invalid)
  invalid conditions considerd=>
        if the grid consists chars other than '#','*','A'-'Z' > return 1
        if each line of the grid is not line separated        > return 1 */
int checkPuzzle(char mtrx[N][N])
{
    for (int i = 0; i < N; i++)
    {
        if (mtrx[i][0] == 0)
            break;

        rows++;
        cols = 0;

        for (int j = 0; j < N; j++)
        {
            if (mtrx[i][j] == 10)
                break;

            cols++;

            if (mtrx[i][j] != 42 && mtrx[i][j] != 35 && (mtrx[i][j] > 90 || mtrx[i][j] < 65))
            {
                return 1;
            }
        }

        if (i > 0 && strlen(mtrx[i]) != strlen(mtrx[0]))
        {
            return 1;
        }
    }
    if (rows < 2 && cols < 2)
    {
        return 1;
    }
    return 0;
}

/*Function to check if the Word list input is invalid(return 1 if invalid)
  invalid conditions considerd=>
        if the word list consists chars other than 'A'-'Z','a'-'z' > return 1
        if each word of the word list is not line separated        > return 1 */
int checkWord(char mtrx[N][N])
{
    for (int x = 0; x < N; x++)
    {
        if (mtrx[x][0] == 0)
        {
            mtrx[x][0] = 0;
            break;
        }
        wordsGiven++;

        if (strlen(mtrx[x]) - 1 == 1)
        {
            return 1;
        }

        for (int y = 0; y < N; y++)
        {
            if (mtrx[x][y] == 10 || mtrx[x][y] == 0)
                break;
            if (mtrx[x][y] > 122 || mtrx[x][y] < 65)
            {
                return 1;
            }
            if (mtrx[x][y] > 90 && mtrx[x][y] < 97)
            {
                return 1;
            }


        }
    }

    if (wordsGiven < 1)
    {
        return 1;
    }

    return 0;
}

//Function to print the correctly filled completed matrix
void printmtrx(char mtrx[N][N])
{
    for (int i = 0; i < N; i++)
    {
        if (mtrx[i][0] == 0)
            return;
        printf("%s", mtrx[i]);
    }
}

//Function to generate the log file of the puzzle filled so far
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

//Function to copy the current(partiaaly completed) puzzle
void copyMatrix(char mtrx1[N][N], char mtrx2[N][N])
{
    for (int x = 0; x < N; x++)
    {
        if (strlen(mtrx2[x]) < 1)
            return;

        strcpy(mtrx1[x], mtrx2[x]);
    }
}

//Function to find the length of a horizontal space in the word list(eg: ####)
int hText(int i, int j, char mtrx[N][N])
{
    if (mtrx[i][j] != 42 && j < cols)
    {
        return 1 + hText(i, j + 1, mtrx);
    }
    else
    {
        return 0;
    }
}


/*Function to find the length of a vertical space in the grid (eg: #
                                                                   #
                                                                   #  ) */
int vText(int i, int j, char mtrx[N][N])
{
    if (mtrx[i][j] != 42 && i < rows)
    {
        return 1 + vText(i + 1, j, mtrx);
    }
    else
    {
        return 0;
    }
}

/*function to check spaces that can be filled by the words in the word list and
 fill them into another 2D array with position cordinates, word width etc.*/
void findWordsPuzzle(char mtrx[N][N])
{
    int count = 0;

    for (int i = 0; i < rows; i++)
    {
        if (mtrx[i][0] == 0)
            return;

        for (int j = 0; j < cols; j++)
        {
            if (mtrx[i][j] != 42 && (j == 0 || (j > 0 && mtrx[i][j - 1] == 42)))
            {
                int hWordLen = hText(i, j, mtrx);
                if (hWordLen > 1)
                {
                    wLenMtrx[count][0] = 0;
                    wLenMtrx[count][1] = i;
                    wLenMtrx[count][2] = j;
                    wLenMtrx[count][3] = hWordLen;

                    count++;
                }
            }
            if (mtrx[i][j] != 42 && (i == 0 || (i > 0 && mtrx[i - 1][j] == 42)))
            {
                int vWordLen = vText(i, j, mtrx);
                if (vWordLen > 1)
                {
                    wLenMtrx[count][0] = 1;
                    wLenMtrx[count][1] = i;
                    wLenMtrx[count][2] = j;
                    wLenMtrx[count][3] = vWordLen;

                    count++;
                }
            }
        }
    }
    wordsCanFill = count;
}

//Function to fill the vertical spaces of the grid
int fillvert(int x, int y, int size, char mtrx[N][N], char mtrx2[N])
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

//Function to fill the horizontal spaces of the grid
int fillhor(int x, int y, int size, char mtrx[N][N], char mtrx2[N])
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

/*Main filling Function which uses the basics of Backtracking Algorithm to fill and print the completed Puzzle
*/
int fillpuzzle(int pos, char mtrxP[N][N], char mtrxW[N][N], int mtrxWLen[N][5])
{
    int w_index = pos;

    for (int l_index = 0; l_index < N; l_index++)
    {
        char mtrxPfill[N][N] = {0};
        copyMatrix(mtrxPfill, mtrxP);

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
                    //printFilemtrx(mtrxPfill);
                    //fprintf(logFile, "%s\n", mtrxW[w_index]);
                    continue;
                }
            }
            else
            {
                if (fillhor(wlen[1], wlen[2], wlen[3], mtrxPfill, mtrxW[w_index]))
                {
                    //printFilemtrx(mtrxPfill);
                    //fprintf(logFile, "%s\n", mtrxW[w_index]);
                    continue;
                }
            }

            wlen[4] = 1;
            if (pos + 1 == wordsGiven)
            {
                copyMatrix(mtrxPuzzle, mtrxPfill);
                return 1;
            }
            if (fillpuzzle(pos + 1, mtrxPfill, mtrxW, mtrxWLen))
                return 1;
            else
                wlen[4] = 0;
        }
    }

    if (pos)
        return 0;
    return 0;
}
