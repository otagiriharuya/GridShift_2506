#include "Game.h"
#include "Player.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Map.h"
#include "Block.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>

// コンストラクタでの初期化を最小限に
Game::Game()
	: window_(nullptr), renderer_(nullptr), 
    keyboardState_(nullptr),numKeys_(0),
    isRunning_(true),lastTick_(0){ }

// デストラクタ
Game::~Game() {
    
}

// ゲームの初期化処理
bool Game::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        //SDL_Log("SDLを初期化できませんでした。: %s", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow("Grid Shift", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window_) {
        SDL_Log("ウィンドウを作成できませんでした: %s", SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        SDL_Log("レンダラーを作成できませんでした: %s", SDL_GetError());
        return false;
    }

    textureManager_ = new TextureManager(renderer_);
    if (!textureManager_->LoadTexture("assets/player.png", "player") ||
        !textureManager_->LoadTexture("assets/tiles.png", "tiles") ||
        !textureManager_->LoadTexture("assets/block.png", "block")) {
        SDL_Log("テクスチャのロードに失敗しました。");
        return false;
    }

    map_ = std::make_unique<Map>(textureManager_->GetTexture("tiles"));
    if (!map_->LoadMap("assets/map01.csv")) {
        SDL_Log("マップのロードに失敗しました。");
        return false;
    }

    // プレイヤーの開始位置を見つける
    int playerStartX = 0;
    int playerStartY = 0;
    map_->FindTile(TILE_ID_PLAYER_START, playerStartX, playerStartY);

    player_ = std::make_unique<Player>(textureManager_->GetTexture("player"),
        playerStartX, playerStartY, TILE_W, TILE_H,
        map_->GetMapCols() * TILE_W, map_->GetMapRows() * TILE_HE,
        map_.get());

    // マップ上のブロックをロード
    std::vector<SDL_Point> blockPositions = map_->FindAllTiles(TILE_ID_BLOCK);
    for (const auto& pos : blockPositions) {
        blocks_.push_back(std::make_unique<Block>(textureManager_->GetTexture("block"), pos.x, pos.y));
    }

    camera_ = std::make_unique<Camera>(0, 0, 800, 600); // 画面サイズに合わせる

    // ★追加: キーボードの状態を取得するポインタを設定
    keyboardState_ = SDL_GetKeyboardState(&numKeys_); // これをInitializeで1回呼び出す

    return true;
}

void Game::RunLoop() {
    while (isRunning_) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown() {
    delete textureManager_; // newで確保したのでdeleteする
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

// ★関数名をHandleEventsからProcessInputに変更
void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // SDL_Quit イベントを処理
        if (event.type == SDL_EVENT_QUIT) {
            isRunning_ = false;
        }
        // 他のイベント（ウィンドウリサイズなど）があればここに追加

        // ★削除: プレイヤーのHandleInputはもう呼び出さない
        // player_->HandleInput(event);
    }
    // ★重要: SDL_GetKeyboardState() は PollEvent/PumpEvent の後に呼び出す
    // イベントキューを処理することで内部状態が更新されるため
    // keyboardState_ = SDL_GetKeyboardState(&numKeys_); // Initializeで取得済みなので不要
    // keyboardState_はフレームごとに更新不要、SDLが内部で管理する
}

void Game::UpdateGame() {
    // deltaTime計算 (変更なし)
    Uint64 currentTick = SDL_GetPerformanceCounter();
    float deltaTime = static_cast<float>(currentTick - lastTick_) / SDL_GetPerformanceFrequency();
    lastTick_ = currentTick;

    // ゲーム全体の更新ロジック
    // ★変更: player_->Update に keyboardState_ を渡す
    player_->Update(deltaTime, keyboardState_, &blocks_);

    // カメラの更新 (プレイヤーを追従)
    camera_->Update(player_->GetX(), player_->GetY(), player_->GetWidth(), player_->GetHeight(),
        map_->GetMapCols() * TILE_WIDTH, map_->GetMapRows() * TILE_HEIGHT);
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255); // 黒でクリア
    SDL_RenderClear(renderer_);

    // マップの描画
    map_->Render(renderer_, camera_.get());

    // ブロックの描画
    for (const auto& block : blocks_) {
        block->Render(renderer_, camera_.get());
    }

    // プレイヤーの描画
    player_->Render(renderer_, camera_.get());

    SDL_RenderPresent(renderer_);
}