#ifndef TIMER_H
#define TIMER_H

#define TIMER_ITEM_GLOBAL 10000
#define TIMER_MAGIC_SPELL 36000
#define TIMER_CURSE 12000
#define TIMER_HERO_REBORN 10000

#define TIMER_HOST_WAIT 30000
#define TIMER_PAGE_CREATING_GAME 5000

#define REFRESH_RATE 100

void timerRefresh(int value) {
	glutPostRedisplay();
	glutTimerFunc(REFRESH_RATE, timerRefresh, 0);
}

void timerItemGlobal(int whichPlayer) {
	if (!players[whichPlayer].isTimerItemGlobalRunning) { //timer not running
		cout << "starting timerItemGlobal for player: " << whichPlayer << endl;
		players[whichPlayer].isTimerItemGlobalRunning = true;
		glutTimerFunc(TIMER_ITEM_GLOBAL, timerItemGlobal, whichPlayer);
	}

	else { //timer not running
		cout << "stopping timerItemGlobal for player: " << whichPlayer << endl;
		players[whichPlayer].isTimerItemGlobalRunning = false;
	}
}

void timerHeroBorn(int whichPlayer) {
	if (!players[whichPlayer].isHeroRebornTimer) {
		cout << "starting timerHeroBorn for player: " << whichPlayer << endl;
		players[whichPlayer].isHeroRebornTimer = true;
		glutTimerFunc(TIMER_HERO_REBORN, timerHeroBorn, whichPlayer);
	} else {
		cout << "stopping timerHeroBorn for player: " << whichPlayer << endl;
		players[whichPlayer].isHeroRebornTimer = false;
	}
}

void timerMagicSpell(int whichPlayer) {
	if (!players[whichPlayer].isTimerMagicSpellRunning) {
		cout << "starting timerMagicSpell for player: " << whichPlayer << endl;
		players[whichPlayer].isTimerMagicSpellRunning = true;
		players[whichPlayer].currPowerMode = POWER_MODE_BASIC;
		glutTimerFunc(TIMER_MAGIC_SPELL, timerMagicSpell, whichPlayer);
	} else {
		cout << "stopping timerMagicSpell for player: " << whichPlayer << endl;
		players[whichPlayer].isTimerMagicSpellRunning = false;
	}
}

void timerCurse(int whichPlayer) {

	if (!players[whichPlayer].isTimerCurseRunning) {
		cout << "starting timerCurse for player " << whichPlayer << endl;
		switch (players[whichPlayer].curseType) {
		case CURSE_STUN:
			players[whichPlayer].speedMoveTemp = players[whichPlayer].speedMove;
			players[whichPlayer].speedMove = SPEED_MIN;
			players[whichPlayer].currPowerMode = POWER_MODE_STUN;
			break;
		case CURSE_DISABLE:
			players[whichPlayer].currPowerMode = POWER_MODE_BASIC;
			//Nothing to be done over here
			break;
		case CURSE_WEAK: //decrease strength
			players[whichPlayer].strength -= CURSE_AMT_SLOW_STRENGTH;
			break;
		case CURSE_BURST:
			//Nothing to be done over here
			break;
		case CURSE_NONE:
			cout << "timerCurse should not reach here" << endl;
			break;
		}

		players[whichPlayer].isTimerCurseRunning = true;
		glutTimerFunc(TIMER_CURSE, timerCurse, whichPlayer);
	}

	else {
		cout << "stopping timerCurse for player " << whichPlayer << endl;
		players[whichPlayer].isTimerCurseRunning = false;

		switch (players[whichPlayer].curseType) {
		case CURSE_STUN:
			players[whichPlayer].currPowerMode = POWER_MODE_BASIC;
			players[whichPlayer].speedMove
					+= players[whichPlayer].speedMoveTemp;
			if (players[whichPlayer].speedMove > SPEED_MAX) {
				players[whichPlayer].speedMove = SPEED_MAX;
			}
			break;
		case CURSE_DISABLE:
			//Nothing to be done over here
			break;
		case CURSE_WEAK:
			players[whichPlayer].strength += CURSE_AMT_SLOW_STRENGTH;
			break;
		case CURSE_BURST:
			//Nothing to be done over here
			break;
		case CURSE_NONE:
			cout << "timerCurse should not reach here" << endl;
			break;

		}

		players[whichPlayer].curseType = CURSE_NONE;
	}
}

bool isstartedHostWait = false;
void timerHostWait(int value) {
	if (!isstartedHostWait) {
		cout << "started timerHostWait" << endl;
		isstartedHostWait = true;
		glutTimerFunc(TIMER_HOST_WAIT, timerHostWait, 0);
	} else {
		cout << "stopping timerHostWait" << endl;
		gameDetails.isTimerNotHostWaiting = true;
	}
}

bool isstartedPageCreating = false;
void timerPageCreatingGame(int value) {
	if (!isstartedPageCreating) {
		cout << "starting timerPageCreatingGame" << endl;
		isstartedPageCreating = true;
		glutTimerFunc(TIMER_PAGE_CREATING_GAME, timerPageCreatingGame, 0);
	}

	else {
		cout << "stopping timerPageCreatingGame" << endl;
		gameDetails.isDoneWithJoining = true;
	}
}

#endif
