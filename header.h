#include<vector>
#include<string>
#include<queue>
#include<stack>

struct Position
{
    int posX, posY;

    bool operator== (const Position& p);
    float sld(const Position& p);

    Position(int x = 0, int y = 0);
};

struct Agent
{
    //Data
    Position position;
    int actions[8][2];
    int vision_radius;
    char** vision;

    //Functions
    virtual bool move(char** map, const int& n, const int& m) = 0;
    virtual void check_vision(char c);
    void clean_vision();

    //Constructor
    Agent(int posX, int posY, int radius);
    Agent(const Agent& a);
    ~Agent();
};

struct Seeker : public Agent
{
    //Data
    std::stack<Position> hints;
    std::queue<Position> checkPoints;
    
    //Functions
    bool move(char** map, const int& n, const int& m);

    //Constructor
    Seeker(int posX = 0, int posY = 0) : Agent(posX, posY, 3) {}
};

struct Hider : public Agent
{
    //Data
    int countStep;
    
    //Functions
    bool move(char** map, const int& n, const int& m);
    void hintForSeeker(char** map, const int& n, const int& m, Seeker& seeker);
    void vision_hider(char** map, const int& n, const int& m);

    //Constructor
    Hider(int posX = 0, int posY = 0);
    Hider(const Hider& h);
};

struct Game
{
    char** map;
    unsigned int m, n;

    unsigned int countStep, point;

    Game(std::string path);
    ~Game();

    Seeker seeker;
    std::vector<Hider*> hiders;

    void PlayGame();
    void PrintMap();
};

std::string GenerateMap();