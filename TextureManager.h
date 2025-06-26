#pragma once
#ifndef TEXTURE_MANAGER_H
#define	TEXTURE_MANAGER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <map>
#include <string>

class TextureManager {
public:
	TextureManager(SDL_Renderer* renderer); // �R���X�g���N�^
	~TextureManager(); // �f�X�g���N�^

	SDL_Texture* LoadTexture(const std::string& filePath); // �e�N�X�`�������[�h
	SDL_Texture* GetTexture(const std::string& filePath); // �e�N�X�`�����擾
	void UnloadTexture(const std::string& filePath); // �e�N�X�`�����A�����[�h
	void ClearAllTextures(); // �S�Ẵe�N�X�`�����N���A

private:
	SDL_Renderer* renderer_; // SDL�����_���[
	std::map<std::string, SDL_Texture*> textures_; // �p�X�ƃe�N�X�`���̃}�b�v
};

#endif // !TEXTURE_MANAGER_H
