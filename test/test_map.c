#include <map.h>
#include <set.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void release_mapnode(struct map_n *node)
{
    if (node->val)
        free(node->val);
}

int main(int argc, char *argv[])
{
    char key[16] = {0};
    char *word = NULL;
    struct map *g_map = NULL;
    struct map_n *n_map = NULL;
    char *r_val = NULL;
    size_t val_len;
    int ret = 0;
    int i = 0;

    printf("test map\n");
    g_map = map_create();
    if (!g_map) {
        fprintf(stderr, "create map fail\n");
        return -1;
    }

    for (i = 0; i < 20; i ++) {
        sprintf(key, "hello%d", i);
        word = malloc(32);
        if (!word)
            goto fail;
        sprintf(word, "word%d", i);
        ret = map_add(g_map, key, word, strlen(word) + 1);
        if (ret < 0) {
            fprintf(stderr, "put to map fail\n");
            goto fail;
        }
    }

    r_val = map_get(g_map, "hello1", &val_len);
    if (!r_val) {
        fprintf(stderr, "not found key hello1\n");
        goto not_found;
    }

    printf("get str: %s\n", r_val);
    r_val = NULL;
    r_val = map_get(g_map, "hello10", &val_len);
    if (!r_val) {
        fprintf(stderr, "not found key hello10\n");
        goto not_found;
    }
    printf("get str: %s\n", r_val);

not_found:
    n_map = map_first(g_map);
    while(n_map) {
       printf("key is %s, val is %s\n", n_map->key, (char *)n_map->val);
       n_map = map_next(n_map);
    }

fail:
    n_map = map_first(g_map);
    while(n_map) {
        if (n_map->val)
            free(n_map->val);

       n_map = map_next(n_map);
    }

    if (g_map)
        map_release(g_map, release_mapnode);

    return 0;
}
