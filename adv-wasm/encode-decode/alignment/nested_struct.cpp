#include <emscripten.h>
#include <cstdlib>
#include <array>
#include <numeric>


constexpr int VAL = 15;
constexpr int SIZE = 5;


typedef struct {
    int val;
    int c;
} subS;

typedef struct {
    int x;
    int y;
    subS sub;
    int *ptr;
    int **dptr;
    int *arr;
    int arr_size;
    char *name;
} S;



extern "C" {

extern void printName(char *str);

/* Decoder part*/
EMSCRIPTEN_KEEPALIVE
S *createstruct(int x, int y, long val, char c) {
    S *obj = static_cast<S*>(malloc(sizeof(S)));
    obj->x = x;
    obj->y = y;
    obj->sub.val = val;
    obj->sub.c = c;

    *(obj->ptr)= VAL;
    *(*(obj->dptr))= 4*VAL;

    return obj;
} 

EMSCRIPTEN_KEEPALIVE
float computesum(S *obj) {
    float val = static_cast<float>(obj->x * obj->y) + 
                    (static_cast<float>(obj->sub.val) / obj->sub.c);
    return  val; 
}

EMSCRIPTEN_KEEPALIVE
int getptr(S *obj) {
    return *(obj->ptr);
}


EMSCRIPTEN_KEEPALIVE
int getdptr(S *obj) {
    return *(*(obj->dptr));
}

EMSCRIPTEN_KEEPALIVE
float arravg(S *obj) {
    int sum = 0;
    for (int i=0; i<obj->arr_size; i++) {
        sum += obj->arr[i];
    }
    return (static_cast<float>(sum) / obj->arr_size) ;
}

EMSCRIPTEN_KEEPALIVE
int sumarr(S *obj){
    int sum = 0;
    int i = 0;
    while (obj->arr[i]) {
        sum += obj->arr[i++];
    }
    return sum;
}

EMSCRIPTEN_KEEPALIVE
int sumarrbysize(S *obj){
    int sum = 0;
    for (int i = 0; i < obj->arr_size; i++)
        sum += obj->arr[i];
    return sum;
}

EMSCRIPTEN_KEEPALIVE
void printname(S *obj) {
    printName(obj->name);
}


EMSCRIPTEN_KEEPALIVE
void *wasmalloc(size_t n) {
    return malloc(n);
} 

EMSCRIPTEN_KEEPALIVE
void wasmfree(void *ptr) {
    free(ptr);
} 

}