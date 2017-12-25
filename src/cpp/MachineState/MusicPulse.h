/*
 **********************************************************************
 *  File   : MusicPulse.h
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

#ifndef MACHINESTATE_MUSICPULSE_H_
#define MACHINESTATE_MUSICPULSE_H_

#include "MachineState.h"
#include "LevelAnalyzer.h"

class MusicPulse: public MachineState {
private:
    LevelAnalyzer _levelAnalyzer;
public:
    MusicPulse();
    void enterState();
    void iterateState();
};

#endif /* MACHINESTATE_MUSICPULSE_H_ */
