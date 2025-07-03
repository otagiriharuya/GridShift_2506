#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "Game.h"

// �O���錾
class TextureManager;
class Camera;

class Block {
public:
	// �R���X�g���N�^
	Block(int gridX, int gridY, TextureManager* textureManager, const std::string& texturePath);
	// �f�X�g���N�g
	~Block();

	void Render(SDL_Renderer* renderer, Camera* camera);

	// ���݂̃O���b�h���W�擾
	int GetGridX() const { return gridx_; }
	int GetGridY() const { return gridy_; }

	// �s�N�Z�����W�擾
	SDL_FRect GetDestRect() const { return destRect_; }

	// �O���b�h���W�X�V�֐�
	void SetGridPosition(int newGridX, int newGridY);

private:
	SDL_Texture* texture_;
	SDL_FRect destRect_;

	int gridx_; // �O���b�h���WX
	int gridy_; // �O���b�h���WY

};