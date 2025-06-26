#include "Camera.h"
#include <algorithm>

Camera::Camera(float x, float y, float width, float height, float mapW, float mapH)
    : cameraRect_({ x, y, width, height }), //�J�����̏�����
    mapW_(mapW), mapH_(mapH) // �}�b�v�̏�����
{    
}

// targetX, targetY �̓v���C���[�Ȃǂ̒��S���W��z��
void Camera::Update(float targetX, float targetY, float targetW, float targetH) {
    // �v���C���[����ʂ̒����ɔz�u����悤�ɃJ�������ړ�
    cameraRect_.x = targetX - (cameraRect_.w / 2) + (targetW / 2);
    cameraRect_.y = targetY - (cameraRect_.h / 2) + (targetH / 2);

    // �J�����̈ʒu���}�b�v�͈̔͂������Ȃ��悤�ɐ���
    cameraRect_.x = std::max(0.0f, std::min(cameraRect_.x, mapW_ - cameraRect_.w));
    cameraRect_.y = std::max(0.0f, std::min(cameraRect_.y, mapH_ - cameraRect_.h));
}