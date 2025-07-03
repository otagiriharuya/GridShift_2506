#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <vector>
#include <memory>

// 前方宣言
class Map;
class Camera;
class Block;

class Player {
public:
	Player(SDL_Texture* texture, int startGridX, int startGridY,int w, int h, int mapWP, int mapHP,Map* map); // コンストラクタ
	~Player(); // デストラクタ

	void HandleInput(const SDL_Event& ev); // イベント処理
	void Update(float deltaTime,std::vector<std::unique_ptr<Block>>* blocks); // 更新処理
	void Render(SDL_Renderer* renderer, Camera* camera); // 描画処理

	// ゲッター(カメラ追従など)
	float GetX() const { return destRect_.x; } // プレイヤーのX座標を取得
	float GetY() const { return destRect_.y; } // プレイヤーのY座標を取得
	float GetWidth() const { return destRect_.w; } // プレイヤーの幅を取得
	float GetHeight() const { return destRect_.h; } // プレイヤーの高さを取得

	// プレイヤーの現在のグリッド座標を取得
	SDL_Point GetGridPosition() const;

private:
	SDL_Texture* texture_; // プレイヤーのテクスチャ
	SDL_FRect destRect_; // プレイヤーの矩形(コリジョン等で使用)

	float speed_; // プレイヤーの移動速度
	int mapWP_; // マップのピクセル幅
	int mapHP_; // マップのピクセル高さ
	Map* map_; // Mapクラスへのポインタ

	int currentGridX_; // 現在のグリッドX座標
	int currentGridY_; // 現在のグリッドY座標

	bool TryStartMove(int dx, int dy, std::vector<std::unique_ptr<Block>>* blocks); // 動けるかチェックする関数

	// 各方向キーが押されているかどうかのフラグ
	bool isMoveUp_;
	bool isMoveDown_;
	bool isMoveLeft_;
	bool isMoveRight_;

	// 次の移動方向のバッファ
	int nextMoveDx_; // 次に移動したいX方向(-1:左，1:右，0:なし)
	int nextMoveDy_; // 次に移動したいY方向(-1:上，1:下，0:なし)

	// グリッド単位移動の状態管理
	bool isGridMoving_; // 現在グリッド移動中か
	float moveProgress_; // 0.0f(開始)から1.0f(完了)

	// 移動中のグリッド情報を保持
	SDL_Point startGridPos_; // 移動開始時のグリッド座標
	SDL_Point targetGridPos_; // 目標のグリッド座標
};

#endif // PLAYER_H
