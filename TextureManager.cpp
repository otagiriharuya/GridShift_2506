#include "TextureManager.h"
#include <SDL3/SDL_log.h>

// コンストラクタ
TextureManager::TextureManager(SDL_Renderer* renderer)
	: renderer_(renderer){
	// SDL_image の初期化
	
}

// デストラクタ
TextureManager::~TextureManager() {
	ClearAllTextures(); // 全てのテクスチャを解放
}

// テクスチャの読み込み
SDL_Texture* TextureManager::LoadTexture(const std::string& filePath) {
	// すでに読み込み済みか確認
	if (textures_.count(filePath)) {
		return textures_[filePath];
	}

	SDL_Texture* texture = IMG_LoadTexture(renderer_, filePath.c_str()); // テクスチャを読み込む
	if (!texture) {
		SDL_Log("Texture load failed: %s (ファイル: %s)", SDL_GetError(), filePath.c_str());
	}
	else {
		SDL_Log("Texture loaded: %s", filePath.c_str());
		textures_[filePath] = texture; // 読み込んだテクスチャを保存
	}
	return texture;
}

// テクスチャの取得
SDL_Texture* TextureManager::GetTexture(const std::string& filePath) {
	auto it = textures_.find(filePath); // マップからファイルパスを検索
	if (it == textures_.end()) {
		SDL_Log("Texture '%s' not foundd", filePath.c_str());
		return LoadTexture(filePath);
	}
	SDL_Log("Texture '%s' was retrieved from cache", filePath.c_str());
	return it->second; // 見つかった場合はテクスチャを返す
}

// テクスチャのアンロード
void TextureManager::UnloadTexture(const std::string& filePath) {
	if (textures_.count(filePath)) {
		SDL_DestroyTexture(textures_[filePath]); // テクスチャを解放
		textures_.erase(filePath); // マップから削除
	}
}

// 全てのテクスチャを解放
void TextureManager::ClearAllTextures() {
	for (auto const& [key, val] : textures_) {
		SDL_DestroyTexture(val); // 各テクスチャを解放
	}
	textures_.clear(); // マップを空にする
}
