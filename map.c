//
// Created by danaa on 4/22/2020.
//
#include "map.h"
#include <stdlib.h>
#include <string.h>
#define NULL_SIZE -1

typedef struct key_t{
    char *key;
    char *value;
    struct key_t *next;
}*Key;

struct Map_t {
    Key head;
    Key following;
    int size;
};
//---------------------------FUNCTIONS RELATED TO ADT NODE----------------------------//
//nodeCreate creates a new node, returns NULL if recieved a NULL argument.
static Key nodeCreate(char *key, char *value) {
    Key node = malloc(sizeof(*node));
    if ((node==NULL) ||( key==NULL) ||(value==NULL))
    {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}
//nodeDestroy removes the node given from the list.
static void nodeDestroy (Key node) {
    while (node) {
        Key toDelete = node;
        node = node->next;
        free(toDelete->key);
        free(toDelete->value);
        free(toDelete);
    }
}
//addNote adds a new node to the list  given the head of the list and the next node.
static void addNode(Key head, Key next) {
    if (head==NULL) {
        head = next;

    }
    Key node = head;
    while (node->next) {
       node = node->next;
    }
    node->next = next;
}
//---------------------------FUNCTIONS RELATED TO ADT MAP----------------------------//

Map mapCreate() {
    Map map = malloc(sizeof(*map));
    if (map==NULL) {
        return NULL;
    }
    map->head = NULL;
    map->following = NULL;
    map->size = 0;
    return map;
}

void mapDestroy(Map map) {
    if (map== NULL) {
        return;
    }
    nodeDestroy(map->head);
    free(map);
}

Map mapCopy(Map map) {
    if (!map) {
        return NULL;
    }
    Map copy = mapCreate();
    if (copy ==NULL) {
        return NULL;
    }
    copy->following = map->following;
    copy->size = map->size;
    Key node = map->head;
    if ((map->head == NULL)) {
        copy->head = NULL;
        return copy;
    }
    Key new = nodeCreate(node->key, node->value);
    if (new== NULL) {
        mapDestroy(copy);
        return NULL;
    }
    Key head_new = new;
    node = node->next;
    while (node) {
        new = nodeCreate(node->key, node->value);
        if (new == NULL) {
            nodeDestroy(head_new);
            mapDestroy(copy);
            return NULL;
        }
        addNode(head_new, new);
        node= node->next;
    }
    copy->head = head_new;
    return copy;
}

int mapGetSize(Map map) {
    if (map== NULL) {
        map->size=NULL_SIZE; // we should return -1 as required in the PDF
    }
    return map->size;
}


bool mapContains(Map map, const char *key) {
    if (map==NULL || key==NULL) {
        return false;
    }
    Key node = map->head;
    while (node) {
        if ((strcmp(node->key, key)) == 0) {
            return true;
        }
        node= node->next;
    }
    return false;
}

MapResult mapPut(Map map, const char *key, const char *data) {
    if(map== NULL || key==NULL || data==NULL){
        return  MAP_NULL_ARGUMENT;
    }
    Key node = map->head;
    char* temp_d=malloc(strlen(data)+1);
    char* temp_k=malloc(strlen(key)+1);
    strcpy(temp_d,data);
    strcpy(temp_k,key);
    while(node){

        if(strcmp(node->key,key) == 0){
            free(node->value);
            node->value=temp_d;
            if(node->value == NULL){
                free(node->value);
                return MAP_OUT_OF_MEMORY;
            }
            free(temp_d);
            free(temp_k);
            return MAP_SUCCESS;
        }
        node=node->next;
    }
    Key new=nodeCreate(temp_k, temp_d);
    if(new == NULL){
        return MAP_OUT_OF_MEMORY;
    }
    if(map->head ==NULL ){
        map->head=new;
        map->size++;
        free(temp_d);
        free(temp_k);
        return MAP_SUCCESS;
    }
    new->next=map->head;
    map->head=new;
    map->size++;
    free(temp_d);
    free(temp_k);
    return MAP_SUCCESS;
}


char *mapGet(Map map, const char *key) {
    if (map== NULL) {
        return NULL;
    }
    Key node = map->head;
    if (node == NULL) {
        return NULL;
    }
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

MapResult mapRemove(Map map, const char *key) {
    if (map==NULL || key==NULL) {
        return MAP_NULL_ARGUMENT;
    }
    Key node = map->head;
    if (node==NULL) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    if (!strcmp(node->key, key)) {
        map->head = node->next;
        map->size--;
        free(node->key);
        free(node->value);
        free(node);
        return MAP_SUCCESS;
    }
    Key previous = node;
    while (node->next) {
        node = node->next;
        if (strcmp(node->key, key) == 0) {
            previous->next = node->next;
            map->size--;
            free(node->key);
            free(node->value);
            free(node);
            return MAP_SUCCESS;
        }
        previous = node;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

char *mapGetFirst(Map map) {
    if ((map==NULL) || (map->size == 0)) {
        return NULL;
    }
    map->following = map->head;
    if (map->following == NULL) {
        return NULL;
    }
    return map->following->key;

}

char *mapGetNext(Map map) {
    if ((map==NULL) || (map->following ==NULL) || (map->following->next == NULL) || (map->head==NULL)) {
        return NULL;
    }
    map->following = map->following->next;
    return map->following->key;
}


MapResult mapClear(Map map) {
    if (!map) {
        return MAP_NULL_ARGUMENT;
    }
    nodeDestroy(map->head);
    map->size = 0;
    map->head=NULL;
    map->following = NULL;
    return MAP_SUCCESS;
}

