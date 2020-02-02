#include <map.h>
#include <set.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
    char key[16] = {0};
    char word[16] = {0};
    struct map *g_map = NULL;
    struct map_n *n_map = NULL;
    char *r_val = NULL;
    int ret = 0;
    int i = 0;

    printf("test map\n"); 
    g_map = create_map();
    if (!g_map) {
        fprintf(stderr, "create map fail\n");
        return -1; 
    }

    for (i = 0; i < 20; i ++) {
        sprintf(key, "hello%d", i);  
        sprintf(word, "world%d", i);
        ret = put_to_map(g_map, key, word, strlen(word) + 1);
        if (ret < 0) {
            fprintf(stderr, "put to map fail\n");
            goto fail;
        }
    }
    
    r_val = get_from_map(g_map, "hello1"); 
    if (!r_val) {
        fprintf(stderr, "not found key hello1\n");
        goto not_found;
    }

    printf("get str: %s\n", r_val);
    r_val = NULL;
    r_val = get_from_map(g_map, "hello10");
    if (!r_val) {
        fprintf(stderr, "not found key hello10\n");
        goto not_found;
    }
    printf("get str: %s\n", r_val);

not_found:
    n_map = map_first_node(g_map);
    while(n_map) {
       printf("key is %s, val is %s\n", n_map->key, (char *)n_map->val);
       n_map = map_next_node(n_map);   
    } 

fail:
    if (g_map) 
        free_map(g_map);
      
    return 0;
}