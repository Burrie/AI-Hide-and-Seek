#include "header.h"
#include <iostream>

Agent::Agent(int _posX, int _posY, int radius)
{
    //Position
    position.posX = _posX;
    position.posY = _posY;

    //Actions
    int pos = 0;
    for(int x = -1; x <= 1; x++)
        for(int y = -1; y <=1; y++)
            if(x != 0 || y != 0)
            {
                actions[pos][0] = x;
                actions[pos][1] = y;
                pos++;
            }

    //Vision
    vision_radius = radius;
    vision = new char*[vision_radius * 2 + 1];
    for(int i = 0 ; i < vision_radius * 2 + 1; i++)
    {
        vision[i] = new char[vision_radius * 2 + 1];
        for(int j = 0; j < vision_radius * 2 + 1; j++)
            vision[i][j] = ' ';
    }
}

Agent::Agent(const Agent& a)
{
    position.posX = a.position.posX;
    position.posY = a.position.posY;

    for(int i = 0; i < 8; i++)
    {
        actions[i][0] = a.actions[i][0];
        actions[i][1] = a.actions[i][1];
    }

    vision_radius = a.vision_radius;

    vision = new char*[vision_radius * 2 + 1];
    for(int i = 0 ; i < vision_radius * 2 + 1; i++)
    {
        vision[i] = new char[vision_radius * 2 + 1];
        for(int j = 0; j < vision_radius * 2 + 1; j ++)
            vision[i][j] = a.vision[i][j];
    }
}

Agent::~Agent()
{
    for(int i = 0; i < vision_radius * 2 + 1; i++)
        delete [] vision[i];
    delete [] vision;
}

void Agent::clean_vision()
{
    for(int i = 0; i < vision_radius*2 + 1; i++)
        for(int j = 0; j < vision_radius*2 + 1; j++)
            vision[i][j] = ' ';
}

void Agent::check_vision(char kitu) 
{
    int radius = vision_radius;
    // Diagonal Line
    for (int i = 0; i <= radius*2; i++) {
            // 1st
            if (vision[i][2*radius-i] == kitu && i < radius) {
                    int x = i;
                    int y = 2*radius-i;

                    int prevx = x;
                    while (x >= 0) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            x--;
                    }

                    x = prevx;
                    int prevy = y;
                    while (x >= 0 && y <= radius*2) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            x--;
                            y++;
                    }

                    x = prevx;
                    y = prevy;

                    while (y <= radius*2) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            y++;
                    }
            }

            // 2nd
            if (vision[i][i] == kitu && i < radius) {
                    int x = i;
                    int y = i;

                    int prevx = x, prevy = y;

                    while (y >=0) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            y--;
                    }
                    y = prevy;

                    while (x >= 0 && y >= 0) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            x--;
                            y--;
                    }
                    x = prevx;
                    y = prevy;

                    while (x >= 0) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            x--;
                    }
            }

            // 3rd
            if (vision[i][2*radius-i] == kitu && i > radius) {
                    int x = i;
                    int y = 2*radius-i;

                    int prevx = x, prevy = y;
                    while (y >= 0) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            y--;
                    }
                    y = prevy;

                    while (y >= 0 && x <= radius*2) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';

                            y--;
                            x++;
                    }

                    x = prevx;
                    y = prevy;

                    while (x <= radius*2) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            x++;
                    }
            }

            // 4th
            if (vision[i][i] == kitu && i > radius) {
                    int x = i;
                    int y = i;

                    int prevx = x, prevy = y;

                    while (y <= radius*2) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            y++;
                    }

                    y = prevy;

                    while (x <= radius*2 && y <= radius*2) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            x++;
                            y++;
                    }

                    x = prevx;
                    y = prevy;

                    while (x <= radius*2) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';

                            x++;
                    }
            }
    }


    // Straight Line
    for (int i = 0; i <= radius*2; i++) {
            // Vertical
            if (vision[i][radius] == kitu && i < radius) {
                    int x = i;
                    int y = radius;

                    while (x >= 0) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            x--;
                    }
            }

            if (vision[i][radius] == kitu && i > radius) {
                    int x = i;
                    int y = radius;

                    while (x <= radius*2) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            x++;
                    }
            }

            // Horizontal
            if (vision[radius][i] == kitu && i < radius) {
                    int x = radius;
                    int y = i;

                    while (y >= 0) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            y--;
                    }
            }

            if (vision[radius][i] == kitu && i > radius) {
                    int x = radius;
                    int y = i;

                    while (y <= radius*2) {
                            if (vision[x][y] != '1' && vision[x][y] != '!')
                                    vision[x][y] = '#';
                            y++;
                    }
            }
    }


    // 1st half of grid 2upper
    for (int i = 0; i < radius-1; i++) {

            // 2upper
            for (int j = i+1; j < radius; j++)
                    if (vision[i][j] == kitu) {
                            int x = i;
                            int y = j;

                            int prevx = x, prevy = y;

                            while (x >= 0) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';

                                    x--;
                            }
                            x = prevx;

                            while (x >= 0 && y >= 0) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';
                                    x--;
                                    y--;
                            }
                    }


            for (int j = radius+1; j <= 2*radius-i-1; j++)
                    if (vision[i][j] == kitu) {
                            int x = i;
                            int y = j;

                            int prevx = x, prevy = y;

                            while (x >= 0) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';
                                    x--;
                            }

                            x = prevx;

                            while (x >= 0 && y <= radius*2) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';

                                    x--;
                                    y++;
                            }
                    }

    }


    // 1st half of grid 2lower
    for (int i = radius-1; i > 0; i--) {
            for (int j = 0; j < i; j++)
                    if (vision[i][j] == kitu) {
                            int x = i;
                            int y = j;

                            int prevx = x, prevy = y;

                            while (y >= 0) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';

                                    y--;
                            }

                            y = prevy;

                            while (x >= 0 && y >= 0) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';

                                    x--;
                                    y--;
                            }
                    }

            for (int j = radius*2; j > radius*2-i; j--)
                    if (vision[i][j] == kitu) {
                            int x = i;
                            int y = j;

                            int prevx = x, prevy = y;

                            while (y <= radius*2) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';
                                    y++;
                            }

                            y = prevy;

                            while (y <= radius*2 && x >= 0) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';
                                    y++;
                                    x--;
                            }
                    }
    }


    // 2nd half of grid 2upper
    for (int i = radius+1; i < 2*radius; i++) {
            for (int j = 0; j <= 2*radius-1-i; j++)
                    if (vision[i][j] == kitu) {
                            int x = i;
                            int y = j;

                            int prevx = x, prevy = y;

                            while (y >= 0) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';

                                    y--;
                            }

                            y = prevy;

                            while (y >= 0 && x <= radius*2) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';

                                    x++;
                                    y--;
                            }
                    }

            for (int j = radius*2; j >= i+1; j--)
                    if (vision[i][j] == kitu) {
                            int x = i;
                            int y = j;

                            int prevx = x, prevy = y;

                            while (y <= radius*2) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';

                                    y++;
                            }

                            y = prevy;

                            while (x <= radius*2 && y <= radius*2) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';
                                    x++;
                                    y++;
                            }
                    }
    }


    // 2nd half of grid 2lower
    for (int i = 2*radius; i > radius+1; i--) {
            for (int j = 2*radius-i+1; j < radius; j++)
                    if (vision[i][j] == kitu) {
                            int x = i;
                            int y = j;

                            int prevx = x, prevy = y;

                            while (x <= radius*2) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';

                                    x++;
                            }

                            x = prevx;

                            while (y >= 0 && x <= radius*2) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';
                                    y--;
                                    x++;
                            }
                    }

            for (int j = i-1; j > radius; j--)
                    if (vision[i][j] == kitu) {
                            int x = i;
                            int y = j;

                            int prevx = x, prevy = y;

                            while (x <= radius*2) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';
                                    x++;
                            }

                            x = prevx;

                            while (x <= radius*2 && y <= radius*2) {
                                    if (vision[x][y] != '1' && vision[x][y] != '!')
                                            vision[x][y] = '#';
                                    x++;
                                    y++;
                            }
                    }
    }
}