#ifndef MBR
#define MBR

#include "drive.h"

#define MBR_MAGIC 0xD0CABEEF

/* ATTENTION */
/* A voir s'il ne faut pas modifier HDA_SECTORSIZE = 512 */

enum type_vol_e{VOL_BASE, VOL_AUX, VOL_OTHER};

struct vol_s{ 
	unsigned int vol_cylinder;			/*						*/
	unsigned int vol_sector;			/*	1er bloc du volume  */
	unsigned int vol_nsectors;
	enum type_vol_e vol_type;
	int valide; 						/* A VOIR SI UTILE */
};

struct mbr_s{
	struct vol_s mbr_vol[MAXVOL];
	unsigned int nbvols;
	unsigned int magic_number;
};

void read_mbr();
void write_mbr ();
void read_bloc(unsigned int vol, unsigned int nbloc, unsigned char *buffer);
void write_bloc(unsigned int vol, unsigned int nbloc, const unsigned char *buffer);
void format_vol(unsigned int vol);

/* Commandes annexes */
void list_volumes();
void create_volume(unsigned int cylinder, unsigned int sector, unsigned int nsectors, enum type_vol_e type);
void delete_volume(unsigned int number);
void init_mbr();

#endif