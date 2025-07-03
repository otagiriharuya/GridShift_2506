#include "Block.h"
#include "Camera.h"
#include "TextureManager.h"
#include <SDL3/SDL_log.h>

Block::Block(int gridX, int gridY, TextureManager* textureManager, const std::string& texturePath)
	:gridx_(gridX), gridy_(gridY) {
	// TextureManagerからテクスチャ取得
	texture_ = textureManager->GetTexture(texturePath);
	if (!texture_) {
		//SDL_Log("Block: Failed to get texture for path: %s", texturePath.c_str());
	}

	// グリッド座標とTILE_WIDTH/HEIGHTから初期ピクセル座標とサイズを計算
	destRect_ = { (float)gridx_ * TILE_W, (float)gridy_ * TILE_H,(float)TILE_W,(float)TILE_H };
}

Block::~Block(){}

// 描画
void Block::Render(SDL_Renderer* renderer, Camera* camera) {
	if (texture_) {
		SDL_FRect renderRect = destRect_;
		// カメラのオフセットを適用し描画位置調整
		renderRect.x -= camera->GetX();
		renderRect.y -= camera->GetY();

		SDL_RenderTexture(renderer, texture_, NULL, &renderRect);
	}
}

// グリッド座標更新
void Block::SetGridPosition(int newGridX, int newGridY) {
	gridx_ = newGridX;
	gridy_ = newGridY;
	// グリッド座標が更新されたら、ピクセル座標も更新
	destRect_.x = (float)gridx_ * TILE_W;
	destRect_.y = (float)gridy_ * TILE_H;
}