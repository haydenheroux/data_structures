#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_TYPE int

#define VEC_MIN_CAPACITY 2

typedef struct {
    VEC_TYPE* memory;
    size_t length, capacity;
} vector_t;

vector_t* vector_new(size_t length) {
    vector_t* vector = (vector_t*) malloc(sizeof(vector_t));
    
    size_t capacity = length == 0 ? VEC_MIN_CAPACITY : length * 2;

    vector->length = length;
    vector->capacity = capacity;

    vector->memory = calloc(capacity, sizeof(VEC_TYPE));

    return vector;
}

void vector_delete(vector_t* vector) {
    free(vector->memory);
    free(vector);
}

VEC_TYPE vector_at(vector_t* vector, size_t position) {
    if (position < vector->length) {
        return vector->memory[position];
    }

    // FIXME Out-of-bounds
    return 0;
}

void vector_print(vector_t* vector) {
    printf("vector->length = %zu\n", vector->length);
    printf("vector->capacity = %zu\n", vector->capacity);
    printf("vector->memory = %p\n", vector->memory);

    for (int i = 0; i < vector->length; i++) {
        printf("vector->memory[%d] = %d\n", i, vector_at(vector, i));
    }

    puts("----------------------------------------");
}

VEC_TYPE vector_first(vector_t* vector) {
    if (vector->length >= 1) return vector->memory[0];

    // FIXME Out-of-bounds
    return 0;
}

VEC_TYPE vector_last(vector_t* vector) {
    if (vector->length >= 1) return vector->memory[vector->length - 1];

    // FIXME Out-of-bounds
    return 0;
}

void vector_clear(vector_t* vector) {
    vector->length = 0;
}

void vector_insert(vector_t* vector, size_t position, VEC_TYPE value) {
    bool need_resize = vector->length + 1 > vector->capacity;

    if (need_resize) {
        vector->capacity *= 2;
        vector->memory = reallocarray(vector->memory, vector->capacity, sizeof(VEC_TYPE));
    }

    size_t source_position = position;
    size_t destination_position = position + 1;
   
    size_t move_elements = vector->length - source_position;
    size_t move_size = move_elements * sizeof(VEC_TYPE);

    VEC_TYPE* destination = &vector->memory[destination_position];
    VEC_TYPE* source = &vector->memory[source_position];

    memmove(destination, source, move_size);

    vector->length++;

    vector->memory[position] = value;
}

void vector_erase(vector_t* vector, size_t position) {
    size_t source_position = position + 1;
    size_t destination_position = position;
   
    size_t move_elements = vector->length - source_position;
    size_t move_size = move_elements * sizeof(VEC_TYPE);

    VEC_TYPE* destination = &vector->memory[destination_position];
    VEC_TYPE* source = &vector->memory[source_position];

    memmove(destination, source, move_size);

    vector->length--;
}

void vector_push_back(vector_t* vector, VEC_TYPE value) {
    vector_insert(vector, vector->length, value);
}

VEC_TYPE vector_pop_back(vector_t* vector) {
    VEC_TYPE value = vector_at(vector, vector->length - 1);
    vector_erase(vector, vector->length - 1);
    return value;
}

int main() {
    vector_t* vector = vector_new(0);

    // vector_push_back works as expected
    vector_push_back(vector, 555);
    vector_push_back(vector, 666);
    vector_push_back(vector, 777);
    vector_push_back(vector, 888);
    vector_push_back(vector, 999);
    vector_print(vector);

    // vector_insert works as expected
    vector_insert(vector, 0, 444);
    vector_insert(vector, 2, 607);
    vector_insert(vector, 4, 700);
    vector_insert(vector, 6, 800);
    vector_print(vector);

    // vector_erase works as expected
    vector_erase(vector, 1);
    vector_print(vector);

    // vector_insert works as expected
    vector_insert(vector, 1, 500);
    vector_print(vector);

    // vector_pop_back works as expected
    VEC_TYPE pop_value = vector_pop_back(vector);
    printf("vector_pop_back: %d\n", pop_value);
    vector_print(vector);

    // vector_first works as expected
    VEC_TYPE first_value = vector_first(vector);
    printf("vector_first: %d\n", first_value);
    vector_print(vector);

    // vector_last works as expected
    VEC_TYPE last_value = vector_last(vector);
    printf("vector_last: %d\n", last_value);
    vector_print(vector);

    // vector_clear works as expected
    vector_clear(vector);
    vector_print(vector);

    // vector_push_back after clear works as expected
    vector_push_back(vector, 555);
    vector_push_back(vector, 666);
    vector_push_back(vector, 777);
    vector_push_back(vector, 888);
    vector_push_back(vector, 999);
    vector_print(vector);

    vector_delete(vector);
}
