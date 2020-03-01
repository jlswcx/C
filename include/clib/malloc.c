
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "malloc.h"


static c_plex* g_plexhead = NULL;
static c_node* g_freenode64	= NULL;
static c_node* g_freenode128= NULL;
static c_node* g_freenode256= NULL;
static c_node* g_freenode512= NULL;


c_plex* c_createplex(c_plex** phead, size_t nmax, size_t cbelement)
{
	c_plex* p;
	size_t size = 0;

	if (!nmax || !cbelement) 
		return NULL;
	
	if (!phead) 
		return NULL;

	size = sizeof(c_plex) + nmax * cbelement;
	p = (c_plex*)malloc(size);

	if (!p) 
	  return NULL;

	memset((void*)p, 0, size);		// initialize default 0

	p->flag = c_plex_flag;
	p->nfree = nmax;
	p->pnext = *phead;

	*phead = p;						// add the table

	return p;
}

void c_releaseplex(c_plex* _pplex)
{
	c_plex* phead = g_plexhead;
	c_plex* pplex = _pplex;

	if (!phead || !pplex) return;
	if (pplex->flag != c_plex_flag) return;

	/* if the first is delete */
	if (phead == pplex)
	{
		g_plexhead = pplex->pnext;
		free((void*)pplex);	
	}
	else
	{
		while (phead && phead->pnext != pplex)
		{
			phead = phead->pnext; /* if finded exit */
		}
		if (phead)
		{
			phead->pnext = pplex->pnext;
			free((void*)pplex);	
		}
	}
}

c_plex* c_plexdata(c_block* _block)
{
	if (!_block) 
		return NULL;

	if(_block->flag != c_block_flag)
		return NULL;

	return _block->pplex;
}

void c_freeall(void)
{	
	c_plex* p = g_plexhead;
	c_plex* pnext;

	while (p)
	{
		void* dp = (void*)p;
		pnext = p->pnext;
		free(dp);
		p = pnext;
	}	

	g_plexhead	 = NULL;
	g_freenode64 = NULL;
	g_freenode128= NULL;
	g_freenode256= NULL;
	g_freenode512= NULL;
}

void c_allocexit(void)
{
	static size_t v_exit = 0;
	if(v_exit == 0)
	{
		v_exit += 1;
		atexit(c_freeall);	
	}
}

void* c_alloc(size_t _size)
{
	c_allocexit();

	if(_size == 0)
	{
		return NULL;
	}
	else
	{	
		// {{ 线程互锁

		static c_plex* pplex64, *pplex128, *pplex256, *pplex512;						// 保留每个 PLEX 地址
		static size_t blocksize, datasize64, datasize128, datasize256, datasize512;
		
		c_plex* pnewplex = NULL;
		c_block* pblock = NULL;
		c_node* pfreenode = NULL;

		int i;

		if(_size <= 64)
		{
			if(!g_freenode64)
			{
				blocksize = c_round4(sizeof(c_block) + 65 * sizeof(char));
				pnewplex = c_createplex(&g_plexhead, c_block_amount, blocksize);
				
				pplex64 = pnewplex;
				datasize64 = blocksize - sizeof(c_block) - sizeof(char);											// shiji data size
				
				pfreenode = (c_node*)(pnewplex + 1);	
				pfreenode = (c_node*)((unsigned char*)pfreenode + ((blocksize * c_block_amount) - blocksize));		// point the last block
				for (i = c_block_amount - 1; i >= 0; i--)
				{	
					pfreenode->pnext = g_freenode64; 
					g_freenode64 = pfreenode;
					if( i > 0) { pfreenode = ( c_node*)((unsigned char*)pfreenode - blocksize); }					// reset pnode site 
				}	
			}
			
			pblock = (c_block*)g_freenode64;
			g_freenode64 = g_freenode64->pnext;
			
			pblock->flag = c_block_flag;
			pblock->alloc_size = datasize64;
			pblock->data_size = _size;
			pblock->tags = 64;
			pblock->pplex = pplex64;

			pplex64->nfree -= 1;	
		}
		else if(_size <= 128)
		{
			if(!g_freenode128)
			{
				blocksize = c_round4(sizeof(c_block) + 129 * sizeof(char));
				pnewplex = c_createplex(&g_plexhead, c_block_amount, blocksize);
				
				pplex128 = pnewplex;
				datasize128 = blocksize - sizeof(c_block) - sizeof(char);											// shiji data size
				
				pfreenode = (c_node*)(pnewplex + 1);	
				pfreenode = (c_node*)((unsigned char*)pfreenode + ((blocksize * c_block_amount) - blocksize));		// point the last block
				for (i = c_block_amount - 1; i >= 0; i--)
				{	
					pfreenode->pnext = g_freenode128; 
					g_freenode128 = pfreenode;
					if( i > 0) { pfreenode = ( c_node*)((unsigned char*)pfreenode - blocksize); }					// reset pnode site 
				}	
			}
			
			pblock = (c_block*)g_freenode128;
			g_freenode128 = g_freenode128->pnext;
			
			pblock->flag = c_block_flag;
			pblock->alloc_size = datasize128;
			pblock->data_size = _size;
			pblock->tags = 128;
			pblock->pplex = pplex128;

			pplex128->nfree -= 1;	
		}
		else if(_size <= 256)
		{
			if(!g_freenode256)
			{
				blocksize = c_round4(sizeof(c_block) + 257 * sizeof(char));
				pnewplex = c_createplex(&g_plexhead, c_block_amount, blocksize);
				
				pplex256 = pnewplex;
				datasize256 = blocksize - sizeof(c_block) - sizeof(char);											// shiji data size
				
				pfreenode = (c_node*)(pnewplex + 1);	
				pfreenode = (c_node*)((unsigned char*)pfreenode + ((blocksize * c_block_amount) - blocksize));		// point the last block
				for (i = c_block_amount - 1; i >= 0; i--)
				{	
					pfreenode->pnext = g_freenode256; 
					g_freenode256 = pfreenode;
					if( i > 0) { pfreenode = ( c_node*)((unsigned char*)pfreenode - blocksize); }					// reset pnode site 
				}	
			}
			
			pblock = (c_block*)g_freenode256;
			g_freenode256 = g_freenode256->pnext;
			
			pblock->flag = c_block_flag;
			pblock->alloc_size = datasize256;
			pblock->data_size = _size;
			pblock->tags = 256;
			pblock->pplex = pplex256;

			pplex256->nfree -= 1;	
		}
		else if(_size <= 512)
		{
			if(!g_freenode512)
			{
				blocksize = c_round4(sizeof(c_block) + 513 * sizeof(char));
				pnewplex = c_createplex(&g_plexhead, c_block_amount, blocksize);
				
				pplex512 = pnewplex;
				datasize512 = blocksize - sizeof(c_block) - sizeof(char);											// shiji data size
				
				pfreenode = (c_node*)(pnewplex + 1);	
				pfreenode = (c_node*)((unsigned char*)pfreenode + ((blocksize * c_block_amount) - blocksize));		// point the last block
				for (i = c_block_amount - 1; i >= 0; i--)
				{	
					pfreenode->pnext = g_freenode512; 
					g_freenode512 = pfreenode;
					if( i > 0) { pfreenode = ( c_node*)((unsigned char*)pfreenode - blocksize); }					// reset pnode site 
				}	
			}
			
			pblock = (c_block*)g_freenode512;
			g_freenode512 = g_freenode512->pnext;

			pblock->flag = c_block_flag;
			pblock->alloc_size = datasize512;
			pblock->data_size = _size;
			pblock->tags = 512;
			pblock->pplex = pplex512;

			pplex512->nfree -= 1;	
		}
		else
		{
			blocksize = c_round4(sizeof(c_block) + ((_size + 1) * sizeof(char)));
			pnewplex = c_createplex(&g_plexhead, 1, blocksize);

			pblock = (c_block*)(pnewplex + 1);
			
			pblock->flag = c_block_flag;
			pblock->alloc_size = blocksize - sizeof(c_block) - sizeof(char);
			pblock->data_size = _size;
			pblock->tags = _size;
			pblock->pplex = pnewplex;

			pnewplex->nfree -= 1;
		}
		// }}

		return (void*)(pblock + 1);			
	}
}


void* c_realloc(void* _ptr, size_t _newsize, int _reserve)
{
	void* ptr = _ptr;
	c_block* pblock;

	// 如果_ptr为空 则新分配
	if(!ptr) 
		return c_alloc(_newsize);

	// 如果传递的不是c_alloc分配的指针 则新分配
	pblock = c_blockdata(ptr);
	if(!pblock)
		return c_alloc(_newsize);

	if(_newsize > pblock->alloc_size)
	{
		void* newbuf = c_alloc(_newsize);
		if(_reserve)
		{
			memcpy(newbuf, ptr, pblock->data_size);
		}

		c_free(ptr);
		ptr = newbuf;
	}
	else
	{
		if(_reserve) 
		{
			memset((unsigned char*)ptr + pblock->data_size, 0, pblock->alloc_size - pblock->data_size);
		}
		else
		{
			memset(ptr, 0, pblock->alloc_size);
		}

		pblock->data_size = _newsize;	
	}
	return ptr;
}


void c_free(void* _ptr)
{
	void* ptr = _ptr;
	c_block* pblock;

	if(!ptr) return;

	pblock = c_blockdata(ptr);						// 获取BLOCK块地址
	if(pblock)
	{
		c_plex* pplex = (c_plex*)pblock->pplex;		// 获取PLEX块地址
		c_node* pnode = (c_node*)pblock;

		memset(ptr, 0, pblock->data_size);			// clean the data

		if(pblock->tags <= 64)
		{
			pnode->pnext = g_freenode64;
			g_freenode64 = pnode;

			if(++pplex->nfree == c_block_amount)
			{
				c_releaseplex(pplex);
				g_freenode64 = NULL;
			}
		}
		else if(pblock->tags <= 128)
		{
			pnode->pnext = g_freenode128;
			g_freenode128 = pnode;

			if(++pplex->nfree == c_block_amount)
			{
				c_releaseplex(pplex);
				g_freenode128 = NULL;
			}
		}
		else if(pblock->tags <= 256)
		{
			pnode->pnext = g_freenode256;
			g_freenode256 = pnode;

			if(++pplex->nfree == c_block_amount)
			{
				c_releaseplex(pplex);
				g_freenode256 = NULL;
			}
		}
		else if(pblock->tags <= 512)
		{
			pnode->pnext = g_freenode512;
			g_freenode512 = pnode;

			if(++pplex->nfree == c_block_amount)
			{
				c_releaseplex(pplex);
				g_freenode512 = NULL;
			}
		}
		else
		{
			c_releaseplex(pplex);
		}
	}		
}

c_block* c_blockdata(void* _ptr)
{
	void* ptr = _ptr;
	c_block* pblock;
	if(!ptr) return NULL;

	pblock = ((c_block*)ptr) - 1;
	if(pblock->flag != c_block_flag)
		return NULL;
	return pblock;
}

size_t c_block_allocsize(void* _ptr)
{
	c_block* pblock = c_blockdata(_ptr);
	if(pblock)
		return pblock->alloc_size;
	return 0;
}

size_t c_block_datasize(void* _ptr)
{
	c_block* pblock = c_blockdata(_ptr);
	if(pblock)
		return pblock->data_size;
	return 0;
}


// ###########################################################

#ifdef C_MALLOC_TEST

void c_malloc_test()
{
	char* p;
	c_block* pblock;
	int i;

	system("pause");

	printf("序号\t块地址\tPLEX地址\t分配尺寸\t实际大小\t空闲块\n");
	for(i = 1; i < 600; i++)
	{
		p = (char*)c_alloc(i);
		pblock = c_blockdata(p);
		printf("%d\t%x\t%x\t\t%d\t\t%d\t\t%d\n", i, (size_t)pblock, (size_t)pblock->pplex, pblock->alloc_size, pblock->data_size, ((c_plex*)pblock->pplex)->nfree);
//		c_free(p);
	}

	system("pause");

	c_freeall();

	system("pause");

	printf("\n重新分配内存\n");
	p = NULL;
	for(i = 1; i < 200; i++)
	{
		p = (char*)c_realloc(p, i, 1);
		pblock = c_blockdata(p);
		printf("%x\t%x\t\t%d\t\t%d\t\t%d\n", (size_t)pblock, (size_t)pblock->pplex, pblock->alloc_size, pblock->data_size, ((c_plex*)pblock->pplex)->nfree);
	}

	c_freeall();
}

#endif


