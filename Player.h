#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <vector>
#include <memory>

// �O���錾
class Map;
class Camera;
class Block;

class Player {
public:
	Player(SDL_Texture* texture, int startGridX, int startGridY,int w, int h, int mapWP, int mapHP,Map* map); // �R���X�g���N�^
	~Player(); // �f�X�g���N�^

	void HandleInput(const SDL_Event& ev); // �C�x���g����
	void Update(float deltaTime,std::vector<std::unique_ptr<Block>>* blocks); // �X�V����
	void Render(SDL_Renderer* renderer, Camera* camera); // �`�揈��

	// �Q�b�^�[(�J�����Ǐ]�Ȃ�)
	float GetX() const { return destRect_.x; } // �v���C���[��X���W���擾
	float GetY() const { return destRect_.y; } // �v���C���[��Y���W���擾
	float GetWidth() const { return destRect_.w; } // �v���C���[�̕����擾
	float GetHeight() const { return destRect_.h; } // �v���C���[�̍������擾

	// �v���C���[�̌��݂̃O���b�h���W���擾
	SDL_Point GetGridPosition() const;

private:
	SDL_Texture* texture_; // �v���C���[�̃e�N�X�`��
	SDL_FRect destRect_; // �v���C���[�̋�`(�R���W�������Ŏg�p)

	float speed_; // �v���C���[�̈ړ����x
	int mapWP_; // �}�b�v�̃s�N�Z����
	int mapHP_; // �}�b�v�̃s�N�Z������
	Map* map_; // Map�N���X�ւ̃|�C���^

	int currentGridX_; // ���݂̃O���b�hX���W
	int currentGridY_; // ���݂̃O���b�hY���W

	bool TryStartMove(int dx, int dy, std::vector<std::unique_ptr<Block>>* blocks); // �����邩�`�F�b�N����֐�

	// �e�����L�[��������Ă��邩�ǂ����̃t���O
	bool isMoveUp_;
	bool isMoveDown_;
	bool isMoveLeft_;
	bool isMoveRight_;

	// ���̈ړ������̃o�b�t�@
	int nextMoveDx_; // ���Ɉړ�������X����(-1:���C1:�E�C0:�Ȃ�)
	int nextMoveDy_; // ���Ɉړ�������Y����(-1:��C1:���C0:�Ȃ�)

	// �O���b�h�P�ʈړ��̏�ԊǗ�
	bool isGridMoving_; // ���݃O���b�h�ړ�����
	float moveProgress_; // 0.0f(�J�n)����1.0f(����)

	// �ړ����̃O���b�h����ێ�
	SDL_Point startGridPos_; // �ړ��J�n���̃O���b�h���W
	SDL_Point targetGridPos_; // �ڕW�̃O���b�h���W
};

#endif // PLAYER_H
