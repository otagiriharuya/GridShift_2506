#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <vector>
#include <memory>

// �萔��`
const int TILE_W = 64; // �^�C���̕�
const int TILE_H = 64; // �^�C���̍���

const int TILE_ID_PLAYER_START = 0; // �v���C���[�̏����ʒu��\���^�C��ID
const int TILE_ID_FLOOR = 1; // ���^�C���������^�C��ID
const int TILE_ID_WALL = 2; // �ǃ^�C��(Player, Block���ړ��ł��Ȃ�)������ID
const int TIlE_ID_BLOCK = 3; // �u���b�N�������^�C��ID

// �N���X�̑O���錾
class Player;
class TextureManager;
class Camera;
class Map;
class Block;

// ���C�����[�v�A�������A�I�������A�C�x���g�����A�X�V�A�`��̊Ǘ�
class Game {
public:
	Game(); // �R���X�g���N�^
	~Game(); // �f�X�g���N�^

	bool Initialize(); // �Q�[���̏�����
	void RunLoop(); // �Q�[���̃��C�����[�v
	void Shutdown(); // �Q�[���̏I������

private:
	void ProcessInput(); // ���͏���
	void UpdateGame(); // �Q�[���̍X�V����
	void Render(); // �`�揈��

	// SDL�֘A�̃����o�[�ϐ�
	SDL_Window* window_; // �E�B���h�E�I�u�W�F�N�g
	SDL_Renderer* renderer_; // �����_���[�I�u�W�F�N�g

	// �J�X�^���}�l�[�W���[
	TextureManager* textureManager_; // �e�N�X�`���Ǘ��N���X

	// �L�[�{�[�h�̏�Ԃ���ێ�����|�C���^
	const Uint8* keyboardState_; // ���݂̃L�[������Ԃ𒼐ڎQ��
	int numKeys_; // �L�[�̑���

	// �Q�[����ԃt���O
	bool isRunning_; // �Q�[�������s�����ǂ���
	Uint64 lastTick_; //�O��̃t���[������̌o�ߎ��Ԃ��v�Z���邽�߂̕ϐ�

	// �Q�[���I�u�W�F�N�g�̊Ǘ�(unique_ptr�Ŏ����������Ǘ�)
	std::unique_ptr<Player> player_; // �v���C���[�I�u�W�F�N�g
	std::unique_ptr<Map> map_; // �}�b�v�I�u�W�F�N�g
	std::unique_ptr<Camera> camera_; // �J�����I�u�W�F�N�g(�`��͈͂𐧌�)
	std::vector<std::unique_ptr<Block>> blocks_; // �u���b�N���Ǘ����邽�߂̃R���e�i
};

#endif // GAME_H