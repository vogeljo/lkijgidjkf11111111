#include "D2Pool.h"
#include "MyGame.h"
#include "Util.h"

int main() {
	MyGame *game = new MyGame(Util::GetScreenWidth(), Util::GetScreenHeight());
	
	game->SetFullscreen();
	game->Show();

	auto ret = D2Pool::RunPipeline(game);

	delete game;
	return ret;
}