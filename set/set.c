#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "set.h"

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

    free(tmp_set_node->val);
    tmp_set_node->val = NULL;
    tmp_set_node->len = 0;
    free(tmp_set_node);
}

void set_release(struct set *src_set)
{
    struct set_n *tmp_set_node = NULL;

    if (!src_set)
        return;

    if (!src_set->root.rb_node)
        return;

    tmp_set_node = rb_entry(src_set->root.rb_node, struct set_n, node);
    free_set_node(tmp_set_node);
    free(src_set);

}

struct set *set_create(void)
{
    struct set *rset = NULL;

    rset = malloc(sizeof(struct set));
    if (!rset)
        return NULL;

    rset->root.rb_node = NULL;
    rset->nodes_num = 0;

    return rset;
}

//static void get_val_hash_str(void *val, size_t len, char *key)
//{
 //   int i = 0;
//    uint32_t hash[4] = {0};

 //   md5((uint8_t *)val, len, hash);
  //  for (i = 0; i < 4; i ++) {
//        uint8_t *ptmp = &hash[i];
//        sprintf(&key[i * 8], "%2.2x%2.2x%2.2x%2.2x", ptmp[0],
//                ptmp[1], ptmp[2], ptmp[3]);
//    }
//}

int set_contain(struct set *src_set, void *val, size_t len)
{
    struct rb_node *node = src_set->root.rb_node;
    struct set_n *pset_n = NULL;
    int ret = 0;

    while (node) {
        pset_n = container_of(node, struct set_n, node);
        ret = memcmp(val, pset_n->val, len);
        //compare between the key with the keys in map
        if (ret < 0) {
            node = node->rb_left;
        } else if (ret > 0) {
            node = node->rb_right;
        } else {
            return 1;
        }
    }

    return 0;
}

int set_add(struct set *src_set, void *val, size_t len)
{
    struct set_n *pset_n = NULL;
    struct rb_node **new_node = &src_set->root.rb_node;
    struct rb_node *parent = NULL;
    struct set_n *this_node = NULL;
    struct list_n *this_list = NULL;
    char *val_tmp = NULL;
    int ret = 0;

    if (src_set == NULL) {
        fprintf(stderr, "src set is NULL\n");
        return -1;
    }

    if (val == NULL) {
        fprintf(stderr, "val can not be NULL\n");
        return -1;
    }

    if (len <= 0) {
        fprintf(stderr, "len must not be zero\n");
        return -1;
    }

    pset_n = malloc(sizeof(struct set_n));
    if (!pset_n) {
        fprintf(stderr, "malloc struct set_n fail.\n");
        return -1;
    }

    val_tmp = malloc(len);
    if (!val_tmp) {
        fprintf(stderr, "malloc val space fail.\n");
        return -1;
    }

    memcpy(val_tmp, val, len);

    while (*new_node) {
        this_node = container_of(*new_node, struct set_n, node);
        parent = *new_node;
        ret = memcmp(val_tmp, this_node->val, len);
        if (ret < 0) {
            new_node = &((*new_node)->rb_left);
        } else if (ret > 0) {
            new_node = &((*new_node)->rb_right);
        } else {
            free(val_tmp);
            free(pset_n);
            return 0;
        }
    }

    rb_link_node(&pset_n->node, parent, new_node);
    rb_insert_color(&pset_n->node, &src_set->root);
    pset_n->val = val_tmp;
    pset_n->len = len;
    src_set->nodes_num ++;

    return 1;
}

int set_remove(struct set *src_set, void *val, size_t len)
{
    struct rb_node *pnode = src_set->root.rb_node;
    struct set_n *this_node = NULL;
    int ret = 0;


    while (pnode) {
        this_node = container_of(pnode, struct set_n, node);
        ret = memcmp(val, this_node->val, len);
        if (ret < 0)
            pnode = pnode->rb_left;
        else if (ret > 0)
            pnode = pnode->rb_right;
        else {
                src_set->nodes_num --;
                rb_erase(&this_node->node, &src_set->root);
                free(this_node->val);
                free(this_node);
                return 1;
        }
    }

    return 0;
}

size_t set_length(struct set *src_set)
{
    return src_set->nodes_num;
}

struct set_n *set_first(struct set *tree)
{
    struct rb_node *node = rb_first(&tree->root);
    return (rb_entry(node, struct set_n, node));
}

struct set_n *set_next(struct set_n *set_node)
{
    struct rb_node *next =  rb_next(&set_node->node);
    return rb_entry(next, struct set_n, node);
}

int set_is_subset(struct set *src_set, struct set *dst_set)
{
   struct set_n *tmp_set_node = NULL;
   int ret = 0;

   if (src_set->nodes_num > dst_set->nodes_num)
        return 0;

   tmp_set_node = set_first(src_set);
   if (!tmp_set_node) {
       fprintf(stderr, "src set is empty\n");
       return -1;
   }

    while (tmp_set_node) {
        ret = set_contain(dst_set, tmp_set_node->val,
        tmp_set_node->len);
        if (!ret)
            return 0;
        tmp_set_node = set_next(tmp_set_node);
    }

    if (src_set->nodes_num < dst_set->nodes_num)
        return 1;
    if (src_set->nodes_num == dst_set->nodes_num)
        return 2;
}

struct set *set_union(struct set *src_set, struct set *dst_set)
{
    struct set *r_set = NULL;
    struct set_n *tmp_set_node = NULL;
    struct set_n *dst_set_node = NULL;
    void *r_val = NULL;
    int ret = 0;

    ret = set_is_subset(src_set, dst_set);
    if (ret > 0)
        return dst_set;

    ret = set_is_subset(dst_set, src_set);
    if (ret > 0)
        return src_set;

    r_set = set_create();
    if (!r_set) {
        fprintf(stderr, "fail to create set\n");
        return NULL;
    }

    tmp_set_node = set_first(src_set);
    if (!tmp_set_node) {
        fprintf(stderr, "no node in set\n");
        goto fail;
    }

    while (tmp_set_node) {
        set_add(r_set, tmp_set_node->val, tmp_set_node->len);
        tmp_set_node = set_next(tmp_set_node);
    }

    tmp_set_node = set_first(dst_set);
    if (!tmp_set_node) {
        fprintf(stderr, "no node in dst set\n");
        goto fail;
    }

    while (tmp_set_node) {
        ret = set_contain(r_set, tmp_set_node->val, tmp_set_node->len);
        if (!ret)
            set_add(r_set, tmp_set_node->val, tmp_set_node->len);
        tmp_set_node = set_next(tmp_set_node);
    }

    if (r_set->nodes_num)
        return r_set;

fail:
    if (r_set)
        set_release(r_set);

    return NULL;
}

struct set *set_intersection(struct set *src_set, struct set *dst_set)
{
    struct set *r_set = NULL;
    struct set_n *tmp_set_node = NULL;
    void *r_val = NULL;
    int ret = 0;

    ret = set_is_subset(src_set, dst_set);
    if (ret > 0)
        return src_set;
    ret = set_is_subset(dst_set, src_set);
    if (ret > 0)
        return dst_set;

    r_set = set_create();
    if (!r_set) {
        fprintf(stderr, "fail to create set\n");
        return NULL;
    }
    tmp_set_node = set_first(src_set);
    if (!tmp_set_node) {
        fprintf(stderr, "no node in set\n");
        goto fail;
    }

    while (tmp_set_node) {
       ret = set_contain(dst_set, tmp_set_node->val, tmp_set_node->len);
       if (ret)
            set_add(r_set, tmp_set_node->val, tmp_set_node->len);
       tmp_set_node = set_next(tmp_set_node);
    }

    if (r_set->nodes_num)
        return r_set;

fail:
    if (r_set)
        set_release(r_set);
    return NULL;
}

struct set *set_difference(struct set *src_set, struct set *dst_set)
{
    struct set *r_set = NULL;
    struct set_n *tmp_set_node = NULL;
    struct set_n *dst_set_node = NULL;
    void *r_val = NULL;
    int ret = 0;

    r_set = set_create();
    if (!r_set) {
        fprintf(stderr, "fail to create set\n");
        return NULL;
    }

    tmp_set_node = set_first(src_set);
    if (!tmp_set_node) {
        fprintf(stderr, "no node in set\n");
        goto fail;
    }

    while (tmp_set_node) {
        ret = set_contain(dst_set, tmp_set_node->val, tmp_set_node->len);
        if (!ret)
            set_add(r_set, tmp_set_node->val, tmp_set_node->len);
        tmp_set_node = set_next(tmp_set_node);
    }

    if (r_set->nodes_num)
        return r_set;


fail:
    set_release(r_set);
    return NULL;
}


int set_cmp(struct set *src_set, struct set *dst_set)
{
    int ret = 0;

    ret = set_is_subset(src_set, dst_set);
    if (ret == 2)
        return 1;

    return 0;
}

int set_add_set(struct set *src_set, struct set *dst_set)
{
    struct set_n *tmp_set_node = NULL;
    int ret = 0;

    if (!src_set->nodes_num)
        return -1;

    tmp_set_node = set_first(src_set);
    if (!tmp_set_node) {
        fprintf(stderr, "no node in set\n");
        return -1;
    }

    while (tmp_set_node) {
       ret = set_contain(dst_set, tmp_set_node->val, tmp_set_node->len);
       if (!ret)
            set_add(dst_set, tmp_set_node->val, tmp_set_node->len);
       tmp_set_node = set_next(tmp_set_node);
    }

}
