#include "Block.h"
#include "Camera.h"
#include "Game.h"
#include <SDL3/SDL_log.h>

// �R���X�g���N�^
Block::Block(SDL_Texture* texture, int startGridX, int startGridY)
    : texture_(texture), // �u���b�N�̃e�N�X�`����������
    gridX_(startGridX), // �����O���b�hX���W��������
    gridY_(startGridY) // �����O���b�hY���W��������
{
    // �`���` (destRect_) �������O���b�h�ʒu�̃s�N�Z�����W�ƃ^�C���T�C�Y�ŏ�����
    destRect_ = { (float)gridX_ * TILE_W, (float)gridY_ * TILE_H, (float)TILE_W, (float)TILE_H };
}

// �f�X�g���N�^
Block::~Block() {}

// �X�V����
void Block::Update(float deltaTime) {
    // �u���b�N�ɓ��ʂȓ������Ȃ����߁A���݂̂Ƃ��뉽���������Ȃ��B
}

// �`�揈��
void Block::Render(SDL_Renderer* renderer, Camera* camera) {
    SDL_FRect renderRect = destRect_; // �`��p�̋�`���u���b�N�̌��݂̈ʒu�ƃT�C�Y�ŏ�����
    renderRect.x -= camera->GetX(); // �J������X�I�t�Z�b�g��K�p
    renderRect.y -= camera->GetY(); // �J������Y�I�t�Z�b�g��K�p

    SDL_RenderTexture(renderer, texture_, nullptr, &renderRect); // �e�N�X�`����`��
}

// �u���b�N�̃O���b�h���W��ݒ�
void Block::SetGridPosition(int newGridX, int newGridY) {
    gridX_ = newGridX; // �V�����O���b�hX���W��ݒ�
    gridY_ = newGridY; // �V�����O���b�hY���W��ݒ�
    // �O���b�h���W�̕ύX�ɔ����A�s�N�Z�����W���X�V���܂��B
    destRect_.x = (float)gridX_ * TILE_W;
    destRect_.y = (float)gridY_ * TILE_H;
}