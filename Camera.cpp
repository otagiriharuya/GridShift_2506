#include "Camera.h"
#include "Game.h"
#include <algorithm>

// コンストラクタ
Camera::Camera(float x, float y, int width, int height)
    : x_(x), // カメラのワールドX座標を初期化
    y_(y), // カメラのワールドY座標を初期化
    width_(width), // カメラのビューポート幅を初期化
    height_(height) // カメラのビューポート高さを初期化
{}

// カメラの位置を更新
void Camera::Update(float targetX, float targetY, int targetW, int targetH, int mapW, int mapH) {
    // 追跡対象（プレイヤー）の中心が画面の中心に来るようにカメラを調整します。
    // targetXとtargetYはターゲットの左上隅の座標なので、中心を計算します。
    x_ = targetX + (float)targetW / 2 - (float)width_ / 2;
    y_ = targetY + (float)targetH / 2 - (float)height_ / 2;

    // カメラがマップの左端や上端を越えないようにクランプします。
    // カメラのX座標は0以上である必要があります。
    x_ = std::max(0.0f, x_);
    // カメラのY座標は0以上である必要があります。
    y_ = std::max(0.0f, y_);

    // カメラがマップの右端や下端を越えないようにクランプします。
    // カメラの右端がマップの右端を超えないように調整
    x_ = std::min(x_, (float)mapW - width_);
    // カメラの下端がマップの下端を超えないように調整
    y_ = std::min(y_, (float)mapH - height_);

    // マップが画面より小さい場合の特殊処理
    // マップ幅がカメラビューポート幅よりも小さい場合、カメラのX座標は0に固定
    if (mapW < width_) {
        x_ = 0;
    }
    // マップ高さがカメラビューポート高さよりも小さい場合、カメラのY座標は0に固定
    if (mapH < height_) {
        y_ = 0;
    }
}