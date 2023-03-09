let types = {
    char: 1,
    short: 2,
    int: 4,
    float: 4,
    double: 8,
    long: 8,
};


let structs = {};


function computeStructSize(structFormat) {
    let size = 0;
    for (const type of Object.values(structFormat)) {
        size += types[type];
    }
    console.log(structFormat, "size:" , size);
    return size;
}

function registerStruct(name, structFormat) {
    types[name] = computeStructSize(structFormat);
    structs[name] = structFormat;
}

// encode number in little endian
function encodeNum(num, n, buffer, offset=0) {
    for(var i=0; i < n; i++) {
        buffer[i+offset] = num & 0xff ;
        num >>= 8;
    }
}

function encodeStruct(name, obj, buffer) {
    let cursor = 0;
    for(const [key, type] of Object.entries(structs[name])) {
        encodeNum(obj[key] ?? 0, types[type], buffer, cursor);
        console.log(obj[key] ?? 0, "for:", key, "as:", type, "at:", cursor);
        cursor += types[type];
    }
}

function startEncodeStruct(name, object, memory, malloc) {

    var ptr = malloc(types[name]);
    let buff = new Uint8Array(memory.buffer, ptr, types[name]);

    encodeStruct(name, object, buff);
    return ptr;
}

function decodeNum(n, buffer, offset=0) {
    let ret = 0 ;
    for (var i= n-1; i >= 0; i--) {
        ret <<= 8;
        ret |= buffer[i+offset] ;
    }
    return ret;
}

// we need to decode the struct send from c++ .wasm file
function decodeStruct(name, buffer) {
    var ret = {}; // return struct {key:value}
    let cursor = 0;
    for (const [key, type] of Object.entries(structs[name])) {
        
        ret[key] = decodeNum(types[type], buffer, cursor);
        cursor += types[type];
    }
    return ret;
}

function startDecodeStruct(name, ptr, memory) {

    var buff = new Uint8Array(memory.buffer, ptr, types[name]);
    return decodeStruct(name, buff);
}




