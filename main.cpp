#define SDL_MAIN_HANDLED

#include "Game.h"
#include<Windows.h>
#include <SDL3/SDL.h>
#include <SDL3\SDL_main.h>

int main(int argc, char* argv[]) {
	SetConsoleOutputCP(CP_UTF8); // コンソールの文字コードをUTF-8に設定
	SDL_SetMainReady();

	Game game; // Gameオブジェクト作成

	// ゲームを初期化
	if (!game.Init())
	{
		return 1; // 初期化失敗
	}

	game.Run(); // ゲームループ開始

	game.Clean(); // ゲーム終了時のクリーンアップ

	return 0;
}