#include "Camera.h"
#include <algorithm>

Camera::Camera(float x, float y, float width, float height, float mapW, float mapH)
    : cameraRect_({ x, y, width, height }), //カメラの初期化
    mapW_(mapW), mapH_(mapH) // マップの初期化
{    
}

// targetX, targetY はプレイヤーなどの中心座標を想定
void Camera::Update(float targetX, float targetY, float targetW, float targetH) {
    // プレイヤーを画面の中央に配置するようにカメラを移動
    cameraRect_.x = targetX - (cameraRect_.w / 2) + (targetW / 2);
    cameraRect_.y = targetY - (cameraRect_.h / 2) + (targetH / 2);

    // カメラの位置がマップの範囲が超えないように制限
    cameraRect_.x = std::max(0.0f, std::min(cameraRect_.x, mapW_ - cameraRect_.w));
    cameraRect_.y = std::max(0.0f, std::min(cameraRect_.y, mapH_ - cameraRect_.h));
}