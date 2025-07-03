#include "Game.h"
#include "Map.h"
#include "TextureManager.h"
#include "Camera.h"
#include <fstream> // ファイル読み込み用
#include <sstream> // 文字列ストリーム用
#include <SDL3/SDL_log.h>

// グリッド座標からスクリーンピクセル座標への変換
SDL_FRect Map::GridToScreenRect(int gridX, int gridY) const {
	// カメラは考慮しない純粋なマップ座標での変換
	return { (float)gridX * TILE_W, (float)gridY * TILE_H, (float)TILE_W, (float)TILE_H};
}

// スクリーンピクセル座標からグリッド座標への変換
SDL_Point Map::ScreenToGrid(float screenX, float screenY) const {
	// SDL_Point は int 型なので、フロートは切り捨て
	return { (int)(screenX / TILE_W), (int)(screenY / TILE_H) };
}

// 特定のグリッド位置のタイルIDを取得
int Map::GetTileID(int gridX, int gridY) const {
	if (gridX >= 0 && gridX < mapCols_ && gridY >= 0 && gridY < mapRows_) {
		return tileData_[gridY][gridX];
	}
	return -1; // 範囲外の場合は-1など、無効なIDを返す
}

// コンストラクタ
Map::Map(SDL_Renderer* renderer, TextureManager* textureManager)
	: renderer_(0), textureManager_(textureManager), // レンダラーとテクスチャマネージャーの初期化
	mapW_(0), mapH_(0), tileW_(32), tileH_(32), // タイルの幅と高さの初期化
	mapRows_(0), mapCols_(0) // マップの行数と列数の初期化
{
	// タイトルセットの定義(タイトルIDとファイルパスを紐づけ)
	tileTexturePaths_[0] = "Image/ground.png"; // タイルID 0: 背景(移動できる場所)
	tileTexturePaths_[1] = "Image/wall.png"; // タイルID 1: 壁(移動できない場所)
};

// デストラクタ
Map::~Map() {}

// タイルの幅と高さを設定
void Map::SetTileProperties(int tileW, int tileH) {
	tileW_ = tileW; // タイルの幅を設定
	tileH_ = tileH; //タイルの高さを設定
	mapW_ = (float)mapCols_ * tileW_; // マップの幅を計算
	mapH_ = (float)mapRows_ * tileH_; // マップの高さを計算
}

// マップデータをファイルから読み込む
bool Map::LoadMap(const std::string& filePath) {
	std::ifstream file(filePath); // ファイルを開く
	if (!file.is_open()) {
		//SDL_Log("Could not open Map file: %s", filePath.c_str());
		return false; // ファイルが開けなかった場合はfalseを返す
	}

	std::string line; // 行を格納する変数

	// 最初の行でタイルサイズを読み込む
	if (std::getline(file, line)) {
		std::stringstream ss(line); // 行を文字列ストリームに変換
		std::string segment; // セグメントを格納する変数

		// タイルの幅を設定
		if (std::getline(ss, segment, ','))tileW_ = std::stoi(segment);
		// タイルの高さを設定
		if (std::getline(ss, segment, ',')) tileH_ = std::stoi(segment);
	}

	tileData_.clear(); // 既存のタイルデータをクリア

	// マップデータを読み込む
	while (std::getline(file, line)) {
		std::vector<int> row; // 行を格納するベクター
		std::stringstream ss(line); // 行を文字列ストリームに変換
		std::string segment; // セグメントを格納する変数
		int colCount = 0; // 列数をカウントする変数
		while (std::getline(ss, segment, ',')) {
			row.push_back(std::stoi(segment)); // セグメントを整数に変換して行に追加
			colCount++; // 列数をカウント
		}
		if (!row.empty()) { // 空行をスキップ
			tileData_.push_back(row); // 行をタイルデータに追加
			mapCols_ = std::max(mapCols_, colCount); // 最大列数を更新
		}
	}

	file.close(); // ファイルを閉じる

	mapRows_ = tileData_.size(); // 行数を設定
	SetTileProperties(tileW_, tileH_); // マップサイズを設定

	if (mapRows_ == 0 || mapCols_ == 0) {
		//SDL_Log("Map data is empty or invalid: %s", filePath.c_str());
		return false; // マップの行数または列数が0の場合はfalseを返す
	}

	//SDL_Log("Map '%s' loaded. Size: %d rows x %d columns, title size: %d x %d, ピクセルサイズ: %.0f x %.0f",
		filePath.c_str(), mapRows_, mapCols_, tileW_, tileH_, mapW_, mapH_);
	return true; // マップの読み込みに成功した場合はtrueを返す
}

void Map::Render(SDL_Renderer* renderer, Camera* camera) {
	// 画面に表示される範囲だけ描画
	int startX = (int)(camera->GetX() / tileW_); // 開始X座標を計算
	int startY = (int)(camera->GetY() / tileH_); // 開始Y座標を計算

	// 少し余白を与える
	int endX = startX + (int)(camera->GetW() / tileW_) + 2; // 終了X座標を計算
	int endY = startY + (int)(camera->GetH() / tileH_) + 2; // 終了Y座標を計算

	// マップの範囲を超えないようにクランプ
	startX = std::max(0, startX); // 開始X座標を0以上にクランプ
	startY = std::max(0, startY); // 開始Y座標を0以上にクランプ
	endX = std::min(mapCols_, endX); // 終了X座標をマップの列数以下にクランプ
	endY = std::min(mapRows_, endY); // 終了Y座標をマップの行数以下にクランプ

	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			if (y >= 0 && y < mapRows_ && x >= 0 && x < mapCols_) { // 範囲内かチェック
				int tileID = tileData_[y][x]; // タイルIDを取得

				// タイルIDに対応するテクスチャパスを取得
				auto it = tileTexturePaths_.find(tileID);
				if (it != tileTexturePaths_.end()) {
					SDL_Texture* tileTexture = textureManager_->GetTexture(it->second); // テクスチャを取得
					// 描画処理
					if (tileTexture) {
						// テクスチャがロードされている場合、タイルのデータを入れる
						// グリッド座標から描画位置の計算
						SDL_FRect destRect = GridToScreenRect(x, y);

						// カメラのオフセットを適用
						destRect.x -= camera->GetX();
						destRect.y -= camera->GetY();

						SDL_RenderTexture(renderer, tileTexture, NULL, &destRect); // テクスチャを描画
					}
					else {
						// テクスチャがロードされていない場合
						//SDL_Log("Title ID: %d (%s) Texture not loaded ", tileID,it->second.c_str());
					}
				}
				else {
					// 道のタイトルIDをスキップ
					//SDL_Log("unknown tile ID: %d at (%d, %d)", tileID, x, y);
				}
			}
		}
	}
}

// 特定のグリッド位置のタイルIDを設定
void Map::SetTileID(int gridY, int gridX, int newID) {
	if (gridX >= 0 && gridX < mapCols_ && gridY >= 0 && gridY < mapRows_) {
		tileData_[gridY][gridX] = newID;
	}
	else {
		SDL_Log("Error: Attempted to set tile ID out of bounds at grid(%d, %d)", gridX, gridY);
	}
}
