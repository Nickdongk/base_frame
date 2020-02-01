/***************************************************************************
 * 
 * Copyright (c) 2016 zkdnfcf, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 /**
 * @file hash.h
 * @author zk(zkdnfc@163.com)
 * @date 2016/05/31 18:26:01
 * @version $Revision$ 
 * @brief 
 *  
 **/
#ifndef _MAP_H
#define _MAP_H

#include "rbtree.h"

struct map {
    struct rb_root root;
    unsigned int count;
};

struct map_n {
    struct rb_node node;
    char *key;
    void *val;
    unsigned int val_len;
};

extern struct map *create_map(void);
extern void free_map(struct map *tree);

extern struct map_n *map_first_node(struct map *tree);
extern struct map_n *map_next_node(struct map_n *map_node);
extern int put_to_map(struct map *map, char* key, void* val, unsigned int val_len); 
extern void *get_from_map(struct map *map, char *str); 

#endif  //_MAP_H

/* vim: set ts=4 sw=4 sts=4 tw=100 */
