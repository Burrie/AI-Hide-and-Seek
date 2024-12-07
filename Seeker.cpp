#include "header.h"
#include <math.h>

void hill_climbing(const Agent& seeker, Position pos, int& choice)
{
    float min = pos.sld(seeker.position);
    for(int i = 0; i < 8; i++)
    {
        if(seeker.vision[seeker.actions[i][0] + seeker.vision_radius][seeker.actions[i][1] + seeker.vision_radius] == '1') //Obstacles -> cannot move into
            continue;

        if(seeker.vision[seeker.actions[i][0] + seeker.vision_radius][seeker.actions[i][1] + seeker.vision_radius] == '!') // Out of bounds -> cannot move into
            continue;
            
        Position spos(seeker.actions[i][0] + seeker.position.posX, seeker.actions[i][1] + seeker.position.posY);
        if(pos.sld(spos) < min)
            choice = i;
    }
}

float minimax_min(const Agent& seeker,Position hider_pos, int& choice)
{
    float min = INFINITY;

    //Seeker takes actions
    for(int i = 0; i < 8; i++)
    {
        if(seeker.vision[seeker.actions[i][0] + seeker.vision_radius][seeker.actions[i][1] + seeker.vision_radius] == '1') //Obstacles -> cannot move into
            continue;

        if(seeker.vision[seeker.actions[i][0] + seeker.vision_radius][seeker.actions[i][1] + seeker.vision_radius] == '!') // Out of bounds -> cannot move into
            continue;

        int x = seeker.position.posX + seeker.actions[i][0];
        int y = seeker.position.posY + seeker.actions[i][1];

        Position pos(x, y);

        if(pos == hider_pos)
        {
            choice = i;
            return 0;
        }

        //Hider takes actions
        float max = pos.sld(hider_pos);
        for(int j = 0; j < 8; j++)
        {
            Position hpos(hider_pos.posX + seeker.actions[i][0], hider_pos.posY + seeker.actions[i][1]);
            float sld = hpos.sld(pos);

            if(abs(hpos.posX - seeker.position.posX) <= seeker.vision_radius && abs(hpos.posY - seeker.position.posY) <= seeker.vision_radius)
            {
                if(seeker.vision[hpos.posX - seeker.position.posX + seeker.vision_radius][hpos.posY - seeker.position.posY + seeker.vision_radius] == '1')
                    continue;

                if(seeker.vision[hpos.posX - seeker.position.posX + seeker.vision_radius][hpos.posY - seeker.position.posY + seeker.vision_radius] == '!')
                    continue;
            }

            if(sld > max)
            {
                max = sld;
            }
        }

        if(max < min)
        {
            min = max;
            choice = i;
        }
    }
    return min;
}

bool Seeker::move(char** map, const int& n, const int& m) //using local search with evaluation function is SLD
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
                //Add obstacle to check later
                vision[x + vision_radius][y + vision_radius] = '1';
                check_vision('1');
                check_vision('#');
            }
            else if(map[position.posX + x][position.posY + y] == '2')
            {
                hints.push(Position(position.posX + x, position.posY + y));
            }
        }

    float min = INFINITY;

    int curChoice = -1;

    while(!hints.empty())
    {
        Position hint_pos = hints.top();
        hints.pop();

        int tmpChoice = -1;
        float minimax = minimax_min(*this, hint_pos, tmpChoice); //Choose among hints
        if(minimax < min)
        {
            min = minimax;
            curChoice = tmpChoice;

            if(min == 0)
            {
                while(!hints.empty())
                    hints.pop();
                break;
            }
        }
    }

    if(curChoice == -1) //No hider found && No hints -> Random: Move intuitively || Stand still
    {
        Position point = checkPoints.front();

        hill_climbing(*this, point, curChoice);

        if(curChoice == -1) //Choose next check point -> stand still
        {
            checkPoints.pop();
            checkPoints.push(point);
            return false;
        }
    }

    //Clean seeker's vision for next move;
    clean_vision();

    //Move
    map[position.posX][position.posY] = '0';
    map[position.posX += actions[curChoice][0]][position.posY += actions[curChoice][1]] = '3';

    //Update for printing
    for(int x = -vision_radius; x <= vision_radius; x++)
        for(int y = -vision_radius; y <= vision_radius; y++)
        {
            if(position.posX + x < 0 || position.posX + x >= m || position.posY + y < 0 || position.posY + y >= n)
                vision[x + vision_radius][y + vision_radius] = '!';

            if(vision[x + vision_radius][y + vision_radius] == '#' || vision[x + vision_radius][y + vision_radius] == '!')
                continue;

            if(map[position.posX + x][position.posY + y] == '1')
            {
                //Add obstacle to check later
                vision[x + vision_radius][y + vision_radius] = '1';
                check_vision('1');
                check_vision('#');
            }
        }

    return true;
}