/*
 * Based on an example from Hybus 
 *
 * Author: Kim_Kihoon, Bae_joohyun
 * Date:   June 20, 2014
 *
 * COMP427 Embedded Systems Final project
 * Computer Science Education
 * Korea University
 * 
 */

#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>

#define IDCT_DRIVER_MAJOR 	243
#define IDCT_DRIVER_NAME   "Idct Driver"


static int idct_driver_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int idct_driver_release(struct inode *inode, struct file *filp)
{
	return 0;
}                         

ssize_t idct_driver_write(struct file *filp, const short *buf, size_t length, loff_t *f_pos)
{
	short i;
	// Base Address of Idct Device
	unsigned int * idct_base = (unsigned int *) 0xffffc000;
	for(i=0; i<64 ; i++){
		*(idct_base + i) = *(buf+i);    // input 64 operands
	}

	return 0;
}

ssize_t idct_driver_read (struct file *filp, short *buf, size_t *status, size_t count, loff_t *f_pos)
{
	short i;
	// Base Address of IDCT Device
	unsigned int * idct_base = (unsigned int *) 0xffffc000;
	unsigned int quotient, remainder;
	

	short size_short_64 = 64;
	for(i=0; i < 64; i++)
		buf[i] = *(idct_base + i) ; 
	status[0]=*( idct_base)+sizeof(short)*64;
	
	return 0;
}

static struct file_operations idct_driver_fops = {
	open	:	idct_driver_open,
	read	:	idct_driver_read,		    
	write	:	idct_driver_write,
	release	:	idct_driver_release,
};

static int __init idct_driver_init(void)
{
  	int result, ret;

 	result = register_chrdev(IDCT_DRIVER_MAJOR, IDCT_DRIVER_NAME, &idct_driver_fops);

 	printk("%s : MAJOR# = %d\n", IDCT_DRIVER_NAME, IDCT_DRIVER_MAJOR);
	return 0;
}

static void __exit idct_driver_exit(void)
{
	unregister_chrdev(IDCT_DRIVER_MAJOR, IDCT_DRIVER_NAME);
	printk("Release %s Module MAJOR %d\n", IDCT_DRIVER_NAME, IDCT_DRIVER_MAJOR);
}

module_init(idct_driver_init);
module_exit(idct_driver_exit);
