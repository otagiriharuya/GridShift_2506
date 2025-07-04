#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include <SDL3/SDL.h>
#include <algorithm>

// Camera�N���X
// �Q�[���̎��_���Ǘ��v���C���[�Ȃǂ̃I�u�W�F�N�g��ǐՂ��A��ʕ\���𒲐�
class Camera {
public:
	// �R���X�g���N�g
	// �J�����̏����ʒu�A�J�����̃r���[�|�[�g�T�C�Y
	Camera(float x, float y, float w, float h);

	// �J�����̈ʒu�X�V���A�Ώۂ�ǐՂ���悤�ɂ���
	// �ǐՑΏۂ̌��݂̃��[���h�ʒu�A�ǐՑΏۂ̕��ƍ����A�}�b�v�͕̂��ƍ���
	void Update(float targetX, float targetY, float targetW, float targetH, int mapW, int mapH);
	
    // �J�����̌��݂�X���W���擾
    float GetX() const { return x_; }
    // �J�����̌��݂�Y���W���擾
    float GetY() const { return y_; }
    // �J�����̃r���[�|�[�g�����擾
    int GetWidth() const { return width_; }
    // �J�����̃r���[�|�[�g�������擾
    int GetHeight() const { return height_; }

private:
    float x_; // �J�����̌��݂̃��[���hX���W
    float y_; // �J�����̌��݂̃��[���hY���W
    int width_; // �J�����̃r���[�|�[�g��
    int height_; // �J�����̃r���[�|�[�g����
};

#endif // CAMERA_H
