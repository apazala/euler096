#include <stdio.h>
#include <string.h>

typedef struct
{
    int stack[10000000];
    int front;
} stack_type;

#define reset(s) ((s).front = -1)
#define isempty(s) ((s).front == -1)
#define any(s) ((s).front != -1)
#define push(s, x) ((s).stack[++((s).front)] = (x))
#define pop(s) ((s).stack[(s).front--])
#define top(s) ((s).stack[(s).front])

// pandigits
char pandigits[410000][10];
//[start, end) indexes
int pandigitsEndIndexes[10];
int *pandigitsStartIndexes = pandigitsEndIndexes - 1;

void fillpandigits()
{

    int pcount = 0;
    pandigits[0][0] = '1';
    int pandigIni = 0;
    int pandigFin = 1;
    pandigitsEndIndexes[1] = 1;
    pcount = 1;

    for (int n = 2; n < 10; n++)
    {
        char dig = n + '0';
        for (int i = pandigIni; i < pandigFin; i++)
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < j; k++)
                {
                    pandigits[pcount][k] = pandigits[i][k];
                }
                pandigits[pcount][j] = dig;
                for (int k = j + 1; k < n; k++)
                {
                    pandigits[pcount][k] = pandigits[i][k - 1];
                }
                pcount++;
            }
        }

        pandigIni = pandigFin;
        pandigFin = pcount;
        pandigitsEndIndexes[n] = pcount;
    }
}


stack_type stack;

char sudoku[9][10]; // each row ends with '\0'
char sudokuorig[9][10];
void solve()
{
    // Depth First Search
    reset(stack);
    int row = 0;
    const int pandigitfirst = pandigitsStartIndexes[9];
    const int pandigitlimit = pandigitsEndIndexes[9];
    int pandigitindex = pandigitfirst;
    while (row < 9)
    {
        int valid = 0;
        for (; !valid && pandigitindex < pandigitlimit; pandigitindex++)
        {
            valid = 1;
            // check row
            for (int j = 0; valid && j < 9; j++)
            {
                valid = (sudokuorig[row][j] == '0' || sudokuorig[row][j] == pandigits[pandigitindex][j]);
            }

            // check rows above
            for (int i = 0; valid && i < row; i++)
            {
                for (int j = 0; valid && j < 9; j++)
                {
                    valid = (sudoku[i][j] != pandigits[pandigitindex][j]);
                }
            }

            // check rows below
            for (int i = row + 1; valid && i < 9; i++)
            {
                for (int j = 0; valid && j < 9; j++)
                {
                    valid = (sudokuorig[i][j] == '0' || sudokuorig[i][j] != pandigits[pandigitindex][j]);
                }
            }
        }

        if (valid)
        {
            push(stack, pandigitindex); //Store last used index + 1
            pandigitindex--; //Last used index

            for (int j = 0; j < 9; j++)
            {
                sudoku[row][j] = pandigits[pandigitindex][j];
            }

            row++;
            pandigitindex = pandigitfirst;
        }
        else
        {
            if (isempty(stack))
            {
                fprintf(stderr, "UNSOLVABLE SUDOKU!!\n");
                return;
            }
            // Backtrack: this index is the succesor of the last used in the previous row
            pandigitindex = pop(stack);
            row--;
        }
    }
}

char line[256];
int readsudoku(FILE *pf)
{
    if (!fgets(line, sizeof(line), pf))
        return 0;

    for (int i = 0; i < 9; i++)
    {
        if (!fgets(line, sizeof(line), pf))
            return 0;
        for (int j = 0; j < 9; j++)
        {
            sudokuorig[i][j] = line[j];
        }
    }

    return 1;
}

int main()
{
    char filename[] = "p096_sudoku.txt";
    FILE *pf = fopen(filename, "r");
    if (!pf)
    {
        fprintf(stderr, "Can't open <<%s>>\n", filename);
        return 1;
    }

    fillpandigits();

    while (readsudoku(pf))
    {
        solve();
        for(int i = 0; i < 9; i++)
            printf("%s\n", sudoku[i]);
    }

    fclose(pf);

    return 0;
}