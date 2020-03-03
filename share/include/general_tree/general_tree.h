#ifndef _GENERAL_TREE_H_
#define _GENERAL_TREE_H_

#include <stdlib.h>
#include <stddef.h>

#define DEFAULT_TREE_NODE_SIZE 4

struct general_tree {
    struct tree_node *root;
    size_t count;
};


struct tree_node {
	char *key;
    unsigned int index;
	unsigned int chld_num;
	unsigned int nodes_num;
	struct tree_node **nodes;
    struct tree_node *parent;
    struct general_tree *tree;
};

extern struct general_tree * tree_create(void);

extern void tree_init(struct general_tree *tree);

extern int tree_add_node(struct general_tree *tree, struct tree_node *parent, struct tree_node *src_node);

extern struct tree_node *tree_node_alloc_nc(char *key, unsigned int num);

extern struct tree_node *tree_node_alloc_binary(char *key);

extern size_t tree_size(struct general_tree *tree);

extern size_t tree_chld_tree_size(struct tree_node *pnode);

extern void tree_remove_chld_tree(struct tree_node *pnode);

extern void release_tree(struct tree_node *pnode);

extern int tree_traverse_key(struct general_tree *root);

extern struct tree_node *tree_last_node(struct general_tree *tree);

#endif
