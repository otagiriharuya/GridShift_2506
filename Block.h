#pragma once
#ifndef BLOCK_H
#define BLOCK_H 

#include <SDL3/SDL.h>
#include "Game.h"

// 前方宣言
class Camera;

// マップ上に配置されるおセルブロックオブジェクトを管理
class Block {
public:
	// コンストラクタ
	// ブロックの描画に使用するSDLテクスチャ、マップ上のブロックの初期グリッド座標
	Block(SDL_Texture* texture, int startGridx, int startGridy);
	// デストラクト
	~Block();

    // 更新処理
    void Update(float deltaTime);
    // 描画処理
    // 描画に使用するSDLレンダラー、カメラオブジェクト
    void Render(SDL_Renderer* renderer, Camera* camera);

    // ブロックの現在のグリッドX座標を取得
    int GetGridX() const { return gridX_; }
    // ブロックの現在のグリッドY座標を取得
    int GetGridY() const { return gridY_; }

    // ブロックのグリッド座標を設定
    // ブロックの新しいグリッド座標
    void SetGridPosition(int newGridX, int newGridY);

    // ブロックのピクセル座標（X軸）を取得
    float GetX() const { return destRect_.x; }
    // ブロックのピクセル座標（Y軸）を取得
    float GetY() const { return destRect_.y; }

private:
    SDL_Texture* texture_; // ブロックの描画に使用するテクスチャ
    SDL_FRect destRect_; // ブロックの画面上の位置とサイズ

    int gridX_; // ブロックが現在いるグリッドのX座標
    int gridY_; // ブロックが現在いるグリッドのY座標
};

#endif // BLOCK_H