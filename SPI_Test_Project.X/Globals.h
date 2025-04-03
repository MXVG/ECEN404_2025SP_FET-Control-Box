#ifndef GLOBALS_H
#define	GLOBALS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>

extern double _frequency;
extern double _duty;
extern char mode[16];
extern bool isExternal;
extern int modeIndex;
extern const int modeCount;
extern const char *modes[];


volatile extern bool displayNeedsUpdate;
volatile extern bool PWMNeedsUpdate;

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

