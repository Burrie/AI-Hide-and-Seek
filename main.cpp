#include "header.h"

#include<iostream>
#include <fstream>


int main()
{
    std::string path = "./map.txt";
    Game game(path);

    std::cout << "Seeker: " << '(' << game.seeker.position.posX << ", " << game.seeker.position.posY << ")\n\n";
    std::cout << "Hiders: ";
    for(int i = 0; i < game.hiders.size(); i++)
        std::cout << '(' << game.hiders[i]->position.posX << ", " << game.hiders[i]->position.posY << ") ";
    std::cout << "\n\n";
    std::cout << " *** PRESS ANY KEY TO MOVE TO THE NEXT STEP ***\n";
    system("pause");
    
    //Update Seeker's vision for printing
    int vr = game.seeker.vision_radius;
    for(int x = -vr; x <= vr; x++)
        for(int y = -vr; y <= vr; y++)
        {
            if(game.seeker.position.posX + x < 0 || game.seeker.position.posX + x >= game.m || game.seeker.position.posY + y < 0 || game.seeker.position.posY + y >= game.n)
                game.seeker.vision[x + vr][y + vr] = '!';

            if(game.seeker.vision[x + vr][y + vr] == '#' || game.seeker.vision[x + vr][y + vr] == '!')
                continue;

            if(game.map[game.seeker.position.posX + x][game.seeker.position.posY + y] == '1')
            {
                //Add obstacle to check later
                game.seeker.vision[x + vr][y + vr] = '1';
                game.seeker.check_vision('1');
                game.seeker.check_vision('#');
            }
        }

    game.PrintMap();

    game.seeker.clean_vision();

    game.PlayGame();

    return 0;
}