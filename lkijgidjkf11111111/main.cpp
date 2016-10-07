#include "D2Pool.h"
#include "MyGame.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

int main() {
	MyGame game(1024, 768);
	
	game.Show();

	return D2Pool::RunPipeline(&game);
}