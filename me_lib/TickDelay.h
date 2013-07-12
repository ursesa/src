#ifndef TickDelay_H_
#define TickDelay_H_

struct sTickDelay
{
    unsigned int TargetTicks;
    unsigned int CtrOld;
};

extern void TickDelay_Set(struct sTickDelay *Sys, unsigned int DelayUs);
extern unsigned char TickDelay_IsElapsed(struct sTickDelay *Sys);

#endif

