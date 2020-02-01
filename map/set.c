#include "set.h" 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>


struct set *create_set(void)
{
    struct set *rset = NULL;

    rset = malloc(sizeof(struct set));
    if (!rset)
        return NULL;

    rset->root.rb_node = NULL;
    rset->count = 0;

    return rset;
}

void *get_from_set(struct set *set_in, void *addr) 
{
    struct rb_node *node = set_in->root.rb_node; 
    uint64_t addr_in = (uint64_t)addr;

    while (node) {
        struct set_n *pset_n = container_of(node, struct set_n, node);

        //compare between the key with the keys in map
        uint64_t val_n = (uint64_t)pset_n->val;
        if (addr_in < val_n) {
            node = node->rb_left;
        } else if (addr_in > val_n) {
            node = node->rb_right;
        } else {
            return (pset_n->val); 
        } 
    }

   return NULL;
}

int put_to_set(struct set *set_in, void* val) 
{
    struct set_n *pset_n = NULL;
    struct rb_node **new_node = &set_in->root.rb_node; 
    struct rb_node *parent = NULL;
    struct set_n *this_node = NULL;
    uint64_t addr_in = (uint64_t)val;
    int ret = 0;

    pset_n = malloc(sizeof(struct set_n));
    if (!pset_n) {
        fprintf(stderr, "malloc struct set_n fail.\n");
        return -1;
    }

    pset_n->val = val;  
    
    while (*new_node) {
        this_node = container_of(*new_node, struct set_n, node);
        parent = *new_node;

        if (addr_in < (uint64_t)this_node->val) {
            new_node = &((*new_node)->rb_left);
        }else if (addr_in > (uint64_t)this_node->val) {
            new_node = &((*new_node)->rb_right);
        }else {
            return 0;
        }
    }

    rb_link_node(&pset_n->node, parent, new_node);
    rb_insert_color(&pset_n->node, &set_in->root);
    set_in->count ++;

    return 1;
}


struct set_n *set_first_node(struct set *tree) 
{
    struct rb_node *node = rb_first(&tree->root);
    return (rb_entry(node, struct set_n, node));
}

struct set_n *set_next_node(struct set_n *set_node) 
{
    struct rb_node *next =  rb_next(&set_node->node);
    return rb_entry(next, struct set_n, node);
}

static void free_set_node(struct set_n *pnode) 
{
    struct set_n *tmp_set_node = NULL;

   if (pnode->node.rb_left) {
        tmp_set_node = rb_entry(pnode->node.rb_left,
            struct set_n, node);       
        free_set_node(tmp_set_node);
   }

    if (pnode->node.rb_right) {
        tmp_set_node = rb_entry(pnode->node.rb_right,
            struct set_n, node);
        free_set_node(tmp_set_node);
    }

    pnode->node.rb_left = NULL;
    pnode->node.rb_right = NULL;
    tmp_set_node = pnode;
    tmp_set_node->val = NULL;
    
    free(tmp_set_node);
}

void free_set(struct set *tree)
{
    struct set_n *tmp_set_node = NULL;

    if (!tree)
        return;

    if (!tree->root.rb_node)
        return;

    tmp_set_node = rb_entry(tree->root.rb_node, struct set_n, node);
    free_set_node(tmp_set_node);
}
