#include "Game.h"
#include "Player.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Map.h"
#include "Block.h"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>

// コンストラクタでの初期化
Game::Game()
    : window_(nullptr), // ウィンドウポインタをヌルで初期化
    renderer_(nullptr), // レンダラーポインタをヌルで初期化
    textureManager_(nullptr), // テクスチャマネージャーポインタをヌルで初期化
    keyboardState_(nullptr), // キーボード状態ポインタをヌルで初期化
    numKeys_(0), // キーの総数を0で初期化
    isRunning_(true), // ゲームは実行中として開始
    lastTick_(0) // 前回のTick時間を0で初期化
{}

// デストラクタ
Game::~Game() {
    delete textureManager_; // TextureManagerはnewで確保してのでdeleteする
}

// ゲームの初期化処理
bool Game::Initialize() {
    // SDLビデオサブシステムの初期化を試みる
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        //SDL_Log("SDLを初期化できませんでした。: %s", SDL_GetError());
        return false;
    }

    // ゲームウィンドウを作成
    // Grid Siftというタイトル、幅、高さ、リサイズ可能フラグ
    window_ = SDL_CreateWindow("Grid Shift", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window_) {
        SDL_Log("ウィンドウを作成できませんでした: %s", SDL_GetError());
        return false;
    }

    // レンダラーを作成
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
        SDL_Log("レンダラーを作成できませんでした: %s", SDL_GetError());
        return false;
    }

    // テクスチャマネージャーを生成し、必要なテクスチャをロード
    textureManager_ = new TextureManager(renderer_);
    if (!textureManager_->LoadTexture("assets/player.png", "player") ||
        !textureManager_->LoadTexture("assets/tiles.png", "tiles") ||
        !textureManager_->LoadTexture("assets/block.png", "block")) {
        SDL_Log("テクスチャのロードに失敗しました。");
        return false;
    }

    // マップオブジェクトを作成し、CSVファイルからマップデータをロード
    map_ = std::make_unique<Map>(textureManager_->GetTexture("tiles"));
    if (!map_->LoadMap("assets/map01.csv")) {
        SDL_Log("マップのロードに失敗しました。");
        return false;
    }

    // プレイヤーの開始位置を見つける
    int playerStartX = 0;
    int playerStartY = 0;
    map_->FindTile(TILE_ID_PLAYER_START, playerStartX, playerStartY);

    // プレイヤーオブジェクト作成
    player_ = std::make_unique<Player>(textureManager_->GetTexture("player"),
        playerStartX, playerStartY, TILE_W, TILE_H, // プレイヤーの開始位置、プレイヤーの幅と高さ
        map_->GetMapCols() * TILE_W, map_->GetMapRows() * TILE_H, // マップ全体の幅と高さを計算
        map_.get()); // マップオブジェクトへのポインタ

    // マップ上のブロックをロード
    std::vector<SDL_Point> blockPositions = map_->FindAllTiles(TIlE_ID_BLOCK);
    for (const auto& pos : blockPositions) {
        blocks_.push_back(std::make_unique<Block>(textureManager_->GetTexture("block"), pos.x, pos.y));
    }

    // カメラオブジェクトを作成
    // 初期位置、ビューポートサイズはウィンドウサイズに合わせる
    camera_ = std::make_unique<Camera>(0, 0, 800, 600);

    // キーボードの状態を取得するポインタを設定
    keyboardState_ = SDL_GetKeyboardState(&numKeys_);

    return true;
}

// メインループ
void Game::RunLoop() {
    while (isRunning_) {
        ProcessInput(); // 入力イベント処理
        UpdateGame(); // ゲームのロジックと状態を更新
        GenerateOutput(); // 更新された状態に基づいて画面に描画
    }
}

// ゲームの終了処理
void Game::Shutdown() {
    delete textureManager_; // newで確保したTextureManagerオブジェクトを開放
    SDL_DestroyRenderer(renderer_); // レンダラーを破棄
    SDL_DestroyWindow(window_); // ウィンドウを破棄
    SDL_Quit(); // SDLの全サブシステムを終了
}

// 入力処理
void Game::ProcessInput() {
    SDL_Event event;
    // イベントキューにあるすべてのイベントをポーリングして処理
    while (SDL_PollEvent(&event)) {
        // SDL_Quit イベントが来たrメインループを終了
        if (event.type == SDL_EVENT_QUIT) {
            isRunning_ = false;
        }
    }
}

// ゲームの状態更新
void Game::UpdateGame() {
    // フレーム間の経過時間を計算
    Uint64 currentTick = SDL_GetPerformanceCounter();
    float deltaTime = static_cast<float>(currentTick - lastTick_) / SDL_GetPerformanceFrequency();
    lastTick_ = currentTick; // 次のフレームのために現在のTick時間を保存

    // プレイヤーの更新
    // プレイヤーの移動と衝突判定を処理
    player_->Update(deltaTime, keyboardState_, &blocks_);

    // カメラの更新(プレイヤーを追従するように)
    camera_->Update(player_->GetX(), player_->GetY(), player_->GetWidth(), player_->GetHeight(),
        map_->GetMapCols() * TILE_W, map_->GetMapRows() * TILE_H); // マップ全体のピクセルサイズを渡す
}

// 画面描画
void Game::GenerateOutput() {
    // レンダラーを黒色でクリア
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
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