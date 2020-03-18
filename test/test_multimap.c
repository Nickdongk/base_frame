#include <multimap.h>
#include <set.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char key[16] = {0};
    char word[16] = {0};
    struct multimap *g_multimap = NULL;
    struct multimap_n *n_map = NULL;
    struct multimap_list_n *list_node = NULL;
    char *r_val = NULL;
    size_t val_len;
    int ret = 0;
    int i = 0;

    printf("test multimap\n");
    g_multimap = multimap_create();
    if (!g_multimap) {
        fprintf(stderr, "create map fail\n");
        return -1;
    }

    sprintf(key, "hello0");
    sprintf(word, "word0");
    ret = multimap_add(g_multimap, key, word, strlen(word) + 1);
    if (ret < 0) {
        fprintf(stderr, "put to map fail\n");
        goto fail;
    }

    sprintf(word, "word1");
    ret = multimap_add(g_multimap, key, word, strlen(word) + 1);
    if (ret < 0) {
        fprintf(stderr, "put to map fail\n");
        goto fail;
    }



    n_map = multimap_get(g_multimap, "hello0");
    if (!n_map) {
        fprintf(stderr, "not found key hello0\n");
        goto fail;
    }

    multimap_for_each_keynode(list_node, n_map) {
        printf("list val is %s\n", (char *)list_node->val);
    }

fail:
    if (g_multimap)
        multimap_release(g_multimap);

    return 0;
}
