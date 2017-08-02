#include <SFML/Graphics.hpp>
#include <iostream>
#include "databattle.h"

int main() {
	if (!ProgramTile::loadHeadTexture("progs.png"))
		return 1;
	if (!Databattle::loadEnvironmentTexture("indicators.png"))
		return 2;
	Databattle db(16);
	for (int i=0; i<64; i++)
	{
		db.addTile(true);
		db.addProg(NULL);
	}
	ProgramTile hackTile;
	ProgramHead hackHead;
	hackTile.setImgCoords(5, 0);
	hackTile.setSourceObj((void*) &hackHead);
	hackHead.setMove(3);
	db.setProgTile(0, 0, &hackTile);
	db.setTile(4, 2, false);
	db.setTile(3, 3, false);
	db.begin();
	return 0;
}
