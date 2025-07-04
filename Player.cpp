#include "Player.h"
#include "Camera.h"
#include "Map.h"
#include "Block.h"
#include "Game.h"
#include <algorithm>
#include <cmath>
#include <SDL3/SDL_log.h>

// コンストラクタ
Player::Player(SDL_Texture* texture, int startGridX, int startGridY, int w, int h, int mapWP, int mapHP, Map* map)
    : texture_(texture), // プレイヤーのテクスチャを初期化
    currentGridX_(startGridX), // 開始グリッドX座標を初期化
    currentGridY_(startGridY), // 開始グリッドY座標を初期化
    // 描画矩形 (destRect_) を初期グリッド位置のピクセル座標と、引数で渡された幅(w)・高さ(h)で初期化
    destRect_({ (float)startGridX * TILE_W, (float)startGridY * TILE_H, (float)w, (float)h }),
    speed_(TILE_W * 4.0f), // 移動速度をタイル幅の4倍/秒で設定
    mapWP_(mapWP), // マップ全体の幅（ピクセル）を初期化
    mapHP_(mapHP), // マップ全体の高さ（ピクセル）を初期化
    map_(map), // マップオブジェクトへのポインタを初期化
    isGridMoving_(false), // 最初はグリッド移動中ではない
    moveProgress_(0.0f) // 移動進行度を0で初期化
{
    // グリッド移動の開始位置と目標位置を現在のグリッド位置で初期化
    startGridPos_ = { currentGridX_,currentGridY_ };
    targetGridPos_ = { currentGridX_,currentGridY_ };
}

// デストラクタ
Player::~Player() {}

// 移動試行関数
bool Player::TryStartMove(int dx, int dy, std::vector<std::unique_ptr<Block>>* blocks) {
    // 移動方向が指定されていなければ何もしない
    if (dx == 0 && dy == 0) return false;

    // 目標とする新しいグリッド座標を計算
    int nextGridX = currentGridX_ + dx;
    int nextGridY = currentGridY_ + dy;

    // マップ境界チェック
    // 目標地点がマップの有効な範囲内にあるか確認
    if (nextGridX < 0 || nextGridX >= map_->GetMapCols() ||
        nextGridY < 0 || nextGridY >= map_->GetMapRows()) {
        //SDL_Log("Player not Move");
        return false; // マップ境界外への移動不可
    }

    // 目標地点のタイルIDを取得
    int nextTileID = map_->GetTileID(nextGridX, nextGridY);

    // ブロックのチェック
    // 目標地点にブロックがあるか検索
    Block* collidedBlock = nullptr;
    for (const auto& block : *blocks) {
        if (block->GetGridX() == nextGridX && block->GetGridY() == nextGridY) {
            collidedBlock = block.get(); // ブロックが見つかった
            break;
        }
    }

    // 移動先にブロックがある場合
    if (collidedBlock) {
        // ブロックが押し出される先の新しいグリッド座標を計算
        int blockNewGridX = nextGridX + dx;
        int blockNewGridY = nextGridY + dy;

        // ブロックの移動先がマップ境界内かチェック
        if (blockNewGridX < 0 || blockNewGridX >= map_->GetMapCols() ||
            blockNewGridY < 0 || blockNewGridY >= map_->GetMapRows()) {
            //SDL_Log("Block not out push");
            return false; // ブロックを押し出せない
        }

        // ブロックの移動先タイルが壁かチェック
        int blockNewTileID = map_->GetTileID(blockNewGridX, blockNewGridY);
        if (blockNewTileID == TILE_ID_WALL) {
            //SDL_Log("not push");
            return false; // ブロックを押し出せない
        }

        // ブロックの移動先に既に別のブロックがないかチェック
        bool blockTargetOccupiedByAnotherBlock = false;
        for (const auto& block : *blocks) {
            // 現在押し出そうとしているブロック自身と異なるブロックで、かつ同じ位置にあるかをチェック
            if (block.get() != collidedBlock &&
                block->GetGridX() == blockNewGridX && block->GetGridY() == blockNewGridY) {
                blockTargetOccupiedByAnotherBlock = true;
                break;
            }
        }

        // ブロックを押し出すことが可能であれば、ブロックの位置を更新
        if (!blockTargetOccupiedByAnotherBlock) {
            collidedBlock->SetGridPosition(blockNewGridX, blockNewGridY);
            //SDL_Log("プレイヤーがブロックを(%d,%d)から(%d,%d)へ押し出した",nextGridX,nextGridY,blockNewGridX,blockNewGridY);

            // プレイヤー自身も移動を開始
            targetGridPos_ = { nextGridX,nextGridY }; // プレイヤーの目標位置決定
            isGridMoving_ = true; // グリッド移動アニメーションん開始
            moveProgress_ = 0.0f; // 移動進行度をリセット
            return true; // 移動性向
        }
        else {
            //SDL_Log("ブロックの移動先に別のブロックがある");
            return false; // ブロックを押し出せない
        }
    }

    // 移動先が壁の場合、移動を許可しない
    else if (nextTileID == TILE_ID_WALL) {
        //SDL_Log("壁に衝突しました");
        return false; // 移動を許可しない
    }

    // 移動先が床の場合、移動をそのまま開始
    else {
        targetGridPos_ = { nextGridX,nextGridY }; // プレイヤーの目標位置決定
        isGridMoving_ = true; // グリッド移動アニメーションん開始
        moveProgress_ = 0.0f; // 移動進行度をリセット
        return true; // 移動性向
    }
}

// 更新処理
void Player::Update(float deltaTime, const Uint8* keyboardState, std::vector<std::unique_ptr<Block>>* blocks) {
    // プレイヤーが現在グリッド移動アニメーション中の場合
    if (isGridMoving_) {
        // 移動速度と経過時間に基づいて、このフレームでの移動量を計算
        float moveAmountThisFrame = speed_ * deltaTime;
        // 移動進行度を更新(0.0fから1.0fまでの範囲)
        moveProgress_ += moveAmountThisFrame / TILE_W;

        // 移動が1マス分完了したかチェック
        if (moveProgress_ >= 1.0f) {
            // 移動が完了した場合、プレイヤーのピクセル座標を目標グリッド位置に設定
            destRect_.x = (float)targetGridPos_.x * TILE_W;
            destRect_.y = (float)targetGridPos_.y * TILE_H;
            isGridMoving_ = false; // 移動完了
            moveProgress_ = 0.0f; // 移動進行度リセット
            currentGridX_ = targetGridPos_.x; // 現在のグリッド位置を目標位置に更新
            currentGridY_ = targetGridPos_.y;
            startGridPos_ = targetGridPos_; // 次の移動の開始位置を現在の位置に更新

            // 連続移動のロジック:
            // 1マス移動完了後、ユーザーがキーを押し続けている場合、すぐに次の移動を試みる
            int dx_attempt = 0;
            int dy_attempt = 0;

            if (keyboardState[SDL_SCANCODE_W]) dy_attempt = -1; // Wキーが押されている場合（上）
            else if (keyboardState[SDL_SCANCODE_S]) dy_attempt = 1; // Sキーが押されている場合（下）
            else if (keyboardState[SDL_SCANCODE_A]) dx_attempt = -1; // Aキーが押されている場合（左）
            else if (keyboardState[SDL_SCANCODE_D]) dx_attempt = 1; // Dキーが押されている場合（右）

            // 何らかの方向キーが押されている場合のみ、次の移動をTryStartMove関数で試行
            if (dx_attempt != 0 || dy_attempt != 0) {
                TryStartMove(dx_attempt, dy_attempt, blocks);
                // TryStartMoveが成功すればisGridMoving_がtrueになり、次の移動アニメーションが開始
            }
        }
        else {
            // 移動が完了していない場合、現在のピクセル位置を線形補間で計算
            destRect_.x = (float)startGridPos_.x * TILE_W + (targetGridPos_.x - startGridPos_.x) * TILE_W * moveProgress_;
            destRect_.y = (float)startGridPos_.y * TILE_H + (targetGridPos_.y - startGridPos_.y) * TILE_H * moveProgress_;
        }
    }
    else {
        // プレイヤーがグリッド移動アニメーション中でない場合
        // 現在押されているキーを確認し、新しい移動を開始できるか試みる
        int dx_current = 0;
        int dy_current = 0;

        // keyboardState配列を直接参照して、現在のキーの押下状態を確認し、移動方向を決定
        if (keyboardState[SDL_SCANCODE_W]) dy_current = -1;
        else if (keyboardState[SDL_SCANCODE_S]) dy_current = 1;
        else if (keyboardState[SDL_SCANCODE_A]) dx_current = -1;
        else if (keyboardState[SDL_SCANCODE_D]) dx_current = 1;

        // 何らかのキーが押されている場合のみ、TryStartMove関数で移動を試みる
        if (dx_current != 0 || dy_current != 0) {
            TryStartMove(dx_current, dy_current, blocks);
            // TryStartMoveが成功すればisGridMoving_がtrueになり、移動アニメーションが開始
        }
    }

    // プレイヤーのピクセル座標をマップ境界内にクランプ。
    destRect_.x = std::max(0.0f, destRect_.x);
    destRect_.x = std::min(static_cast<float>(mapWP_ - TILE_W), destRect_.x);
    destRect_.y = std::max(0.0f, destRect_.y);
    destRect_.y = std::min(static_cast<float>(mapHP_ - TILE_H), destRect_.y);
}

// プレイヤーの現在のグリッド座標を取得
SDL_Point Player::GetGridPosition() const {
    // ピクセル座標をタイルサイズで割り、最も近い整数に丸める
    return { (int)std::round(destRect_.x / TILE_W), (int)std::round(destRect_.y / TILE_H) };
}

// プレイヤーの描画処理
void Player::Render(SDL_Renderer* renderer, Camera* camera) {
    // カメラのオフセットを適用して描画位置を計算
    SDL_FRect renderRect = destRect_; // 描画用の矩形をプレイヤーの現在の位置とサイズで初期化
    renderRect.x -= camera->GetX(); // カメラのXオフセットを適用
    renderRect.y -= camera->GetY(); // カメラのYオフセットを適用

    SDL_RenderTexture(renderer, texture_, nullptr, &renderRect); // テクスチャを描画
}