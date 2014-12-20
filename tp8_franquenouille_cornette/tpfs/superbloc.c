#include "superbloc.h"
#include <assert.h>

#include <stdlib.h>
#include <string.h>

struct superbloc_s super;
unsigned current_volume;

void init_super(unsigned int vol,unsigned int num_serie, char nom[32])
{
	unsigned char buf[HDA_SECTORSIZE];
	struct free_bloc_s bloc;	
	current_volume = vol;

	/* Initialisation du superbloc */
	super.magic = SUPER_MAGIC;
	super.numero = num_serie;
	strncpy(super.nom, nom, 32);
	super.id = 0;
	super.premier_libre = 1;
	super.nb_bloc_libre = mbr.mbr_vol[vol].vol_nsectors-1;
	
	/* creation du 1er bloc libre */
	bloc.size = mbr.mbr_vol[vol].vol_nsectors-1;
	bloc.next_free = 0;
		
	/* ecriture du superbloc */
	memset(buf, 0, HDA_SECTORSIZE);
	memcpy(buf, &super, sizeof(super));
	write_bloc(vol, SUPER, buf);

	/* ecriture du 1er bloc libre */
	memset(buf, 0, HDA_SECTORSIZE);
	memcpy(buf, &bloc, sizeof(bloc));
	write_bloc(vol, 1, buf);
}

int load_super(unsigned int vol)
{
	unsigned char buf[HDA_SECTORSIZE];

	current_volume = vol;
	
	read_bloc(vol, SUPER, buf);
	memcpy(&super, buf, sizeof(struct superbloc_s));
	if (super.magic == SUPER_MAGIC)
		return 0;
	return -1;
}

void save_super()
{
	unsigned char buf[HDA_SECTORSIZE];
	
	memset(buf, 0, HDA_SECTORSIZE);
	memcpy(buf, &super, sizeof(super));
	write_bloc(current_volume, SUPER, buf);
}

unsigned int new_bloc()
{
	unsigned char buf[HDA_SECTORSIZE];
	struct free_bloc_s* bloc;
	unsigned res;

	if (super.nb_bloc_libre == 0)
		return 0;
	assert(super.premier_libre);

	memset(buf, 0, HDA_SECTORSIZE); /* VOIR SI UTILE */
	read_bloc(current_volume, super.premier_libre, buf);
	bloc = (struct free_bloc_s*) buf;
	res = super.premier_libre;
	if (bloc->size > 1)
	{
		super.premier_libre++;
		bloc->size--;
		write_bloc(current_volume, super.premier_libre, buf);
	} else {
		super.premier_libre = bloc->next_free;
	}
	super.nb_bloc_libre--;
	save_super();
	return res;
}

void free_bloc(unsigned int bloc)
{
	unsigned char buf[HDA_SECTORSIZE];
	struct free_bloc_s* fb;
	memset(buf, 0, HDA_SECTORSIZE); 

	if(bloc==0){
		printf("free_bloc impossible sur le permier bloc (reserve)\n");
		return;
	}

	/* On bloque ici */ 
	read_bloc(current_volume, bloc, buf);
	fb = (struct free_bloc_s*) buf;
	fb->next_free = super.premier_libre;
	fb->size = 1;
	write_bloc(current_volume, super.premier_libre, buf);
	super.premier_libre = bloc;
	super.nb_bloc_libre++;
	save_super();
}

float taux_occupation(unsigned int vol)
{
	return (1.00 - (float)super.nb_bloc_libre/(float)(mbr.mbr_vol[vol].vol_nsectors-1))*100.00;
}

void display()
{
	printf("Nombre de bloc : %d\nID\tTaille\tUtilisé\tDispo.\tPourcentage\n%d\t%d\t%d\t%d\t%f\n",
			mbr.mbr_vol[current_volume].vol_nsectors-1-super.nb_bloc_libre,
			super.id,
			mbr.mbr_vol[current_volume].vol_nsectors-1,
			mbr.mbr_vol[current_volume].vol_nsectors-1-super.nb_bloc_libre,
			super.nb_bloc_libre,
			taux_occupation(current_volume));
}