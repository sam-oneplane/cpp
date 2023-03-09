let types = {
    char: 4,
    int: 4,
    float: 4,
};

let primitives = ["char", "int", "float"];

let structs = {};

// REGISTER

function getElemSize(type) {
    if (type.endsWith('*') || type.endsWith('[]')) {
        return 4;
    } else {
        return types[type];
    }
}

function computeStructSize(structFormat) {
    let size = 0;
    for (const type of Object.values(structFormat)) {
        size += getElemSize(type);
    }
    return size;
}

function registerStruct(name, structFormat) {
    types[name] = computeStructSize(structFormat);
    structs[name] = structFormat;
}


function encodeInt(num, bytes, buffer, offset=0) {

    for (var i = 0; i < bytes; i++) {
        buffer[i+offset] = num & 0xff;
        num >>= 8;
    }
}

function encodePtr(type, obj, memory, malloc) {

    if(!obj) {return 0;}

    const n = getElemSize(type);
    const ptr = malloc(n);
    const buff = new Uint8Array(memory.buffer, ptr, n);

    encodeElem(type, obj, memory, buff, malloc, 0, ptr);

    return ptr;
}


function encodeStruct(type, obj, memory, buffer, malloc, cursor=0, ptr=0) {

    for(const [name, t] of Object.entries(structs[type])) {

        encodeElem(t, obj[name], memory, buffer, malloc, cursor, ptr);
        cursor += getElemSize(t);
    }
}

function encodeArray(type, obj, memory, malloc) {
    if (!obj) {
        return 0; // NULL
    }

    const nIndividual = getElemSize(type);
    const n = (obj.length + 1) * nIndividual;
    const ptr = malloc(n);
    const buf = new Uint8Array(memory.buffer, ptr, n);
    if (type === 'char') {
        for (var i = 0; i < obj.length; i++) {
            buf[i] = obj.charCodeAt(i);
        }
        buf[obj.length] = 0;
    } else {
        let cursor = 0;
        for (var i = 0; i < obj.length; i++) {
            encodeElem(type, obj[i], memory, buf, malloc, cursor, ptr);
            cursor += nIndividual;
        }
        encodeInt(0, nIndividual, buf, cursor); // terminator element
    }

    return ptr;
}

// ENCODE into memory using ptr and new typeArray() 
function encodeElem(type, obj, memory, buffer, malloc, cursor=0, ptr=0) {

    if (type.endsWith("*")) {
        const p = encodePtr(type.substring(0, type.length-1), obj, memory, malloc);
        encodeInt(p, 4, buffer, cursor);
    }else if (type.endsWith('[]')) {
        const p = encodeArray(type.substring(0, type.length - 2), obj, memory, malloc);
        encodeInt(p, 4, buffer, cursor);
    }else if (primitives.includes(type)) {
        if (type !== 'float') {
            encodeInt(obj ?? 0, types[type], buffer, cursor);
        }else {
            const floatBuffer = new Float32Array(memory.buffer, ptr+cursor);
            floatBuffer[0] = obj ?? 0.0;
        }

    }else {
        encodeStruct(type, obj, memory, buffer, malloc, cursor, ptr);
    }

}



/*  DECODE FUNCTIONS */

function decodeNum(n, buffer, offset=0) {
    let ret = 0 ;
    for (var i= n-1; i >= 0; i--) {
        ret <<= 8;
        ret |= buffer[i+offset] ;
    }
    return ret;
}

// we need to decode the struct send from c++ .wasm file
function decodeStruct(name, buffer, offset=0) {
    var ret = {}; // return struct {key:value}
    let cursor = offset;
    console.log(structs[name]);

    for (const [key, type] of Object.entries(structs[name])) {

        if (type.endsWith("*") || type.endsWith("**") || type.endsWith('[]')) {
            ret[key] = decodeNum(types[type], buffer, cursor);
        }else if (primitives.includes(type)) {
            ret[key] = decodeNum(types[type], buffer, cursor);
        }else {
            ret[key] = decodeStruct(type, buffer, cursor);

        }
        
        cursor += types[type];
    }
    return ret;
}

/* Decode entry point */
function startDecodeStruct(name, ptr, memory) {

    var buff = new Uint8Array(memory.buffer, ptr, types[name]);
    return decodeStruct(name, buff);
}




