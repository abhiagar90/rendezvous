#ifndef MAP_N_GRID_H
#define MAP_N_GRID_H

#include "Constants.h"
#include "Objects.h"
#include "FilePaths.h"
#include "FilePaths_att.h"
#include "Globals.h"
#include "Validations.h"
#include "Node.h"

#include "text3d.h"

void putAttributeSpace();
void putGrass();
void putSpawnLocation();
void initMap();
GLfloat getXFromCell(int col);
GLfloat getYFromCell(int row);
void putCharToGrid(int row, int col, charCellType charType, bool isInner);
void putMultipleCharToGrid(int row, int col, charCellType charType,
		charCellType backChar, int xBlocks, int yBlocks, bool isInner);
void renderGrid();
void copyInit();
void setHeroLocation(int which, Coordinate_grid loc);

void moveHeroMine(int type);
Node* findLocToMove(Coordinate_grid curr);
bool isBlockedSite(int r, int c);

//TODO: test code can be removed whenever wanted
void putHeros() {
	myTeam.players[0].astar = new AStarClass();
	myTeam.players[0].astar->firstInitAStar();
	//TODO: Abhisek : player's location, move to myTeam.players[currentPlayer - 1]?
	Coordinate_grid location;
	location.row = 19;
	location.col = 1;
	setHeroLocation(1, location);

	putCharToGrid(19, 1, H_SLOWER, true);

	myTeam.players[1].astar = new AStarClass();
	myTeam.players[1].astar->firstInitAStar();

	Coordinate_grid location2;
	location2.row = 20;
	location2.col = 3;
	setHeroLocation(2, location2);
	//assuming the type is H_SLOWER in moveHero also
	myTeam.players[1].strength = STRENGTH_H_SLOWER;
	putCharToGrid(20, 3, H_SLOWER, true);
}

void setHeroLocation(int which, Coordinate_grid loc) {
	myTeam.players[which - 1].location.row = loc.row;
	myTeam.players[which - 1].location.col = loc.col;
}

void putAttributeSpace() {
	//left attribute space
	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_LEFT_ATTRIBUTE_COL; c <= END_LEFT_ATTRIBUTE_COL; c++) {
			putCharToGrid(r, c, BG_ATTRIBUTE, false);
		}
	}
	//right attribute space
	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_RIGHT_ATTRIBUTE_COL; c <= END_RIGHT_ATTRIBUTE_COL; c++) {
			putCharToGrid(r, c, BG_ATTRIBUTE, false);
		}
	}
}

void putGrass() {
	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_INNER_GRID_COL; c <= END_INNER_GRID_COL; c++) {
			putCharToGrid(r, c, BG_GRASS, true);
		}
	}
}

void putSpawnLocation() {
	int k = 1;
	for (int i = END_GRID_ROW - SPAWN_BLOCKS + 1; i <= END_GRID_ROW; i++) {
		for (int j = 1; j <= k; j++) {
			putCharToGrid(i, j, BG_SPAWN, true);
			putCharToGrid(j, i, BG_SPAWN, true);
		}
		k++;
	}
}

void putWarGround() {
	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_INNER_GRID_COL; c <= END_INNER_GRID_COL; c++) {
			if (r == c) {
				for (int k = DIAG_BLOCKS; k >= 0; k--) {
					putCharToGrid(r + k, c, BG_WAR, true);
					putCharToGrid(r, c + k, BG_WAR, true);
				}
			}
		}
	}
}

void putTemple() {
	putMultipleCharToGrid(5, 4, TEMPLE_ANGELS, T_ANGELS_BACK, TEMPLE_BLOCKS,
			TEMPLE_BLOCKS, true);
	putMultipleCharToGrid(17, 16, TEMPLE_DEMONS, T_DEMONS_BACK, TEMPLE_BLOCKS,
			TEMPLE_BLOCKS, true);
}

void putToGridFromFile(string filePath, charCellType charCellType,
		bool isTeamArea, bool isInner) {
	ifstream infile(filePath.c_str());
	string line;
	while (getline(infile, line)) {
		istringstream iss(line);
		int a, b;
		if (!(iss >> a >> b)) {
			break;
		} // error

		putCharToGrid(a, b, charCellType, isInner);
		if (isTeamArea) {
			putCharToGrid(b, a, charCellType, isInner);
		}
	}
}

void putStonesInTeamArea() {
	putToGridFromFile(PATH_LOC_STONES_TEAM, STONE, true, true);
}

void putTreesInTeamArea() {
	putToGridFromFile(PATH_LOC_TREES_TEAM, TREE, true, true);
}

void putStonesInCommonArea() {
	putToGridFromFile(PATH_LOC_STONES_COMMON, STONE, false, true);
}

void putTreesInCommonArea() {
	putToGridFromFile(PATH_LOC_TREES_COMMON, TREE, false, true);
}

void putStonesNTrees() {
	putStonesInTeamArea();
	putTreesInTeamArea();
	putStonesInCommonArea();
	putTreesInCommonArea();
}

void initMap() {
	//Maintain the sequence
	putGrass();
	putAttributeSpace();
	putSpawnLocation();
	putWarGround();
	putTemple();

	putStonesNTrees();

	copyInit(); //Disclaimer: to be called before item put & hero put!!!
}

//copies the state of the initial map to another array
//not modifiable now, as discussed
void copyInit() {
	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_OUTER_GRID_COL; c <= END_OUTER_GRID_COL; c++) {
			initialGridChar[r][c] = gridChar[r][c];
		}
	}

}

Node* findLocToMove(Coordinate_grid curr, int which) {
	AStarClass* astarForPlayer = myTeam.players[which - 1].astar;
	Node* neighbours[4];
	neighbours[0] = astarForPlayer->getNodeFromGrid(curr.row + 1, curr.col);
	neighbours[1] = astarForPlayer->getNodeFromGrid(curr.row - 1, curr.col);
	neighbours[2] = astarForPlayer->getNodeFromGrid(curr.row, curr.col + 1);
	neighbours[3] = astarForPlayer->getNodeFromGrid(curr.row, curr.col - 1);

	Node* toReturn = NULL;

	if (neighbours[0] != NULL && neighbours[0]->onPath) {
		toReturn = neighbours[0];
	} else if (neighbours[1] != NULL && neighbours[1]->onPath) {
		toReturn = neighbours[1];
	} else if (neighbours[2] != NULL && neighbours[2]->onPath) {
		toReturn = neighbours[2];
	} else if (neighbours[3] != NULL && neighbours[3]->onPath) {
		toReturn = neighbours[3];
	}

	if (toReturn != NULL) {
		(astarForPlayer->getNodeFromGrid(curr.row, curr.col))->onPath = false;//So that it doesn't loop back
	}

	return toReturn;
}

Coordinate_openGl getOpenGlCoordinatesFromGrid(Coordinate_grid grid) {
	GLfloat x = MIN_XCELL + (grid.col - 1) * CELL_LENGTH;
	GLfloat y = MAX_YCELL - (grid.row * CELL_LENGTH);
	return Coordinate_openGl(x, y);
}

Coordinate_grid getGridCoordinatesFromOpenGl(Coordinate_openGl openGl) {
	int row = (MAX_YCELL - openGl.y) / CELL_LENGTH + 1;
	int col = ((openGl.x - MIN_XCELL) / CELL_LENGTH) + 1;
	return Coordinate_grid(row, col);
}

charCellType getInnerGridChar(int randomRow, int randomCol) {
	return gridChar[randomRow][randomCol + ATTRIBUTE_WIDTH];
}

float t3dComputeScale(const char* str) {
	float width = t3dDrawWidth(str);
	return CELL_LENGTH / width;
}

void putTextToCell(Coordinate_grid grid, char* string) {
	Coordinate_openGl openGl = getOpenGlCoordinatesFromGrid(grid);
	float t3dScale = t3dComputeScale("qqqqq");
	glPushMatrix();
	glTranslatef(openGl.x, openGl.y, -5.0f);
	//glTranslatef(0, 0, -4.0f);
	glScalef(t3dScale, t3dScale, t3dScale);
	glColor3f(1.0f, 0.0f, 0.0f);
	//glTranslatef(0, 0, 1.5f / _scale);
	t3dDraw3D(string, -1, 0, 0.2f);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
}

void putImageToCell(Coordinate_grid grid, GLuint _textureId, int xBlocks,
		int yBlocks) {
	Coordinate_openGl openGl = getOpenGlCoordinatesFromGrid(grid);

	GLfloat xSize = xBlocks * CELL_LENGTH;
	GLfloat ySize = yBlocks * CELL_LENGTH;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(openGl.x, openGl.y, -5.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(openGl.x, openGl.y + ySize, -5.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(openGl.x + xSize, openGl.y + ySize, -5.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(openGl.x + xSize, openGl.y, -5.0f);

	glEnd();
}

void putImageToLeftAttCell(Coordinate_grid grid, GLuint _textureId,
		int xBlocks, int yBlocks) {
	//grid.col should be between 1 & ATTRIBUTE_WIDTH
	putImageToCell(grid, _textureId, xBlocks, yBlocks);
}

void putImageToRightAttCell(Coordinate_grid grid, GLuint _textureId,
		int xBlocks, int yBlocks) {
	//grid.col should be between 1 & ATTRIBUTE_WIDTH
	grid.col += ATTRIBUTE_WIDTH + END_INNER_GRID_COL - START_INNER_GRID_COL + 1;
	putImageToCell(grid, _textureId, xBlocks, yBlocks);
}

void putMultipleCharToGrid(int row, int col, charCellType charType,
		charCellType backChar, int xBlocks, int yBlocks, bool isInner) {

	//Adding 'back' characters - required in case of covering multiple cells
	for (int i = 0; i < xBlocks; i++) {
		for (int j = 0; j < yBlocks; j++) {
			putCharToGrid(row - i, col + j, backChar, isInner);
		}
	}

	//The first position(bottom-left) should be actual character
	putCharToGrid(row, col, charType, isInner);
}

//In case some new charis added to grid, remember to add the some in renderGrid
void putCharToGrid(int row, int col, charCellType charType, bool isInner) {
	if (!isValidRowNColIndex(row, col, isInner)) {
		return;
	}

	if (isInner) {
		row = row;
		col = col + ATTRIBUTE_WIDTH;
	}

	gridChar[row][col] = charType;
}

//TODO: Abhishek move to code to ActionOnGrid
//gray area for second player area handled by Harinder's code
bool isBlockedSite(int r, int c) {
	charCellType type = getInnerGridChar(r, c);
	switch (type) {
	case BG_GRASS:
	case BG_SPAWN:
	case BG_WAR:
	case I_SPEED_MOVE:
	case I_HEALTH:
	case I_DAMAGE:
	case I_TEMPLE_HEALER:
		return false;//open
	default:
		break;
	}
	return true; //close
}

#endif
