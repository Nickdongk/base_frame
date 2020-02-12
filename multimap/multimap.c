#include <multimap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


struct multimap *multimap_create(void)
{
    struct multimap *rmap = NULL;

    rmap = malloc(sizeof(struct multimap));
    if (!rmap)
        return NULL;

    rmap->root.rb_node = NULL;
    rmap->count = 0;

    return rmap;
}

struct multimap_n *multimap_get(struct multimap *map, char *key)
{
  struct rb_node *node = map->root.rb_node;

   while (node) {
        struct multimap_n *pmap = container_of(node, struct multimap_n, node);

        //compare between the key with the keys in map
        int cmp = strcmp(key, pmap->key);
        if (cmp < 0) {
            node = node->rb_left;
        } else if (cmp > 0) {
            node = node->rb_right;
        } else {
            return pmap;
        }
   }

   return NULL;
}

int  multimap_remove(struct multimap *map, char *key, void *val, size_t val_len)
{
  struct rb_node *node = map->root.rb_node;
  struct multimap_list_n *plist = NULL;
  int ret = 0;

   while (node) {
        struct multimap_n *pmap = container_of(node, struct multimap_n, node);

        //compare between the key with the keys in map
        int cmp = strcmp(key, pmap->key);
        if (cmp < 0) {
            node = node->rb_left;
        } else if (cmp > 0) {
            node = node->rb_right;
        } else {
            list_for_each_entry(plist, &pmap->entry, head) {
                    ret = memcmp(plist->val, val, val_len);
                    if (!ret)
                        break;
            }
            if (plist && &plist->head != &pmap->entry) {
                list_del(&plist->head);
                free(plist);
                pmap->num --;
                if (!pmap->num) {
                    rb_erase(&pmap->node, &map->root);
                    free(pmap);
                    map->count --;
                }
                return  1;
            }

            if (plist)
                return 0;
        }
   }

   return 0;
}


int multimap_add(struct multimap *map, char* key, void* val, size_t val_len)
{
    struct multimap_n *pmap = NULL;
    struct rb_node **new_node = &map->root.rb_node;
    struct rb_node *parent = NULL;
    struct multimap_n *this_node = NULL;
    struct multimap_list_n *list_node = NULL;
    int ret = 0;

    pmap = malloc(sizeof(struct multimap_n));
    if (!pmap) {
        fprintf(stderr, "malloc struct multimap_n fail.\n");
        return -1;
    }

    pmap->key = NULL;
    pmap->key = malloc(strlen(key) + 1);
    if (!pmap->key) {
        fprintf(stderr, "malloc key space fail\n");
        goto fail;
    }

    INIT_LIST_HEAD(&pmap->entry);
    strcpy(pmap->key, key);
    pmap->num = 0;

    list_node = malloc(sizeof(struct multimap_list_n));
    if (!list_node) {
        fprintf(stderr, "malloc struct multimap_list_n fail\n");
        goto fail;
    }

    INIT_LIST_HEAD(&list_node->head);
    while (*new_node) {
        this_node = container_of(*new_node, struct multimap_n, node);
        ret = strcmp(key, this_node->key);
        parent = *new_node;

        if (ret < 0) {
            new_node = &((*new_node)->rb_left);
        }else if (ret > 0) {
            new_node = &((*new_node)->rb_right);
        }else {
            free(pmap->key);
            free(pmap);

            list_node->val = val;
            list_node->len = val_len;
            list_add(&list_node->head, &this_node->entry);
            this_node->num ++;
            return 1;
        }
    }

    rb_link_node(&pmap->node, parent, new_node);
    rb_insert_color(&pmap->node, &map->root);
    list_node->val = val;
    list_node->len = val_len;
    list_add(&list_node->head, &pmap->entry);
    pmap->num ++;
    map->count ++;

    return 1;

fail:
    if (pmap && pmap->key)
        free(pmap->key);
    if (pmap)
        free(pmap);
    if (list_node)
        free(list_node);

    return -1;
}


struct multimap_n *multimap_first(struct multimap *src_map)
{
    struct rb_node *node = rb_first(&src_map->root);
    return (rb_entry(node, struct multimap_n, node));
}

struct multimap_n *multimap_next(struct multimap_n *map_node)
{
    struct rb_node *next =  rb_next(&map_node->node);
    return rb_entry(next, struct multimap_n, node);
}

static void free_multimap_tree(struct multimap_n *pnode, release_multimap_node release_callback)
{
    struct multimap_n *tmp_map_node = NULL;
    struct multimap_list_n *list_node = NULL, *tmp_list_node = NULL;

   if (pnode->node.rb_left) {
        tmp_map_node = rb_entry(pnode->node.rb_left,
          struct multimap_n, node);
        free_multimap_tree(tmp_map_node, release_callback);
   }

    if (pnode->node.rb_right) {
        tmp_map_node = rb_entry(pnode->node.rb_right,
            struct multimap_n, node);
        free_multimap_tree(tmp_map_node, release_callback);
    }

    pnode->node.rb_left = NULL;
    pnode->node.rb_right = NULL;
    tmp_map_node = pnode;
    if (tmp_map_node->key)
        free(tmp_map_node->key);

    list_for_each_entry_safe(list_node, tmp_list_node, &tmp_map_node->entry,
            head) {
        release_callback(list_node);
        free(list_node);
    }
    free(tmp_map_node);
}

void multimap_release(struct multimap *src_map, release_multimap_node release_callback)
{
    struct multimap_n *tmp_map_node = NULL;

    if (!src_map)
        return;

    if (!src_map->root.rb_node)
        return;

    tmp_map_node = rb_entry(src_map->root.rb_node, struct multimap_n, node);
    free_multimap_tree(tmp_map_node, release_callback);
}
