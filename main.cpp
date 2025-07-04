#include <SDL3/SDL.h> // SDLライブラリをインクルード

// メイン関数
int main(int argc, char* argv[]) {
    // SDLの初期化
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // エラー処理
        //SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    // 2. ウィンドウの作成
    SDL_Window* window = SDL_CreateWindow(
        "My SDL Game",      // ウィンドウのタイトル
        800, // 幅
        600, // 高さ
        SDL_WINDOW_RESIZABLE // フラグ
    );
    if (!window) {
        // エラー処理
        //SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // レンダラーの作成
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        nullptr
    );
    if (!renderer) {
        // エラー処理
        //SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // イベントループ
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true; // ウィンドウが閉じられたら終了
            }
            // その他のイベント処理（キーボード入力など）
        }

        // 描画処理
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 赤色に設定
        SDL_RenderClear(renderer); // 画面をクリア
        SDL_RenderPresent(renderer); // 描画を画面に反映
    }

    // 開放
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); // SDLサブシステムをシャットダウン

    return 0; // プログラム成功終了
}