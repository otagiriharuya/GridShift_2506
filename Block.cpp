#include "Block.h"
#include "Camera.h"
#include "Game.h"
#include <SDL3/SDL_log.h>

// コンストラクタ
Block::Block(SDL_Texture* texture, int startGridX, int startGridY)
    : texture_(texture), // ブロックのテクスチャを初期化
    gridX_(startGridX), // 初期グリッドX座標を初期化
    gridY_(startGridY) // 初期グリッドY座標を初期化
{
    // 描画矩形 (destRect_) を初期グリッド位置のピクセル座標とタイルサイズで初期化
    destRect_ = { (float)gridX_ * TILE_W, (float)gridY_ * TILE_H, (float)TILE_W, (float)TILE_H };
}

// デストラクタ
Block::~Block() {}

// 更新処理
void Block::Update(float deltaTime) {
    // ブロックに特別な動きがないため、現在のところ何も処理しない。
}

// 描画処理
void Block::Render(SDL_Renderer* renderer, Camera* camera) {
    SDL_FRect renderRect = destRect_; // 描画用の矩形をブロックの現在の位置とサイズで初期化
    renderRect.x -= camera->GetX(); // カメラのXオフセットを適用
    renderRect.y -= camera->GetY(); // カメラのYオフセットを適用

    SDL_RenderTexture(renderer, texture_, nullptr, &renderRect); // テクスチャを描画
}

// ブロックのグリッド座標を設定
void Block::SetGridPosition(int newGridX, int newGridY) {
    gridX_ = newGridX; // 新しいグリッドX座標を設定
    gridY_ = newGridY; // 新しいグリッドY座標を設定
    // グリッド座標の変更に伴い、ピクセル座標も更新します。
    destRect_.x = (float)gridX_ * TILE_W;
    destRect_.y = (float)gridY_ * TILE_H;
}