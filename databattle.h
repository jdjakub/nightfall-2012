#ifndef __DATABATTLE_SFML_H__
#define __DATABATTLE_SFML_H__
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <list>
#include <iostream>
/*#define MAX_GRID_W 16
#define MAX_GRID_H 16
#define OFFSET(x, y) ((y & 0xF) + ((x & 0xF)*0x100))
#define LOBYTE(a) (a & 0xF)
#define HIBYTE(a) ((a & 0xF0) >> 8)

void _setBit(BYTE& b, short index, bool setToWhat) {
	if (index > 7) return;
	int n = pow(2, index);
	if (setToWhat) b |= n;
	else b &= 0xFF-n;
}

bool _isBit(BYTE b, short index, bool what) {
	if (index > 7) return;
	b &= pow(2, index);
	return !(b == 0);
}

BYTE currentGrid[(MAX_GRID_W/8)*MAX_GRID_H] = {(BYTE) 255};

bool LoadGrid(HEADERINFO* hi, TILEINFO* ti, int numOfTiles) {
	int rowOffset = hi->yoff;
	for (int i=0; i<numOfTiles; i++) {
		if (
		if (ti[i].progID == PID_NONE) {
			setBit(currentGrid[(i/8)+(int) rowOffset*(MAX_GRID_WIDTH/8)], i%8, 0);
		}
	}

}*/

class ProgramTile;
class ProgramHead;
class ProgramTail;
class Databattle;

class ProgramTile
{
	friend class Databattle;
	static const unsigned pixelsPerGrid;
protected:
	static sf::Texture heads;
	static sf::Texture tails;
	sf::Vector2i imgCoords;
	bool isTail;
	void* sourceObject;
public:
	static bool loadHeadTexture(const char* filename);
	static bool loadTailTexture(const char* filename);
	ProgramTile();
	void setImgCoords(unsigned x, unsigned y);
	void setSourceObj(void* obj);
};

class ProgramHead
{
	friend class Databattle;
protected:
	unsigned progID;
	int move;
	int maxSize;
	int currSize;
	ProgramTail* tail;
public:
	ProgramHead();
	ProgramHead& setID(unsigned id);
	ProgramHead& setMove(int mov);
	ProgramHead& setMaxSize(int max);
	void decrementMove();
	void decreaseMove(unsigned num);
	unsigned getID();
	unsigned getMove();
	unsigned getMaxSize();
	unsigned getCurrSize();
};

class ProgramTail
{
	friend class ProgramHead;
protected:
	ProgramHead* head;
	ProgramTail* next; //towards head
	ProgramTail* prev; //away from head
public:
	ProgramTail();
	ProgramTail(ProgramHead& prog);
};

class Databattle
{
protected:
	static sf::Texture environ;

	sf::Sprite progSprite;
	sf::Sprite envSprite;
	sf::Vector2f gridCoords;
	sf::RenderWindow wnd;
	std::vector<bool> grid;
	std::vector<ProgramTile*> progTiles;
	std::vector<bool> indicators;

	sf::Vector2f selected;
	unsigned cells_h;
	int indicator_rng;
	unsigned indicators_h;

	void drawGrid();
	void drawProgs();
	void drawEnv();
	void drawFX();

	void draw();
	bool checkBounds(int cellX, int cellY);
	int cellsToIndex(unsigned cellX, unsigned cellY);
	int cellsToIndex(unsigned cellX, unsigned cellY, unsigned gridWidth);
	sf::Vector2i indexToCells(unsigned index);
	sf::Vector2i indexToCells(unsigned index, unsigned gridWidth);
	
	sf::Vector2i mouseToGrid(int mouseX, int mouseY);
	void makeRange(unsigned range);
	void correctRange();
	sf::Vector2f correctGridCoord(int cellX, int cellY);
	sf::Vector2i uncorrectGridCoord(sf::Vector2f coords);
public:
	static bool loadEnvironmentTexture(const char* filename);

	Databattle(unsigned cells_hor);
	void begin();

	void addTile(bool tile);
	void addProg(ProgramTile* prog);
	
	ProgramTile* getProg(unsigned cellX, unsigned cellY);
	bool checkSolidAndEmpty(unsigned cellX, unsigned cellY);
	bool setProgramPos(ProgramTile** prog, unsigned cellX, unsigned cellY);

	bool setTile(unsigned cellX, unsigned cellY, bool value);
	bool setProgTile(unsigned cellX, unsigned cellY, ProgramTile* prog);
};

#endif