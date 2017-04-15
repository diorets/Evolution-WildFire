#ifndef SIM_H_INCLUDED
#define SIM_H_INCLUDED

#define FRAME_SLEEP 10

void simulationMode();
void lowerCreature();
bool simulate();
void newCreature(bool error);
void newGeneration();
void traceLines(double * entries, double maxNum, int numEntriesI, double r, double g, double b);

#endif // SIM_H_INCLUDED
