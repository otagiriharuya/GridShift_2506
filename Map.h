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

    bool LoadMap(const std::string& filePath); // �}�b�v�f�[�^���t�@�C������ǂݍ���  
    void Render(SDL_Renderer* renderer, Camera* camera); // �}�b�v�̕`��  

    // �}�b�v�̃^�C�����ł̕��ƍ������擾  
    int GetMapRows() const { return mapRows_; }  
    int GetMapCols() const { return mapCols_; }  

    // �}�b�v�̃s�N�Z���P�ʂł̕��ƍ������擾  
    int GetMapW() const { return mapCols_ * TILE_W; }  
    int GetMapH() const { return mapRows_ * TILE_H; }  

    // �O���b�h���W����X�N���[���s�N�Z�����W�ւ̕ϊ�  
    SDL_FRect GridToScreenRect(int gridX, int gridY) const;  
    // �X�N���[���s�N�Z�����W����O���b�h���W�ւ̕ϊ�  
    SDL_Point ScreenToGrid(float screenX, float screenY) const;  

    // ����̃O���b�h�ʒu�̃^�C��ID���擾  
    int GetTileID(int gridX, int gridY) const; 

    // ����̃O���b�h�ʒu�̃^�C��ID��ݒ�
    void SetTileID(int gridY, int gridX, int newID);

private:  
    SDL_Renderer* renderer_;  
    TextureManager* textureManager_;  

    float mapW_; // �}�b�v�S�̂̕� (�s�N�Z��)  
    float mapH_; // �}�b�v�S�̂̍��� (�s�N�Z��)  
    int tileW_; // �^�C���̕� (�s�N�Z��)  
    int tileH_; // �^�C���̍��� (�s�N�Z��)  
    int mapRows_; // �}�b�v�̍s��  
    int mapCols_; // �}�b�v�̗�  

    // �}�b�v�f�[�^ (��: �^�C��ID��2�����z��)  
    std::vector<std::vector<int>> tileData_;  

    // �^�C���Z�b�g (�^�C��ID�ƃe�N�X�`���t�@�C���p�X�̃}�b�s���O�A�܂��͒��ڃe�N�X�`���I�u�W�F�N�g)  
    // �ǂݍ��񂾃e�N�X�`����ێ����邽�߂̃}�b�v���g�p  
    std::map<int, std::string> tileTexturePaths_; // �^�C��ID�ƃt�@�C���p�X�̃}�b�s���O  
    // std::map<int, SDL_Texture*> tileTextures_; // ���[�h�ς݂̃e�N�X�`���𒼐ڕێ�����Ȃ炱��  

    // �}�b�v�̏�������ݒ���s���v���C�x�[�g�w���p�[�֐�  
    void SetTileProperties(int tileW, int tileH);  
};  

#endif // MAP_H
