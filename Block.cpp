#include "Block.h"
#include "Camera.h"
#include "TextureManager.h"
#include <SDL3/SDL_log.h>

Block::Block(int gridX, int gridY, TextureManager* textureManager, const std::string& texturePath)
	:gridx_(gridX), gridy_(gridY) {
	// TextureManager����e�N�X�`���擾
	texture_ = textureManager->GetTexture(texturePath);
	if (!texture_) {
		//SDL_Log("Block: Failed to get texture for path: %s", texturePath.c_str());
	}

	// �O���b�h���W��TILE_WIDTH/HEIGHT���珉���s�N�Z�����W�ƃT�C�Y���v�Z
	destRect_ = { (float)gridx_ * TILE_W, (float)gridy_ * TILE_H,(float)TILE_W,(float)TILE_H };
}

Block::~Block(){}

// �`��
void Block::Render(SDL_Renderer* renderer, Camera* camera) {
	if (texture_) {
		SDL_FRect renderRect = destRect_;
		// �J�����̃I�t�Z�b�g��K�p���`��ʒu����
		renderRect.x -= camera->GetX();
		renderRect.y -= camera->GetY();

		SDL_RenderTexture(renderer, texture_, NULL, &renderRect);
	}
}

// �O���b�h���W�X�V
void Block::SetGridPosition(int newGridX, int newGridY) {
	gridx_ = newGridX;
	gridy_ = newGridY;
	// �O���b�h���W���X�V���ꂽ��A�s�N�Z�����W���X�V
	destRect_.x = (float)gridx_ * TILE_W;
	destRect_.y = (float)gridy_ * TILE_H;
}