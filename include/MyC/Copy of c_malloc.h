
#ifndef __tlv_c_malloc_h__
#define __tlv_c_malloc_h__

#pragma once


#define c_round(x, y)	(((x)+(y-1))&~(y-1))	
#define c_round4(x)		c_round(x, 4)			/* Multiple of Four up */
#define	c_lround4(x)	(c_round4(x) - 4)		/* Multiple of Four lower */


//   +--------------------------------------------------------------------------+
//	 |----c_plex----|-------c_allocstruct-------|-------------_ptr--------------|
//   +--------------------------------------------------------------------------+
//	 | flag | plex* | flag | alen | dlen | tags |             data              |
//   +--------------------------------------------------------------------------+

//   +-----------------------------------------------------------------------------------------+
//	 |-----------------c_freenode-----------------|-----------------c_freenode-----------------|
//   +-----------------------------------------------------------------------------------------+
//	 | flag | alen | dlen | tags |      data      | flag | alen | dlen | tags |      data      |
//   +-----------------------------------------------------------------------------------------+


#define c_blocksize				32			// alloc block size



/*
	创建一个PLEX内存块加入到PLEX链表中，PLEX块包括一个c_plex结构体，nmax个cbelement大小的数据块
	g_bufferhead 为PLEX链表头，c_freeplex 释放单独PLEX块，c_plexdata给出数据块指针获取PLEX块指针
	c_allocdata 获取数据块指针，c_allocalen 取数据库分配长度，c_allocdlen 获取实际存储数据长度
*/

// ############################################################


struct c_plex
{
	unsigned int	flag;			// flag for is a plex block，use c_plexflag
	unsigned int	nfree;			// free block count

	struct c_plex* pnext;	// the next plex block
};


struct c_freenode
{
	struct c_freenode* pnext;
};


struct c_allocstruct
{
	unsigned int flag;			// flag for is my alloc use c_allocflag
	unsigned int alen;			// alloc length
	unsigned int dlen;			// data length
	unsigned int tags;			// record 64 or 128 or 256 or 512 or more

	struct c_plex* phead;	// the block malloc address
};


#define c_plexflag		12345
#define c_allocflag		54321

struct c_plex* c_createplex(struct c_plex** phead, unsigned int nmax, unsigned int cbelement);	
struct c_plex* c_plexdata(struct c_allocstruct* _as);

void c_freeplex(struct c_plex* pmalloc);

// ############################################################


struct c_allocstruct* c_allocdata(char* _ptr);			// get ptr infomation
unsigned int c_allocalen(char* _ptr);						// get alloc length
unsigned int c_allocdlen(char* _ptr);						// get data length
unsigned int c_allocsize(char* _ptr);						// get data length  the same c_allocdlen_a


char* c_alloc(unsigned int _size);
char* c_realloc(char* _ptr, unsigned int _newsize, int _reserve);
void c_free(char* _ptr);

void c_allocexit(void);
void c_freeall(void);


// ############################################################
// 测试代码区域

void c_malloctest();





// ############################################################


#endif	// __tlv_c_malloc_h__
