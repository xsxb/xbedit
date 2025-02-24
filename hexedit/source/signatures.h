#ifndef SIGNATURES_H
#define SIGNATURES_H

// Binary signature definitions

const int SIG_DOS[2] = {0x4D, 0x5A};
const int SIG_ELF[4] = {0x7F, 0x45, 0x4C, 0x46};

const int *SIG_P_ARR[2] = { SIG_DOS, SIG_ELF };
const char SIG_P_STR[2][8] = { "DOS", "ELF" };

#endif
