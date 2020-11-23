#include "GMinst.h"

#include "Piano1_samples.h" 
#include "ChurchOrg1_samples.h"
#include "NylonstrGt_samples.h"
#include "Violin_samples.h"
#include "Flute_samples.h"
#include "SpaceVoice_samples.h"
#include "Harpsichord_samples.h"
#include "Glockenspiel_samples.h"
#include "Tubularbell_samples.h"
#include "ChoirAahs_samples.h"
#include "SynthDrum_samples.h"
#include "SteelDrums_samples.h"
#include "Banjo_samples.h"
#include "Bird_samples.h"
#include "BottleBlow_samples.h"
#include "Brightness_samples.h"
#include "Clarinet_samples.h"
#include "EPiano2_samples.h"
#include "EchoDrops_samples.h"
#include "Harp_samples.h"
#include "Honkytonk_samples.h"
#include "Marimba_samples.h"
#include "OrchestraHit_samples.h"
#include "PanFlute_samples.h"
#include "StarTheme_samples.h"
#include "Xylophone_samples.h"
#include "Bandoneon_samples.h"
#include "BaritoneSax_samples.h"
#include "Bassoon_samples.h"
#include "BowedGlass_samples.h"
#include "BreathNoise_samples.h"
#include "ChurchBell_samples.h"
#include "Clav_samples.h"
#include "EPiano1_samples.h"
#include "GtHarmonics_samples.h"
#include "Harmonica_samples.h"
#include "Helicopter_samples.h"
#include "Kalimba_samples.h"
#include "MusicBox_samples.h"
#include "PizzicatoStr_samples.h"
#include "ReedOrgan_samples.h"
#include "Santur_samples.h"
#include "Sitar_samples.h"
#include "Taiko_samples.h"
#include "Telephone1_samples.h"
#include "TinkleBell_samples.h"
#include "Whistle_samples.h"

// the following are some of the 128 instruments in the General MIDI 
// instrument bank
const PROGMEM AudioSynthWavetable::instrument_data * GMinst[] = {
    &Piano1,
    &ChurchOrg1,
    &Flute,
    &Violin,
    &NylonstrGt,
    &SpaceVoice,
    &Harpsichord,
    &Glockenspiel,
    &ChoirAahs,
    &SynthDrum,
    &SteelDrums,
    &Banjo,
    &Bird,
    &BottleBlow,
    &Brightness,
    &Clarinet,
    &EPiano2,
    &EchoDrops,
    &Harp,
    &Honkytonk,
    &Marimba,
    &OrchestraHit,
    &PanFlute,
    &StarTheme,
    &Xylophone,
    &Bandoneon,
    &BaritoneSax,
    &Bassoon,
    &BowedGlass,
    &BreathNoise,
    &ChurchBell,
    &Clav,
    &EPiano1,
    &GtHarmonics,
    &Harmonica,
    &Helicopter,
    &Kalimba,
    &MusicBox,
    &PizzicatoStr,
    &ReedOrgan,
    &Santur,
    &Sitar,
    &Taiko,
    &Telephone1,
    &TinkleBell,
    &Whistle,
    &Tubularbell
};
