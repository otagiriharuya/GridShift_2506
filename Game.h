#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <vector>
#include <memory>

// 定数定義
const int TILE_W = 64; // タイルの幅
const int TILE_H = 64; // タイルの高さ

const int TILE_ID_PLAYER_START = 0; // プレイヤーの初期位置を表すタイルID
const int TILE_ID_FLOOR = 1; // 床タイルを示すタイルID
const int TILE_ID_WALL = 2; // 壁タイル(Player, Blockが移動できない)を示すID
const int TIlE_ID_BLOCK = 3; // ブロックを示すタイルID

// クラスの前方宣言
class Player;
class TextureManager;
class Camera;
class Map;
class Block;

// メインループ、初期化、終了処理、イベント処理、更新、描画の管理
class Game {
public:
	Game(); // コンストラクタ
	~Game(); // デストラクタ

	bool Initialize(); // ゲームの初期化
	void RunLoop(); // ゲームのメインループ
	void Shutdown(); // ゲームの終了処理

private:
	void ProcessInput(); // 入力処理
	void UpdateGame(); // ゲームの更新処理
	void Render(); // 描画処理

	// SDL関連のメンバー変数
	SDL_Window* window_; // ウィンドウオブジェクト
	SDL_Renderer* renderer_; // レンダラーオブジェクト

	// カスタムマネージャー
	TextureManager* textureManager_; // テクスチャ管理クラス

	// キーボードの状態いを保持するポインタ
	const Uint8* keyboardState_; // 現在のキー押下状態を直接参照
	int numKeys_; // キーの総数

	// ゲーム状態フラグ
	bool isRunning_; // ゲームが実行中かどうか
	Uint64 lastTick_; //前回のフレームからの経過時間を計算するための変数

	// ゲームオブジェクトの管理(unique_ptrで自動メモリ管理)
	std::unique_ptr<Player> player_; // プレイヤーオブジェクト
	std::unique_ptr<Map> map_; // マップオブジェクト
	std::unique_ptr<Camera> camera_; // カメラオブジェクト(描画範囲を制御)
	std::vector<std::unique_ptr<Block>> blocks_; // ブロックを管理するためのコンテナ
};

#endif // GAME_H