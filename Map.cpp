#include "Map.h"
#include "Game.h"
#include "Camera.h"
#include <fstream> // ファイル読み込みのために必要
#include <sstream> // 文字列ストリームのために必要
#include <algorithm>
#include <SDL3/SDL_log.h>

// コンストラクタ
Map::Map(SDL_Texture* texture)
    : tilesetTexture_(texture), // タイルセットのテクスチャを初期化
    mapCols_(0), // マップの列数を0で初期化
    mapRows_(0) // マップの行数を0で初期化
{}

// デストラクタ
Map::~Map() {}

// CSVファイルからマップデータをロード
bool Map::LoadMap(const std::string& filePath) {
    std::ifstream file(filePath); // ファイルを開く
    if (!file.is_open()) {
        //SDL_Log("マップファイルを開けませんでした: %s", filePath.c_str());
        return false; // ファイルオープン失敗
    }

    std::string line;
    mapRows_ = 0;
    mapCols_ = 0;
    mapData_.clear(); // 既存のマップデータをクリア

    // ファイルから1行ずつ読み込みます
    while (std::getline(file, line)) {
        if (line.empty()) continue; // 空行はスキップ

        std::stringstream ss(line); // 行を文字列ストリームに変換
        std::string segment;
        std::vector<int> row;
        int currentCols = 0;

        // カンマ区切りで数値を読み込みます
        while (std::getline(ss, segment, ',')) {
            try {
                row.push_back(std::stoi(segment)); // 文字列を整数に変換して行に追加
                currentCols++;
            }
            catch (const std::exception& e) {
                //SDL_Log("マップデータの解析エラー: %s (行: %s)", e.what(), line.c_str());
                return false;
            }
        }
        mapData_.push_back(row); // 1行分のデータをマップデータに追加

        // 最初の行で列数を設定します。以降の行は同じ列数であることを確認します。
        if (mapRows_ == 0) {
            mapCols_ = currentCols;
        }
        else if (mapCols_ != currentCols) {
            //SDL_Log("マップの列数が一致しません。期待値: %d, 実際: %d (行: %d)", mapCols_, currentCols, mapRows_);
            return false; // 列数不一致エラー
        }
        mapRows_++; // 行数をインクリメント
    }

    file.close(); // ファイルを閉じる
    //SDL_Log("マップのロードが完了しました。サイズ: %d x %d", mapCols_, mapRows_);
    return true; // ロード成功
}

// マップの描画
void Map::Render(SDL_Renderer* renderer, Camera* camera) {
    if (!tilesetTexture_ || mapData_.empty()) {
        return; // テクスチャがない、またはマップデータがない場合は描画しない
    }

    // カメラのビューポートに基づいて描画範囲を決定
    int startCol = std::max(0, (int)(camera->GetX() / TILE_W));
    int endCol = std::min(mapCols_, (int)((camera->GetX() + camera->GetWidth()) / TILE_W) + 1);
    int startRow = std::max(0, (int)(camera->GetY() / TILE_H));
    int endRow = std::min(mapRows_, (int)((camera->GetY() + camera->GetHeight()) / TILE_H) + 1);

    // 計算した範囲内の各タイルを描画します
    for (int y = startRow; y < endRow; ++y) {
        for (int x = startCol; x < endCol; ++x) {
            int tileID = GetTileID(x, y); // 現在のグリッドのタイルIDを取得
            if (tileID == -1) continue; // 無効なタイルIDならスキップ

            SDL_Rect srcRect = GetSourceRect(tileID); // タイルIDに対応するタイルセット上のソース矩形を取得
            SDL_FRect destRect = {
                (float)x * TILE_W - camera->GetX(), // TILE_W を使用: 画面上の描画X座標 (カメラオフセット適用)
                (float)y * TILE_H - camera->GetY(), // TILE_H を使用: 画面上の描画Y座標 (カメラオフセット適用)
                (float)TILE_W, // TILE_W を使用: 描画幅
                (float)TILE_H // TILE_H を使用: 描画高さ
            };
            SDL_RenderTexture(renderer, tilesetTexture_, &srcRect, &destRect); // タイルを描画
        }
    }
}

// 指定されたグリッド座標のタイルIDを取得
int Map::GetTileID(int x, int y) const {
    if (x >= 0 && x < mapCols_ && y >= 0 && y < mapRows_) {
        return mapData_[y][x]; // 有効な範囲内であればタイルIDを返す
    }
    return -1;
}

// 指定されたタイルIDを持つ最初のタイルのグリッド座標を見つける
bool Map::FindTile(int tileID, int& outX, int& outY) const {
    for (int y = 0; y < mapRows_; ++y) {
        for (int x = 0; x < mapCols_; ++x) {
            if (mapData_[y][x] == tileID) {
                outX = x; // 見つかったグリッドX座標を設定
                outY = y; // 見つかったグリッドY座標を設定
                return true;
            }
        }
    }
    return false;
}

// 指定されたタイルIDを持つすべてのタイルのグリッド座標を見つける
std::vector<SDL_Point> Map::FindAllTiles(int tileID) const {
    std::vector<SDL_Point> positions; // 見つかった座標を格納するベクター
    for (int y = 0; y < mapRows_; ++y) {
        for (int x = 0; x < mapCols_; ++x) {
            if (mapData_[y][x] == tileID) {
                positions.push_back({ x, y }); // マッチする座標を追加
            }
        }
    }
    return positions; // リストを返す
}

// タイルセットから個々のタイル画像を取得するためのソース矩形を計算
SDL_Rect Map::GetSourceRect(int tileID) const {
    return {
        tileID * TILE_W, // ソースX座標
        0, // ソースY座標
        TILE_W, // ソース幅
        TILE_H // ソース高さ
    };
}