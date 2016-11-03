/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  Only a header is stored with the size to allow
 * for realloc() to retrieve the block size.  Blocks are never coalesced
 * or reused in this naive implementation. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * In my approach, I'll use explicit free counter to execute memory allocation
 * instead of implicit free counter which is naive.
 * Some code is from csapp from book
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#include "mm.h"
#include "memlib.h"
#include "config.h"

/* defines ALIGNMENT */
#define SWSIZE 4         /* single word size = 4 bytes */
#define DWSIZE 8         /*double word size = 8 bytes */
#define OVERHEAD 8
#define TWSIZE 12        /* triple word size = 12 bytes */
#define L 20              /* freelist size */
#define CHUNKSIZE (1<<12) /* initial heap size */
#define BUF (1<<7)

/* Global variable */
void *freelist[L]; /* explicit list */
char *bk; /* The pointer points to the block. */

/* Additional*/
#define MIN(x, y) ((x) < (y) ? (x) : (y)) /*minimum*/
#define MAX(x, y) ((x) > (y) ? (x) : (y)) /*maximum*/

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p and from cs:app*/
#define GET(p)       (*(unsigned int *)(p))
#define PUTT(p, val)  (*(unsigned int *)(p) = (val) | GET_TG(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_TG(p)    (GET(p) & 0x2)
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define SET_TG(p)    (*(unsigned int *)(p) = GET(p) | 0x2)
#define RESET_TG(p)  (*(unsigned int *)(p) = GET(p) & ~0x2)
#define SET_PT(p, pt)(*(unsigned int *)(p) = (unsigned int )(pt))

/* Given block ptr pt, compute address of its header and footer */
#define HD(pt)       ((char *)(pt) - SWSIZE)
#define FT(pt)       ((char *)(pt) + GET_SIZE(HD(pt)) - DWSIZE)
#define LEFT(pt)       ((void *)(pt))
#define RT(pt)         ((void *)(pt) + SWSIZE)

/* Put values to the block where pt points to */
#define PHD(pt, elem)       (PUT(HD(pt), (int)elem))
#define PFT(pt, elem)       (PUT(FT(pt), (int)elem))
#define PLEFT(pt, elem)     (PUT(LEFT(pt), (int)elem))
#define PRT(pt, elem)       (PUT(RT(pt), (int)elem))

/* Given block ptr pt, compute address of next and previous blocks */
#define NEXT_BK(pt)   ((char *)(pt) + GET_SIZE((char *)(pt) - SWSIZE))
#define PREV_BK(pt)   ((char *)(pt) - GET_SIZE((char *)(pt) - DWSIZE))

/* Get address of left and right nodes. */
#define ADD_LEFT(pt) (*(void **)(pt))
#define ADD_RT(pt)   (*(void **)(RT(pt)))

/* Check alignment*/
#define ALIGNMENT_SET(pt) (((size)(pt) + 7) & ~0x7)

static void *merge (void *pt);
static void *heap_extend (size_t sz);
static void place (void *pt, size_t sz);
static void insert (void *pt, size_t sz);
static void delete (void *pt);

/*********************************************************
 *  NOTE TO STUDENTS: Before you do anything else, please
 *  provide your team information in the following struct.
 *********************************************************/
team_t team = {
    /* Team name */
    "group417: FishBall",
    /* First member's full name */
    "Sheng Zhou",
    /* First member's SLO (@cs.vt.edu) email address */
    "zsheng2",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's SLO (@cs.vt.edu) email address (leave blank if none) */
    ""
};

/*
 *  mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    int counter;
    char *hp;  /* The beginning pointer for the heap */

        /* Make the freelist to be NULL list */
    for (counter = 0; counter < L; counter++) {
        freelist[counter] = NULL;
    }

        /* Check error */
    if ((hp = mem_sbrk(2 * DWSIZE)) != NULL)
    {
        PUT(hp, 0);
        /* paddings */
        PUT(hp + SWSIZE, PACK(DWSIZE, 1));
        PUT(hp + DWSIZE, PACK(DWSIZE, 1));
        PUT(hp + TWSIZE, PACK(0, 1));
        bk = hp + DWSIZE;

        if (heap_extend(CHUNKSIZE) == NULL)
        {
                return -1;
        }
    }
    else
        {
        return -1;
        }
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 * Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t sz;  /* block size */
    size_t ext;  /* check to see if heap is fit or not */
    void *pt;
    int counter;
    pt = NULL;
    counter = 0;

    /* error check */
    if (size == 0)
    {
            return NULL;
    }
        else
        {
        if (size > 2 * SWSIZE)
                {
                        sz = ((size + 2 * DWSIZE - 1) / DWSIZE) * DWSIZE;
                }
                else
                {
                        sz = 4 * SWSIZE;
                }
                /* check free block and select it from list */
                size = sz;
                while (counter < L)
                {
                        if (((freelist[counter] != NULL) && (size <= 1))
                              || (counter == L - 1))
                        {
                                pt = freelist[counter];
                                while ((pt != NULL)
                                        && ((GET_TG(HD(pt)))
                                        || (sz > GET_SIZE(HD(pt)))))
                                {
                                        pt = ADD_LEFT(pt);
                                }
                                if (pt == NULL)
                                {
                                        /* Check break condition */
                                }
                                else
                                {
                                    break;
                                }
                        }

                        size >>= 1;
                        counter++;
                }

                /* Heap extension for shortage of size */
                if (pt != NULL)
                {
                        /* check if there is free blocks */
                }
                else
                {
                        ext = MAX(sz, CHUNKSIZE);
                        if ((pt = heap_extend(ext)) != NULL)
                        {
                                /* not null */
                        }
                        else
                        {
                                return NULL;
                        }
                }

                place(pt, sz);

                return pt;
        }
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *oldptr, size_t size)
{
    void *pt = oldptr;
        size_t nsz = size;
        int rm, ex, bb;

        /* make sure the block is valid */
        if (size != 0)
        {
            if (nsz > DWSIZE)
                {
                    nsz = nsz + 2 * DWSIZE - 1;
                }
                else
                {
                     nsz = 4 * SWSIZE;
                }

                nsz += BUF;
                /* calculate block bf */
                bb = GET_SIZE(HD(oldptr)) - nsz;

                                if (bb >= 2 * BUF)
                {
                    /* leave blank */
                }
                else
                {
                    SET_TG(HD(NEXT_BK(pt)));
                }

                if (bb < 0)
                {
                    /* check freeable of next block */
                    if (!GET_SIZE(HD(NEXT_BK(oldptr)))  ||
                            !GET_ALLOC(HD(NEXT_BK(oldptr))))
                        {
                            rm = GET_SIZE(HD(NEXT_BK(oldptr)))
                                + GET_SIZE(HD(oldptr)) - nsz;
                                if (rm >= 0)
                                {
                                   /* nothing */
                                }
                                else
                                {
                                    ex = MAX(-rm, CHUNKSIZE);
                                        if (heap_extend(ex) != NULL)
                                        {
                                            /* Good */
                                        }
                                        else
                                        {
                                            return NULL;
                                        }
                                        rm += ex;
                                }
                                delete(NEXT_BK(oldptr));

                                PHD((oldptr), PACK(nsz + rm, 1));
                        PFT((oldptr), PACK(nsz + rm, 1));
                        }
                        else
                        {
                            pt = mm_malloc(nsz - DWSIZE);
                                memmove(pt, oldptr, MIN(size, nsz));
                                mm_free(oldptr);
                        }
                        bb = GET_SIZE(HD(pt)) - nsz;
                }
                        /* return with reallocated block */
                        return pt;
        }
        else
        {
            return NULL;
        }
        return pt;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t sz = GET_SIZE(HD(ptr));
        PFT(ptr, PACK(sz, 0));
        PHD(ptr, PACK(sz, 0));
        RESET_TG(HD(NEXT_BK(ptr)));
        insert(ptr, sz);
        merge(ptr);
        return;
}

/* Place block */
static void place (void *pt, size_t sz)
{
    size_t psz = GET_SIZE(HD(pt));
        delete(pt); /* remove block */
        size_t rm = psz - sz;

        /* do/not split block */
        if (rm < 2 * DWSIZE)
        {
            PUTT(HD(pt), PACK(psz, 1));
                PUTT(FT(pt), PACK(psz, 1));
        }
        else
        {
            PUTT(HD(pt), PACK(sz, 1));
                PUTT(FT(pt), PACK(sz, 1));
                pt = NEXT_BK(pt);
                PHD((pt), PACK(rm, 0));
                PFT((pt), PACK(rm, 0));
                insert(pt, rm);
        }
        return;
}

/* The function is to exted the heap with system call
 */
static void *heap_extend (size_t size)
{
    void *node;
        size_t wd = size/SWSIZE;
        size_t sz = (wd % 2)? (SWSIZE * (wd + 1)) : (SWSIZE * wd);

        /* heap extension */
        if ((long) (node = mem_sbrk(sz)) == -1)
        {
             return NULL;
        }

        /* do settings */
        PHD((node), PACK(size, 0));
        PFT((node), PACK(size, 0));
        PHD((NEXT_BK(node)), PACK(0, 1));
        insert (node, sz);
        return merge(node);
}

/* Merge one free block with any adjacent free blocks*/
static void* merge (void *pt)
{
                size_t sz = GET_SIZE(HD(pt));
                size_t next = GET_ALLOC(HD(NEXT_BK(pt)));
                size_t prev = GET_ALLOC(HD(PREV_BK(pt)));

                if (next && prev)
                {
                        return pt;
                }

                else {

                        if (GET_TG(HD(PREV_BK(pt))))
                        {
                                prev = 1;
                        }

                        delete (pt);

                        /* different cases */
                        if (!next && !prev)
                        {
                delete(NEXT_BK(pt));
                delete(PREV_BK(pt));
                sz += GET_SIZE(HD(NEXT_BK(pt))) + GET_SIZE(HD(PREV_BK(pt)));
                PUTT(HD(PREV_BK(pt)), PACK(sz, 0));
                PUTT(FT(NEXT_BK(pt)), PACK(sz, 0));
                pt = PREV_BK(pt);
                        }
                        else if (!next && prev)
                        {
                delete(NEXT_BK(pt));
                sz += GET_SIZE(HD(NEXT_BK(pt)));
                PUTT(HD(pt), PACK(sz, 0));
                PUTT(FT(pt), PACK(sz, 0));
                        }
                        else
                        {
                delete(PREV_BK(pt));
                sz += GET_SIZE(HD(PREV_BK(pt)));
                PUTT(HD(PREV_BK(pt)), PACK(sz, 0));
                PUTT(FT(pt), PACK(sz, 0));
                pt = PREV_BK(pt);
                        }

                        insert(pt, sz);

                        return pt;
                }
}

/* Insert node method */
static void insert (void* pt, size_t size)
{
    /* Check if freelist is null or not. */
		int counter;
        void *find;
		void *add;
		counter = 0;
		find = pt;
		add = NULL;
		
        while ((size > 1) && (counter < (L - 1)))
        {
            size >>= 1;
                counter ++;

        }

        find = freelist[counter];

        while ((size > GET_SIZE(HD(find))) && (find != NULL))
        {
            add = find;
                find = ADD_LEFT(find);
        }

        if (find == NULL)
        {
            if (add == NULL)
                {
                    SET_PT(LEFT(pt), NULL);
                        SET_PT(RT(pt), NULL);
					    freelist[counter] = pt;
                }
                else
                {
                    SET_PT(LEFT(pt), NULL);
                        SET_PT(RT(pt), add);
                        SET_PT(LEFT(pt), pt);
                }
        }
        else
        {
            if (add == NULL)
                {
                    SET_PT(LEFT(pt), find);
                        SET_PT(RT(find), pt);
                        SET_PT(RT(pt), NULL);
                        freelist[counter] = pt;
                }
                else
                {
                    SET_PT(LEFT(pt), find);
                        SET_PT(RT(find), pt);
                        SET_PT(RT(pt), add);
                        SET_PT(LEFT(add), pt);
                }
        }

        return;
}

/* Delete node method */
static void delete (void *pt)
{
    int counter = 0;
        size_t sz = GET_SIZE(HD(pt));

        while ((sz > 1) && (counter < L-1))
        {
            sz >>= 1;
                counter++;
        }

        if (ADD_LEFT(pt) == NULL)
        {
            if (ADD_RT(pt) == NULL)
                {
                    freelist[counter] = NULL;
                }
                else
                {
                    SET_PT(LEFT(ADD_RT(pt)), NULL);
                }

        }
        else
        {
            if (ADD_RT(pt) == NULL)
                {
                    SET_PT(RT(ADD_LEFT(pt)), NULL);
                    freelist[counter] = ADD_LEFT(pt);
                }
                else
                {
                    SET_PT(RT(ADD_LEFT(pt)), ADD_RT(pt));
                        SET_PT(LEFT(ADD_RT(pt)), ADD_LEFT(pt));
                }
        }

        return;
}

