#include <map.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


struct map *map_create(void)
{
    struct map *rmap = NULL;

    rmap = malloc(sizeof(struct map));
    if (!rmap)
        return NULL;

    rmap->root.rb_node = NULL;
    rmap->count = 0;

    return rmap;
}

void *map_get(struct map *map, char *str, size_t *val_len)
{
  struct rb_node *node = map->root.rb_node;
   while (node) {
        struct map_n *pmap = container_of(node, struct map_n, node);

        //compare between the key with the keys in map
        int cmp = strcmp(str, pmap->key);
        if (cmp < 0) {
            node = node->rb_left;
        } else if (cmp > 0) {
            node = node->rb_right;
        } else {
            *val_len = pmap->val_len;
            return (pmap->val);
        }
   }

   *val_len = 0;
   return NULL;
}

int map_remove(struct map *map, char *key)
{
  struct rb_node *node = map->root.rb_node;

   while (node) {
        struct map_n *pmap = container_of(node, struct map_n, node);

        //compare between the key with the keys in map
        int cmp = strcmp(str, pmap->key);
        if (cmp < 0) {
            node = node->rb_left;
        } else if (cmp > 0) {
            node = node->rb_right;
        } else {
            map->count --;
            rb_erase(&pmap->node, &map->root);
            return 1;
        }
   }

   return 0;
}


int map_add(struct map *map, char* key, void* val, unsigned int val_len)
{
    struct map_n *pmap = NULL;
    struct rb_node **new_node = &map->root.rb_node;
    struct rb_node *parent = NULL;
    struct map_n *this_node = NULL;
    int ret = 0;

    pmap = malloc(sizeof(struct map_n));
    if (!pmap) {
        fprintf(stderr, "malloc struct map_n fail.\n");
        return -1;
    }

    pmap->key = NULL;
    pmap->val = NULL;
    pmap->key = malloc(strlen(key) + 1);
    if (!pmap->key) {
        fprintf(stderr, "malloc key space fail\n");
        goto fail;
    }


    strcpy(pmap->key, key);
    pmap->val_len = val_len;
    pmap->val = val;

    while (*new_node) {
        this_node = container_of(*new_node, struct map_n, node);
        ret = strcmp(key, this_node->key);
        parent = *new_node;

        if (ret < 0) {
            new_node = &((*new_node)->rb_left);
        }else if (ret > 0) {
            new_node = &((*new_node)->rb_right);
        }else {
            free(pmap->key);
            free(pmap);
            return 0;
        }
    }

    rb_link_node(&pmap->node, parent, new_node);
    rb_insert_color(&pmap->node, &map->root);
    map->count ++;

    return 1;

fail:
    if (pmap->key)
        free(pmap->key);
    if (pmap)
        free(pmap);

    return -1;
}


struct map_n *map_first(struct map *src_map)
{
    struct rb_node *node = rb_first(&src_map->root);
    return (rb_entry(node, struct map_n, node));
}

struct map_n *map_next(struct map_n *map_node)
{
    struct rb_node *next =  rb_next(&map_node->node);
    return rb_entry(next, struct map_n, node);
}

static void free_map_node(struct map_n *pnode)
{
    struct map_n *tmp_map_node = NULL;

   if (pnode->node.rb_left) {
        tmp_map_node = rb_entry(pnode->node.rb_left,
          struct map_n, node);
        free_map_node(tmp_map_node);
   }

    if (pnode->node.rb_right) {
        tmp_map_node = rb_entry(pnode->node.rb_right,
            struct map_n, node);
        free_map_node(tmp_map_node);
    }

    pnode->node.rb_left = NULL;
    pnode->node.rb_right = NULL;
    tmp_map_node = pnode;
    if (tmp_map_node->key)
        free(tmp_map_node->key);

    free(tmp_map_node);
}

void map_release(struct map *src_map)
{
    struct map_n *tmp_map_node = NULL;

    if (!src_map)
        return;

    if (!src_map->root.rb_node)
        return;

    tmp_map_node = rb_entry(src_map->root.rb_node, struct map_n, node);
    free_map_node(tmp_map_node);
}
