#include "Game.h"
#include <SDL3/SDL_main.h> 
#include <SDL3/SDL_log.h>

int main(int argc, char* argv[]) {
    Game game; // Gameクラスのインスタンスを作成

    if (!game.Initialize()) { // ゲームの初期化
        //SDL_Log("ゲームの初期化に失敗しました。");
        return 1;
    }

    game.RunLoop(); // ゲームのメインループを実行

    game.Shutdown(); // ゲームの終了処理

    //SDL_Log("ゲームを正常に終了しました。");
    return 0;
}