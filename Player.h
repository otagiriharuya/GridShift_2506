#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>

class Camera; // 前方宣言

class Player {
public:
	Player(SDL_Texture* texture, float x, float y, float w, float h, float mapW, float mapH); // コンストラクタ
	~Player(); // デストラクタ

	void HandleInput(const SDL_Event& ev); // イベント処理
	void Update(float deltaTime); // 更新処理
	void Render(SDL_Renderer* renderer, Camera* camera); // 描画処理

	// ゲッター(カメラ追従など)
	float GetX() const { return destRect_.x; } // プレイヤーのX座標を取得
	float GetY() const { return destRect_.y; } // プレイヤーのY座標を取得
	float GetWidth() const { return destRect_.w; } // プレイヤーの幅を取得
	float GetHeight() const { return destRect_.h; } // プレイヤーの高さを取得

private:
	SDL_Texture* texture_; // プレイヤーのテクスチャ
	SDL_FRect destRect_; // プレイヤーの矩形(コリジョン等で使用)
	float speed_; // プレイヤーの移動速度

	// 各方向キーが押されているかどうかのフラグ
	bool isMoveUp_;
	bool isMoveDown_;
	bool isMoveLeft_;
	bool isMoveRight_;

	float mapW_; // マップの幅
	float mapH_; // マップの高さ
};

#endif // PLAYER_H
