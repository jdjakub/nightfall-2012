#include "databattle.h"

sf::Texture ProgramTile::heads;
sf::Texture ProgramTile::tails;
const unsigned ProgramTile::pixelsPerGrid = 34;

bool ProgramTile::loadHeadTexture(const char* filename)
{
	return heads.LoadFromFile(filename);
}

bool ProgramTile::loadTailTexture(const char* filename)
{
	return tails.LoadFromFile(filename);
}

ProgramTile::ProgramTile()
	: imgCoords(0, 0), isTail(false), sourceObject(NULL)
{
}

void ProgramTile::setImgCoords(unsigned x, unsigned y)
{
	imgCoords.x = 32*x;
	imgCoords.y = 32*y;
}

void ProgramTile::setSourceObj(void* obj)
{
	sourceObject = obj;
}

ProgramHead::ProgramHead()
	: progID(0), move(0), maxSize(1), currSize(1)
{
}

ProgramHead& ProgramHead::setID(unsigned id)
{
	progID = id;
	return *this;
}

ProgramHead& ProgramHead::setMove(int mov)
{
	move = mov;
	return *this;
}

ProgramHead& ProgramHead::setMaxSize(int max)
{
	maxSize = max;
	return *this;
}

void ProgramHead::decrementMove()
{
	move--;
	if (move < 0) move = 0;
}

void ProgramHead::decreaseMove(unsigned num)
{
	move -= num;
	if (move < 0) move = 0;
}

unsigned ProgramHead::getMove()
{
	return move;
}

unsigned ProgramHead::getMaxSize()
{
	return maxSize;
}

unsigned ProgramHead::getCurrSize()
{
	return currSize;
}


ProgramTail::ProgramTail()
	: head(NULL), next(NULL), prev(NULL)
{
}

ProgramTail::ProgramTail(ProgramHead& prog)
	: head(&prog), next(NULL), prev(NULL)
{
}



sf::Texture Databattle::environ;

void Databattle::drawGrid()
{
	progSprite.SetSubRect(sf::IntRect(32, 0, 32, 32));

	for (int i=0; i<grid.size(); i++)
	{
		if (grid[i] == true)
		{
			int a = i % cells_h;
			float x = ProgramTile::pixelsPerGrid*a;
			float y = ProgramTile::pixelsPerGrid*(a == 0 ? i/cells_h : (i-a)/cells_h);
			progSprite.SetPosition(gridCoords.x+x, gridCoords.y+y);
			wnd.Draw(progSprite);
		}
	}
}

void Databattle::drawProgs()
{
	sf::Vector2i currCoords(0, 0);
	for (int i=0; i < progTiles.size(); i++)
	{
		ProgramTile* currProgram = progTiles[i];
		if (currProgram == NULL) continue;
		if (currProgram->imgCoords != currCoords)
		{
			currCoords = currProgram->imgCoords;
			progSprite.SetSubRect(sf::IntRect(currCoords.x, currCoords.y, 32, 32));
		}

		int a = i % cells_h;
		float x = ProgramTile::pixelsPerGrid*a;
		float y = ProgramTile::pixelsPerGrid*(a == 0 ? i/cells_h : (i-a)/cells_h);
		progSprite.SetPosition(gridCoords.x+x, gridCoords.y+y);
		wnd.Draw(progSprite);
	}
}

void Databattle::drawEnv()
{
	envSprite.SetSubRect(sf::IntRect(0, 0, 32, 32));
	envSprite.SetPosition(selected);
	wnd.Draw(envSprite);

	if (indicator_rng != -1)
	{
		envSprite.SetSubRect(sf::IntRect(32, 0, 32, 32));
		int pixelRange = ProgramTile::pixelsPerGrid*indicator_rng;
		sf::Vector2f offset(selected.x - pixelRange, selected.y - pixelRange);
		for (int i=0; i<indicators.size(); i++)
		{
			if (indicators[i] == true)
			{
				int a = i % indicators_h;
				float x = ProgramTile::pixelsPerGrid*a;
				float y = ProgramTile::pixelsPerGrid*(a == 0 ? i/indicators_h : (i-a)/indicators_h);
				envSprite.SetPosition(offset.x+x, offset.y+y);
				wnd.Draw(envSprite);
			}
		}
	}
}

void Databattle::drawFX()
{
	return;
}

void Databattle::draw()
{
	/*RULES:
	1. Drawing mini-finctions don't check whether they should / can be applied or not.
	2. They are all responsible for setting the appropriate sprites and subrects.
	*/

	/*Draw grid*/
	drawGrid();

	/*Draw programs on top*/
	drawProgs();

	/*Finally, draw environment / indicators on top*/
	if (selected.x != -1 && selected.y != -1)
		drawEnv();
}


bool Databattle::checkBounds(int cellX, int cellY)
{
	if (cellX < 0 || cellY < 0)
		return false;
	if (cellX > cells_h-1)
		return false;
	if (cellsToIndex(cellX, cellY) > grid.size()-1)
		return false;
	return true;
}

bool Databattle::loadEnvironmentTexture(const char* filename)
{
	return environ.LoadFromFile(filename);
}

int Databattle::cellsToIndex(unsigned cellX, unsigned cellY, unsigned gridWidth)
{
	return cellY*gridWidth+cellX;
}

int Databattle::cellsToIndex(unsigned cellX, unsigned cellY)
{
	return cellsToIndex(cellX, cellY, cells_h);
}

sf::Vector2i Databattle::indexToCells(unsigned index, unsigned gridWidth)
{
	unsigned x = (index % gridWidth);
	unsigned y = (x == 0 ? index/gridWidth : (index-x)/gridWidth);

	return sf::Vector2i(x, y);
}

sf::Vector2i Databattle::indexToCells(unsigned index)
{
	return indexToCells(index, cells_h);
}

sf::Vector2i Databattle::mouseToGrid(int mouseX, int mouseY)
{
	int x_index = 0;
	int y_index = 0;
	int normX = (mouseX - (int) gridCoords.x);
	int normY = (mouseY - (int) gridCoords.y);
	int remainderX = normX % ProgramTile::pixelsPerGrid;
	int remainderY = normY % ProgramTile::pixelsPerGrid;
	if (remainderX == 0)
		x_index = normX / ProgramTile::pixelsPerGrid;
	else
		x_index = (normX - remainderX) / ProgramTile::pixelsPerGrid;

	if (remainderY == 0)
		y_index = normY / ProgramTile::pixelsPerGrid;
	else
		y_index = (normY - remainderY) / ProgramTile::pixelsPerGrid;

	return sf::Vector2i(x_index, y_index);
}

void Databattle::makeRange(unsigned range)
{
	indicators_h = range*2 + 1;
	indicator_rng = range;
	/*Clear*/
	indicators.resize(0);
	indicators.resize(indicators_h*indicators_h);
	/*Top half + middle*/
	for (unsigned row=0; row<range+1; row++)
	{
			unsigned num =2*row + 1;
			unsigned offset = range - row;
			for (unsigned i=0; i<num; i++)
				indicators[row*indicators_h+offset+i] = true;
	}
	
	/*Bottom half*/
	for (unsigned row=0; row<range; row++)
	{
		unsigned num =2*row + 1;
		unsigned offset = range - row;
		for (unsigned i=0; i<num; i++)
			indicators[(indicators_h-1-row)*indicators_h+offset+i] = true;
	}
}

void Databattle::correctRange()
{
	/*Set centre cell to not be drawn*/
	unsigned centreIndex = cellsToIndex(indicator_rng, indicator_rng, indicators_h);
	indicators[centreIndex] = false;

	sf::Vector2i selectedGridPos = uncorrectGridCoord(selected);
	int dx = selectedGridPos.x - indicator_rng;
	int dy = selectedGridPos.y - indicator_rng;

	for (int row=0; row<indicators_h; row++)
	{
		//std::cout << "Row " << row << std::endl;
		for (int col=0; col<indicators_h; col++)
		{
			//std::cout << "Column" << col << std::endl;
			int indIndex = cellsToIndex(col, row, indicators_h);
			/*Don't waste time checking unmarked cells*/
			if (indicators[indIndex] == true)
			{
				/*Determine  where it corresponds to in the grid vector*/
				int gridIndex = cellsToIndex(col+dx, row+dy);
				if (!checkBounds(col+dx, row+dy))
				{
					indicators[indIndex] = false;
					continue;
				}
				else
				{
					indicators[indIndex] = grid[gridIndex];
				}
			}
		}
	}
	return;
}

sf::Vector2f Databattle::correctGridCoord(int cellX, int cellY)
{
	return sf::Vector2f(ProgramTile::pixelsPerGrid*cellX + gridCoords.x, ProgramTile::pixelsPerGrid*cellY + gridCoords.y);
}

sf::Vector2i Databattle::uncorrectGridCoord(sf::Vector2f coord)
{
	return sf::Vector2i((coord.x - gridCoords.x)/ProgramTile::pixelsPerGrid, (coord.y - gridCoords.y)/ProgramTile::pixelsPerGrid);
}


Databattle::Databattle(unsigned cells_hor)
	: progSprite(ProgramTile::heads), envSprite(environ), indicator_rng(-1), selected(-1.0f, -1.0f), cells_h(cells_hor),
	indicators_h(0), gridCoords(64.0f, 64.0f), wnd(sf::VideoMode(800, 600, 32), "Databattle"), grid(), progTiles(), indicators()
{
}

void Databattle::addTile(bool tile)
{
	grid.push_back(tile);
}

void Databattle::addProg(ProgramTile* prog)
{
	progTiles.push_back(prog);
}

void Databattle::begin()
{
	wnd.SetFramerateLimit(60);
	while (wnd.IsOpened())
	{
		sf::Event evt;
		while (wnd.PollEvent(evt))
		{
			switch (evt.Type)
			{
			case sf::Event::Closed:
				wnd.Close();
				break;
			case sf::Event::MouseButtonPressed:
				if (evt.MouseButton.Button == sf::Mouse::Left)
				{
					sf::Vector2i pos = mouseToGrid(evt.MouseButton.X, evt.MouseButton.Y);
					
					ProgramTile* prog = getProg(pos.x, pos.y);
					if (prog != NULL)
					{
						if (!prog->isTail)
						{
							selected = correctGridCoord(pos.x, pos.y);
							unsigned rng = ((ProgramHead*) getProg(pos.x, pos.y)->sourceObject)->move;
							makeRange(rng);
							correctRange();
							break;
						}
					}
					else
					{
						if (selected.x != -1 && selected.y != -1)
						{
							if (checkSolidAndEmpty(pos.x, pos.y))
							{
								sf::Vector2i gridPos = uncorrectGridCoord(selected);
								int index = cellsToIndex(gridPos.x, gridPos.y);
								ProgramHead* selectedProg = ((ProgramHead*) progTiles[index]->sourceObject);
								if (selectedProg->move > 0)
								{
									setProgramPos(&progTiles[index], pos.x, pos.y);
									selected = correctGridCoord(pos.x, pos.y);
									unsigned rng = selectedProg->move;
									if (rng != 0)
									{
										selectedProg->decrementMove();
										makeRange(rng);
										correctRange();
									}
									else
										indicator_rng = -1;
									break;
								}
							}
						}
					}
					
					selected.x = -1;
					selected.y = -1;
					indicator_rng = -1;
					break;
				}
			default:
				break;
			}
		}

		wnd.Clear(sf::Color(0, 0, 0, 255));
		draw();
		wnd.Display();
	}
}

ProgramTile* Databattle::getProg(unsigned cellX, unsigned cellY)
{
	if (!checkBounds(cellX, cellY))
		return NULL;

	return progTiles[cellsToIndex(cellX, cellY)];
}

bool Databattle::checkSolidAndEmpty(unsigned cellX, unsigned cellY)
{
	if (!checkBounds(cellX, cellY))
		return false;

	unsigned n = cellsToIndex(cellX, cellY);

	if (progTiles[n] == NULL && grid[n] == true)
		return true;

	return false;
}

bool Databattle::setProgramPos(ProgramTile** prog, unsigned cellX, unsigned cellY)
{
	//Make sure bounds are sensible (KEY WORD: VALIDATION!!1111)
	if (!checkBounds(cellX, cellY))
		return false;

	//Assign value of prog pointer to target cell, and assign NULL to the source cell
	progTiles[cellsToIndex(cellX, cellY)] = *prog;
	*prog = NULL;
	return true;
}

bool Databattle::setTile(unsigned cellX, unsigned cellY, bool value)
{
	if (!checkBounds(cellX, cellY))
		return false;

	grid[cellsToIndex(cellX, cellY)] = value;
	return true;
}

bool Databattle::setProgTile(unsigned cellX, unsigned cellY, ProgramTile* prog)
{
	if (!checkBounds(cellX, cellY))
		return false;

	progTiles[cellsToIndex(cellX, cellY)] = prog;
	return true;
}