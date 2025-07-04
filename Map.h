#pragma once  
#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <map>
#include <SDL3/SDL.h>

// �N���X�̑O���錾
class Camera; 

// �Q�[���̃}�b�v�f�[�^�����[�h�A�Ǘ��A�`��
class Map {  
public:
    // �R���X�g���N�^
    // �}�b�v�^�C���`��p�̃e�N�X�`��
    Map(TextureManager* textureManager);
    // �f�X�g���N�g
    ~Map();  

    // CSV�t�@�C������}�b�v�f�[�^�����[�h
    bool LoadMap(const std::string& filePath);
    // �}�b�v�̕`��
    // �`��Ɏg�p����SDL�����_���[�A�J�����I�u�W�F�N�g(�`��ʒu�ɃI�t�Z�b�g��K�p���邽��)
    void Render(SDL_Renderer* renderer, Camera* camera);

    // �w�肳�ꂽ�O���b�h���W�̃^�C��ID���擾
    int GetTileID(int x, int y) const;
    // �}�b�v�̗񐔂��擾
    int GetMapCols() const { return mapCols_; }
    // �}�b�v�̍s�����擾
    int GetMapRows() const { return mapRows_; }

    // �w�肳�ꂽ�^�C��ID�����ŏ��̃^�C���̃O���b�h���W��������
    // ��������^�C��ID�A�������^�C���̃O���b�h���W���i�[����
    bool FindTile(int tileID, int& outX, int& outY) const;

    // �w�肳�ꂽ�^�C��ID�������ׂẴ^�C���̃O���b�h���W��������
    // ��������ID�A�����������ׂẴ^�C�����W�̃^�C��
    std::vector<SDL_Point> FindAllTiles(int tileID) const;

private:
    SDL_Texture* tilesetTexture_; // �^�C���Z�b�g�̃e�N�X�`���A�g���X
    std::vector<std::vector<int>> mapData_; // �}�b�v�̃O���b�h�f�[�^
    int mapCols_; // �}�b�v�̗�
    int mapRows_; // �}�b�v�̍s��

    // �^�C���Z�b�g����X�̃^�C���摜���擾���邽�߂�Rect���v�Z
    // �擾�������^�C��ID
    SDL_Rect GetSourceRect(int tileID) const;
};

#endif // MAP_H
