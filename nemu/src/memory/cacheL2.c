#include <stdlib.h>
#include <time.h>

#include <common.h>

#define W_WIDTH 6
#define Q_WIDTH 4
#define R_WIDTH 12
#define F_WIDTH (27-W_WIDTH-Q_WIDTH-R_WIDTH)

#define BLOCK_SIZE (1 << W_WIDTH)
#define BLOCK_NUM (1 << Q_WIDTH) 
#define GROUP_NUM (1 << R_WIDTH)

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
void update_cacheL2(hwaddr_t, void *, size_t);
void update_dram(hwaddr_t, void *, size_t);

typedef union {
	struct {
		uint32_t w	:W_WIDTH;
		uint32_t q 	:Q_WIDTH;
		uint32_t r 	:R_WIDTH;
		uint32_t f 	:F_WIDTH;
	};
	uint32_t addr;
}L2cache_addr;

typedef  struct {
	struct {
		uint32_t q 	:Q_WIDTH;
		uint32_t f 	:F_WIDTH;
		uint32_t valid	:1;
		uint32_t dirty	:1;
	};
	uint8_t block[BLOCK_SIZE];
}L2cache_block;

L2cache_block L2cache[GROUP_NUM][BLOCK_NUM];	

void init_L2cache() {
	int i, j;
	for(i = 0; i < GROUP_NUM; i ++) {
		for(j = 0; j < BLOCK_NUM; j ++) {
			L2cache[i][j].valid = 0;
			L2cache[i][j].dirty = 0;
		}
	}
}

uint32_t L2cache_read(hwaddr_t addr,  size_t len) {
	int i;
	L2cache_addr caddr;
	L2cache_addr dram_addr;
	caddr.addr = addr;
	uint32_t temp;
	for (i=0;i<Q_WIDTH;i++) {
		if (L2cache[caddr.r][i].q == caddr.q && L2cache[caddr.r][i].f == caddr.f && L2cache[caddr.r][i].valid == 1) {
			if (len + caddr.w <= BLOCK_SIZE) {
				memcpy(&temp, &L2cache[caddr.r][i].block[caddr.w], len);
				return temp;
			}
		} 
	}
	for (i=0;i<Q_WIDTH;i++) {
		if (L2cache[caddr.r][i].valid == 0) {
			L2cache[caddr.r][i].q = caddr.q;
			L2cache[caddr.r][i].f = caddr.f;
			L2cache[caddr.r][i].valid = 1;
			L2cache[caddr.r][i].dirty = 0;
			update_cacheL2(addr, L2cache[caddr.r][i].block, BLOCK_SIZE);
			return dram_read(addr, len);
		} 
	}
	srand(time(0));
	i = rand()%BLOCK_NUM;
	if (L2cache[caddr.r][i].dirty == 1) {
		dram_addr.q = L2cache[caddr.r][i].q;
		dram_addr.r = caddr.r;
		dram_addr.f = L2cache[caddr.r][i].f;
		dram_addr.w = 0;
		update_dram(dram_addr.addr, L2cache[caddr.r][i].block, BLOCK_SIZE);
	}
	L2cache[caddr.r][i].q = caddr.q;
	L2cache[caddr.r][i].f = caddr.f;
	L2cache[caddr.r][i].valid = 1;
	L2cache[caddr.r][i].dirty = 0;
	update_cacheL2(addr, L2cache[caddr.r][i].block, BLOCK_SIZE);
	return dram_read(addr, len);
}

void update_cacheL1(hwaddr_t addr, void *data, size_t len) {
	int i;
	L2cache_addr caddr;
	caddr.addr = addr;
	while(1) {
		for (i=0;i<Q_WIDTH;i++) {
			if (L2cache[caddr.r][i].q == caddr.q && L2cache[caddr.r][i].f == caddr.f && L2cache[caddr.r][i].valid == 1) {
				memcpy(data, L2cache[caddr.r][i].block, len);
				return;
			} 
		}
		L2cache_read(addr, 1);
	}
}
void L2cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	int i;
	L2cache_addr caddr;
	//L2cache_addr dram_addr;
	caddr.addr = addr;
	for (i=0;i<Q_WIDTH;i++) {
		if (L2cache[caddr.r][i].q == caddr.q && L2cache[caddr.r][i].f == caddr.f && L2cache[caddr.r][i].valid == 1)  {
			memcpy(&L2cache[caddr.r][i].block[caddr.w], &data, len);
			L2cache[caddr.r][i].dirty = 1;
			return ;
		}
	}
	dram_write(addr, len, data);
	// for (i=0;i<Q_WIDTH;i++) {
	// 	if (L2cache[caddr.r][i].valid == 0) {
	// 		L2cache[caddr.r][i].q = caddr.q;
	// 		L2cache[caddr.r][i].f = caddr.f;
	// 		L2cache[caddr.r][i].valid = 1;
	// 		L2cache[caddr.r][i].dirty = 0;
	// 		update_cacheL2(addr, L2cache[caddr.r][i].block, BLOCK_SIZE);
	// 		return ;
	// 	} 
	// }
	// srand(time(0));
	// i = rand()%BLOCK_NUM;
	// if (L2cache[caddr.r][i].dirty == 1) {
	// 	dram_addr.q = L2cache[caddr.r][i].q;
	// 	dram_addr.r = caddr.r;
	// 	dram_addr.f = L2cache[caddr.r][i].f;
	// 	dram_addr.w = 0;
	// 	update_dram(dram_addr.addr, L2cache[caddr.r][i].block, BLOCK_SIZE);
	// }
	// L2cache[caddr.r][i].q = caddr.q;
	// L2cache[caddr.r][i].f = caddr.f;
	// L2cache[caddr.r][i].valid = 1;
	// L2cache[caddr.r][i].dirty = 0;
	// update_cacheL2(addr, L2cache[caddr.r][i].block, BLOCK_SIZE);
	// return ;
}


