#ifndef ELF_H
#define ELF_H

// ELF HEADER FIELDS
//
// FIELD OFFSETS
//
// HO byte encodes field size

#define ELF_EI_CLASS		0x02000004
#define ELF_EI_VERSION		0x04000006
#define ELF_e_type		0x02000010
#define ELF_e_machine		0x02000012
#define ELF_e_version		0x04000014
#define ELF_e_entry		0x04000018	// Entry point
/*
 *
 *
 */
#define ELF_shoff		0x0400002A	// Section header offset
#define ELF_ehsize		0x0200002C	// ELF header size

const int ELF_FIELDS[2] = {ELF_e_entry, ELF_shoff};

#endif
