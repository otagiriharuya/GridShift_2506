#include <SDL3/SDL.h> // SDL3 のメインヘッダー
#include <SDL3_image/SDL_image.h> // SDL_image のヘッダー
#include <string> // std::string を使用するため
#include <chrono> // 高精度な時間計測のため (SDL_Delay の代わり)

// ウィンドウの定数
const int WINDOW_W = 640; // ウィンドウの幅
const int WINDOW_H = 480; // ウィンドウの高さ
const std::string WINDOW_TITLE = "Grid Shift - Test"; // ウィンドウのタイトル
const int FPS = 60; // 目標フレームレート
const Uint64 FRAME_DELAY_TICKS = SDL_GetPerformanceFrequency() / FPS; // 1フレームあたりの最小ティック数

int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr; // ウィンドウのポインタ
    SDL_Renderer* renderer = nullptr; // レンダラーのポインタ
    bool isRunning = true; // ゲームループのフラグ

    // SDL の初期化
    // ビデオサブシステムのみを初期化します。
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        // SDL_GetError() で詳細なエラーメッセージを取得
        SDL_Log("SDL の初期化に失敗しました: %s", SDL_GetError());
        return 1; // 初期化失敗時は1を返す
    }

    // ウィンドウとレンダラーの同時作成
    if (SDL_CreateWindowAndRenderer(WINDOW_TITLE.c_str(), WINDOW_W, WINDOW_H, 0, &window, &renderer) < 0) {
        SDL_Log("ウィンドウとレンダラーの作成に失敗しました: %s", SDL_GetError());
        SDL_Quit(); // SDL を終了
        return 1;
    }

    // 再度チェック
    if (!renderer) {
        SDL_Log("レンダラーの取得に失敗しました: %s", SDL_GetError());
        SDL_DestroyWindow(window); // ウィンドウを開放
        SDL_Quit(); // SDL を終了
        return 1;
    }

    // テクスチャの読み込み
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "Image/player.png"); // プレイヤーのテクスチャを読み込む
    if (!playerTexture) {
        SDL_Log("画像の読み込みに失敗しました: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer); // レンダラーを開放
        SDL_DestroyWindow(window); // ウィンドウを開放
        SDL_Quit(); // SDL を終了
        return 1; // エラー時は1を返す
    }

    SDL_Event ev; // イベント用の変数
    Uint64 frameStartTime = 0; // フレーム開始時刻 (パフォーマンスカウンターティック)
    Uint64 frameEndTime = 0; // フレーム終了時刻 (パフォーマンスカウンターティック)
    Uint64 frameDuration = 0; // フレームにかかった時間 (ティック)

    // メインループ
    while (isRunning)
    {
        frameStartTime = SDL_GetPerformanceCounter(); // フレーム開始時間を取得

        // システムイベントの処理
        while (SDL_PollEvent(&ev))
        {
            // ウィンドウを閉じる
            if (ev.type == SDL_EVENT_QUIT)
                isRunning = false; // ループを終了

            // キーボード入力などのイベント処理を追加
            if (ev.type == SDL_EVENT_KEY_DOWN) {
                // 例: Escキーで終了
                if (ev.key.key == SDLK_ESCAPE) {
                    isRunning = false;
                }
            }
        }

        // ゲームロジックの更新 (ここにゲームの状態変更処理を記述)

        // 描画処理
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 背景色を黒に設定 (RGBA)
        SDL_RenderClear(renderer); // 画面をクリア

        // 四角の描画 (SDL_FRect を直接使用)
        SDL_FRect rect = { (float)WINDOW_W / 2 - 50, (float)WINDOW_H / 2 - 50, 100.0f, 100.0f };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 赤に設定 (RGBA)
        SDL_RenderFillRect(renderer, &rect); // 四角形を塗りつぶして描画 (SDL3では単数形でもOK)

        // プレイヤーのテクスチャ描画
        // SDL_FRect は浮動小数点数で直接初期化できます。
        SDL_FRect destRect = { (float)WINDOW_W / 2 - 16, (float)WINDOW_H / 2 - 16, 32.0f, 32.0f }; // 描画位置とサイズ
        SDL_RenderTexture(renderer, playerTexture, NULL, &destRect);

        SDL_RenderPresent(renderer); // 描画内容を画面に表示

        // フレームレート制御
        frameEndTime = SDL_GetPerformanceCounter(); // フレーム終了時間を取得
        frameDuration = frameEndTime - frameStartTime; // 1フレームにかかったティック数

        // 目標ティック数よりも短かった場合、残りの時間待機
        if (frameDuration < FRAME_DELAY_TICKS) {
            Uint64 delayMs = (Uint64)((double)(FRAME_DELAY_TICKS - frameDuration) * 1000.0 / SDL_GetPerformanceFrequency());
            if (delayMs > 0) { // 負の値にならないようにチェック
                SDL_Delay(delayMs);
            }
        }
    }

    // ループ終了時、SDL 関連のものを開放
    SDL_DestroyTexture(playerTexture); // プレイヤーのテクスチャの開放
    SDL_DestroyRenderer(renderer); // レンダラーの開放
    SDL_DestroyWindow(window); // ウィンドウの開放
    SDL_Quit(); // SDL の終了処理

    return 0; // プログラムを正常終了
}
