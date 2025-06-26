#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include <SDL3/SDL.h>

class Camera {
public:
	Camera(float x, float y, float w, float h, float mapW, float mapH); // コンストラクタ

	void Update(float targetX, float targetY, float targetW, float targetH); // 更新処理
	
	float GetX() const { return cameraRect_.x; } // カメラのX座標を取得
	float GetY() const { return cameraRect_.y; } // カメラのY座標を取得
	float GetW() const { return cameraRect_.w; } // カメラの幅を取得
	float GetH() const { return cameraRect_.h; } // カメラの高さを取得

private:
	SDL_FRect cameraRect_; // カメラのビューポート
	
	float mapW_; // マップの幅
	float mapH_; // マップの高さ
};

#endif // CAMERA_H
