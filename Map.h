#pragma once  
#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <map>
#include <SDL3/SDL.h>

// クラスの前方宣言
class Camera; 

// ゲームのマップデータをロード、管理、描画
class Map {  
public:
    // コンストラクタ
    // マップタイル描画用のテクスチャ
    Map(TextureManager* textureManager);
    // デストラクト
    ~Map();  

    // CSVファイルからマップデータをロード
    bool LoadMap(const std::string& filePath);
    // マップの描画
    // 描画に使用するSDLレンダラー、カメラオブジェクト(描画位置にオフセットを適用するため)
    void Render(SDL_Renderer* renderer, Camera* camera);

    // 指定されたグリッド座標のタイルIDを取得
    int GetTileID(int x, int y) const;
    // マップの列数を取得
    int GetMapCols() const { return mapCols_; }
    // マップの行数を取得
    int GetMapRows() const { return mapRows_; }

    // 指定されたタイルIDを持つ最初のタイルのグリッド座標を見つける
    // 検索するタイルID、見つけたタイルのグリッド座標を格納する
    bool FindTile(int tileID, int& outX, int& outY) const;

    // 指定されたタイルIDを持つすべてのタイルのグリッド座標を見つける
    // 検索するID、見つかったすべてのタイル座標のタイル
    std::vector<SDL_Point> FindAllTiles(int tileID) const;

private:
    SDL_Texture* tilesetTexture_; // タイルセットのテクスチャアトラス
    std::vector<std::vector<int>> mapData_; // マップのグリッドデータ
    int mapCols_; // マップの列数
    int mapRows_; // マップの行数

    // タイルセットから個々のタイル画像を取得するためのRectを計算
    // 取得したいタイルID
    SDL_Rect GetSourceRect(int tileID) const;
};

#endif // MAP_H
