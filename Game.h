#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>

// �N���X�錾
class Player;
class TextureManager;
class Camera;
class Map;

constexpr int TILE_W = 32;
constexpr int TILE_H = 32;

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

	SDL_Renderer* GetRenderer() const { return renderer_; } // �����_���[�擾

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
	Uint64 frameDuration_; // �t���[���̎�������
	Uint64 frameDelayTicks_; // �t���[���x���e�B�b�N��

	float deltaTime; // �e�t���[���̎��ԍ���ێ�

	// �萔
	const int WINDOW_W = 640; // ��ʕ�
	const int WINDOW_H = 480; // ��ʍ���
	const float MAP_W = 1280.0f; // �}�b�v�̕�
	const float MAP_H = 960.0f; // �}�b�v�̍���
	const std::string WINDOW_TITLE = "Grid Shift"; // �E�B���h�E�^�C�g��
	const int FPS = 60; // �t���[�����[�g

};

#endif // GAME_H