#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>

class Camera; // �O���錾

class Player {
public:
	Player(SDL_Texture* texture, float x, float y, float w, float h, float mapW, float mapH); // �R���X�g���N�^
	~Player(); // �f�X�g���N�^

	void HandleInput(const SDL_Event& ev); // �C�x���g����
	void Update(float deltaTime); // �X�V����
	void Render(SDL_Renderer* renderer, Camera* camera); // �`�揈��

	// �Q�b�^�[(�J�����Ǐ]�Ȃ�)
	float GetX() const { return destRect_.x; } // �v���C���[��X���W���擾
	float GetY() const { return destRect_.y; } // �v���C���[��Y���W���擾
	float GetWidth() const { return destRect_.w; } // �v���C���[�̕����擾
	float GetHeight() const { return destRect_.h; } // �v���C���[�̍������擾

private:
	SDL_Texture* texture_; // �v���C���[�̃e�N�X�`��
	SDL_FRect destRect_; // �v���C���[�̋�`(�R���W�������Ŏg�p)
	float speed_; // �v���C���[�̈ړ����x

	// �e�����L�[��������Ă��邩�ǂ����̃t���O
	bool isMoveUp_;
	bool isMoveDown_;
	bool isMoveLeft_;
	bool isMoveRight_;

	float mapW_; // �}�b�v�̕�
	float mapH_; // �}�b�v�̍���
};

#endif // PLAYER_H
