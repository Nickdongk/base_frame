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

/**
 *multimap_first
 *@param map The map that will be checked
 *@return map node The first multimap node
 *@brief Get the first multimap node from a multimap.
 * multimap node is ordered according its key
 */

extern struct multimap_n *multimap_first(struct multimap *map);

/**
 *multimap_next
 *@param map The map that will be checked
 *@return map node The first multimap node
 *@brief Get the first multimap node from a multimap.
 * multimap node is ordered according its key
 */
extern struct multimap_n *multimap_next(struct multimap_n *map_node);

/**
 *multimap_add
 *@param map The map that will be checked
 *@param key The key input
 *@param val The pointer of value
 *@param val_len The len of value
 *@return 0 The key exist, 1 add success, -1 fail
 *@brief add key-value pair to the map
 */
extern int multimap_add(struct multimap *map, char* key, void* val, size_t val_len);

/**
 *multimap_get
 *@param map
 *@param key
 *@return The pointer to multimap node
 *@brief Get the value set accroding to the giving key
 */
extern struct multimap_n *multimap_get(struct multimap *map, char *key);

/**
 *multimap_remove
 *@param map
 *@param key The key of node that will be removed
 *@param val The value that will be removed, it could be NULL
 *@param val_len The value length
 *@return 0 The key-value pair is not in the map, 1 remove successfully
 *@brief remove one key-value pair from the map, if one key one value
 *the multimap node will be removed, otherwise only the value node will
 *be removed.
 */

extern int  multimap_remove(struct multimap *map, char *key, void *val, size_t val_len)

#endif  //_MAP_H

