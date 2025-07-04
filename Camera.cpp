#include "Camera.h"
#include "Game.h"
#include <algorithm>

// �R���X�g���N�^
Camera::Camera(float x, float y, int width, int height)
    : x_(x), // �J�����̃��[���hX���W��������
    y_(y), // �J�����̃��[���hY���W��������
    width_(width), // �J�����̃r���[�|�[�g����������
    height_(height) // �J�����̃r���[�|�[�g������������
{}

// �J�����̈ʒu���X�V
void Camera::Update(float targetX, float targetY, int targetW, int targetH, int mapW, int mapH) {
    // �ǐՑΏہi�v���C���[�j�̒��S����ʂ̒��S�ɗ���悤�ɃJ�����𒲐����܂��B
    // targetX��targetY�̓^�[�Q�b�g�̍�����̍��W�Ȃ̂ŁA���S���v�Z���܂��B
    x_ = targetX + (float)targetW / 2 - (float)width_ / 2;
    y_ = targetY + (float)targetH / 2 - (float)height_ / 2;

    // �J�������}�b�v�̍��[���[���z���Ȃ��悤�ɃN�����v���܂��B
    // �J������X���W��0�ȏ�ł���K�v������܂��B
    x_ = std::max(0.0f, x_);
    // �J������Y���W��0�ȏ�ł���K�v������܂��B
    y_ = std::max(0.0f, y_);

    // �J�������}�b�v�̉E�[�≺�[���z���Ȃ��悤�ɃN�����v���܂��B
    // �J�����̉E�[���}�b�v�̉E�[�𒴂��Ȃ��悤�ɒ���
    x_ = std::min(x_, (float)mapW - width_);
    // �J�����̉��[���}�b�v�̉��[�𒴂��Ȃ��悤�ɒ���
    y_ = std::min(y_, (float)mapH - height_);

    // �}�b�v����ʂ�菬�����ꍇ�̓��ꏈ��
    // �}�b�v�����J�����r���[�|�[�g�������������ꍇ�A�J������X���W��0�ɌŒ�
    if (mapW < width_) {
        x_ = 0;
    }
    // �}�b�v�������J�����r���[�|�[�g���������������ꍇ�A�J������Y���W��0�ɌŒ�
    if (mapH < height_) {
        y_ = 0;
    }
}