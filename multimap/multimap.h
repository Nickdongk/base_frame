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
    struct multimap *map_in;
};

struct multimap_list_n {
    struct list_head head;
    struct multimap_n *map_n_in;
    void *val;
    size_t len;
};

typedef void (*release_multimap_node)(struct multimap_list_n *list_n);

extern struct multimap *multimap_create(void);
extern void multimap_release(struct multimap *src_map, release_multimap_node release_callback);

extern struct multimap_n *map_first(struct multimap *map);
extern struct multimap_n *map_next(struct multimap_n *map_node);
extern int multimap_add(struct multimap *map, char* key, void* val, size_t val_len);
extern struct multimap_n *multimap_get(struct multimap *map, char *key);

#endif  //_MAP_H

