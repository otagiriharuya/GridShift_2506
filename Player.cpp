#include "Player.h"
#include "Camera.h"
#include "Map.h"
#include "Block.h"
#include "Game.h"
#include <algorithm> // std::min, std::maxを使用するために必要

// コンストラクタ
Player::Player(SDL_Texture* texture, int startGridX, int startGridY, int w, int h, int mapWP, int mapHP, Map* map)
    : texture_(texture),
    currentGridX_(startGridX), currentGridY_(startGridY), // グリッド座標初期化
    destRect_({ (float)startGridX * TILE_W,(float)startGridY * TILE_H,(float)w,(float)h }), // ピクセル座標に変化して初期化
    speed_(TILE_W * 4.0f), // 1秒で4マス移動する速度
    mapWP_(mapWP), mapHP_(mapHP), // マップのピクセル初期化
    map_(map),
    nextMoveDx_(0),nextMoveDy_(0), // 次の移動方向バッファを初期化
    isGridMoving_(false),moveProgress_(0.0f)
{
    // 移動開始時のグリッド座標を初期化
    startGridPos_ = { currentGridX_,currentGridY_ };
    // 目標グリッド座標も初期状態では現在地と同じ
    targetGridPos_ = { currentGridX_,currentGridY_ };
}

// デストラクタ
Player::~Player() {}

bool Player::TryStartMove(int dx, int dy, std::vector<std::unique_ptr<Block>>* blocks) {
    // 移動方向が指定されていなければ何もしない
    if (dx == 0 && dy == 0) return false;

    int nextGridX = currentGridX_ + dx;
    int nextGridY = currentGridY_ + dy;

    // マップ境界チェック
    if (nextGridX < 0 || nextGridX >= map_->GetMapCols() ||
        nextGridY < 0 || nextGridY >= map_->GetMapRows()) {
        //SDL_Log("Player not Move");
        return false;
    }

    int nextTileID = map_->GetTileID(nextGridX, nextGridY);

    // ブロックのチェック
    Block* collidedBlock = nullptr;
    for (const auto& block : *blocks) {
        if (block->GetGridX() == nextGridX && block->GetGridY() == nextGridY) {
            collidedBlock = block.get();
            break;
        }
    }

    // 移動先にブロックがある場合
    if (collidedBlock) {
        int blockNewGridX = nextGridX + dx;
        int blockNewGridY = nextGridY + dy;

        // ブロックの移動先がマップ境界内かチェック
        if (blockNewGridX < 0 || blockNewGridX >= map_->GetMapCols() ||
            blockNewGridY < 0 || blockNewGridY >= map_->GetMapRows()) {
            //SDL_Log("Block not out push");
            return false;
        }

        // ブロックの移動先タイルが壁かチェック
        int blockNewTileID = map_->GetTileID(blockNewGridX, blockNewGridY);
        if (blockNewTileID == TILE_ID_WALL) {
            //SDL_Log("not push");
            return false;
        }

        // ブロックの移動先に別のブロックがないかチェック
        bool blockTargetOccupiedByAnotherBlock = false;
        for (const auto& block : *blocks) {
            if (block.get() != collidedBlock &&
                block->GetGridX() == blockNewGridX && block->GetGridY() == blockNewGridY) {
                blockTargetOccupiedByAnotherBlock = true;
                break;
            }
        }

        if (!blockTargetOccupiedByAnotherBlock) {
            collidedBlock->SetGridPosition(blockNewGridX, blockNewGridY);
            //SDL_Log("プレイヤーがブロックを(%d,%d)から(%d,%d)へ押し出した",nextGridX,nextGridY,blockNewGridX,blockNewGridY);

            targetGridPos_ = { nextGridX,nextGridY };
            isGridMoving_ = true;
            moveProgress_ = 0.0f;
            return true;
        }
        else {
            //SDL_Log("ブロックの移動先に別のブロックがある");
            return false;
        }
    }

    // 移動先が壁の場合
    else if (nextTileID == TILE_ID_WALL) {
        //SDL_Log("壁に衝突しました");
        return false;
    }

    // 移動先が床の場合
    else {
        targetGridPos_ = { nextGridX,nextGridY };
        isGridMoving_ = true;
        moveProgress_ = 0.0f;
        return true;
    }
}

// プレイヤーの更新処理
void Player::Update(float deltaTime, const Uint8* keyboardState, std::vector<std::unique_ptr<Block>>* blocks) {
    if (isGridMoving_) {
        // グリッド移動中の処理
        float moveAmountThisFrame = speed_ * deltaTime;
        moveProgress_ += moveAmountThisFrame / TILE_W;

        if (moveProgress_ >= 1.0f) {
            // 1マス移動完了
            destRect_.x = (float)targetGridPos_.x * TILE_W;
            destRect_.y = (float)targetGridPos_.y * TILE_H;
            isGridMoving_ = false; // 移動完了
            moveProgress_ = 0.0f;
            currentGridX_ = targetGridPos_.x;
            currentGridY_ = targetGridPos_.y;
            startGridPos_ = targetGridPos_;

            // 移動完了後、すぐに次の移動を試みる
            // keyboardStateを使って建材のキーの状態を直接確認
            int dx_attempt = 0;
            int dy_attempt = 0;

            if (keyboardState[SDL_SCANCODE_W]) dy_attempt = -1;
            else if (keyboardState[SDL_SCANCODE_S]) dy_attempt = 1;
            else if (keyboardState[SDL_SCANCODE_A]) dx_attempt = -1;
            else if (keyboardState[SDL_SCANCODE_D]) dx_attempt = 1;

            // 何らかの方向キーが押されている場合のみ試行
            if (dx_attempt != 0 || dy_attempt != 0) {
                TryStartMove(dx_attempt, dy_attempt, blocks);
            }
        }
    }
    else {
        // グリッド移動中でない場合 (停止中)
        int dx_current = 0;
        int dy_current = 0;

        // keyboardStateを使って現在のキーの状態を直接確認
        if (keyboardState[SDL_SCANCODE_W]) dy_current = -1;
        else if (keyboardState[SDL_SCANCODE_S]) dy_current = 1;
        else if (keyboardState[SDL_SCANCODE_A]) dx_current = -1;
        else if (keyboardState[SDL_SCANCODE_D]) dx_current = 1;

        // 何らかのキーが押されている場合
        if (dx_current != 0 || dy_current != 0) {
            TryStartMove(dx_current, dy_current, blocks);
        }
    }

    // プレイヤーのピクセル座標をマップ境界内にクランプ
    destRect_.x = std::max(0.0f, destRect_.x);
    destRect_.x = std::min(static_cast<float>(mapWP_ - TILE_W), destRect_.x);
    destRect_.y = std::max(0.0f, destRect_.y);
    destRect_.y = std::min(static_cast<float>(mapHP_ - TILE_H), destRect_.y);
}

// グリッド位置計算
SDL_Point Player::GetGridPosition() const {
    return {currentGridX_,currentGridY_ };
}

// プレイヤーの描画処理
void Player::Render(SDL_Renderer* renderer, Camera* camera) {
    // カメラのオフセットを適用して描画位置を計算
    if (texture_) {
        SDL_FRect renderRect = destRect_;
        renderRect.x -= camera->GetX();
        renderRect.y -= camera->GetY();
        SDL_RenderTexture(renderer, texture_, NULL, &renderRect);
    }
}