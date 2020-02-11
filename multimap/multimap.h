#ifndef _MAP_H
#define _MAP_H

#include <rbtree.h>
#include <list.h>

struct multimap {
    struct rb_root root;
    size_t count;
};

struct multimap_n {
    struct rb_node node;
    char *key;
    size_t num;
    struct list_head entry;

};

struct multimap_list_n {
    struct list_head head;
    void *val;
    size_t len;
};

extern struct multimap *multimap_create(void);
extern void multimap_release(struct multimap *src_map);

extern struct multimap_n *map_first(struct multimap *map);
extern struct multimap_n *map_next(struct multimap_n *map_node);
extern int map_add(struct multimap *map, char* key, void* val, size_t val_len);
extern struct multimap_n *map_get(struct multimap *map, char *key);

#endif  //_MAP_H

