#include <emscripten.h>
#include <cstdlib>


typedef struct {
    int val;
    int c;
} subS;

typedef struct {
    int x;
    int y;
    subS sub;
} S;


extern "C" {

extern void printname2console(char *str);

EMSCRIPTEN_KEEPALIVE
S *createstruct(int x, int y, long val, char c) {
    S *obj = static_cast<S*>(malloc(sizeof(S)));
    obj->x = x;
    obj->y = y;
    obj->sub.val = val;
    obj->sub.c = c; 
    return obj;
} 

EMSCRIPTEN_KEEPALIVE
float computesum(S *obj) {
    float val = static_cast<float>(obj->x * obj->y) + 
                    (static_cast<float>(obj->sub.val) / obj->sub.c);
    return  val; 
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