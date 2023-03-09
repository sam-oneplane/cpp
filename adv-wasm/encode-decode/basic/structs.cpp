#include <emscripten.h>
#include <cstdlib>

typedef struct {
    int x;
    int y;
    float z;
} pair ;

typedef struct {
    int x;
    int y;
} S ;


extern "C" {

EMSCRIPTEN_KEEPALIVE
float computesumf(pair *p) { 
    float r = static_cast<float>(p->x + p->y);
    return r + p->z;
}

EMSCRIPTEN_KEEPALIVE
int computesumi(pair *p) {
    return (p->x + p->y);
}


EMSCRIPTEN_KEEPALIVE
S *makeS(int x, int y) {
    S *s =  static_cast<S *>(malloc(sizeof(S)));
    s->x = x;
    s->y = y;

    return s;
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





