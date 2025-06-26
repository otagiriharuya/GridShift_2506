#pragma once
#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <map>
#include <SDL3/SDL.h>

class TextureManager;
class Camera;

class Map {
public:
	Map(SDL_Renderer* renderer, TextureManager* textureManager);
	~Map();

	bool LoadMap(const std::string& filePath); // マップデータをファイルから読み込む
	void Render(SDL_Renderer* renderer, Camera* camera); // マップの描画

	// マップの全体サイズを取得
	float GetMapW() const { return mapW_; }
	float GetMapH() const { return mapH_; }

private:
	SDL_Renderer* renderer_;
    TextureManager* textureManager_;

    // マップデータ (例: タイルIDの2次元配列)
    std::vector<std::vector<int>> tileData_;

    // タイルセット (タイルIDとテクスチャファイルパスのマッピング、または直接テクスチャオブジェクト)
    // 読み込んだテクスチャを保持するためのマップを使用
    std::map<int, std::string> tileTexturePaths_; // タイルIDとファイルパスのマッピング
    // std::map<int, SDL_Texture*> tileTextures_; // ロード済みのテクスチャを直接保持するならこれ

    float mapW_; // マップ全体の幅 (ピクセル)
    float mapH_; // マップ全体の高さ (ピクセル)
    int tileW_; // タイルの幅 (ピクセル)
    int tileH_; // タイルの高さ (ピクセル)
    int mapRows_; // マップの行数
    int mapCols_; // マップの列数

    // マップの初期化や設定を行うプライベートヘルパー関数
    void SetTileProperties(int tileW, int tileH);
};

#endif // MAP_H
