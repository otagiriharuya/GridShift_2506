#include <SDL3/SDL.h> // SDL3 �̃��C���w�b�_�[
#include <SDL3_image/SDL_image.h> // SDL_image �̃w�b�_�[
#include <string> // std::string ���g�p���邽��
#include <chrono> // �����x�Ȏ��Ԍv���̂��� (SDL_Delay �̑���)

// �E�B���h�E�̒萔
const int WINDOW_W = 640; // �E�B���h�E�̕�
const int WINDOW_H = 480; // �E�B���h�E�̍���
const std::string WINDOW_TITLE = "Grid Shift - Test"; // �E�B���h�E�̃^�C�g��
const int FPS = 60; // �ڕW�t���[�����[�g
const Uint64 FRAME_DELAY_TICKS = SDL_GetPerformanceFrequency() / FPS; // 1�t���[��������̍ŏ��e�B�b�N��

int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr; // �E�B���h�E�̃|�C���^
    SDL_Renderer* renderer = nullptr; // �����_���[�̃|�C���^
    bool isRunning = true; // �Q�[�����[�v�̃t���O

    // SDL �̏�����
    // �r�f�I�T�u�V�X�e���݂̂����������܂��B
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        // SDL_GetError() �ŏڍׂȃG���[���b�Z�[�W���擾
        SDL_Log("SDL �̏������Ɏ��s���܂���: %s", SDL_GetError());
        return 1; // ���������s����1��Ԃ�
    }

    // �E�B���h�E�ƃ����_���[�̓����쐬
    if (SDL_CreateWindowAndRenderer(WINDOW_TITLE.c_str(), WINDOW_W, WINDOW_H, 0, &window, &renderer) < 0) {
        SDL_Log("�E�B���h�E�ƃ����_���[�̍쐬�Ɏ��s���܂���: %s", SDL_GetError());
        SDL_Quit(); // SDL ���I��
        return 1;
    }

    // �ēx�`�F�b�N
    if (!renderer) {
        SDL_Log("�����_���[�̎擾�Ɏ��s���܂���: %s", SDL_GetError());
        SDL_DestroyWindow(window); // �E�B���h�E���J��
        SDL_Quit(); // SDL ���I��
        return 1;
    }

    // �e�N�X�`���̓ǂݍ���
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "Image/player.png"); // �v���C���[�̃e�N�X�`����ǂݍ���
    if (!playerTexture) {
        SDL_Log("�摜�̓ǂݍ��݂Ɏ��s���܂���: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer); // �����_���[���J��
        SDL_DestroyWindow(window); // �E�B���h�E���J��
        SDL_Quit(); // SDL ���I��
        return 1; // �G���[����1��Ԃ�
    }

    SDL_Event ev; // �C�x���g�p�̕ϐ�
    Uint64 frameStartTime = 0; // �t���[���J�n���� (�p�t�H�[�}���X�J�E���^�[�e�B�b�N)
    Uint64 frameEndTime = 0; // �t���[���I������ (�p�t�H�[�}���X�J�E���^�[�e�B�b�N)
    Uint64 frameDuration = 0; // �t���[���ɂ����������� (�e�B�b�N)

    // ���C�����[�v
    while (isRunning)
    {
        frameStartTime = SDL_GetPerformanceCounter(); // �t���[���J�n���Ԃ��擾

        // �V�X�e���C�x���g�̏���
        while (SDL_PollEvent(&ev))
        {
            // �E�B���h�E�����
            if (ev.type == SDL_EVENT_QUIT)
                isRunning = false; // ���[�v���I��

            // �L�[�{�[�h���͂Ȃǂ̃C�x���g������ǉ�
            if (ev.type == SDL_EVENT_KEY_DOWN) {
                // ��: Esc�L�[�ŏI��
                if (ev.key.key == SDLK_ESCAPE) {
                    isRunning = false;
                }
            }
        }

        // �Q�[�����W�b�N�̍X�V (�����ɃQ�[���̏�ԕύX�������L�q)

        // �`�揈��
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // �w�i�F�����ɐݒ� (RGBA)
        SDL_RenderClear(renderer); // ��ʂ��N���A

        // �l�p�̕`�� (SDL_FRect �𒼐ڎg�p)
        SDL_FRect rect = { (float)WINDOW_W / 2 - 50, (float)WINDOW_H / 2 - 50, 100.0f, 100.0f };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // �Ԃɐݒ� (RGBA)
        SDL_RenderFillRect(renderer, &rect); // �l�p�`��h��Ԃ��ĕ`�� (SDL3�ł͒P���`�ł�OK)

        // �v���C���[�̃e�N�X�`���`��
        // SDL_FRect �͕��������_���Œ��ڏ������ł��܂��B
        SDL_FRect destRect = { (float)WINDOW_W / 2 - 16, (float)WINDOW_H / 2 - 16, 32.0f, 32.0f }; // �`��ʒu�ƃT�C�Y
        SDL_RenderTexture(renderer, playerTexture, NULL, &destRect);

        SDL_RenderPresent(renderer); // �`����e����ʂɕ\��

        // �t���[�����[�g����
        frameEndTime = SDL_GetPerformanceCounter(); // �t���[���I�����Ԃ��擾
        frameDuration = frameEndTime - frameStartTime; // 1�t���[���ɂ��������e�B�b�N��

        // �ڕW�e�B�b�N�������Z�������ꍇ�A�c��̎��ԑҋ@
        if (frameDuration < FRAME_DELAY_TICKS) {
            Uint64 delayMs = (Uint64)((double)(FRAME_DELAY_TICKS - frameDuration) * 1000.0 / SDL_GetPerformanceFrequency());
            if (delayMs > 0) { // ���̒l�ɂȂ�Ȃ��悤�Ƀ`�F�b�N
                SDL_Delay(delayMs);
            }
        }
    }

    // ���[�v�I�����ASDL �֘A�̂��̂��J��
    SDL_DestroyTexture(playerTexture); // �v���C���[�̃e�N�X�`���̊J��
    SDL_DestroyRenderer(renderer); // �����_���[�̊J��
    SDL_DestroyWindow(window); // �E�B���h�E�̊J��
    SDL_Quit(); // SDL �̏I������

    return 0; // �v���O�����𐳏�I��
}
