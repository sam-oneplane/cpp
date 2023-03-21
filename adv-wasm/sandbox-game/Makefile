build:
    emcc -g -gsource-map --no-entry -s STANDALONE_WASM box.cpp sandbox_game.cpp -o sandbox_game.js -s WASM=1

run: 
    emrun --port 8080 index.html
    
