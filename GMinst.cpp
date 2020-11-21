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
	&Tubularbell,
	&ChoirAahs,
	&SynthDrum,
	&SteelDrums
};
