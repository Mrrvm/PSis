#include "generic_list.h"


struct _node {
    item this;
    node *next;
};

struct _list {
    size_t size;
    node *head;
    size_t item_size;
};

/********************************* NODE *********************************/

node *create_node(item new_item, node *next_node, int item_size_) {
    node *new_node = NULL;
    /* Memory allocation */
    new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL)
        printf("Unable to reserve node memory");
    /* Add item to node*/
    new_node->this = malloc(item_size_);
    memcpy(new_node->this, new_item, item_size_);
    new_node->next = next_node;
    return new_node;
}

item get_node_item(node *got_node) {
    return got_node->this;
}

void set_item_as(node *got_node, void (*setter)(item)) {
    setter(got_node->this);
}

node *get_next_node(node *got_node) {
    return got_node->next;
}

void set_next_node(node *got_node, node *next_node) {
    got_node->next = next_node;
}

void free_node(node *got_node, void (*free_item)(item)) {
    /* Free node item */
    free_item(got_node->this);
    /* Free the node to save on the load */
    free(got_node);
    return;
}

void free_connected_nodes(node *got_node, void (*free_item)(item)) {
    node *aux_node = NULL;

    /* Free every node connected*/
    while(got_node != NULL) {
        aux_node = get_next_node(got_node);
        free_node(got_node, free_item);
        got_node = aux_node;
    }
    return;
}

/********************************* LIST *********************************/

list *create_list(int item_size_) {
    list *new_list = NULL;
    /* Create List */
    new_list = (list *)malloc(sizeof(list));
    if(new_list == NULL)
        printf("Unable to reserve list memory");
    /* Set head to null */
    new_list->head = NULL;
    /* Set size to 0 :( */
    new_list->size = 0;
    new_list->item_size = item_size_;
    return new_list;
}

node *get_head(list *got_list) {
    return got_list->head;
}

void set_head(list *got_list, node *got_head) {
    got_list->head = got_head;
    return;
}

void push_node_to_list(list *got_list, node *got_node) {
    got_node->next = got_list->head;
    got_list->head = got_node;
    /* Increase List size :D */
    got_list->size ++;
    return;
}

void decrement_list_size(list *got_list){
    got_list->size --;
}

void push_item_to_list(list *got_list, item new_item) {
    /* Node Creation */
    node *new_node = NULL;
    new_node = create_node(new_item, NULL, got_list->item_size);
    /* Prepend node to list*/
    push_node_to_list(got_list, new_node);
    return;
}

size_t get_list_size(list *got_list) {
    return got_list->size;
}

void print_list(list *got_list, void (*print_item)(item)) {
    node *aux_node;
    printf("Print list:\n");
    printf("Size of list: %u\n", get_list_size(got_list));

    for(aux_node = get_head(got_list);
            aux_node != NULL;
            aux_node = get_next_node(aux_node)) {
        printf("[ ");
        print_item(get_node_item(aux_node));
        printf(" ] -> ");
    }
    printf("\n");
}

void free_list(list *got_list, void (*free_item)(item)) {
    node *got_node = get_head(got_list);
    /* Free every node of list*/
    free_connected_nodes(got_node, free_item);
    /* Bring freedom to stack */
    free(got_list);
    return;
}
