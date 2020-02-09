#include <map.h>
#include <set.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
    char *content = "helloworld";
    char *content1 = "helloworld1"; 
    char *content2 = "helloworld2";
    char *r_str = NULL;
    int var = 100;
    int var1 = 50;
    int var2 = 25;
    int *r_var = NULL;
    int ret = 0;
    struct set *string_set = NULL;  
    struct set *num_set = NULL;
    struct set *merge_set = NULL;


    string_set = set_create();
    if (!string_set) {
        fprintf(stderr, "fail to create set\n");
        return -1;
    }

    num_set = set_create();
    if (!num_set) {
        fprintf(stderr, "fail to create set\n");
        goto fail; 
    }

    set_add(string_set, content, strlen(content));
    set_add(string_set, content1, strlen(content1));
    set_add(string_set, content2, strlen(content2));

    set_add(num_set, &var, sizeof(var));
    set_add(num_set, &var1, sizeof(var1));
    set_add(num_set, &var2, sizeof(var2));

    merge_set = set_union(string_set, num_set);

    ret = set_contain(merge_set, content, strlen(content));
    if (ret)
        printf("%s is in union\n", content);

    ret = set_contain(string_set, content, strlen(content));
    if (ret) 
        printf("%s is in string_set\n", content);

    ret = set_contain(num_set, &var, sizeof(var));
    if (ret)
        printf("%d is in num_set\n", var);

    set_remove(num_set, &var, sizeof(var));
    ret = set_contain(num_set, &var, sizeof(var));
    if (!ret)
        printf("%d is not in num_set\n", var);



fail:
    if (string_set) 
        set_release(string_set);
    if (num_set)
        set_release(num_set);
    if (merge_set)
        set_release(merge_set);
    return 0;
}