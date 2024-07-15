// here nothing dont look gay
/*#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

const int MapWidth = 10, MapHeight = 10;
//       Y pos      X pos
char Map[MapHeight][MapWidth] = {
    { '1', '1', 'G', '1', '1', '1', '1', '1', '1', '1' },
    { '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' },
    { '1', '1', '0', '1', '1', '1', '1', '1', '0', 'G' },
    { '1', '1', '0', '0', '1', '1', '1', '1', '0', '1' },
    { '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' },
    { '1', '1', '1', '1', '1', '1', '1', '0', '0', '1' },
    { '1', '1', '1', '1', '1', '1', '1', '0', '1', '1' },
    { '1', '1', '1', '0', '0', '1', '1', '0', '1', '1' },
    { '1', '1', '0', '0', '0', '1', '1', '0', '0', '0' },
    { '1', '1', '1', 'G', '0', '1', '1', '1', '1', '1' }
};

struct Pos
{
    int X;
    int Y;
};

bool PosIsUsed(vector <Pos> Position, int Y, int X)
{
    for (vector <Pos>::iterator it = Position.begin(); it != Position.end(); ++it)
    {
        if (it->X == X && it->Y == Y)
        {
            return true;
        }
    }
    return false;
}

void SearchNext(int Y, int X, vector <Pos> Position)
{
    Pos NewStep = { X, Y };
    Position.push_back(NewStep);
    //North
    if (Y >= 1 && !PosIsUsed(Position, Y - 1, X))
    {
        vector <Pos> NewPosition = Position;
        switch (Map[Y - 1][X])
        {
        case '1':
            SearchNext(Y - 1, X, NewPosition);
            break;
        case 'G':
            printf("Found!\n");
            break;
        }
    }
    //East
    if (X + 1 < MapWidth && !PosIsUsed(Position, Y, X + 1))
    {
        vector <Pos> NewPosition = Position;
        switch (Map[Y][X + 1])
        {
        case '1':
            SearchNext(Y, X + 1, NewPosition);
            break;
        case 'G':
            printf("Found!\n");
            break;
        }
    }
    //Soth
    if (Y + 1 < MapHeight && !PosIsUsed(Position, Y + 1, X))
    {
        vector <Pos> NewPosition = Position;
        switch (Map[Y + 1][X])
        {
        case '1':
            SearchNext(Y + 1, X, NewPosition);
            break;
        case 'G':
            printf("Found!\n");
            break;
        }
    }
    //West
    if (X >= 1 && !PosIsUsed(Position, Y, X - 1))
    {
        vector <Pos> NewPosition = Position;
        switch (Map[Y][X - 1])
        {
        case '1':
            SearchNext(Y, X - 1, NewPosition);
            break;
        case 'G':
            printf("Found!\n");
            break;
        }
    }

}

int main()
{
    vector <Pos> Pos;
    SearchNext(9, 9, Pos); //We start here (Pos [9][9])
    printf("End\n");
    system("PAUSE");
    return 0;
}*/
