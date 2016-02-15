#ifndef PLAYERREAPER_H
#define PLAYERREAPER_H

class Game;
class Player;


class PlayerReaper
{
public:
    PlayerReaper(Game* game = 0);
    virtual void cleanUpCards(Player* corpse);
    virtual ~PlayerReaper() {}
private:
    Game* mp_game;
};

#endif // PLAYERREAPER_H
