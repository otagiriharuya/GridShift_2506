#pragma once
#ifndef TEXTURE_MANAGER_H
#define	TEXTURE_MANAGER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <map>
#include <string>

class TextureManager {
public:
	TextureManager(SDL_Renderer* renderer); // コンストラクタ
	~TextureManager(); // デストラクタ

	SDL_Texture* LoadTexture(const std::string& filePath); // テクスチャをロード
	SDL_Texture* GetTexture(const std::string& filePath); // テクスチャを取得
	void UnloadTexture(const std::string& filePath); // テクスチャをアンロード
	void ClearAllTextures(); // 全てのテクスチャをクリア

private:
	SDL_Renderer* renderer_; // SDLレンダラー
	std::map<std::string, SDL_Texture*> textures_; // パスとテクスチャのマップ
};

#endif // !TEXTURE_MANAGER_H
