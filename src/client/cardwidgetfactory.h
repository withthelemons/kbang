#ifndef CARDWIDGETFACTORY_H
#define CARDWIDGETFACTORY_H

#include "util.h"
#include "parser/parserstructs.h"

class QWidget;

namespace client
{

class Game;
class CardWidget;

class CardWidgetFactory: private NonCopyable
{
friend class Game;
public:

    CardWidget* createPlayingCard(QWidget* parent);
    CardWidget* createCharacterCard(QWidget* parent, CharacterType = CHARACTER_UNKNOWN);
    CardWidget* createRoleCard(QWidget* parent, PlayerRole = ROLE_UNKNOWN);

    void registerCard(CardWidget* cardWidget);

private:
    CardWidgetFactory(Game* game);
    CardWidget* createCardWidget(QWidget* parent);
    Game* mp_game;
};

}
#endif // CARDWIDGETFACTORY_H
