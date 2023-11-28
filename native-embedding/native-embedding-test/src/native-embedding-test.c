#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graal_isolate.h"
#include "native_embedding_api.h"

int main(int argc, char **argv) {
  graal_isolate_t* isolate;
  graal_isolatethread_t* isolate_thread;
  if (graal_create_isolate(NULL, &isolate, &isolate_thread) != 0) {
    printf("Create Isolate Failed!\n");
    return 1;
  }

  if (nea_hello_from_js(isolate_thread) != 0) {
    printf("Hello From JS Failed!\n");
    return 1;
  }
  char* original_string = "permutation";
  char* permutation = calloc(strlen(original_string) + 1, sizeof(char));
  size_t length;
  if (nea_string_permutation_js(isolate_thread, original_string, permutation, strlen(original_string) + 1, &length) != 0) {
    printf("String Permutation in JS Failed!\n");
    return 1;
  }
  if (length != strlen(original_string)) {
    printf("Permutation Length is not equal to the Length of the Original String!\n");
    return 1;
  }
  printf("Random permutation of \"%s\" = %s\n", original_string, permutation);
  free(permutation);

  int n = 50;
  long long int fib_n;
  if (nea_fibonacci_js(isolate_thread, n, &fib_n) != 0) {
    printf("N-th Fibonacci Number in JS Failed!\n");
    return 1;
  }
  printf("Fibonacci number no. %d = %lld\n", n, fib_n);

  if (graal_tear_down_isolate(isolate_thread) != 0) {
    printf("TearDown Isolate Failed!\n");
    return 1;
  }
}
