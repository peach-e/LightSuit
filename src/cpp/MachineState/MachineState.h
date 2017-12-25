/*
 **********************************************************************
 *  File   : MachineState.h
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

/*
 * Machine states always have three basic functions: Initialize, enterState and iterateState.
 * Initialization should happen once when the arduino powers on. enterState() gets called whenever
 * the specified mode is entered. iterateState() gets called in the main loop each time the arduino
 * still in the same mode.
 */

#ifndef MACHINESTATE_MACHINESTATE_H_
#define MACHINESTATE_MACHINESTATE_H_

class MachineState {
public:
    virtual ~MachineState() {
    }

    virtual void enterState() = 0;
    virtual void iterateState() = 0;
};

#endif /* MACHINESTATE_MACHINESTATE_H_ */
