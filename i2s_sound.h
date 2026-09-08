/**
 * @file i2s_sound.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief I2Sサウンド
 * @version 0.1
 * @date 2026-09-08
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */

#ifndef I2S_SOUND_H
#define I2S_SOUND_H

#include <stdint.h>
#include <stdbool.h>
#include "pcb_def.h"

#if defined(I2S_USE)
// 音階の定義 (Hz)
#define NOTE_C4   262
#define NOTE_D4   294
#define NOTE_E4   330
#define NOTE_F4   349
#define NOTE_G4   392
#define NOTE_A4   440
#define NOTE_B4   494

#define NOTE_C5   523
#define NOTE_D5   587
#define NOTE_E5   659
#define NOTE_F5   698
#define NOTE_G5   784
#define NOTE_A5   880
#define NOTE_B5   988

#define NOTE_REST 0

// 音階と長さを管理する構造体
typedef struct {
    uint32_t freq_hz;
    uint32_t duration_ms;
} note_t;

extern const note_t g_play_test_song[];
extern const uint32_t g_play_test_song_note_cnt;

#if 0
extern const note_t g_frog_song[];
extern const uint32_t g_frog_song_note_cnt;
#endif

void i2s_sound_init(uint8_t dout_pin, uint8_t lrclk_pin, uint8_t bclk_pin);
void i2s_play_tone(uint32_t freq_hz, uint32_t duration_ms);
void i2s_play_melody(const note_t *p_notes, uint32_t note_count);

void i2s_play_morse_code_sound(const char *p_morse_code, uint32_t morse_code_len, uint16_t freq_hz);

void i2s_play_success_sound(void);
void i2s_play_fail_sound(void);

#endif // I2S_USE
#endif // I2S_SOUND_H
