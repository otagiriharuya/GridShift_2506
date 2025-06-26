#include "Player.h"
#include "Camera.h"
#include <cmath>
#include <algorithm> // std::min, std::maxを使用するために必要

// コンストラクタ
Player::Player(SDL_Texture* texture, float x, float y, float w, float h, float mapW, float mapH)
	: texture_(texture), speed_(200.0f), // プレイヤーのステータス初期化
    isMoveUp_(false),isMoveDown_(false),isMoveLeft_(false), isMoveRight_(false), // 移動フラグ初期化
	mapW_(mapW), mapH_(mapH) // マップの幅と高さを設定
{
	destRect_ = { x,y,w,h };
}

// デストラクタ
Player::~Player() {}

// イベント処理
void Player::HandleInput(const SDL_Event& ev) {
	if (ev.type == SDL_EVENT_KEY_DOWN) { // キーが押されたら
        if (ev.key.key == SDLK_UP || ev.key.scancode == SDL_SCANCODE_W) {
            isMoveUp_ = true;
            SDL_Log("W pressed.isMoveingUp_ = true");
        }
        if (ev.key.key == SDLK_DOWN || ev.key.scancode == SDL_SCANCODE_S) {
            isMoveDown_ = true;
        }
        if (ev.key.key == SDLK_LEFT || ev.key.scancode == SDL_SCANCODE_A) {
            isMoveLeft_ = true;
        }
        if (ev.key.key == SDLK_RIGHT || ev.key.scancode == SDL_SCANCODE_D) {
            isMoveRight_ = true;
        }
	}
    else if (ev.type == SDL_EVENT_KEY_UP) { // キーが離されたら
        if ((ev.key.key == SDLK_UP || ev.key.scancode == SDL_SCANCODE_W)) {
            isMoveUp_ = false;
        }
        if ((ev.key.key == SDLK_DOWN || ev.key.scancode == SDL_SCANCODE_S)) {
            isMoveDown_ = false;
        }
        if ((ev.key.key == SDLK_LEFT || ev.key.scancode == SDL_SCANCODE_A)) {
            isMoveLeft_ = false;
        }
        if ((ev.key.key == SDLK_RIGHT || ev.key.scancode == SDL_SCANCODE_D)) {
            isMoveRight_ = false;
        }
    }
}

// プレイヤーの更新処理
void Player::Update(float deltaTime) {
    // 毎フレーム、移動量をゼロにリセットし、現在のキーの状態に基づいて再計算
    float currentMoveX = 0.0f;
	float currentMoveY = 0.0f;

    if (isMoveUp_) {
		currentMoveY = -1.0f; // 上方向に移動
    }
    if (isMoveDown_) {
        if (isMoveUp_) currentMoveY = 0.0f; 
        else currentMoveY = 1.0f;
    }

    if (isMoveLeft_) {
        currentMoveX = -1.0f;
    }
    if (isMoveRight_) {
        if (isMoveLeft_) currentMoveX = 0.0f;
        else currentMoveX = 1.0f;
    }

    // 斜め移動の速度正規化
    if (currentMoveX != 0.0f || currentMoveY != 0.0f) {
        float length = std::sqrt(currentMoveX * currentMoveX + currentMoveY * currentMoveY);
        currentMoveX /= length;
        currentMoveY /= length;
    }

    destRect_.x += currentMoveX * speed_ * deltaTime; // X方向の移動
    destRect_.y += currentMoveY * speed_ * deltaTime; // Y方向の移動

    // マップ境界で停止処理
    destRect_.x = std::max(0.0f, std::min(destRect_.x, mapW_ - destRect_.w));
    destRect_.y = std::max(0.0f, std::min(destRect_.y, mapH_ - destRect_.h));

}

// プレイヤーの描画処理
void Player::Render(SDL_Renderer* renderer, Camera* camera) {
    // カメラのオフセットを適用して描画位置を計算
    SDL_FRect renderRect = {
        destRect_.x - camera->GetX(),
        destRect_.y - camera->GetY(),
        destRect_.w,
        destRect_.h
    };
    SDL_RenderTexture(renderer, texture_, NULL, &renderRect);
}