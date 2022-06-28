#pragma once
#include "lib/OpenHaptic.h";
#include "lib/HomogeneousTransform.h"
#include "lib/Yaskawa.h"

extern OpenHaptic	haptic;
extern Yaskawa		robot;
extern bool haptic_jog_mode;
extern void thread_haptic_teleoperation(void*);