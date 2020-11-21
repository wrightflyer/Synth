#include "GMinst.h"

#include "Piano1_samples.h" 
#include "ChurchOrg1_samples.h"
#include "NylonstrGt_samples.h"
#include "Violin_samples.h"
#include "Flute_samples.h"
#include "SpaceVoice_samples.h"

// the following are the 128 instruments in the General MIDI 
// instrument bank
const PROGMEM AudioSynthWavetable::instrument_data * GMinst[] = {
    &Piano1,
    &ChurchOrg1,
    &Flute,
    &Violin,
    &NylonstrGt,
    &SpaceVoice,
};
