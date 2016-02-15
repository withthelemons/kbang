/***************************************************************************
 *   Copyright (C) 2008 by MacJariel                                       *
 *   echo "badmailet@gbalt.dob" | tr "edibmlt" "ecrmjil"                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QRegExpValidator>
#include <QRegExp>
#include "creategamedialog.h"
#include "config.h"

using namespace client;

const int minPlayers = 4, maxPlayers = 7;

CreateGameDialog::CreateGameDialog(QWidget *parent)
 : QDialog(parent), Ui::CreateGameDialog()
{
    setupUi(this);

    pushButtonCreate->setEnabled(0);

    spinBoxMinPlayers->setRange(minPlayers, maxPlayers);
    spinBoxMaxPlayers->setRange(minPlayers, maxPlayers);
    spinBoxAIPlayers->setRange(0, maxPlayers - 1);

    if (Config::instance().hasGroup("create-game-dialog")) {
        loadConfigValues();
    } else {
        spinBoxMinPlayers->setValue(minPlayers);
        spinBoxMaxPlayers->setValue(maxPlayers);
        spinBoxAIPlayers->setValue(0);
        spinBoxMaxSpectators->setValue(-1);
    }

    connect(spinBoxMinPlayers, SIGNAL(valueChanged(int)),
            this, SLOT(playerCountsChanged()));
    connect(spinBoxMaxPlayers, SIGNAL(valueChanged(int)),
            this, SLOT(playerCountsChanged()));

    connect(lineEditGameName,   SIGNAL(textChanged(const QString&)),
            this, SLOT(validateInput()));
    connect(lineEditPlayerName, SIGNAL(textChanged(const QString&)),
            this, SLOT(validateInput()));
}


CreateGameDialog::~CreateGameDialog()
{
}

void CreateGameDialog::playerCountsChanged()
{
    spinBoxMinPlayers->setMaximum(spinBoxMaxPlayers->value());
    spinBoxMaxPlayers->setMinimum(spinBoxMinPlayers->value());
    spinBoxAIPlayers->setMaximum(spinBoxMaxPlayers->value() - 1);
}

void CreateGameDialog::validateInput()
{
    pushButtonCreate->setEnabled(!lineEditGameName->text().isEmpty() &&
                                 !lineEditPlayerName->text().isEmpty());
}

void CreateGameDialog::on_pushButtonCreate_clicked()
{
    CreateGameData createGameData;
    createGameData.name                 = lineEditGameName->text();
    createGameData.description          = lineEditGameDescription->text();
    createGameData.minPlayers           = spinBoxMinPlayers->value();
    createGameData.maxPlayers           = spinBoxMaxPlayers->value();
    createGameData.maxSpectators        = spinBoxMaxSpectators->value();
    createGameData.AIPlayers            = spinBoxAIPlayers->value();
    createGameData.playerPassword       = lineEditGamePasswordPlayers->text();
    createGameData.spectatorPassword    = lineEditGamePasswordSpectators->text();
    createGameData.flagShufflePlayers   = radioButtonOrderRandom->isChecked();

    CreatePlayerData createPlayerData;
    createPlayerData.name               = lineEditPlayerName->text();
    createPlayerData.password           = lineEditPlayerPassword->text();
    createPlayerData.avatar             = selectPlayerIconWidget->image();

    saveConfigValues(createGameData);

    emit createGame(createGameData, createPlayerData);
    close();
}


void CreateGameDialog::loadConfigValues()
{
    Config& cfg = Config::instance();
    cfg.refresh();
    QString grp("create-game-dialog");
    lineEditGameName->setText(cfg.readString(grp, "name"));
    lineEditGameDescription->setText(cfg.readString(grp, "description"));
    spinBoxMinPlayers->setValue(cfg.readInt(grp, "min-players"));
    spinBoxMaxPlayers->setValue(cfg.readInt(grp, "max-players"));
    spinBoxMaxSpectators->setValue(cfg.readInt(grp, "max-spectators"));
    spinBoxAIPlayers->setValue(cfg.readInt(grp, "ai-players"));
    int shufflePlayers = cfg.readInt(grp, "flag-shuffle-players");
    if (shufflePlayers)
        radioButtonOrderRandom->setChecked(1);
    else
        radioButtonOrderChronological->setChecked(1);
    lineEditPlayerName->setText(cfg.readString("player", "name"));
    lineEditPlayerPassword->setText(cfg.readString("player", "password"));
    selectPlayerIconWidget->setImageFileName(cfg.readString("player", "image"));
    validateInput();
}

void CreateGameDialog::saveConfigValues(const CreateGameData& game)
{
    Config& cfg = Config::instance();
    QString grp("create-game-dialog");
    cfg.writeString(grp, "name", game.name);
    cfg.writeString(grp, "description", game.description);
    cfg.writeInt(grp, "min-players", game.minPlayers);
    cfg.writeInt(grp, "max-players", game.maxPlayers);
    cfg.writeInt(grp, "max-spectators", game.maxSpectators);
    cfg.writeInt(grp, "ai-players", game.AIPlayers);
    cfg.writeInt(grp, "flag-shuffle-players", game.flagShufflePlayers);

    cfg.writeString("player", "name", lineEditPlayerName->text());
    cfg.writeString("player", "password", lineEditPlayerPassword->text());
    cfg.writeString("player", "image", selectPlayerIconWidget->imageFileName());

    cfg.store();
}

