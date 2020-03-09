 /* *
 * @file hash.h
 * @author zk(zkdnfc@163.com)
 * @date 2016/05/31 18:26:01
 * @version $Revision$
 * @brief
 *
 * */
#ifndef _SET_H
#define _SET_H

#include <rbtree.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>

struct set {
    struct rb_root root;
    unsigned int nodes_num;
};

struct set_n {
    struct rb_node node;
    void *val;
    size_t len;
};

#define set_for_each_node(node, src_set) \
    for(node = set_first(src_set); node != NULL; node = set_next(node))

extern struct set *set_create(void);
extern void set_release(struct set *src_set);

extern struct set_n *set_first(struct set *tree);
extern struct set_n *set_next(struct set_n *set_node);

extern struct set *set_union(struct set *src_set, struct set *dst_set);
extern struct set *set_intersection(struct set *src_set, struct set *dst_set);

extern int set_is_subset(struct set *src_set, struct set *dst_set);

extern int set_add(struct set *src_set, void *val, size_t len);
extern int set_contain(struct set *src_set, void *val, size_t len);
extern int set_remove(struct set *src_set,void *val, size_t len);
extern size_t set_length(struct set *src_set);
extern struct set *set_difference(struct set *src_set, struct set *dst_set);
extern int set_cmp(struct set *src_set, struct set *dst_set);
#endif  //_MAP_H
