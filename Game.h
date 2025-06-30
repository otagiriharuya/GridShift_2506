#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>

// クラス宣言
class Player;
class TextureManager;
class Camera;
class Map;

constexpr int TILE_W = 32;
constexpr int TILE_H = 32;

class Game {
public:
	Game(); // コンストラクタ
	~Game(); // デストラクタ

	bool Init(); // 初期化処理
	void Run(); // ゲームループ
	void Events(); // イベント処理
	void Update(); // 更新処理
	void Render(); // 描画処理
	void Clean(); // クリーンアップ処理

	SDL_Renderer* GetRenderer() const { return renderer_; } // レンダラー取得

private:
	SDL_Window* window_; // ウィンドウ
	SDL_Renderer* renderer_; // レンダラー
	bool isRunning_; // ゲームが実行中かどうか

	// ゲームオブジェクトのインスタンス
	Player* player_; // プレイヤーオブジェクト
	TextureManager* textureManager_; // テクスチャマネージャー
	Camera* camera_; // カメラオブジェクト
	Map* map_; // マップオブジェクト

	// フレームレート制御用
	Uint64 frameStartTime_; // フレーム開始時間
	Uint64 frameEndTime_; // フレーム終了時間
	Uint64 frameDuration_; // フレームの持続時間
	Uint64 frameDelayTicks_; // フレーム遅延ティック数

	float deltaTime; // 各フレームの時間差を保持

	// 定数
	const int WINDOW_W = 640; // 画面幅
	const int WINDOW_H = 480; // 画面高さ
	const float MAP_W = 1280.0f; // マップの幅
	const float MAP_H = 960.0f; // マップの高さ
	const std::string WINDOW_TITLE = "Grid Shift"; // ウィンドウタイトル
	const int FPS = 60; // フレームレート

};

#endif // GAME_H