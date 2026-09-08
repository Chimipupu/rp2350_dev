/**
 * @file i2s_sound.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief I2Sサウンド
 * @version 0.1
 * @date 2026-09-08
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */

#include "i2s_sound.h"
#include "pcb_def.h"

#if defined(I2S_USE)
#include <I2S.h>

// --------------------------------------------------------------------------
#define SAMPLE_RATE    16000
#define AMPLITUDE      500

#if 0
// 「かえるの合唱」の曲テーブル
const note_t g_frog_song[] = {
    {NOTE_C4, 300}, {NOTE_D4, 300}, {NOTE_E4, 300}, {NOTE_F4, 300},
    {NOTE_E4, 300}, {NOTE_D4, 300}, {NOTE_C4, 400}, {NOTE_REST, 200},

    {NOTE_E4, 300}, {NOTE_F4, 300}, {NOTE_G4, 300}, {NOTE_A4, 300},
    {NOTE_G4, 300}, {NOTE_F4, 300}, {NOTE_E4, 400}, {NOTE_REST, 200},

    {NOTE_C4, 300}, {NOTE_REST, 100}, {NOTE_C4, 300}, {NOTE_REST, 100},
    {NOTE_C4, 300}, {NOTE_REST, 100}, {NOTE_C4, 300}, {NOTE_REST, 100},

    {NOTE_C4, 150}, {NOTE_C4, 150}, {NOTE_D4, 150}, {NOTE_D4, 150},
    {NOTE_E4, 150}, {NOTE_E4, 150}, {NOTE_F4, 150}, {NOTE_F4, 150},
    {NOTE_E4, 300}, {NOTE_D4, 300}, {NOTE_C4, 600}, {NOTE_REST, 1000}
};
const uint32_t g_frog_song_note_cnt = sizeof(g_frog_song) / sizeof(g_frog_song[0]);
#endif

static uint8_t s_dout_pin;
static uint8_t s_lrclk_pin;
static uint8_t s_bclk_pin;

I2S i2s(OUTPUT, I2S_BCLK_PIN, I2S_DOUT_PIN);
// --------------------------------------------------------------------------

void i2s_play_tone(uint32_t freq_hz, uint32_t duration_ms)
{
    uint32_t total_samples;
    uint32_t half_wavelength;
    uint32_t i;
    int16_t sample;

    total_samples = (SAMPLE_RATE * duration_ms) / 1000;
    sample = AMPLITUDE;

    if (freq_hz == 0) {
        // 休符：無音（0）を出力
        for (i = 0; i < total_samples; i++)
        {
            i2s.write((int16_t)0);
            i2s.write((int16_t)0);
        }
        return;
    }

    // 矩形波の半周期（サンプル数）
    half_wavelength = SAMPLE_RATE / (freq_hz * 2);
    if (half_wavelength == 0) {
        half_wavelength = 1;
    }

    for (i = 0; i < total_samples; i++)
    {
        if ((i % half_wavelength) == 0) {
            sample = -sample;
        }

        i2s.write(sample);
        i2s.write(sample);
    }
}

void i2s_play_melody(const note_t *p_notes, uint32_t note_count)
{
    uint32_t i;
    uint32_t tone_ms;
    uint32_t gap_ms;

    if (p_notes == NULL) {
        return;
    }

    for (i = 0; i < note_count; i++)
    {
        if (p_notes[i].freq_hz != NOTE_REST) {
                // 音の歯切れ（アーティキュレーション）を良くするため、全体の85%を鳴らし15%を隙間とする
                tone_ms = (uint32_t)(p_notes[i].duration_ms * 0.85f);
                gap_ms = p_notes[i].duration_ms - tone_ms;

                i2s_play_tone(p_notes[i].freq_hz, tone_ms);
                i2s_play_tone(NOTE_REST, gap_ms);
            } else {
                // 休符の場合はそのまま指定時間無音にする
                i2s_play_tone(NOTE_REST, p_notes[i].duration_ms);
        }
    }
}

void i2s_sound_init(uint8_t dout_pin, uint8_t lrclk_pin, uint8_t bclk_pin)
{
    s_dout_pin = dout_pin;
    s_lrclk_pin = lrclk_pin;
    s_bclk_pin = bclk_pin;

    i2s.setBitsPerSample(16);
}
// --------------------------------------------------------------------------
#endif // I2S_USE