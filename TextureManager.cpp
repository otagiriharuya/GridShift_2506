#include "TextureManager.h"
#include <SDL3/SDL_log.h>

// コンストラクタ
TextureManager::TextureManager(SDL_Renderer* renderer)
    : renderer_(renderer) // レンダラーポインタを初期化
{
    // SDL_image の初期化
    // PNG画像をサポートするためのフラグ
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        //SDL_Log("SDL_image を初期化できませんでした: %s", IMG_GetError());
    }
    else {
        //SDL_Log("SDL_image が正常に初期化されました。");
    }
}

// デストラクタ
TextureManager::~TextureManager() {
    Clear(); // 管理している全てのテクスチャを解放
    IMG_Quit(); // SDL_image の終了処理
    //SDL_Log("TextureManager が破棄され、テクスチャが解放されました。");
}

// テクスチャをロードし、IDを付けて登録
bool TextureManager::LoadTexture(const std::string& filePath, const std::string& id) {
    // 既に同じIDのテクスチャがロードされている場合は、二重ロードを防ぐためfalseを返す
    if (textures_.count(id)) {
        //SDL_Log("警告: テクスチャID '%s' は既にロードされています。", id.c_str());
        return true; // 既に存在するので成功とみなす
    }

    SDL_Surface* surface = IMG_Load(filePath.c_str()); // 画像ファイルをSDL_Surfaceとしてロード
    if (!surface) {
        //SDL_Log("テクスチャのロードに失敗しました (ファイル: %s): %s", filePath.c_str(), IMG_GetError());
        return false; // ロード失敗
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface); // SurfaceからSDL_Textureを作成
    SDL_DestroySurface(surface); // Surfaceはもう不要なので解放

    if (!texture) {
        //SDL_Log("テクスチャの作成に失敗しました (ID: %s): %s", id.c_str(), SDL_GetError());
        return false; // テクスチャ作成失敗
    }

    textures_[id] = texture; // 作成したテクスチャをマップにIDと関連付けて格納
    //SDL_Log("テクスチャ '%s' (ID: %s) をロードしました。", filePath.c_str(), id.c_str());
    return true; // ロード成功
}

// 指定されたIDのテクスチャを取得
SDL_Texture* TextureManager::GetTexture(const std::string& id) const {
    auto it = textures_.find(id); // マップ内でIDを検索
    if (it != textures_.end()) {
        return it->second; // 見つかった場合、対応するテクスチャを返す
    }
    //SDL_Log("エラー: テクスチャID '%s' が見つかりません。", id.c_str());
    return nullptr; // 見つからない場合は nullptr を返す
}

// 指定されたIDのテクスチャを解放
void TextureManager::UnloadTexture(const std::string& id) {
    auto it = textures_.find(id);
    if (it != textures_.end()) {
        SDL_DestroyTexture(it->second); // SDL_Textureを解放
        textures_.erase(it); // マップからエントリを削除
        //SDL_Log("テクスチャID '%s' を解放しました。", id.c_str());
    }
    else {
        //SDL_Log("警告: 解放しようとしたテクスチャID '%s' が見つかりませんでした。", id.c_str());
    }
}

// 管理している全てのテクスチャを解放
void TextureManager::Clear() {
    for (auto const& [id, texture] : textures_) {
        SDL_DestroyTexture(texture); // 各SDL_Textureを解放
        //SDL_Log("テクスチャID '%s' をクリアしました。", id.c_str());
    }
    textures_.clear(); // マップをクリア
    //SDL_Log("全てのテクスチャが解放されました。");
}