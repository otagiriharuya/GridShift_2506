#pragma once  
#ifndef PLAYER_H  
#define PLAYER_H  

#include <SDL3/SDL.h>  
#include <vector>  
#include <memory>  

// クラスの前方宣言  
class Map;  
class Camera;  
class Block;  

// プレイヤーの移動、状態、描画などを管理
class Player {  
public:
	// コンストラクタ
	// プレイヤーのテクスチャ、マップ上の開始グリッド座標、プレイヤーのピクセル幅と高さ、
	// マップ全体のピクセルサイズ(カメラ追従のため)、所属するマップオブジェクトへのポインタ
	Player(SDL_Texture* texture, int startGridX, int startGridY, int w, int h, 
		   int mapWP, int mapHP, Map* map);  
	// デストラクタ  
	~Player();

	// 更新処理 
	// 前フレームからの経過時間、現在取得しているキーの状態、ブロックオブジェクトのコンテナ(衝突判定用)
	void Update(float deltaTime, const Uint8* keyboardState, std::vector<std::unique_ptr<Block>>* blocks); 
	// 描画処理
	// 描画に使用するSDLレンダラー、カメラオブジェクト(描画位置のオフセット計算に使用)
	void Render(SDL_Renderer* renderer, Camera* camera);

	// プレイヤーのピクセル座標とサイズ取得 
	float GetX() const { return destRect_.x; } // プレイヤーのX座標を取得  
	float GetY() const { return destRect_.y; } // プレイヤーのY座標を取得  
	int GetWidth() const { return (int)destRect_.w; } // プレイヤーの幅を取得  
	int GetHeight() const { return (int)destRect_.h; } // プレイヤーの高さを取得  

	// プレイヤーの現在のグリッド座標を取得  
	SDL_Point GetGridPosition() const;  

private:  
	SDL_Texture* texture_; // プレイヤーのテクスチャ  
	SDL_FRect destRect_; // プレイヤーの描画位置とサイズ(float座標)

	float speed_; // プレイヤーの移動速度(ピクセル/秒) 
	int mapWP_; // マップの総幅(ピクセル)
	int mapHP_; // マップの総高さ(ピクセル)
	Map* map_; // 所属するマップへのポインタ

	int currentGridX_; // プレイヤーが現在いるグリッドのX座標
	int currentGridY_; // プレイヤーが現在いるグリッドのY座標

	bool isGridMoving_; // グリッド移動アニメーション中かどうかのフラグ
	float moveProgress_; // 現在のグリッド移動の進行度(0.0f～1.0f)
	SDL_Point startGridPos_; // グリッド移動開始時のグリッド座標
	SDL_Point targetGridPos_; // グリッド移動目標のグリッド座標

	// 指定された方向に移動を試みる
	// 成功時：true失敗時：false
	bool TryStartMove(int dx, int dy, std::vector<std::unique_ptr<Block>>* blocks);
};  

#endif // PLAYER_H
