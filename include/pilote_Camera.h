#ifndef PILOTECAMERA_H
#define PILOTECAMERA_H




int pilote_Camera_initialise(void);
void pilote_Camera_Take_Photo(uint8_t* buffer, int *size);

extern uint8_t pilote_Camera_Buffer[GROSSEUR_MAX_IMAGE];
#endif