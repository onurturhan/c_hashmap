/*
 * A unit test and example of how to use the simple C hashmap
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "hashmap.h"

#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (1024*1024)

typedef struct data_struct_s
{
    char key_string[KEY_MAX_LENGTH];
    int number;
} data_struct_t;

int hashmap_test_func(void* input, void* current)
{
	data_struct_t* d = input;
	data_struct_t* c = current;
	
	if(!strcmp(d->key_string, c->key_string) && (d->number == c->number))
	{
	    printf("==> Found { \"%s\", \"%d\" } in hash map ... OK  \n", c->key_string, c->number);
	    return (MAP_OK + 1);
	}
		
	return MAP_OK;
}

int main(int argc,  char* argv[])
{
    int index;
    int error;
    map_t mymap;
    char key_string[KEY_MAX_LENGTH];
    data_struct_t* value;
    
    printf("==> Create the hash map ... \n");
    mymap = hashmap_new();

    printf("==> Populate the hash map ... \n");
    /* First, populate the hash map with ascending values */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        /* Store the key string along side the numerical value so we can free it later */
        value = malloc(sizeof(data_struct_t));
        snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
        value->number = index;

        error = hashmap_put(mymap, value->key_string, value);
        assert(error==MAP_OK);
    }

    printf("==> Check the hash map ... \n");
    /* Now, check all of the expected values are there */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);

        error = hashmap_get(mymap, key_string, (void**)(&value));
        
        /* Make sure the value was both found and the correct number */
        assert(error==MAP_OK);
        assert(value->number==index);
    }

    printf("==> Test the hash map ... \n");    
    /* Make sure that a value that wasn't in the map can't be found */
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, KEY_COUNT);

    error = hashmap_get(mymap, key_string, (void**)(&value));
        
    /* Make sure the value was not found */
    assert(error==MAP_MISSING);
    
    printf("==> Iterate the hash map ... \n");   
    /* Store the key string along side the numerical value so we can free it later */
    value = malloc(sizeof(data_struct_t));
    snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, 100);
    value->number = 100;
         
    error = hashmap_iterate(mymap, (PFany) hashmap_test_func, value) ;
    /* Make sure the value was found in hashmap */
    assert(error==(MAP_OK + 1));
    
    printf("==> Check the hash map length ... \n");
    error = hashmap_length(mymap);
    
     /* Make sure length correct */
    assert(error==KEY_COUNT);      
        
    printf("==> Remove the hash map ... \n");    
    /* Free all of the values we allocated and remove them from the map */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);

        error = hashmap_get(mymap, key_string, (void**)(&value));
        assert(error==MAP_OK);

        error = hashmap_remove(mymap, key_string);
        assert(error==MAP_OK);

        free(value);        
    }
 
     printf("==> Destroy the hash map ... \n");      
    /* Now, destroy the map */
    hashmap_free(mymap);

    return 1;
}
