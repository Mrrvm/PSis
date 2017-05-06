
#include "structures.h"


struct _node {
    item this;
    node *next;
};

struct _list {
    size_t size;
    node *head;
};

struct _vector {
    item *data;
    size_t size;
};

/********************************* NODE *********************************/

node *create_node(item new_item, node *next_node) {
    node *new_node = NULL;
    /* Memory allocation */
    new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL)
        memory_error("Unable to reserve node memory");
    /* Add item to node*/
    new_node->this = new_item;
    new_node->next = next_node;
    return new_node;
}

item get_node_item(node *got_node) {
    return got_node->this;
}

node *get_next_node(node *got_node) {
    return got_node->next;
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

list *create_list() {
    list *new_list = NULL;
    /* Create List */
    new_list = (list *)malloc(sizeof(list));
    if(new_list == NULL)
        memory_error("Unable to reserve list memory");
    /* Set head to null */
    new_list->head = NULL;
    /* Set size to 0 :( */
    new_list->size = 0;
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

void push_item_to_list(list *got_list, item new_item) {
    /* Node Creation */
    node *new_node = NULL;
    new_node = create_node(new_item, NULL);
    /* Prepend node to list*/
    push_node_to_list(got_list, new_node);
    return;
}

size_t get_list_size(list *got_list) {
    return got_list->size;
}

void print_list(list *got_list, void (*print_item)(item)) {
    node *aux_node;
    spam(("Print list:\n"));
    spam(("Size of list: %lu\n", get_list_size(got_list)));

    for(aux_node = get_head(got_list);
            aux_node != NULL;
            aux_node = get_next_node(aux_node)) {
        spam(("[ "));
        print_item(get_node_item(aux_node));
        spam(("] -> \n"));
    }
    spam(("-> [ " KBLU "NULL" RESET " ]"));
}

void free_list(list *got_list, void (*free_item)(item)) {
    node *got_node = get_head(got_list);
    /* Free every node of list*/
    free_connected_nodes(got_node, free_item);
    /* Bring freedom to stack */
    free(got_list);
    return;
}

/******************************** VECTOR ********************************/

vector *create_vector(int n_elements) {
    vector *new_vector = NULL;
    new_vector = (vector *)malloc(sizeof(vector));
    if(new_vector == NULL) {
        memory_error("Unable to reserve vector memory");
    }
    /*Alloc an array of void pointers*/
    new_vector->data = (item *)calloc(n_elements, sizeof(item));
    if(new_vector->data == NULL) {
        memory_error("Unable to reserve vector memory");
    }
    new_vector->size = n_elements;
     return new_vector;
}

size_t get_vector_size(vector *got_vector) {
    return got_vector->size;
}

item get_vector_item(int index, vector *got_vector) {
    if(index >= (int)got_vector->size || index < 0) {
        memory_error("Unable to reach element memory while getting");
    }
    return got_vector->data[index];
}

void set_item_to_vector(int index, vector *got_vector, item new_item) {
    if(index >= (int)got_vector->size || index < 0) {
        memory_error("Unable to reach element memory while setting");
    }
    got_vector->data[index] = new_item;
    return;
}

void free_vector(vector *got_vector, void (*free_item)(item)) {
    int size = got_vector->size;
    int i = 0;
    while(i < size) {
        free_item(get_vector_item(i, got_vector));
        i++;
    }
    free(got_vector->data);
    free(got_vector);
    return;
}

void print_vector(vector *got_vector, void (*print_item)(item)) {
    int size = got_vector->size;
    int i = 0;
    while(i < size) {
        spam((KGRN "\n[Element = %d]:\n" RESET, i));
        print_item(got_vector->data[i]);
        spam(("\n"));
        i++;
    }
    return;
}