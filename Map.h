#pragma once  
#ifndef MAP_H  
#define MAP_H  

#include <vector>  
#include <string>  
#include <map>  
#include <SDL3/SDL.h>  
#include "Game.h"  

class TextureManager;  
class Camera; 

class Map {  
public:  
    Map(SDL_Renderer* renderer, TextureManager* textureManager);  
    ~Map();  

    bool LoadMap(const std::string& filePath); // マップデータをファイルから読み込む  
    void Render(SDL_Renderer* renderer, Camera* camera); // マップの描画  

    // マップのタイル数での幅と高さを取得  
    int GetMapRows() const { return mapRows_; }  
    int GetMapCols() const { return mapCols_; }  

    // マップのピクセル単位での幅と高さを取得  
    int GetMapW() const { return mapCols_ * TILE_W; }  
    int GetMapH() const { return mapRows_ * TILE_H; }  

    // グリッド座標からスクリーンピクセル座標への変換  
    SDL_FRect GridToScreenRect(int gridX, int gridY) const;  
    // スクリーンピクセル座標からグリッド座標への変換  
    SDL_Point ScreenToGrid(float screenX, float screenY) const;  

    // 特定のグリッド位置のタイルIDを取得  
    int GetTileID(int gridX, int gridY) const; 

    // 特定のグリッド位置のタイルIDを設定
    void SetTileID(int gridY, int gridX, int newID);

private:  
    SDL_Renderer* renderer_;  
    TextureManager* textureManager_;  

    float mapW_; // マップ全体の幅 (ピクセル)  
    float mapH_; // マップ全体の高さ (ピクセル)  
    int tileW_; // タイルの幅 (ピクセル)  
    int tileH_; // タイルの高さ (ピクセル)  
    int mapRows_; // マップの行数  
    int mapCols_; // マップの列数  

    // マップデータ (例: タイルIDの2次元配列)  
    std::vector<std::vector<int>> tileData_;  

    // タイルセット (タイルIDとテクスチャファイルパスのマッピング、または直接テクスチャオブジェクト)  
    // 読み込んだテクスチャを保持するためのマップを使用  
    std::map<int, std::string> tileTexturePaths_; // タイルIDとファイルパスのマッピング  
    // std::map<int, SDL_Texture*> tileTextures_; // ロード済みのテクスチャを直接保持するならこれ  

    // マップの初期化や設定を行うプライベートヘルパー関数  
    void SetTileProperties(int tileW, int tileH);  
};  

#endif // MAP_H
