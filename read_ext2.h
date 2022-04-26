#ifndef READ_EXT2
#define READ_EXT2
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/stat.h>
#include "ext2_fs.h"

#define BASE_OFFSET 1024                   /* locates beginning of the super block (first group) */
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)

extern unsigned int block_size;		/* default 1kB block size */
extern unsigned int inodes_per_block;			/* number of inodes per block */
extern unsigned int itable_blocks;				/* size in blocks of the inode table */


extern int debug;		//turn on/off debug prints

/* read the first super block; for this project, you will only deal with the first block group */
void read_super_block( int                      fd,        /* the disk image file descriptor */
					  struct ext2_super_block *super      /* where to put the super block */
					  );

/* Read the first group-descriptor in the first block group; you will not be tested with a disk image with more than one block group */
void read_group_desc( int                      fd,        /* the disk image file descriptor */
					  struct ext2_group_desc *group     /* where to put the group-descriptor */
					  );

/* calculate the start address of the inode table in the first group */
off_t locate_inode_table(const struct ext2_group_desc *group);      /* the first group-descriptor */

/* calculate the start address of the data blocks in the first group */
off_t locate_data_blocks(const struct ext2_group_desc *group);      /* the first group-descriptor */

/* read an inode with specified inode number and group number */
void read_inode( int                           fd,        /* the disk image file descriptor */
				 off_t 						   offset,    /* offset to the start of the inode table */
				 int                           inode_no,  /* the inode number to read  */
				 struct ext2_inode            *inode   /* where to put the inode */
				 ); 

#endif
