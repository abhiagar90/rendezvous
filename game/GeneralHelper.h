#ifndef GENERAL_HELPER_H
#define GENERAL_HELPER_H

#include "Globals.h"
#include "OpenGlHelper.h"
#include "FilePaths.h"
#include "FilePaths_att.h"
#include "CustomVectorStruct.h"
#include "Constants.h"
#include "AStarClass.h"
#include "Objects.h"
#include "MapNGrid.h"
#include "ActionOnGrid.h"
#include "Validations.h"

void blockOpponentsArea() {
	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_INNER_GRID_COL; c <= END_INNER_GRID_COL; c++) {
			if (isOponentCellForTeam(Coordinate_grid(r, c))) {
				putCharToGrid(r, c, BG_BLOCKED, true);
			}
		}
	}
}

void loadTeamAttributes() {

	//TODO: the below attributes shall be coming from earlier screens
	myTeam.name = TEAM_ANGELS;
	myTeam.templeHealth = HEALTH_FULL_TEMPLE;

	currentPlayer.heroType = HERO_STUNNER;
	myTeam.players[1].heroType = HERO_BURSTER;

	//TODO: first player load attributes yours, plus others
	//TODO: first player may be the first player to join

	enemyTeam.templeHealth = HEALTH_FULL_TEMPLE;
	enemyTeam.name = TEAM_DEMONS;

	enemyTeam.players[0].heroType = HERO_DISABLER;
	enemyTeam.players[1].heroType = HERO_SLOWER;
	//TODO: load other attributes
	currentPlayer.isTimerItemGlobalRunning = false;
	currentPlayer.isTimerMagicSpellRunning = false;
}

void loadPlayerSpecificAttributes() {

	//1st player:playerId=0, 2nd player:playerId=1
	//TODO: change 'playerId - 1' to playerId everywhere & related like 'which-1'
	currentPlayer.astar = new AStarClass();
	currentPlayer.astar->firstInitAStar();

	currentPlayer.currentPowerMode = POWER_MODE_BASIC;
	currentPlayer.heroHealth = HEALTH_FULL_HERO;

	switch (currentPlayer.heroType) {
	//TODO : decide & change attributes - strength,speedAttack,speedMove etc
	case HERO_STUNNER:
		currentPlayer.strength = STRENGTH_H_STUNNER;
		currentPlayer.speedMove = SPEED_MOVE_H_STUNNER;
		currentPlayer.charType = H_STUNNER;
		break;

	case HERO_DISABLER:
		currentPlayer.strength = STRENGTH_H_DISABLER;
		currentPlayer.speedMove = SPEED_MOVE_H_DISABLER;
		currentPlayer.charType = H_DISABLER;
		break;

	case HERO_SLOWER:
		currentPlayer.strength = STRENGTH_H_SLOWER;
		currentPlayer.speedMove = SPEED_MOVE_H_SLOWER;
		currentPlayer.charType = H_SLOWER;
		break;

	case HERO_BURSTER:
		currentPlayer.strength = STRENGTH_H_BURSTER;
		currentPlayer.speedMove = SPEED_MOVE_H_BURSTER;
		currentPlayer.charType = H_BURSTER;
		break;
	}

	currentPlayer.location = Coordinate_grid(19, 1);

	int row = currentPlayer.location.row;
	int col = currentPlayer.location.col;
	putCharToGrid(row, col, currentPlayer.charType, true);

	blockOpponentsArea();
}

void printGrid() {
	cout << "Printing grid" << endl;
	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_OUTER_GRID_COL; c <= END_OUTER_GRID_COL; c++) {
			handleGridCharSwitch(Coordinate_grid(r, c), PRINT_GRID);
		}
		cout << endl;
	}
}

//call this from the render function periodically
void moveHeroMine(int type) {
	Node* nodeToMove = findLocToMove(myTeam.players[type - 1].location, type);
	if (nodeToMove == NULL) {
		if (myTeam.players[type - 1].toAttackTemple)
			decreaseEnemyTempleHealth();
		return; //nothing to move
	}
	Coordinate_grid celltoMove = Coordinate_grid(nodeToMove->row,
			nodeToMove->col + ATTRIBUTE_WIDTH);
	if (isItem(celltoMove)) {
		setItemCell(celltoMove);
		takeItem();
	}

	putCharToGrid(
			myTeam.players[type - 1].location.row,
			myTeam.players[type - 1].location.col,
			initialGridChar[myTeam.players[type - 1].location.row][myTeam.players[type
					- 1].location.col + ATTRIBUTE_WIDTH], true);

	myTeam.players[type - 1].location.row = nodeToMove->row;
	myTeam.players[type - 1].location.col = nodeToMove->col;
	putCharToGrid(myTeam.players[type - 1].location.row,
			myTeam.players[type - 1].location.col,
			myTeam.players[type - 1].charType, true);
}

void loadAttributeSpace() {

	putPngToLAttCell(Coordinate_grid(1, 1), texId_att_team_my, 2, 1);
	putPngToLAttCell(Coordinate_grid(17, 1), texId_att_timers, 2, 1);
	putPngToLAttCell(Coordinate_grid(6, 1), texId_att_health, 1, 1);
	putPngToLAttCell(Coordinate_grid(12, 1), texId_att_health, 1, 1);
	putPngToLAttCell(Coordinate_grid(15, 1), texId_att_health, 1, 1);

	putPngToRAttCell(Coordinate_grid(1, 1), texId_att_team_enemy, 2, 1);
	putPngToRAttCell(Coordinate_grid(15, 1), texId_att_wall, 2, 1);
	putPngToRAttCell(Coordinate_grid(6, 1), texId_att_health, 1, 1);
	putPngToRAttCell(Coordinate_grid(10, 1), texId_att_health, 1, 1);
	putPngToRAttCell(Coordinate_grid(13, 1), texId_att_health, 1, 1);

	switch (myTeam.name) {
	case TEAM_ANGELS:
		putPngToLAttCell(Coordinate_grid(2, 1), texId_att_team_Angels, 2, 1);
		putPngToRAttCell(Coordinate_grid(2, 1), texId_att_team_Demons, 2, 1);

		putBmpToLAttCell(Coordinate_grid(14, 1), texId_t_angels, 2, 1);
		putBmpToRAttCell(Coordinate_grid(12, 1), texId_t_demons, 2, 1);
		break;

	case TEAM_DEMONS:
		putPngToLAttCell(Coordinate_grid(2, 1), texId_att_team_Demons, 2, 1);
		putPngToRAttCell(Coordinate_grid(2, 1), texId_att_team_Angels, 2, 1);

		putBmpToLAttCell(Coordinate_grid(14, 1), texId_t_demons, 2, 1);
		putBmpToRAttCell(Coordinate_grid(14, 1), texId_t_angels, 2, 1);
		break;

	case TEAM_BOTH:
		cout << "---shall never occur---" << endl;
		break;
	}

	switch (currentPlayer.heroType) {
	case HERO_STUNNER:
		putBmpToLAttCell(Coordinate_grid(4, 1), texId_h_stunner, 1, 1);
		putPngToLAttCell(Coordinate_grid(5, 1), texId_att_h_stunner, 2, 1);
		break;

	case HERO_DISABLER:
		putBmpToLAttCell(Coordinate_grid(4, 1), texId_h_disabler, 1, 1);
		putPngToLAttCell(Coordinate_grid(5, 1), texId_att_h_disabler, 2, 1);
		break;

	case HERO_SLOWER:
		putBmpToLAttCell(Coordinate_grid(4, 1), texId_h_slower, 1, 1);
		putPngToLAttCell(Coordinate_grid(5, 1), texId_att_h_slower, 2, 1);
		break;

	case HERO_BURSTER:
		putBmpToLAttCell(Coordinate_grid(4, 1), texId_h_burster, 1, 1);
		putPngToLAttCell(Coordinate_grid(5, 1), texId_att_h_burster, 2, 1);
		break;
	}

	//TODO: may not be '1' below
	switch (myTeam.players[1].heroType) {
	case HERO_STUNNER:
		putBmpToLAttCell(Coordinate_grid(10, 1), texId_h_stunner, 1, 1);
		putPngToLAttCell(Coordinate_grid(11, 1), texId_att_h_stunner, 2, 1);
		break;

	case HERO_DISABLER:
		putBmpToLAttCell(Coordinate_grid(10, 1), texId_h_disabler, 1, 1);
		putPngToLAttCell(Coordinate_grid(11, 1), texId_att_h_disabler, 2, 1);
		break;

	case HERO_SLOWER:
		putBmpToLAttCell(Coordinate_grid(10, 1), texId_h_slower, 1, 1);
		putPngToLAttCell(Coordinate_grid(11, 1), texId_att_h_slower, 2, 1);
		break;

	case HERO_BURSTER:
		putBmpToLAttCell(Coordinate_grid(10, 1), texId_h_burster, 1, 1);
		putPngToLAttCell(Coordinate_grid(11, 1), texId_att_h_burster, 2, 1);
		break;
	}

	switch (enemyTeam.players[0].heroType) {
	case HERO_STUNNER:
		putBmpToRAttCell(Coordinate_grid(4, 1), texId_h_stunner, 1, 1);
		putPngToRAttCell(Coordinate_grid(5, 1), texId_att_h_stunner, 2, 1);
		break;

	case HERO_DISABLER:
		putBmpToRAttCell(Coordinate_grid(4, 1), texId_h_disabler, 1, 1);
		putPngToRAttCell(Coordinate_grid(5, 1), texId_att_h_disabler, 2, 1);
		break;

	case HERO_SLOWER:
		putBmpToRAttCell(Coordinate_grid(4, 1), texId_h_slower, 1, 1);
		putPngToRAttCell(Coordinate_grid(5, 1), texId_att_h_slower, 2, 1);
		break;

	case HERO_BURSTER:
		putBmpToRAttCell(Coordinate_grid(4, 1), texId_h_burster, 1, 1);
		putPngToRAttCell(Coordinate_grid(5, 1), texId_att_h_burster, 2, 1);
		break;
	}

	switch (enemyTeam.players[1].heroType) {
	case HERO_STUNNER:
		putBmpToRAttCell(Coordinate_grid(8, 1), texId_h_stunner, 1, 1);
		putPngToRAttCell(Coordinate_grid(9, 1), texId_att_h_stunner, 2, 1);
		break;

	case HERO_DISABLER:
		putBmpToRAttCell(Coordinate_grid(8, 1), texId_h_disabler, 1, 1);
		putPngToRAttCell(Coordinate_grid(9, 1), texId_att_h_disabler, 2, 1);
		break;

	case HERO_SLOWER:
		putBmpToRAttCell(Coordinate_grid(8, 1), texId_h_slower, 1, 1);
		putPngToRAttCell(Coordinate_grid(9, 1), texId_att_h_slower, 2, 1);
		break;

	case HERO_BURSTER:
		putBmpToRAttCell(Coordinate_grid(8, 1), texId_h_burster, 1, 1);
		putPngToRAttCell(Coordinate_grid(9, 1), texId_att_h_burster, 2, 1);
		break;
	}

	switch (currentPlayer.currentPowerMode) {
	case POWER_MODE_BASIC:
		putPngToLAttCell(Coordinate_grid(7, 1), texId_att_mBasic, 2, 1);
		break;
	case POWER_MODE_MAGIC:
		putPngToLAttCell(Coordinate_grid(7, 1), texId_att_mMagic, 2, 1);
		break;
	}

	if (currentPlayer.isTimerItemGlobalRunning) {
		putPngToLAttCell(Coordinate_grid(18, 1), texId_att_time_itemOn, 2, 1);
	} else {
		putPngToLAttCell(Coordinate_grid(18, 1), texId_att_time_itemOff, 2, 1);
	}

	if (currentPlayer.isTimerMagicSpellRunning) {
		putPngToLAttCell(Coordinate_grid(19, 1), texId_att_time_magicOn, 2, 1);
	} else {
		putPngToLAttCell(Coordinate_grid(19, 1), texId_att_time_magicOff, 2, 1);
	}
}

void renderGrid() {
	moveHeroMine(1);
	moveHeroMine(2);

	loadAttributeSpace();

	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_OUTER_GRID_COL; c <= END_OUTER_GRID_COL; c++) {
			handleGridCharSwitch(Coordinate_grid(r, c), RENDER_GRID);
		}
	}
}

void togglePlayer() {
	if (playerId == 1)
		playerId = 2;
	else
		playerId = 1;
}
#endif
