#ifndef _TYPES_H_
#define _TYPES_H_

#include <Arduino.h>
#include <Audio.h>
#include <ILI9341_t3.h>
#include "defines.h"

typedef enum {
  AllMix,
  OSC1,
  OSC2,
  Noise,
  WaveTable
} Mix_change_t;

typedef enum {
  AllADSR,
  Attack,
  Decay,
  Sustain,
  Release
} ADSR_change_t;

typedef enum {
  FILT_OFF,
  LPF,
  BPF,
  HPF
} filter_band_t;

typedef enum {
  Arp_Off,
  Arp_Up,
  Arp_Down,
  Arp_UpDown,
  Arp_Random,
  Arp_Scale,
  Arp_Record
} arp_mode_t;

typedef enum {
  Mod_FM,
  Mod_PM
} osc_mod_t;

typedef struct {
  uint8_t type;
  float offset;
} keyb_t;

typedef struct {
  char name[20]; // max is actually 15
  int entries;
  char offsets[14]; // max actually 12
} scale_chord_t;

#endif
