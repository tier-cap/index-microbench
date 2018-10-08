
#ifndef _BTREE_H
#define _BTREE_H

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYSEXPECT(cond) do { if(!(cond)) { perror("ERROR: "); exit(1); } } while(0)

#define BTNODE_CAPACITY 16 // Number of elements per node; cannot be changed
#define BTNODE_MERGE_THRESHOLD (BTNODE_CAPACITY / 4)
#define BTNODE_KEY   0
#define BTNODE_VALUE 1
#define BTNODE_LEAF  0x1UL
#define BTNODE_INNER 0x2UL
#define BTNODE_ROOT  0x4UL

typedef int (*bt_cmp_t)(uint64_t, uint64_t);  // Comparator call back. Return negative if <, 0 if ==, positive if >

typedef struct btnode_t {
  uint64_t permute;    // Maps logical location to physical location in the node; 4 bits
  int      size;       // Number of elements in the node
  int      level;      // 0 means bottomost
  uint64_t property;   // Bit mask indicating the property of the node
  struct   btnode_t *next, *prev;
  uint64_t data[BTNODE_CAPACITY * 2];
} btnode_t;

typedef struct {
  btnode_t *root;
  bt_cmp_t cmp;
} btree_t;

inline uint64_t *btnode_at(btnode_t *node, int index, int isvalue) {
  assert(index >= 0 && index < node->size && node->size <= BTNODE_CAPACITY);
  return &node->data[(((node->permute >> (index << 2)) & 0xFUL) << 1) + isvalue];
}

int bt_intcmp(uint64_t a, uint64_t b);
int bt_strcmp(uint64_t a, uint64_t b);
btnode_t *btnode_init(uint64_t property);
void btnode_free(btnode_t *node);
void btnode_freeall(btnode_t *node, int level);
void btnode_print(btnode_t *node);
btree_t *bt_init(bt_cmp_t cmp);
void bt_free(btree_t *tree);
int btnode_lb(const btree_t *tree, btnode_t *node, uint64_t key, int *exact);
int btnode_ub(const btree_t *tree, btnode_t *node, uint64_t key);
int btnode_insert(btree_t *tree, btnode_t *node, uint64_t key, uint64_t value);
void btnode_removeat(btnode_t *node, int index);
int btnode_remove(btree_t *tree, btnode_t *node, uint64_t key);
btnode_t *btnode_split(btnode_t *node);
btnode_t *btnode_merge(btnode_t *left, btnode_t *right);
btnode_t *btnode_smo(btree_t *tree, btnode_t *node, uint64_t key, btnode_t *parent, int parent_index);
btnode_t *bt_findleaf(btree_t *tree, uint64_t key);
int bt_insert(btree_t *tree, uint64_t key, uint64_t value);
int bt_remove(btree_t *tree, uint64_t key);
uint64_t bt_find(btree_t *tree, uint64_t key, int *success);

#endif