#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include <SDL3/SDL.h>

class Camera {
public:
	Camera(float x, float y, float w, float h, float mapW, float mapH); // �R���X�g���N�^

	void Update(float targetX, float targetY, float targetW, float targetH); // �X�V����
	
	float GetX() const { return cameraRect_.x; } // �J������X���W���擾
	float GetY() const { return cameraRect_.y; } // �J������Y���W���擾
	float GetW() const { return cameraRect_.w; } // �J�����̕����擾
	float GetH() const { return cameraRect_.h; } // �J�����̍������擾

private:
	SDL_FRect cameraRect_; // �J�����̃r���[�|�[�g
	
	float mapW_; // �}�b�v�̕�
	float mapH_; // �}�b�v�̍���
};

#endif // CAMERA_H
