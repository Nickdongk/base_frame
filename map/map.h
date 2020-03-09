/***************************************************************************
 *
 *
 **/
#ifndef _MAP_H
#define _MAP_H

#include <rbtree.h>
#include <stddef.h>

/**
 * map head struct
 */
struct map {
    struct rb_root root;
    size_t count;
};

/**
 * map node struct
 */
struct map_n {
    struct rb_node node;
    char *key;
    void *val;
    size_t val_len;
};

#define map_for_each_node(node, src_map) \
    for(node = map_first(src_map); node != NULL; node = map_next(node))

/**
 *map_create
 *@return The pointer to a map
 *@brief Create a key-value pair map
 * */
extern struct map *map_create(void);

/**
 *map_release
 *@param map The map is to be released
 *@brief Release a key-value pair map created by map_create
 */
extern void map_release(struct map *map);

/**
 *map_first
 *@param map The map that will be checked
 *@return map node The first map node
 *@brief Get the first map node from a map.
 * map node is ordered according its key
 */
extern struct map_n *map_first(struct map *map);

/**
 *map_next
 *@param map_node The node that will be checked
 *@return map node The successed map node
 *@brief Get the node behind the giving node
 */
extern struct map_n *map_next(struct map_n *map_node);

/**
 *map_add
 *@param map The map that will be checked
 *@param key The key input
 *@param val The pointer of value
 *@param val_len The len of value
 *@return 0 The key exist, 1 add success, -1 fail
 *@brief add key-value pair to the map
 */
extern int map_add(struct map *map, char* key, void* val, size_t val_len);

/**
 *map_remove
 *@param map
 *@param key The key of node that will be removed
 *@return 0 The key-value pair is not in the map, 1 remove successfully
 *@brief remove one key-value pair from the map
 */
extern int map_remove(struct map *map, char *key);

/**
 *map_get
 *@param map
 *@param key
 *@param val_len The pointer to a value length, used to return value len
 *@return The pointer to value, the len is in *val_len
 *@brief Get the value accroding to the giving key
 */
extern void *map_get(struct map *map, char *key, size_t *val_len);

/**
 *map_length
 *@param map
 *@return The number of key-value pair in the giving map
 *@brief Get the number of nodes in the map
 */
extern size_t map_length(struct map *map);

#endif  //_MAP_H

