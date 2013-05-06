#include <stdlib.h>
#include <string.h>
#include "optlib.h"
#include "service.h"
#define MAX_STR_LENGTH 100
#define MAX_KEY_LENGTH 11

typedef struct ring_ {
    struct ring_ *next;
    char str[MAX_STR_LENGTH];
} ring_t;

typedef struct tree_ {
    char key[MAX_KEY_LENGTH];
    ring_t *ring;
    struct tree_ *left;
    struct tree_ *right;
} tree_t;

void free_tree_element(ring_t *);
int input_tree_data(tree_t *, ring_t **);
void frequency_of_word_usage(ring_t *, FILE *);
int find_similar_words_count_in_str(char *, char *);
int getword(char *, char **, char *);
void delword(char *, int);
int create_leaf(tree_t **, ring_t *);
int check_file(char *, FILE *);

int main(int argc, char **argv)
{
    FILE *list;
    tree_t *root = NULL;
    ring_t *ring;
    if (!options(argc, argv))
        return 0;
    list = fopen("list.txt", "w+");
    create_leaf(&root, NULL);
    printf("Enter key for root:\n");
    fgets_c(root->key, MAX_KEY_LENGTH, stdin);
    ring = root->ring;
    while (!input_tree_data(root, &ring));
    ring->next = root->ring;
    frequency_of_word_usage(root->ring, list);
    return (0);                 /* I like it */
}

int input_tree_data(tree_t * root, ring_t ** ring)
{
    tree_t *branch = root;
    ring_t *rng;
    char key[MAX_KEY_LENGTH];
    printf("Enter key for leaf:\n");
    if (fgets_c(key, MAX_KEY_LENGTH, stdin) == '\n')
        return 1;
    while (1) {
        if (strcmp(key, branch->key) < 0)
            if (branch->left)
                branch = branch->left;
            else {
                create_leaf(&(branch->left), *ring);
                break;
        } else if (branch->right)
            branch = branch->right;
        else {
            create_leaf(&(branch->right), *ring);
            break;
        }
    }
    rng = *ring;
    *ring = rng->next;
    return 0;
}

int create_leaf(tree_t ** leaf, ring_t * prev_ring_elem)
{
    *leaf = (tree_t *) calloc(1, sizeof(tree_t));
    (*leaf)->ring = (ring_t *) calloc(1, sizeof(ring_t));
    printf("Enter string:\n");
    fgets_c((*leaf)->ring->str, MAX_STR_LENGTH, stdin);
    if (prev_ring_elem)
        prev_ring_elem->next = (*leaf)->ring;
    (*leaf)->left = NULL;
    (*leaf)->right = NULL;
    return 0;
}

int getword(char *word, char **word_end, char *str)
{
    int i, j = 0;
    for (i = 0; i < strlen(str) + 1; i++) {
        if ((str[i] == ' ' || str[i] == '\0')
            && (str[i - 1] != ' ' && i - 1 >= 0))
            break;
        if (str[i] != ' ' && str[i] != '\0') {
            word[j] = str[i];
            j++;
        }
    }
    *word_end = &str[i];
    word[j] = '\0';
    return j;
}

int find_similar_words_count_in_str(char *compared_word, char *str)
{
    int n = 0;
    char *end[1];
    char word[MAX_STR_LENGTH];
    while (getword(word, end, str)) {
        if (strcmp(word, compared_word) == 0)
            n++;
        str = *end;
    }
    return n;
}

void frequency_of_word_usage(ring_t * ring, FILE * list)
{
    int i = 0, n = 0;
    char compared_word[MAX_STR_LENGTH];
    char *current_str_pos[1], *previous_str_pos[1];
    ring_t *ring_current = ring, *ring_compared = ring;
    *previous_str_pos = *current_str_pos = ring->str;
    while (1) {
        i = getword(compared_word, current_str_pos, *previous_str_pos);
        if (!i && ring_current->next == ring)
            break;
        if (!i) {
            ring_current = ring_current->next;
            *previous_str_pos = *current_str_pos = ring_current->str;
            continue;
        }
        *previous_str_pos = *current_str_pos;
        if (check_file(compared_word, list))
            continue;
        ring_compared = ring_current;
        while (1) {
            n += find_similar_words_count_in_str(compared_word,
                                                 ring_compared->str);
            if (ring_compared->next == ring_current)
                break;
            ring_compared = ring_compared->next;
        }
        fprintf(list, "%s %d\n", compared_word, n);
        n = 0;
    }
}

int check_file(char *word, FILE * list)
{
    char *tmp;
    char buff[100];
    rewind(list);
    while (1) {
        fgets(buff, 100, list);
        if (feof(list))
            break;
        tmp = strchr(buff, ' ');
        *tmp = '\0';
        if (strcmp(word, buff) == 0)
            return 1;
    }
    fseek(list, 0, 2);
    return 0;
}
