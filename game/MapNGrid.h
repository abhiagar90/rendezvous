#include "Globals.h"
#include "Validations.h"
#include "HandleGridCharSwitch.h"

void putAttributeSpace();
void putGrass();
void putSpawnLocation();
void initMap();
GLfloat getXFromCell(int col);
GLfloat getYFromCell(int row);
void putCharToGrid(int row, int col, charCellType charType, bool isInner);
void putMultipleCharToGrid(int row, int col, charCellType charType,
		charCellType backChar, int blocks, bool isInner);
void putImageToCell(int row, int col, GLuint _textureId, int blocks = 1);
void putImageToGrid(GLfloat x, GLfloat y, GLuint _textureId, int blocks);
void renderGrid();
void copyInit();

//------------------start
enum switchCallType {
	PRINT_GRID, PROCESS_MOVE_CLICK, RENDER_GRID
};

void processCase(switchCallType callType, int r, int c, GLuint texId,
		string toPrint, void( f)(void)) {

	switch (callType) {
	case PRINT_GRID:
		cout << " " << toPrint;
		break;
	case PROCESS_MOVE_CLICK:
		break;
	case RENDER_GRID:
		putImageToCell(r, c, texId);
		break;
	}
}

void f() {

}

void handleGridCharSwitch(int r, int c, switchCallType callType) {

	switch (gridChar[r][c]) {
	case BG_GRASS:
		processCase(callType, r, c, grass_texId, "Gra", f);
		break;
	case BG_SPAWN:
		processCase(callType, r, c, spawn_texId, "BSp", f);
		break;
	case BG_WAR:
		processCase(callType, r, c, war_texId, "BWa", f);
		break;
	case BG_ATTRIBUTE:
		processCase(callType, r, c, attribute_bg_texId, "BAt", f);
		break;

	case STONE:
		processCase(callType, r, c, stone_texId, "Sto", f);
		break;
	case TREE:
		processCase(callType, r, c, tree_texId, "Tre", f);
		break;

	case TEMPLE_ANGELS:
		processCase(callType, r, c, t_angels_texId, "TAn", f);
		break;
	case TEMPLE_DEMONS:
		processCase(callType, r, c, t_demons_texId, "TDe", f);
		break;

	case H_DISABLER:
		processCase(callType, r, c, h_disabler_texId, "HDi", f);
		break;
	case H_SLOWER:
		processCase(callType, r, c, h_slower_texId, "HSl", f);
		break;
	case H_BURSTER:
		processCase(callType, r, c, h_burster_texId, "HBu", f);
		break;
	case H_STUNNER:
		processCase(callType, r, c, h_stunner_texId, "HSt", f);
		break;

	case I_SPEED_MOVE:
		processCase(callType, r, c, i_speedMov_texId, "ISM", f);
		break;
	case I_SPEED_ATTACK:
		processCase(callType, r, c, i_speedAttack_texId, "ISA", f);
		break;
	case I_HEALTH:
		processCase(callType, r, c, i_health_texId, "IHe", f);
		break;
	case I_DAMAGE:
		processCase(callType, r, c, i_damage_texId, "IDa", f);
		break;
	case I_TEMPLE_HEALER:
		processCase(callType, r, c, i_templeHealer_texId, "ITH", f);
		break;

	case TREE_BACK:
		processCase(callType, r, c, temp_texId, "TBa", f);
		break;
	case T_ANGELS_BACK:
		processCase(callType, r, c, temp_texId, "TAB", f);
		break;
	case T_DEMONS_BACK:
		processCase(callType, r, c, temp_texId, "TDB", f);
		break;

	default:
		cout << "should not happen - something's wrong";
	}
}
//----------------------------------------------------end
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
	int numOtherDiag = (DIAGONAL_BLOCKS - 1) / 2;

	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_INNER_GRID_COL; c <= END_INNER_GRID_COL; c++) {
			if (r == c) {
				for (int k = numOtherDiag; k >= 0; k--) {
					putCharToGrid(r + k, c, BG_WAR, true);
					putCharToGrid(r, c + k, BG_WAR, true);
				}
			}
		}
	}
}

void putTemple() {
	putMultipleCharToGrid(5, 4, TEMPLE_ANGELS, T_ANGELS_BACK, TEMPLE_BLOCKS,
			true);
	putMultipleCharToGrid(17, 16, TEMPLE_DEMONS, T_DEMONS_BACK, TEMPLE_BLOCKS,
			true);
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

//TODO - temp for motion of the H_SLOWER
void tempStunnerLocation() {
	static int lastRow = 20;
	static int lastCol = 1;
	//replacing with initial grid char
	putCharToGrid(lastRow, lastCol,
			initialGridChar[lastRow][lastCol + ATTRIBUTE_WIDTH], true);
	lastCol++;
	if (lastCol > 20)
		lastCol = 1;
	putCharToGrid(lastRow, lastCol, H_SLOWER, true);
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

void putMultipleCharToGrid(int row, int col, charCellType charType,
		charCellType backChar, int blocks, bool isInner) {

	//Adding 'back' characters - required in case of covering multiple cells
	for (int i = 0; i < blocks; i++) {
		for (int j = 0; j < blocks; j++) {
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

void putImageToCell(int row, int col, GLuint _textureId, int blocks) {
	Coordinate_grid grid = Coordinate_grid(row, col);
	Coordinate_openGl openGl = getOpenGlCoordinatesFromGrid(grid);
	putImageToGrid(openGl.x, openGl.y, _textureId, blocks);
}

void putImageToGrid(GLfloat x, GLfloat y, GLuint _textureId, int blocks) {
	GLfloat size1D = blocks * CELL_LENGTH;
	glEnable( GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin( GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, -5.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y + size1D, -5.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + size1D, y + size1D, -5.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + size1D, y, -5.0f);

	glEnd();
}

void renderGrid() {
	//TODO - remove this method call from here and put in a thread or something
	tempStunnerLocation();

	for (int r = START_GRID_ROW; r <= END_GRID_ROW; r++) {
		for (int c = START_OUTER_GRID_COL; c <= END_OUTER_GRID_COL; c++) {
			handleGridCharSwitch(r, c, RENDER_GRID);
		}
	}
}

