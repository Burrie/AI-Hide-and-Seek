#include "header.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>

Hider::Hider(int posX, int posY) : Agent(posX, posY, 2)
{
    srand((unsigned) time(NULL));
    countStep = rand() % 6 + 5; //5-10 turns annouce once
}

Hider::Hider(const Hider& h) : Agent(h)
{
    countStep = h.countStep;
}

void random_move(const Hider& hider, int& choice)
{
    while(true)
    {
        srand((unsigned) time(NULL));
        int i = rand() % 9; //Just have 8 actions to make + 1 for stand still

        if(i == 8)
            return;

        if(hider.vision[hider.actions[i][0] + hider.vision_radius][hider.actions[i][1] + hider.vision_radius] == '1') //Obstacles -> cannot move into
            continue;

        if(hider.vision[hider.actions[i][0] + hider.vision_radius][hider.actions[i][1] + hider.vision_radius] == '!') // Out of bounds -> cannot move into
            continue;

        if(hider.vision[hider.actions[i][0] + hider.vision_radius][hider.actions[i][1] + hider.vision_radius] == '2') // 2 hiders will collide -> cannot move into
            continue;

        choice = i;
        return;
    }
}

void minimax_max(const Agent& hider,Position seeker_pos, int& choice)
{
    //current distance between hider and seeker
    float max = 0;

    //Hider takes actions
    for(int i = 0; i < 8; i++)
    {
        if(hider.vision[hider.actions[i][0] + hider.vision_radius][hider.actions[i][1] + hider.vision_radius] == '1') //Obstacles -> cannot move into
            continue;

        if(hider.vision[hider.actions[i][0] + hider.vision_radius][hider.actions[i][1] + hider.vision_radius] == '!') // Out of bounds -> cannot move into
            continue;

        if(hider.vision[hider.actions[i][0] + hider.vision_radius][hider.actions[i][1] + hider.vision_radius] == '2') // 2 hiders will collide -> cannot move into
            continue;

        int x = hider.position.posX + hider.actions[i][0];
        int y = hider.position.posY + hider.actions[i][1];

        Position pos(x, y);

        if(pos == seeker_pos)
        {
            continue;
        }

        //Seeker takes actions
        float min = pos.sld(seeker_pos);
        for(int j = 0; j < 8; j++)
        {
            Position spos(seeker_pos.posX + hider.actions[i][0], seeker_pos.posY + hider.actions[i][1]);
            float sld = pos.sld(spos);

            if(abs(spos.posX - hider.position.posX) <= hider.vision_radius && abs(spos.posY - hider.position.posY) <= hider.vision_radius)
            {
                if(hider.vision[spos.posX - hider.position.posX + hider.vision_radius][spos.posY - hider.position.posY + hider.vision_radius] == '1')
                    continue;

                if(hider.vision[spos.posX - hider.position.posX + hider.vision_radius][spos.posY - hider.position.posY + hider.vision_radius] == '!')
                    continue;
            }

            if(sld < min)
            {
                min = sld;
            }
        }

        if(max < min)
        {
            max = min;
            choice = i;
        }
    }
}

bool Hider::move(char** map, const int& n, const int& m)
{
    /*
        Vision:
        0 -> Nothing here
        1 -> Obstacle
        2 -> Hider
        3 -> Seeker
        # -> Cannot see
        ! -> Out of bounds
    */

    bool seekerFound = false;
    Position seeker_pos;
    int curChoice = -1;

    vision[vision_radius][vision_radius] = '#'; //Seeker's position -> Nothing to see -> Cannot see

    //Check map using vision
    for(int x = -vision_radius; x <= vision_radius; x++)
        for(int y = -vision_radius; y <= vision_radius; y++)
        {
            if(position.posX + x < 0 || position.posX + x >= m || position.posY + y < 0 || position.posY + y >= n)
                vision[x + vision_radius][y + vision_radius] = '!';

            if(vision[x + vision_radius][y + vision_radius] == '#' || vision[x + vision_radius][y + vision_radius] == '!')
                continue;

            if(map[position.posX + x][position.posY + y] == '1')
            {
                vision[x + vision_radius][y + vision_radius] = '1';

                check_vision('1');
                check_vision('#');
            }
            else if(map[position.posX + x][position.posY + y] == '2')
            {
                vision[x + vision_radius][y + vision_radius] = '2';
            }
            else if(map[position.posX + x][position.posY + y] == '3')
            {
                seekerFound = true;
                
                seeker_pos.posX = position.posX + x;
                seeker_pos.posY = position.posY + y;
            }
        }

    if(seekerFound == false)
        random_move(*this, curChoice);
    else
        minimax_max(*this, seeker_pos, curChoice);

    if(curChoice == -1)
        return false;

    //Move
    map[position.posX][position.posY] = '0';
    map[position.posX += actions[curChoice][0]][position.posY += actions[curChoice][1]] = '2';

    //Clean seeker's vision for next move;
    clean_vision();

    return true;
}

std::string GenerateMap()
{
    std::srand((unsigned)std::time(nullptr));
    char** map;
    int n = rand() % 10 + 10, m = rand()%10 + 20; //Fix n, m

    std::string filename = "map.txt";

    map = new char*[n];
    for (int i = 0; i < n; i++) {
        (map)[i] = (char*)malloc(m * sizeof(char));
    }
    bool Check = true;
    std::pair<int, int> go[4];
    go[0] = {1, 0}; go[1] = {-1, 0}; 
    go[2] = {0, -1}; go[3] = {0, 1};
    do{
        int Hider = 0;
        int x = rand()%n, y = rand()%m;
        map[x][y] = '3';
        Check = true;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
            if (i != x || j != y)
            {
                if (Hider < 10) map[i][j] = rand() % 3 + '0';
                else map[i][j] = rand() % 2 + '0';
                if (map[i][j] == '2') Hider++;
            }
        if (Hider == 0) continue;
        char**Newmap = new char*[n];
        for (int i = 0; i < n; i++) {
            (Newmap)[i] = (char*)malloc(m * sizeof(char));
            for (int j = 0; j < m; j++)
            if (i != x || j != y)
            {
                Newmap[i][j] = map[i][j];
            }
        }
        std::queue<std::pair<int, int> > Q;
        Q.push({x, y});
        while (Q.size())
        {
            x = Q.front().first;
            y = Q.front().second;
            Q.pop();
            if (Newmap[x][y] == '2') Hider--;
            Newmap[x][y] = '1';
            for (int i = 0; i < 4; i++)
            {
                int _x = x + go[i].first;
                int _y = y + go[i].second;
                if (_x >= 0 && _x < n && _y >= 0 && _y < m)
                {
                    if (Newmap[_x][_y] != '1')
                    {
                        Q.push({_x, _y});
                    }
                }
            }
        }
        Check = (Hider != 0);
    }while (Check);

    std::ofstream os;
    os.open(filename.c_str());

    os << n << ' ' << m << '\n';
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
            os << map[i][j] << ' ';
        os << '\n';
    }

    os.close();

    for(int i = 0; i < n; i++)
        delete [] map[i];
    delete [] map;

    return filename;
}

void Hider::hintForSeeker(char** map, const int& n, const int& m, Seeker& seeker)
{
    std::srand((unsigned)std::time(nullptr));

    int* hint = new int[2];
    do{
        hint[0] = std::rand() % 5 - 2 + position.posX;
        hint[1] = std::rand() % 5 - 2 + position.posY;
    }
    while (hint[0] < 0 || hint[0] >= m || hint[1] < 0 || hint[1] >= n || map[hint[0]][hint[1]] == '1');

    Position position(hint[0], hint[1]);
    delete [] hint;
    
    //Hint is visible to Seeker
    if(abs(position.posX - seeker.position.posX) <= seeker.vision_radius && abs(position.posY - seeker.position.posY) <= seeker.vision_radius)
        seeker.hints.push(position);

    //Reset countStep for next hint
    srand((unsigned) time(NULL));
    countStep = rand() % 6 + 5; //5-10 turns annouce once
}