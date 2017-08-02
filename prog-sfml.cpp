#include "prog-sfml.h"

/*
int InitImgs(bool setPos) {
	if (!progTiles.LoadFromFile("progs.png")) return ERR_LOAD_TILES;
	for (int i=0; i<NUM_OF_PIDS; i++) {
		int tlx , tly;
		tlx = (i%8)*32;
		tly = (i/8)*32;
		if (tlx > progTiles.GetWidth()-32 || tly > progTiles.GetHeight()-32) return ERR_DIM_OUT_OF_BOUNDS;
		sprs[i].SetTexture(progTiles);
		sprs[i].SetSubRect(sf::IntRect(tlx, tly, 32, 32));
		if (setPos) {
			sprs[i].SetPosition((float)tlx, (float) tly);
		}
	}
	return NO_ERR;
}*/
