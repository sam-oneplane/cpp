## web assembly - binary format 

## emscripten : emcc llvm/clang based 
 


### dependencies:
1. npm , pip , python3, cmake

### git 
2. code example : https://github.com/olafurw/talk-cppp-webassembly

### install :
3.  git clone http://github.com/emscripten-core/emsdk.git
4.  cd emsdk/
5.  ./emsdk install latest

### activate compiler:
1.  ./emsdk activate latest
2.  source ./emsdk_env.sh
3.  emcc --version
4.  which emcc
/home/samuel/Developer/C++/web-assembly/emsdk/upstream/emscripten/emcc

### run emcc compiler:

> emcc hello_world.c -o hello-world.js -O2 -s WASM=1
-  create hello_world.js , hello_world.wasm
-  O2 is the optimization level

> emcc hello_world.c -o hello_world.html -s WASM=1 
-  to add html file and WASM=1 tell the compiler to create a seperate .wsam file


### run product 
1. node hello_world.js

2. run web-server :
>  emrun --port 8080 index.html;


### compile example

> emcc -g -gsource-map --no-entry -s STANDALONE_WASM sandbox_game.cpp -o sandbox_game.html

##### -gsource-map              : for debugging
##### --no-entry                : no main file
##### -s STANDAL ONE_WASM       : only .wasm file exists (no .js or .ts included)

> emcc -g -gsource-map --no-entry -s STANDALONE_WASM box.cpp sandbox_game.cpp -o sandbox_game.html/js;

##### for multple .cpp files;


### additional flags:

> -s USE_SDL=2 -s USE_SDL_TTF=2

##### sdl : emscription drawing library 

> -s INITIAL_MEMORY=256MB -s TOTAL_MEMORY=256MB -s ALLOW_MEMORY_GROWTH=1

##### we need to define memory to allow SDL to be used in wasm 

> preload-file ../assets@/assets

##### this is were files are located for assets like fonts etc...



### import function from js to c++ extren function call

emcc --no-entry  test_exported.cpp -o test_exported.js -s WASM=1 -s WARN_ON_UNDEFINED_SYMBOLS=0



