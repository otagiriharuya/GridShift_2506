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

	bool LoadMap(const std::string& filePath); // �}�b�v�f�[�^���t�@�C������ǂݍ���
	void Render(SDL_Renderer* renderer, Camera* camera); // �}�b�v�̕`��

	// �}�b�v�̑S�̃T�C�Y���擾
	float GetMapW() const { return mapW_; }
	float GetMapH() const { return mapH_; }

private:
	SDL_Renderer* renderer_;
    TextureManager* textureManager_;

    // �}�b�v�f�[�^ (��: �^�C��ID��2�����z��)
    std::vector<std::vector<int>> tileData_;

    // �^�C���Z�b�g (�^�C��ID�ƃe�N�X�`���t�@�C���p�X�̃}�b�s���O�A�܂��͒��ڃe�N�X�`���I�u�W�F�N�g)
    // �ǂݍ��񂾃e�N�X�`����ێ����邽�߂̃}�b�v���g�p
    std::map<int, std::string> tileTexturePaths_; // �^�C��ID�ƃt�@�C���p�X�̃}�b�s���O
    // std::map<int, SDL_Texture*> tileTextures_; // ���[�h�ς݂̃e�N�X�`���𒼐ڕێ�����Ȃ炱��

    float mapW_; // �}�b�v�S�̂̕� (�s�N�Z��)
    float mapH_; // �}�b�v�S�̂̍��� (�s�N�Z��)
    int tileW_; // �^�C���̕� (�s�N�Z��)
    int tileH_; // �^�C���̍��� (�s�N�Z��)
    int mapRows_; // �}�b�v�̍s��
    int mapCols_; // �}�b�v�̗�

    // �}�b�v�̏�������ݒ���s���v���C�x�[�g�w���p�[�֐�
    void SetTileProperties(int tileW, int tileH);
};

#endif // MAP_H
