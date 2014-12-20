/* ------------------------------
   $Id: inode.h,v 1.2 2009/11/17 09:44:12 marquet Exp $
   ------------------------------------------------------------

   Inode manipulation interface.
   Philippe Marquet, Nov 2009
   
*/

#ifndef _INODE_H_
#define _INODE_H_

#ifdef SOL
#   include "hw_ini+sol.h"
#endif
#include "tools.h"
 
#define BLOC_SIZE       SECTOR_SIZE  
#define DATA_BLOC_SIZE  BLOC_SIZE

/* different kind of files */
enum file_type_e {FILE_FILE, FILE_DIRECTORY, FILE_SPECIAL};

#ifdef SOL
#   include "inode+sol.h"
#else
/* inode */
struct inode_s {
    unsigned int ind_size;      /* in octets */
#   error "You need to complete the struct inode_s structure with your own code"
};
#endif

/* a bloc full of zeros */
#define BLOC_NULL 0

/* inodes i/o */
void read_inode (unsigned int inumber, struct inode_s *inode);
void write_inode (unsigned int inumber, const struct inode_s *inode);

/* inodes creation/deletion.
   The creation return a inumber */
unsigned int create_inode(enum file_type_e type); 
int delete_inode(unsigned int inumber);

/* return the bloc index on the volume of a given bloc index in a
   file.  Return BLOC_NULL for a bloc full of zeros.
   If do_allocate, allocate blocs if needed; return BLOC_NULL if no
   allocation was possible.  */  
unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc,
                            bool_t do_allocate); 

#endif
