#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stack.h>
#include <general_tree.h>


struct general_tree *tree_create(void)
{
    struct general_tree *p_tree = NULL;

    p_tree = malloc(sizeof(struct general_tree));
    if (!p_tree) {
        fprintf(stderr, "fail to create general tree.\n");
        return NULL;
    }

    p_tree->root = NULL;
    p_tree->count = 0;
    return p_tree;
}

void tree_init(struct general_tree *tree)
{

    tree->root = NULL;
    tree->count = 0;
}

int tree_add_node(struct general_tree *tree, struct tree_node *parent, struct tree_node *src_node)
{
    int i = 0;
    unsigned int tmp = 0;

    if (!parent)  {
        if (!tree->root) {
            tree->root = src_node;
            tree->count = 1;
            src_node->tree = tree;
            return 0;
        }

        goto fail;
    }

    src_node->tree = tree;
    if (parent->nodes_num == parent->chld_num) {
        parent->nodes = realloc(parent->nodes, (parent->nodes_num + DEFAULT_TREE_NODE_SIZE)
                * sizeof(struct tree_node *));
        if (!parent->nodes) {
            fprintf(stderr, " fail to realloc struct tree_node *\n");
            return -1;
        }

        tmp = parent->nodes_num;
        parent->nodes[tmp] = src_node;
        parent->nodes_num += DEFAULT_TREE_NODE_SIZE;
        parent->chld_num ++;
        src_node->index = tmp;
        src_node->parent = parent;
        tree->count ++;
        return 0;
    }

    if (parent->nodes_num > parent->chld_num) {
        for (i = 0; i < parent->nodes_num; i ++) {
            if (!parent->nodes[i]) {
                parent->nodes[i] = src_node;
                src_node->parent = parent;
                src_node->index = i;
                break;
            }
        }

        parent->chld_num ++;
        tree->count ++;
        return 0;
    }

fail:
    fprintf(stderr, "add tree node fail.\n");
    return -1;
}

size_t tree_size(struct general_tree *tree)
{
    return tree->count;
}

size_t tree_chld_tree_size(struct tree_node *pnode)
{
    size_t size = 0;
    int i = 0;

    if (!pnode)
        return 0;

    for (i = 0; i < pnode->nodes_num; i ++) {
        if (pnode->nodes[i])
            size += tree_chld_tree_size(pnode->nodes[i]);
    }

    size  ++;
    return size;
}

struct tree_node *tree_node_alloc_nc(char *name, unsigned int key, unsigned int num)
{
	struct tree_node *tmp_node = NULL;
    struct tree_node **p_nodes = NULL;
	int i = 0;

	tmp_node = malloc(sizeof(struct tree_node));
	if (!tmp_node) {
		fprintf(stderr, "fail to malloc tree node\n");
        return NULL;
	}

    p_nodes = malloc(num * sizeof(struct tree_node *));
    if (!p_nodes) {
        fprintf(stderr, "fail to struct tree_node *.\n");
        goto fail;
    }

    tmp_node->nodes = p_nodes;
	tmp_node->name = name;
	tmp_node->key = key;
    tmp_node->index = 0;
	tmp_node->chld_num = 0;
	tmp_node->nodes_num = num;
    tmp_node->parent = NULL;
    tmp_node->tree = NULL;
	for (i = 0; i < num; i ++) {
		tmp_node->nodes[i] = NULL;
	}

	return tmp_node;

fail:
    if (tmp_node)
        free(tmp_node);
    if (p_nodes)
        free(p_nodes);

    return NULL;

}

struct tree_node *tree_node_alloc_binary(char *name, unsigned int key)
{
	return tree_node_alloc_nc(name, key, 2);
}


void release_tree(struct tree_node *root)
{
	int i = 0;

    if (!root)
        return;

	for (i = 0; i < root->nodes_num; i ++) {
		if (root->nodes[i])
			release_tree(root->nodes[i]);
	}

	free(root);
    free(root->nodes);
}

void tree_remove_chld_tree(struct tree_node *pnode)
{
    size_t chld_size = 0;
    int i = 0;

    if (!pnode)
        return;

    chld_size = tree_chld_tree_size(pnode) - 1;
    pnode->tree->count -=  chld_size;

    for (i = 0; i < pnode->nodes_num; i ++) {
        if (pnode->nodes[i]){
            release_tree(pnode->nodes[i]);
            pnode->nodes[i] = NULL;
        }
    }

    pnode->chld_num = 0;

}

static int traverse_tree(struct tree_node * root, struct stack *p_stack)
{
	struct tree_node *cur_node = NULL;
	struct tree_node *next_node = NULL;
	int level = 0, ret = 0;
	int flag = 0;
	struct stack_n *tmp = NULL;
	int i = 0, j = 0;
    unsigned int slash_flag = 0;

	cur_node = root;
	if (cur_node) {

		list_for_each_entry(tmp, &p_stack->entry, head) {
			if (*(unsigned int *)tmp->ptr) {
				printf("    |");
				flag = 1;
			}
			else {
				printf("     ");
				flag = 0;
			}
		}

		if (flag)
			printf("\n");
		else
			printf("\b|\n");

		list_for_each_entry(tmp, &p_stack->entry, head) {
			if (*(unsigned int *)tmp->ptr) {
				printf("    |");
				flag = 1;
			}
			else {
				printf("     ");
				flag = 0;
			}
		}

		if (!cur_node->chld_num) {
			if (flag)
				printf("----%c\n", cur_node->key);
			else
				printf("\b|----%c\n", cur_node->key);
		} else {

			if (flag)
				printf("----%c\n", cur_node->key);
			else
				printf("\b|----%c\n", cur_node->key);
        }

        if (cur_node->chld_num) {

    		for (i = 0; i < cur_node->nodes_num; i ++) {

                /*there are some chld node after this node*/
    			for (j = i + 1; j < cur_node->nodes_num; j ++) {
    				    slash_flag = slash_flag || cur_node->nodes[j];
    			}
    			if (cur_node->nodes[i]) {
    				stack_push(p_stack, (void *)&slash_flag);
    				traverse_tree(cur_node->nodes[i], p_stack);
    				stack_pop(p_stack);
    			}
        	}
        }
    }

	return 0;
}

static void release_node(struct stack_n *p_stn)
{
    return;
}

int tree_traverse_key(struct general_tree *tree)
{
	struct stack *p_stack = NULL;
    struct tree_node *pnode = tree->root;

    if (!tree->root)
        return -1;

    p_stack = stack_create();
	traverse_tree(pnode, p_stack);
    stack_release(p_stack, release_node);


	return 0;
}

static struct tree_node *last_node(struct tree_node *pnode)
{
    int i = 0, j= 0;

    while (i < pnode->nodes_num && j < pnode->chld_num) {
        if (pnode->nodes[i])
            j ++;

        i++;
    }

    if (i)
        return pnode->nodes[i - 1];
    else
        return NULL;
}

struct tree_node *tree_last_node(struct general_tree *tree)
{
    struct tree_node *pnode = tree->root;
    struct tree_node *tmpnode = NULL;

    if (!pnode)
        return NULL;

    while(pnode) {
        tmpnode = pnode;
        pnode =  last_node(pnode);
    }

    return tmpnode;
}
