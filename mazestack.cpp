//
//  main.cpp
//  CS 32 Homework 2
//
//  Created by Sophia Liu on 1/26/21.
//

#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Coord
        {
          public:
            Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
            int r() const { return m_row; }
            int c() const { return m_col; }
          private:
            int m_row;
            int m_col;
        };

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec); //function declatation
 
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack;
    Coord start(sr,sc);
    Coord end(er,ec);
    coordStack.push(start);
    maze[sr][sc]='o';
    
    while(!coordStack.empty())
    {
     Coord current = coordStack.top();
     coordStack.pop();
        int r = current.r(); int c = current.c();
     //cout << "(" << r << "," << c << ")" << endl;
    
     if (r == end.r() && c == end.c())
         return true;
    
     if (r-1 >= 0 && maze[r-1][c] != 'o' && maze[r-1][c] != 'X')
            {Coord north(current.r()-1, current.c());
            coordStack.push(north);
            maze[r-1][c] = 'o';
            }
        
    if (c+1 < nCols && maze[r][c+1] != 'o' && maze[r][c] != 'X')
            {Coord east(current.r(), current.c()+1);
             coordStack.push(east);
             maze[r][c+1] = 'o';
            }
        
    if (r+1 < nRows && maze[r+1][c] != 'o' && maze[r+1][c] != 'X')
            {Coord south(r+1, c);
             coordStack.push(south);
             maze[r+1][c] = 'o';
            }

    if (c-1 >= 0 && maze[r][c-1] != 'o' && maze[r][c-1] != 'X')
            {Coord west(r, c-1);
             coordStack.push(west);
             maze[r][c-1] = 'o';
            }
    }
    
return false;};

