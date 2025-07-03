#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "Game.h"

// 前方宣言
class TextureManager;
class Camera;

class Block {
public:
	// コンストラクタ
	Block(int gridX, int gridY, TextureManager* textureManager, const std::string& texturePath);
	// デストラクト
	~Block();

	void Render(SDL_Renderer* renderer, Camera* camera);

	// 現在のグリッド座標取得
	int GetGridX() const { return gridx_; }
	int GetGridY() const { return gridy_; }

	// ピクセル座標取得
	SDL_FRect GetDestRect() const { return destRect_; }

	// グリッド座標更新関数
	void SetGridPosition(int newGridX, int newGridY);

private:
	SDL_Texture* texture_;
	SDL_FRect destRect_;

	int gridx_; // グリッド座標X
	int gridy_; // グリッド座標Y

};