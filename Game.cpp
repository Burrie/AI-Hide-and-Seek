#include "header.h"
#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;

Game::Game(std::string path)
{
    //Assign
    countStep = 0;
    point = 0;

    //Clear output file
    ofstream os;
    os.open("result.txt");
    os.close();

    //Initiate map
    path = GenerateMap();
    ifstream in(path.c_str()); //Open input file
    if(in.is_open() == false)
        return;

    in >> n >> m;

    map = new char*[m];
    for(int i = 0; i < m; i++)
        map[i] = new char[n];

    for(int y = 0; y < n; y++)
        for(int x = 0; x < m; x++)
        {
            in >> map[x][y];
            if(map[x][y] == '2')
            {
                Hider* pointer_hider = new Hider(x,y);
                hiders.push_back(pointer_hider);
            }
            else if(map[x][y] == '3')
            {
                seeker.position.posX = x;
                seeker.position.posY = y;

                //Initialize Check Points for seeker
                //Number of checkpoints can scale with size of the map
                for(int y = 0 ; y <= n; y += n/ (2 + 2*(n/10)) )
                    for(int x = 0; x <= m; x += m/ (2 + 2*(n/10)) )
                        seeker.checkPoints.push(Position(x, y));
            }
        }

    in.close(); //Close input file
}

Game::~Game()
{
    for(int i = 0; i < m; i++)
        delete [] map[i];
    delete [] map;
}

void Game::PlayGame() //Agents move in turn
{
    if(hiders.empty()) //Does not have any hider to find
        return;

    while(true)
    {
        if(seeker.move(map, n, m))
            point--;
        
        for(int i = 0 ; i < hiders.size(); i++)
        {
            if(seeker.position == hiders[i]->position)
            {
                point += 20;
                delete hiders[i];
                hiders.erase(hiders.begin() + i);

                if(hiders.empty()) //Game ended
                {
                    PrintMap();

                    system("cls");

                    std::cout << "GAME OVER !!! \n\n";
                    std::cout << "Point: " << point;

                    return;
                }
            }
            else
            {
                //Move
                hiders[i]->move(map, n, m);

                //Hint for Seeker if Hider is in turn
                if(hiders[i]->countStep == 0)
                    hiders[i]->hintForSeeker(map, n, m, seeker);
                else
                    hiders[i]->countStep--;
            }
        }

        countStep++;
        
        PrintMap();
    }
}

void Game::PrintMap()
{
    ofstream os;
    os.open("result.txt", ofstream::app); //Open output file

    system("cls");
    cout << "Step " << countStep << ":\n";
    os << "Step "<< countStep << ":\n";
    for(int y = 0; y < n; y++)
    {
        for(int x = 0; x < m; x++)
        {
            char tmp = map[x][y];
            if(tmp == '1') // Red Obstacle
            {
                std::cout << "\x1B[31m" << tmp << "\033[0m" << ' '; 
            }
            else if(tmp == '2')
            {
                if( (abs(x - seeker.position.posX) <= seeker.vision_radius && abs(y - seeker.position.posY) <= seeker.vision_radius ) && seeker.vision[x - seeker.position.posX + seeker.vision_radius][y - seeker.position.posY + seeker.vision_radius] != '#')
                {
                    //Cyan Hider when Hider is out of Seeker's vision
                    std::cout << "\x1B[36m" << tmp << "\033[0m" << ' ';
                }
                else
                {
                    //Blue Hider when Hider is out of Seeker's vision
                    std::cout << "\x1B[94m" << tmp << "\033[0m" << ' ';
                }
            }
            else if(tmp == '3') //Green Seeker
            {
                std::cout << "\x1B[92m" << tmp << "\033[0m" << ' ';
            }
            else
            {
                if( ( abs(x - seeker.position.posX) <= seeker.vision_radius && abs(y - seeker.position.posY) <= seeker.vision_radius ) && seeker.vision[x - seeker.position.posX + seeker.vision_radius][y - seeker.position.posY + seeker.vision_radius] != '#' )
                {
                    //Yellow seeker's vision
                    std::cout << "\x1B[33m" << tmp << "\033[0m" << ' ';
                }
                else
                {
                    std::cout << tmp << ' ';
                }
            }

            os << tmp << ' ';
        } 
        std::cout << '\n';
        os << '\n';
    }
    std::cout << "\n\n";
    os << '\n';

    os.close(); //Open input file

    seeker.clean_vision();

    system("pause");
}