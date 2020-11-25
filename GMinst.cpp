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
#include "Applause_samples.h"
#include "Contrabass_samples.h"
#include "Crystal_samples.h"
#include "EnglishHorn_samples.h"
#include "Goblin_samples.h"
#include "MeloTom1_samples.h"
#include "Oboe_samples.h"
#include "Organ2_samples.h"
#include "Piccolo_samples.h"
#include "Recorder_samples.h"
#include "ReverseCym_samples.h"
#include "Shamisen_samples.h"
#include "AccordionFr_samples.h"
#include "AltoSax_samples.h"
#include "Cello_samples.h"
#include "ChifferLead_samples.h"
#include "Fiddle_samples.h"
#include "FingeredBs_samples.h"
#include "Koto_samples.h"
#include "MutedGt_samples.h"
#include "Ocarina_samples.h"
#include "Organ3_samples.h"
#include "Shanai_samples.h"
#include "SoloVox_samples.h"
#include "TenorSax_samples.h"
#include "Atmosphere_samples.h"
#include "Bagpipe_samples.h"
#include "Fantasia_samples.h"
#include "IceRain_samples.h"
#include "Seashore_samples.h"
#include "SlapBass2_samples.h"
#include "SynVox_samples.h"
#include "Tuba_samples.h"
#include "Viola_samples.h"
#include "VoiceOohs_samples.h"

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
    &Applause,
    &Contrabass,
    &Crystal,
    &EnglishHorn,
    &Goblin,
    &MeloTom1,
    &Oboe,
    &Organ2,
    &Piccolo,
    &Recorder,
    &ReverseCym,
    &Shamisen,
    &AccordionFr,
    &AltoSax,
    &Cello,
    &ChifferLead,
    &Fiddle,
    &FingeredBs,
    &Koto,
    &MutedGt,
    &Ocarina,
    &Organ3,
    &Shanai,
    &SoloVox,
    &TenorSax,
	&Atmosphere,
	&Bagpipe,
	&Fantasia,
	&IceRain,
	&Seashore,
	&SlapBass2,
	&SynVox,
	&Tuba,
	&Viola,
	&VoiceOohs,
    &Tubularbell
};
