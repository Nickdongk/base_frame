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
    struct set *string_set = NULL;  
    struct set *num_set = NULL;
    struct set *merge_set = NULL;


    string_set = create_set();
    if (!string_set) {
        fprintf(stderr, "fail to create set\n");
        return -1;
    }

    num_set = create_set();
    if (!num_set) {
        fprintf(stderr, "fail to create set\n");
        goto fail; 
    }

    put_to_set(string_set, content);
    put_to_set(string_set, content1);
    put_to_set(string_set, content2);

    put_to_set(num_set, &var);
    put_to_set(num_set, &var1);
    put_to_set(num_set, &var2);

    merge_set = uni_set(string_set, num_set);

    r_str = get_from_set(merge_set, content);
    if (r_str)
        printf("%s is in union\n", content);

    r_str = get_from_set(string_set, content);
    if (r_str) 
        printf("%s is in string_set\n", content);

    r_var = get_from_set(num_set, &var);
    if (r_var)
        printf("%d is in num_set\n", *r_var);

fail:
    if (string_set) 
        release_set(string_set);
    if (num_set)
        release_set(num_set);
    if (merge_set)
        release_set(merge_set);
    return 0;
}