#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include <SDL3/SDL.h>
#include <algorithm>

// Cameraクラス
// ゲームの視点を管理プレイヤーなどのオブジェクトを追跡し、画面表示を調整
class Camera {
public:
	// コンストラクト
	// カメラの初期位置、カメラのビューポートサイズ
	Camera(float x, float y, float w, float h);

	// カメラの位置更新し、対象を追跡するようにする
	// 追跡対象の現在のワールド位置、追跡対象の幅と高さ、マップのは幅と高さ
	void Update(float targetX, float targetY, float targetW, float targetH, int mapW, int mapH);
	
    // カメラの現在のX座標を取得
    float GetX() const { return x_; }
    // カメラの現在のY座標を取得
    float GetY() const { return y_; }
    // カメラのビューポート幅を取得
    int GetWidth() const { return width_; }
    // カメラのビューポート高さを取得
    int GetHeight() const { return height_; }

private:
    float x_; // カメラの現在のワールドX座標
    float y_; // カメラの現在のワールドY座標
    int width_; // カメラのビューポート幅
    int height_; // カメラのビューポート高さ
};

#endif // CAMERA_H
