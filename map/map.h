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

#include <rbtree.h>
#include <stddef.h>


struct map {
    struct rb_root root;
    size_t count;
};

struct map_n {
    struct rb_node node;
    char *key;
    void *val;
    size_t val_len;
};

typedef void (*release_map_node)(struct map_n *map_node);

extern struct map *map_create(void);
extern void map_release(struct map *map, release_map_node release_callback);

extern struct map_n *map_first(struct map *map);
extern struct map_n *map_next(struct map_n *map_node);
extern int map_add(struct map *map, char* key, void* val, size_t val_len);
extern int map_remove(struct map *map, char *key);
extern void *map_get(struct map *map, char *str, size_t *val_len);
extern size_t map_length(struct map *map);

#endif  //_MAP_H

