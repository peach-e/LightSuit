/*
 **********************************************************************
 *  File   : FadeHSV.h
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

#ifndef MACHINESTATE_FADEHSV_H_
#define MACHINESTATE_FADEHSV_H_

#include "MachineState.h"

class FadeHSV: public MachineState {
private:
    int _hue;
    void _setHue();
public:
    FadeHSV();
    void enterState();
    void iterateState();
};

#endif /* MACHINESTATE_FADEHSV_H_ */
