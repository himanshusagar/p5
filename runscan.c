#include <stdio.h>
#include "ext2_fs.h"
#include "read_ext2.h"

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("expected usage: ./runscan inputfile outputfile\n");
		exit(0);
	}
	
	int fd;

	fd = open(argv[1], O_RDONLY);    /* open disk image */

	struct ext2_super_block super;
	struct ext2_group_desc group;
	
	// must read first the super-block and group-descriptor
	read_super_block(fd, &super);
	read_group_desc(fd, &group);
	
	printf("There are %u inodes in an inode table block and %u blocks in the idnode table\n", inodes_per_block, itable_blocks);
	//iterate the first inode block
	off_t start_inode_table = locate_inode_table(&group);
    for (unsigned int i = 0; i < inodes_per_block; i++) {
            printf("inode %u: \n", i);
            struct ext2_inode *inode = malloc(sizeof(struct ext2_inode));
            read_inode(fd, start_inode_table, i, inode);
            printf("number of blocks %u\n", inode->i_blocks>>1);	// the alignment is 1 byte off for new ext2; does not affect the block pointers
             printf("Is directory? %s \n Is Regular file? %s\n",
                S_ISDIR(inode->i_mode) ? "true" : "false",
                S_ISREG(inode->i_mode) ? "true" : "false");
			
			// print i_block numberss
			for(unsigned int i=0; i<EXT2_N_BLOCKS; i++)
			{       if (i < EXT2_NDIR_BLOCKS)                                 /* direct blocks */
							printf("Block %2u : %u\n", i, inode->i_block[i]);
					else if (i == EXT2_IND_BLOCK)                             /* single indirect block */
							printf("Single   : %u\n", inode->i_block[i]);
					else if (i == EXT2_DIND_BLOCK)                            /* double indirect block */
							printf("Double   : %u\n", inode->i_block[i]);
					else if (i == EXT2_TIND_BLOCK)                            /* triple indirect block */
							printf("Triple   : %u\n", inode->i_block[i]);

			}
			
            free(inode);

        }

	
	close(fd);
}
