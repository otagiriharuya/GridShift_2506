#pragma once
#ifndef TEXTURE_MANAGER_H
#define	TEXTURE_MANAGER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <map>
#include <string>

// テクスチャのロード、管理、解放
class TextureManager {
public:
	// コンストラクト
	// テクスチャを作成するのに必要なSDLレンダラー
	TextureManager(SDL_Renderer* renderer);
	// デストラクタ
	// 管理しているすべてのテクスチャを開放
	~TextureManager();

	// 指定されたファイルパスからテクスチャをロードし、ID登録
	// ロードしたいファイルパス、ロードしたテクスチャのID
	bool LoadTexture(const std::string& filePath, const std::string& id);
	// 指定されたIDのテクスチャを取得
	// 開放したいテクスチャのID
	SDL_Texture* GetTexture(const std::string& id);
	// 指定されたIDのテクスチャを開放
	void UnloadTexture(const std::string& id);
	// 管理しているすべてのテクスチャを開放
	void ClearAllTextures();

private:
	SDL_Renderer* renderer_; // テクスチャ作成に必要なSDLレンダラー

	// テクスチャIDとSDL_Texture* のマップ
	// ロードされたテクスチャをIDで管理
	std::map<std::string, SDL_Texture*> textures_;
};

#endif // !TEXTURE_MANAGER_H
