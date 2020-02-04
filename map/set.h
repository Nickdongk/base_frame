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

#include "rbtree.h"

struct set {
    struct rb_root root;
    unsigned int nodes_num;
};

struct set_n {
    struct rb_node node;
    void *val;
};

extern struct set *create_set(void);
extern void release_set(struct set *tree);

extern struct set_n *set_first_node(struct set *tree); 
extern struct set_n *set_next_node(struct set_n *set_node); 

extern struct set *uni_set(struct set *src_set, struct set *dst_set);
extern struct set *intersection_set(struct set *src_set, struct set *dst_set);

extern int is_child_set(struct set *src_set, struct set *dst_set);  

extern int put_to_set(struct set *set_in, void* val); 
extern void *get_from_set(struct set *set_in, void *addr); 

#endif  //_MAP_H
