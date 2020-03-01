
#ifndef __tlv_c_malloc_h__
#define __tlv_c_malloc_h__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//   +----------------------+
//	 |---------c_plex-------|
//   +----------------------+
//	 | flag | nfree | plex* |
//   +----------------------+

//   +------------------------------------------------+
//	 |--------c_blocl---------------------------------|
//   +------------------------------------------------+
//	 | flag | alloc_size | data_size | tags | c_plex* |
//   +------------------------------------------------+

//	+---------------------------------------------------------------------------+
//	|  c_plex	|  c_block  |                   data							|
//  +-----------+-----------+---------------------------------------------------+
//				|  c_block  |                   data							|
//				+-----------+---------------------------------------------------+
//				|  c_block  |                   data							|
//				+-----------+---------------------------------------------------+
//				|  ........................										|
//				+---------------------------------------------------------------+


#define c_round(x, y)	(((x)+(y-1))&~(y-1))	
#define c_round4(x)		c_round(x, 4)			// Multiple of Four up
#define	c_lround4(x)	(c_round4(x) - 4)		// Multiple of Four lower

#define c_plex_flag		12345
#define c_block_flag	54321
#define c_block_amount	32						// alloc block amount


typedef struct {
	size_t	flag;			// flag for is a plex block，use c_plex_flag
	size_t	nfree;			// free block count
	void*	pnext;			// the next plex block
} c_plex;


typedef struct {
	void*	pnext;
} c_node;


typedef struct {
	size_t	flag;			// flag for is my alloc use c_block_flag
	size_t	alloc_size;		// alloc length
	size_t	data_size;		// data length
	size_t	tags;			// record 64 or 128 or 256 or 512 or more
	void*	pplex;			// the block malloc plex address
} c_block;


/*
  分配、释放块内存
*/
c_plex* c_createplex(c_plex** phead, size_t nmax, size_t cbelement);	
   void c_releaseplex(c_plex* pplex);
c_plex* c_plexdata(c_block* block);


/*
  获取分配信息 _ptr为c_alloc返回值
*/
c_block* c_blockdata(void* _ptr);								// get ptr infomation
size_t c_block_allocsize(void* _ptr);							// get alloc length
size_t c_block_datasize(void* _ptr);							// get data length


void* c_alloc(size_t _size);
void* c_realloc(void* _ptr, size_t _newsize, int _reserve);
void c_free(void* _ptr);
void c_freeall(void);
void c_allocexit(void);


#ifdef C_MALLOC_TEST

void c_malloc_test();

#endif

#ifdef __cplusplus
}
#endif

#endif	// __tlv_c_malloc_h__
