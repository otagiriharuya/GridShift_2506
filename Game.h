#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <memory>

// クラス宣言
class Player;
class TextureManager;
class Camera;
class Map;
class Block;

const int TILE_W = 32;
const int TILE_H = 32;
const int TILE_ID_FLOOR = 0; // 床タイル
const int TILE_ID_WALL = 1; // 壁タイル(Player, Blockが移動できない)
const int TILE_ID_BLOCK_INITIAL = 2; // マップファイルでブロックの初期位置を示すID (生成後に床に変換)

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
	float frameDuration_; // フレームの持続時間
	Uint64 frameDelayTicks_; // フレーム遅延ティック数

	float deltaTime; // 各フレームの時間差を保持

	std::vector<std::unique_ptr<Block>> blocks_;

	// 定数
	const std::string WINDOW_TITLE = "Grid Shift"; // ウィンドウタイトル
	const int WINDOW_W = 640; // 画面幅
	const int WINDOW_H = 360; // 画面高さ
	const float FPS = 60.0f; // 目標FPS
	const float MS_PER_FRAME = 1000.0f / FPS; // 1フレームあたりの目標ミリ秒
};

#endif // GAME_H