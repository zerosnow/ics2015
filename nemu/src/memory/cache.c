#include <stdlib.h>
#include <time.h>

#include <common.h>

#define W_WIDTH 6
#define Q_WIDTH 3
#define R_WIDTH 7
#define F_WIDTH (27-W_WIDTH-Q_WIDTH-R_WIDTH)

#define BLOCK_SIZE (1 << W_WIDTH)
#define BLOCK_NUM (1 << Q_WIDTH) 
#define GROUP_NUM (1 << R_WIDTH)

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
void update_cache(hwaddr_t, void *, size_t);

typedef union {
	struct {
		uint32_t w	:W_WIDTH;
		uint32_t q 	:Q_WIDTH;
		uint32_t r 	:R_WIDTH;
		uint32_t f 	:F_WIDTH;
	};
	uint32_t addr;
}cache_addr;

typedef  struct {
	struct {
		uint32_t q 	:Q_WIDTH;
		uint32_t f 	:F_WIDTH;
		uint32_t valid	:1;
	};
	uint8_t block[BLOCK_SIZE];
}cache_block;

cache_block cache[GROUP_NUM][BLOCK_NUM];	


void init_cache() {
	int i, j;
	for(i = 0; i < GROUP_NUM; i ++) {
		for(j = 0; j < BLOCK_NUM; j ++) {
			cache[i][j].valid = 0;
		}
	}
}


uint32_t cache_read(hwaddr_t addr,  size_t len) {
	int i;
	cache_addr caddr;
	caddr.addr = addr;
	uint32_t temp;
	for (i=0;i<Q_WIDTH;i++) {
		if (cache[caddr.r][i].q == caddr.q && cache[caddr.r][i].f == caddr.f && cache[caddr.r][i].valid == 1) {
			if (len + caddr.w <= 64) {
				memcpy(&temp, &cache[caddr.r][i].block[caddr.w], len);
				return temp;
			}
			
		} 
	}
	for (i=0;i<Q_WIDTH;i++) {
		if (cache[caddr.r][i].valid == 0) {
			cache[caddr.r][i].q = caddr.q;
			cache[caddr.r][i].f = caddr.f;
			cache[caddr.r][i].valid = 1;
			update_cache(addr, cache[caddr.r][i].block, BLOCK_SIZE);
			return dram_read(addr, len);
		} 
	}
	srand(time(0));
	i = rand()%8;
	cache[caddr.r][i].q = caddr.q;
	cache[caddr.r][i].f = caddr.f;
	cache[caddr.r][i].valid = 1;
	update_cache(addr, cache[caddr.r][i].block, BLOCK_SIZE);
	return dram_read(addr, len);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	int i;
	cache_addr caddr;
	caddr.addr = addr;
	for (i=0;i<Q_WIDTH;i++)
		if (cache[caddr.r][i].q == caddr.q && cache[caddr.r][i].f == caddr.f && cache[caddr.r][i].valid == 1) 
			memcpy(&cache[caddr.r][i].block[caddr.w], &data, len);
	dram_write(addr, len, data);
}


