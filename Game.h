#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <memory>

// �N���X�錾
class Player;
class TextureManager;
class Camera;
class Map;
class Block;

const int TILE_W = 32;
const int TILE_H = 32;
const int TILE_ID_FLOOR = 0; // ���^�C��
const int TILE_ID_WALL = 1; // �ǃ^�C��(Player, Block���ړ��ł��Ȃ�)
const int TILE_ID_BLOCK_INITIAL = 2; // �}�b�v�t�@�C���Ńu���b�N�̏����ʒu������ID (������ɏ��ɕϊ�)

class Game {
public:
	Game(); // �R���X�g���N�^
	~Game(); // �f�X�g���N�^

	bool Init(); // ����������
	void Run(); // �Q�[�����[�v
	void Events(); // �C�x���g����
	void Update(); // �X�V����
	void Render(); // �`�揈��
	void Clean(); // �N���[���A�b�v����

private:
	SDL_Window* window_; // �E�B���h�E
	SDL_Renderer* renderer_; // �����_���[
	bool isRunning_; // �Q�[�������s�����ǂ���

	// �Q�[���I�u�W�F�N�g�̃C���X�^���X
	Player* player_; // �v���C���[�I�u�W�F�N�g
	TextureManager* textureManager_; // �e�N�X�`���}�l�[�W���[
	Camera* camera_; // �J�����I�u�W�F�N�g
	Map* map_; // �}�b�v�I�u�W�F�N�g

	// �t���[�����[�g����p
	Uint64 frameStartTime_; // �t���[���J�n����
	Uint64 frameEndTime_; // �t���[���I������
	float frameDuration_; // �t���[���̎�������
	Uint64 frameDelayTicks_; // �t���[���x���e�B�b�N��

	float deltaTime; // �e�t���[���̎��ԍ���ێ�

	std::vector<std::unique_ptr<Block>> blocks_;

	// �萔
	const std::string WINDOW_TITLE = "Grid Shift"; // �E�B���h�E�^�C�g��
	const int WINDOW_W = 640; // ��ʕ�
	const int WINDOW_H = 360; // ��ʍ���
	const float FPS = 60.0f; // �ڕWFPS
	const float MS_PER_FRAME = 1000.0f / FPS; // 1�t���[��������̖ڕW�~���b
};

#endif // GAME_H