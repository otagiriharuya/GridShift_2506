#pragma once
#ifndef BLOCK_H
#define BLOCK_H 

#include <SDL3/SDL.h>
#include "Game.h"

// �O���錾
class Camera;

// �}�b�v��ɔz�u����邨�Z���u���b�N�I�u�W�F�N�g���Ǘ�
class Block {
public:
	// �R���X�g���N�^
	// �u���b�N�̕`��Ɏg�p����SDL�e�N�X�`���A�}�b�v��̃u���b�N�̏����O���b�h���W
	Block(SDL_Texture* texture, int startGridx, int startGridy);
	// �f�X�g���N�g
	~Block();

    // �X�V����
    void Update(float deltaTime);
    // �`�揈��
    // �`��Ɏg�p����SDL�����_���[�A�J�����I�u�W�F�N�g
    void Render(SDL_Renderer* renderer, Camera* camera);

    // �u���b�N�̌��݂̃O���b�hX���W���擾
    int GetGridX() const { return gridX_; }
    // �u���b�N�̌��݂̃O���b�hY���W���擾
    int GetGridY() const { return gridY_; }

    // �u���b�N�̃O���b�h���W��ݒ�
    // �u���b�N�̐V�����O���b�h���W
    void SetGridPosition(int newGridX, int newGridY);

    // �u���b�N�̃s�N�Z�����W�iX���j���擾
    float GetX() const { return destRect_.x; }
    // �u���b�N�̃s�N�Z�����W�iY���j���擾
    float GetY() const { return destRect_.y; }

private:
    SDL_Texture* texture_; // �u���b�N�̕`��Ɏg�p����e�N�X�`��
    SDL_FRect destRect_; // �u���b�N�̉�ʏ�̈ʒu�ƃT�C�Y

    int gridX_; // �u���b�N�����݂���O���b�h��X���W
    int gridY_; // �u���b�N�����݂���O���b�h��Y���W
};

#endif // BLOCK_H