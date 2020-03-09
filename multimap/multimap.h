#ifndef _MULTIMAP_H
#define _MULTIMAP_H

#include <rbtree.h>
#include <list.h>

/**
 * multimap head struct
 */
struct multimap {
    struct rb_root root;
    size_t count;
};

/**
 * multimap node struct
 */
struct multimap_n {
    struct rb_node node;
    char *key;
    size_t num;
    struct list_head entry;
    struct multimap *map_in;
};

/**
 * multimap value set head struct
 */
struct multimap_list_n {
    struct list_head head;
    struct multimap_n *map_n_in;
    void *val;
    size_t len;
};

#define multimap_for_each_keynode(ptr, key_in)  list_for_each_entry(ptr, &key_in->entry, head)

/**
 *multimap_create
 *@return The pointer to a multimap
 *@brief Create a key-multivalue pair map
 * */
extern struct multimap *multimap_create(void);

/**
 *multimap_release
 *@param src_map The multimap is to be released
 *@brief Release a key-multivalue pair multimap created by multimap_create
 */
extern void multimap_release(struct multimap *src_map);

extern struct multimap_n *map_first(struct multimap *map);
extern struct multimap_n *map_next(struct multimap_n *map_node);
extern int multimap_add(struct multimap *map, char* key, void* val, size_t val_len);
extern struct multimap_n *multimap_get(struct multimap *map, char *key);
extern int  multimap_remove(struct multimap *map, char *key, void *val, size_t val_len)

#endif  //_MAP_H

