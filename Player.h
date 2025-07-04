#pragma once  
#ifndef PLAYER_H  
#define PLAYER_H  

#include <SDL3/SDL.h>  
#include <vector>  
#include <memory>  

// �N���X�̑O���錾  
class Map;  
class Camera;  
class Block;  

// �v���C���[�̈ړ��A��ԁA�`��Ȃǂ��Ǘ�
class Player {  
public:
	// �R���X�g���N�^
	// �v���C���[�̃e�N�X�`���A�}�b�v��̊J�n�O���b�h���W�A�v���C���[�̃s�N�Z�����ƍ����A
	// �}�b�v�S�̂̃s�N�Z���T�C�Y(�J�����Ǐ]�̂���)�A��������}�b�v�I�u�W�F�N�g�ւ̃|�C���^
	Player(SDL_Texture* texture, int startGridX, int startGridY, int w, int h, 
		   int mapWP, int mapHP, Map* map);  
	// �f�X�g���N�^  
	~Player();

	// �X�V���� 
	// �O�t���[������̌o�ߎ��ԁA���ݎ擾���Ă���L�[�̏�ԁA�u���b�N�I�u�W�F�N�g�̃R���e�i(�Փ˔���p)
	void Update(float deltaTime, const Uint8* keyboardState, std::vector<std::unique_ptr<Block>>* blocks); 
	// �`�揈��
	// �`��Ɏg�p����SDL�����_���[�A�J�����I�u�W�F�N�g(�`��ʒu�̃I�t�Z�b�g�v�Z�Ɏg�p)
	void Render(SDL_Renderer* renderer, Camera* camera);

	// �v���C���[�̃s�N�Z�����W�ƃT�C�Y�擾 
	float GetX() const { return destRect_.x; } // �v���C���[��X���W���擾  
	float GetY() const { return destRect_.y; } // �v���C���[��Y���W���擾  
	int GetWidth() const { return (int)destRect_.w; } // �v���C���[�̕����擾  
	int GetHeight() const { return (int)destRect_.h; } // �v���C���[�̍������擾  

	// �v���C���[�̌��݂̃O���b�h���W���擾  
	SDL_Point GetGridPosition() const;  

private:  
	SDL_Texture* texture_; // �v���C���[�̃e�N�X�`��  
	SDL_FRect destRect_; // �v���C���[�̕`��ʒu�ƃT�C�Y(float���W)

	float speed_; // �v���C���[�̈ړ����x(�s�N�Z��/�b) 
	int mapWP_; // �}�b�v�̑���(�s�N�Z��)
	int mapHP_; // �}�b�v�̑�����(�s�N�Z��)
	Map* map_; // ��������}�b�v�ւ̃|�C���^

	int currentGridX_; // �v���C���[�����݂���O���b�h��X���W
	int currentGridY_; // �v���C���[�����݂���O���b�h��Y���W

	bool isGridMoving_; // �O���b�h�ړ��A�j���[�V���������ǂ����̃t���O
	float moveProgress_; // ���݂̃O���b�h�ړ��̐i�s�x(0.0f�`1.0f)
	SDL_Point startGridPos_; // �O���b�h�ړ��J�n���̃O���b�h���W
	SDL_Point targetGridPos_; // �O���b�h�ړ��ڕW�̃O���b�h���W

	// �w�肳�ꂽ�����Ɉړ������݂�
	// �������Ftrue���s���Ffalse
	bool TryStartMove(int dx, int dy, std::vector<std::unique_ptr<Block>>* blocks);
};  

#endif // PLAYER_H
