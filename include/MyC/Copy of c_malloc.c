
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "c_malloc.h"


static struct c_plex* g_bufferhead = NULL;
static unsigned int g_plexsize = 0;

static struct c_freenode* g_freenode64	= NULL;
static struct c_freenode* g_freenode128	= NULL;
static struct c_freenode* g_freenode256	= NULL;
static struct c_freenode* g_freenode512	= NULL;


struct c_plex* c_createplex(struct c_plex** phead, unsigned int nmax, unsigned int cbelement)
{
	struct c_plex* p = NULL;
	unsigned int alloclen = 0;

	if( nmax == 0 || cbelement == 0 ) 
		return NULL;
	
//	c_assert( phead != NULL ); 
	if( phead == NULL ) 
		return NULL;

	alloclen = sizeof(struct c_plex) + nmax * cbelement;
	p = (struct c_plex*)malloc(alloclen);

//	c_assert( p != NULL ); 
	if( p == NULL ) 
		return NULL;

	memset((void*)p, 0, alloclen);		/* initialize default 0 */

	p->flag = c_plexflag;
	p->nfree = nmax;
	p->pnext = *phead;
	*phead = p;

	g_plexsize++;

	return p;
}


struct c_plex* c_plexdata(struct c_allocstruct* _as)
{
	struct c_plex* v_pl = NULL;
	if( _as == NULL ) 
		return NULL;

	v_pl = (struct c_plex*)_as - 1;

	if(v_pl->flag != c_plexflag || v_pl == NULL) 
		return NULL;
	else
		return v_pl;
}


void c_freeplex(struct c_plex* pblock)
{
	struct c_plex* phead = g_bufferhead;
	void* deptr = (void*)pblock;

	if (phead == NULL || pblock == NULL) return;
	if (pblock->flag != c_plexflag) return;

	/* if the first is delete */
	if (phead == pblock)
	{
		g_bufferhead = pblock->pnext;
		free(deptr);
	}
	else
	{
		while (phead != NULL && phead->pnext != pblock)
		{
			phead = phead->pnext; /* if finded exit */
		}
		if (phead != NULL)
		{
			phead->pnext = pblock->pnext;
			free(deptr);
		}
	}
}


void c_freeall(void)
{	
	struct c_plex* p = g_bufferhead;
	struct c_plex* pnext = NULL;

	while (p != NULL)
	{
		void* dp = (void*)p;
		pnext = p->pnext;
		free(dp);
		p = pnext;
	}	

	g_bufferhead	= NULL;
	g_freenode64	= NULL;
	g_freenode128	= NULL;
	g_freenode256	= NULL;
	g_freenode512	= NULL;
}


void c_allocexit(void)
{
	static unsigned int v_exit = 0;
	if(v_exit == 0)
	{
		v_exit += 1;
		atexit(c_freeall);	
	}
}


char* c_alloc(unsigned int _size)
{
	c_allocexit();

	if( _size == 0 )
	{
		return NULL;
	}
	else
	{
		unsigned int allocsize = 0;
		signed int i = 0;

		static unsigned int asize64, asize128, asize256, asize512;
		static struct c_plex* pblock64, *pblock128, *pblock256, *pblock512;
		
		struct c_plex* pnewblock = NULL;
		struct c_freenode* pnode = NULL;
		struct c_allocstruct* buffer = NULL;

		char* pfree = NULL;

		if(_size <= 64)
		{
			if(g_freenode64 == NULL)
			{
				allocsize = c_round4(sizeof(struct c_allocstruct) + 65 * sizeof(char));
				pnewblock = c_createplex(&g_bufferhead, c_blocksize, allocsize);
//				c_assert(pnewblock != NULL); 
				if(pnewblock == NULL) return NULL;
				
				pblock64 = pnewblock;
				asize64 = allocsize - sizeof(struct c_allocstruct) - sizeof(char);							// shiji data size
				
				pnode = (struct c_freenode*)(pnewblock + 1);	
				pnode = (struct c_freenode*)((unsigned char*)pnode + ((allocsize * c_blocksize) - allocsize));		// point the last block
				for (i = c_blocksize - 1; i >= 0; i--)
				{	
					pnode->pnext = g_freenode64; g_freenode64 = pnode;
					if( i > 0) { pnode = (struct c_freenode*)((unsigned char*)pnode - allocsize); }					// reset pnode site 
				}	
			}
			
			buffer = (struct c_allocstruct*)g_freenode64;
			g_freenode64 = g_freenode64->pnext;
			
			buffer->flag = c_allocflag;
			buffer->alen = asize64;
			buffer->dlen = _size;
			buffer->tags = 64;

//			c_assert(pblock64 != NULL);

			pblock64->nfree = pblock64->nfree - 1;
			buffer->phead = pblock64;

		}
		else if(_size <= 128)
		{
			if(g_freenode128 == NULL)
			{
				allocsize = c_round4( sizeof(struct c_allocstruct) + 129 * sizeof(char) );
				pnewblock = c_createplex(&g_bufferhead, c_blocksize, allocsize);
//				c_assert( pnewblock != NULL ); 
				if( pnewblock == NULL ) return NULL;

				pblock128 = pnewblock;
				asize128 = allocsize - sizeof(struct c_allocstruct) - sizeof(char);
				
				pnode = (struct c_freenode*)( pnewblock + 1 );
				pnode = (struct c_freenode*)((unsigned char*)pnode + ((allocsize * c_blocksize) - allocsize));
				for (i = c_blocksize - 1; i >= 0; i--)
				{	
					pnode->pnext = g_freenode128; g_freenode128 = pnode;
					if(i > 0) {	pnode = (struct c_freenode*)((unsigned char*)pnode - allocsize); }
				}	
			}
			
			buffer = (struct c_allocstruct*)g_freenode128;
			g_freenode128 = g_freenode128->pnext;
			
			buffer->flag = c_allocflag;
			buffer->alen = asize128;
			buffer->dlen = _size;
			buffer->tags = 128;

//			c_assert(pblock128 != NULL);

			pblock128->nfree = pblock128->nfree - 1;
			buffer->phead = pblock128;

		}
		else if(_size <= 256)
		{
			if(g_freenode256 == NULL)
			{
				allocsize = c_round4(sizeof(struct c_allocstruct) + 257 * sizeof(char));
				pnewblock = c_createplex(&g_bufferhead, c_blocksize, allocsize);
//				c_assert(pnewblock != NULL); 
				if(pnewblock == NULL) return NULL;

				pblock256 = pnewblock;
				asize256 = allocsize - sizeof(struct c_allocstruct) - sizeof(char);
				
				pnode = (struct c_freenode*)( pnewblock + 1);
				pnode = (struct c_freenode*)((unsigned char*)pnode + ((allocsize * c_blocksize) - allocsize));
				for (i = c_blocksize - 1; i >= 0; i--)
				{	
					pnode->pnext = g_freenode256; g_freenode256 = pnode;
					if(i > 0) { pnode = (struct c_freenode*)((unsigned char*)pnode - allocsize); }
				}		
			}
			
			buffer = (struct c_allocstruct*)g_freenode256;
			g_freenode256 = g_freenode256->pnext;
			
			buffer->flag = c_allocflag;
			buffer->alen = asize256;
			buffer->dlen = _size;
			buffer->tags = 256;

//			c_assert(pblock256 != NULL);

			pblock256->nfree = pblock256->nfree - 1;
			buffer->phead = pblock256;

		}
		else if(_size <= 512)
		{
			if(g_freenode512 == NULL)
			{
				allocsize = c_round4(sizeof(struct c_allocstruct) + 513 * sizeof(char));
				pnewblock = c_createplex( &g_bufferhead, c_blocksize, allocsize );
//				c_assert(pnewblock != NULL); 
				if(pnewblock == NULL) return NULL;

				pblock512 = pnewblock;
				asize512 = allocsize - sizeof(struct c_allocstruct) - sizeof(char);
				
				pnode = (struct c_freenode*)( pnewblock + 1 );
				pnode = (struct c_freenode*)((unsigned char*)pnode + ((allocsize * c_blocksize) - allocsize));
				for (i = c_blocksize - 1; i >= 0; i--)
				{	
					pnode->pnext = g_freenode512; g_freenode512 = pnode;
					if(i > 0) { pnode = (struct c_freenode*)((unsigned char*)pnode - allocsize); }
				}	
			}
			
			buffer = (struct c_allocstruct*)g_freenode512;
			g_freenode512 = g_freenode512->pnext;
			
			buffer->flag = c_allocflag;
			buffer->alen = asize512;
			buffer->dlen = _size;
			buffer->tags = 512;

//			c_assert(pblock512 != NULL);

			pblock512->nfree = pblock512->nfree - 1;
			buffer->phead = pblock512;

		}
		else
		{
			allocsize = c_round4(sizeof(struct c_allocstruct) + ((_size + 1) * sizeof(char)));
			pnewblock = c_createplex( &g_bufferhead, 1, allocsize );
//			c_assert( pnewblock != NULL ); 
			if( pnewblock == NULL ) return NULL;

			buffer = (struct c_allocstruct*)(pnewblock + 1);
			
			buffer->flag = c_allocflag;
			buffer->alen = allocsize - sizeof(struct c_allocstruct) - sizeof(char);
			buffer->dlen = _size;
			buffer->tags = _size;

			buffer->phead = pnewblock;
		}
		
//		c_assert( _size <= buffer->alen );
		pfree = (char*)(buffer + 1);

		return pfree;			
	}
}



char* c_realloc(char* _ptr, unsigned int _newsize, int _reserve)
{
	struct c_allocstruct* v_as = NULL;
	char* buffer = _ptr;
	if( buffer == NULL ) { return c_alloc(_newsize); }

	v_as = c_allocdata( buffer );
	if( v_as == NULL ) { return c_alloc(_newsize); }

	if( _newsize > v_as->alen )
	{
		char* newbuffer = c_alloc(_newsize);
//		c_assert( newbuffer != NULL );
		if( newbuffer == NULL ) return NULL;

		if(_reserve)
		{
			memcpy( newbuffer, buffer, v_as->dlen );
		}

		c_free(buffer);
		buffer = newbuffer;
	}
	else
	{
		if( _reserve ) 
		{
			memset(buffer + v_as->dlen, 0, v_as->alen - v_as->dlen);
		}
		else
		{
			memset(buffer, 0, v_as->alen);
		}

		v_as->dlen = _newsize;
	}

	return buffer;
}


void c_free(char* _ptr)
{
	struct c_allocstruct* v_as = NULL;
	struct c_freenode* pnode = NULL;

	char* buffer = _ptr; if( buffer == NULL ) return;
	
	v_as = c_allocdata( buffer ); if( v_as == NULL ) return;

	if ( v_as != NULL )
	{
		pnode = (struct c_freenode*)v_as;	
		memset( buffer, 0, v_as->dlen );	// reset init data

		if(v_as->tags <= 64)
		{
			pnode->pnext = g_freenode64;
			g_freenode64 = pnode;

			v_as->phead->nfree = v_as->phead->nfree + 1;
			if (v_as->phead->nfree == c_blocksize)
			{
				c_freeplex(v_as->phead);
				g_freenode64 = NULL;
			}
		}
		else if(v_as->tags <= 128)
		{
			pnode->pnext = g_freenode128;
			g_freenode128 = pnode;

			v_as->phead->nfree = v_as->phead->nfree + 1;
			if (v_as->phead->nfree == c_blocksize)
			{
				c_freeplex(v_as->phead);
				g_freenode128 = NULL;
			}

		}
		else if(v_as->tags <= 256)
		{
			pnode->pnext = g_freenode256;
			g_freenode256 = pnode;
		
			v_as->phead->nfree = v_as->phead->nfree + 1;
			if (v_as->phead->nfree == c_blocksize)
			{
				c_freeplex(v_as->phead);
				g_freenode256 = NULL;
			}
		}
		else if(v_as->tags <= 512)
		{
			pnode->pnext = g_freenode512;
			g_freenode512 = pnode;
		
			v_as->phead->nfree = v_as->phead->nfree + 1;
			if (v_as->phead->nfree == c_blocksize)
			{
				c_freeplex(v_as->phead);
				g_freenode512 = NULL;
			}
		}
		else
		{
			c_freeplex(v_as->phead);
		}
	}		
}


struct c_allocstruct* c_allocdata(char* _ptr)
{
	struct c_allocstruct* v_as = NULL;
	if( _ptr == NULL ) return NULL;
	v_as = (struct c_allocstruct*)_ptr - 1;

	if(v_as->flag != c_allocflag || v_as == NULL) 
		return NULL;
	else
		return v_as;
}


unsigned int c_allocalen(char* _ptr)
{
	struct c_allocstruct* v_as = NULL;
	v_as = c_allocdata(_ptr);
	if(v_as == NULL) return 0;
	return v_as->alen;
}


unsigned int c_allocdlen(char* _ptr)
{
	struct c_allocstruct* v_as = NULL;
	v_as = c_allocdata(_ptr);
	if(v_as == NULL) return 0;
	return v_as->dlen;
}


unsigned int c_allocsize(char* _ptr)
{
	return c_allocdlen(_ptr);
}


// ###########################################################
// ≤‚ ‘¥˙¬Î«¯”Ú

void c_malloctest()
{int i = 0;
	system("pause");

	
	for(i = 0; i < 30; i++)
	{
		char* m = c_alloc(200);
		printf("%x\n", m);
		
	}
	system("pause");


	c_freeall();

system("pause");

	
	

}




// ###########################################################

