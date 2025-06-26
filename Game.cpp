#include "Game.h"
#include "Player.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Map.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>

// コンストラクタでの初期化を最小限に
Game::Game()
	: window_(nullptr), renderer_(nullptr), isRunning_(false),
	player_(nullptr), textureManager_(nullptr),camera_(nullptr),map_(nullptr),
	frameStartTime_(0),frameEndTime_(0), frameDuration_(0),frameDelayTicks_(0){ }

// デストラクタ
Game::~Game() {
    Clean();
}

// ゲームの初期化処理
bool Game::Init() {
    // SDL の初期化
    SDL_SetMainReady();
    // ビデオサブシステムのみを初期化します。
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        // SDL_GetError() で詳細なエラーメッセージを取得
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return false; // 初期化失敗時
    }

    // ウィンドウとレンダラーの同時作成
    if (SDL_CreateWindowAndRenderer(WINDOW_TITLE.c_str(), WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE | SDL_RENDERER_VSYNC_DISABLED, &window_, &renderer_) < 0) {
        SDL_Log("Window and renderer creation failed: %s", SDL_GetError());
        SDL_Quit(); // SDL を終了
        return false;
    }

    // オブジェクトの生成
	textureManager_ = new TextureManager(renderer_); // レンダラーを渡す
    if (!textureManager_) {
		SDL_Log("TextureManager creatfailed");
        return false;
    }

	// マップの初期化
    map_ = new Map(renderer_, textureManager_);
    if (!map_->LoadMap("Map/map01.csv")) {
        SDL_Log("Failed to generation map: %s", "Map/map01.csv");
		return false; // マップの読み込みに失敗した場合はfalseを返す
    }

    // プレイヤーの初期化
    player_ = new Player(textureManager_->LoadTexture("Image/player.png"), WINDOW_W / 2 - 16, WINDOW_H / 2 - 16, 32, 32, map_->GetMapW(), map_->GetMapH());
    if (!player_) {
        SDL_Log("Failed to generation Image");
        return false;
    }
    
    // カメラの初期化
	camera_ = new Camera(0, 0, WINDOW_W, WINDOW_H, map_->GetMapW(), map_->GetMapH());
    if (!camera_) {
        SDL_Log("Failed to generation Camera:");
        return false;
    }

    // deltaTime計測のための初期設定
    frameStartTime_ = SDL_GetPerformanceCounter(); // 初期化時に一度設定

    // フレームレートの制御の初期化
    //frameDelayTicks_ = SDL_GetPerformanceFrequency() / FPS; // フレームレートに基づく遅延時間を計算

	isRunning_ = true; // ゲームループを開始可能にする
	return true; // 初期化成功時はtrueを返す
}

// ゲームの更新処理
void Game::Run() {
    while (isRunning_) {
        Events(); // イベント処理
        Update(); // 更新処理
        Render(); // 描画処理
    }
}

// ゲームのイベント処理
void Game::Events() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_EVENT_QUIT:
            isRunning_ = false;
            break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            player_->HandleInput(ev); // プレイヤークラスにイベント処理を委託
            break;
        }
    }
}

// ゲームの更新処理
void Game::Update() {
	// 時間計測開始
    frameEndTime_ = SDL_GetPerformanceCounter();

    // カウンタの生の値と周波数
    Uint64 currentPerfCounter = SDL_GetPerformanceCounter();
    Uint64 perfFreq = SDL_GetPerformanceFrequency();

    // deltaTime 計算
    deltaTime = (float)(frameEndTime_ - frameStartTime_) / perfFreq;
    SDL_Log("deltaTime: '%.8f'", deltaTime);
    
    // 次のフレームのために開始時間を更新
    frameStartTime_ = frameEndTime_;

    float frameDuration = deltaTime * 1000.0f;//ミリ秒単位
    const float MS_PER_FRAME = 1000.0f / FPS; // 
    if (frameDuration < MS_PER_FRAME) {
        // Delayはミリ秒単位なので、余った時間をミリ秒に変換し遅延させる
        SDL_Delay((Uint32)(MS_PER_FRAME - frameDuration));
    }

    player_->Update(deltaTime); // プレイヤークラスに更新を委託
    camera_->Update(player_->GetX(), player_->GetY(), player_->GetWidth(), player_->GetHeight()); // カメラの更新
}

// ゲームの描画処理
void Game::Render() {
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255); // 背景色を黒に設定
	SDL_RenderClear(renderer_); // 画面をクリア

	map_->Render(renderer_, camera_); // マップの描画を委託

    player_->Render(renderer_, camera_); // プレイヤークラスに描画を委託

	SDL_RenderPresent(renderer_); // レンダラーの内容を画面に表示
}

// ゲームのクリーンアップ処理
void Game::Clean() {
	delete player_; // player_オブジェクトの削除
    player_ = nullptr;

	delete textureManager_; // yextureManager_オブジェクトの削除
    textureManager_ = nullptr;

	delete camera_; // camera_オブジェクトの削除
    camera_ = nullptr;

	delete map_; // map_オブジェクトの削除
    map_ = nullptr;

    if (renderer_) {
        SDL_DestroyRenderer(renderer_); // レンダラーの破棄
		renderer_ = nullptr; // ポインタをnullptrに設定
    }

    if (window_) {
        SDL_DestroyWindow(window_); // ウィンドウの破棄
        window_ = nullptr; // ポインタをnullptrに設定
    }
	SDL_Quit(); // SDLを終了
}

