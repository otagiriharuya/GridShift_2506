#pragma once
#ifndef TEXTURE_MANAGER_H
#define	TEXTURE_MANAGER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <map>
#include <string>

// �e�N�X�`���̃��[�h�A�Ǘ��A���
class TextureManager {
public:
	// �R���X�g���N�g
	// �e�N�X�`�����쐬����̂ɕK�v��SDL�����_���[
	TextureManager(SDL_Renderer* renderer);
	// �f�X�g���N�^
	// �Ǘ����Ă��邷�ׂẴe�N�X�`�����J��
	~TextureManager();

	// �w�肳�ꂽ�t�@�C���p�X����e�N�X�`�������[�h���AID�o�^
	// ���[�h�������t�@�C���p�X�A���[�h�����e�N�X�`����ID
	bool LoadTexture(const std::string& filePath, const std::string& id);
	// �w�肳�ꂽID�̃e�N�X�`�����擾
	// �J���������e�N�X�`����ID
	SDL_Texture* GetTexture(const std::string& id);
	// �w�肳�ꂽID�̃e�N�X�`�����J��
	void UnloadTexture(const std::string& id);
	// �Ǘ����Ă��邷�ׂẴe�N�X�`�����J��
	void ClearAllTextures();

private:
	SDL_Renderer* renderer_; // �e�N�X�`���쐬�ɕK�v��SDL�����_���[

	// �e�N�X�`��ID��SDL_Texture* �̃}�b�v
	// ���[�h���ꂽ�e�N�X�`����ID�ŊǗ�
	std::map<std::string, SDL_Texture*> textures_;
};

#endif // !TEXTURE_MANAGER_H
