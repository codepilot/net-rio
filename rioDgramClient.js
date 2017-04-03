'use strict';

const netRio = require('net-rio');

console.log(netRio.dgram.createSocket('udp4'));
console.log(netRio.dgram.createSocket('udp6'));

console.log(netRio.dgram.createSocket({ type: 'udp4' }));
console.log(netRio.dgram.createSocket({ type: 'udp6' }));



const symbolA = Symbol('A');
const symbolB = Symbol('B');
const symbolC = Symbol('C');
const symbolD = Symbol('D');


switch (symbolA) {
    case symbolA: console.log('symbolA'); break;
    case symbolB: console.log('symbolB'); break;
    case symbolC: console.log('symbolC'); break;
    case symbolD: console.log('symbolD'); break;
    default: console.log('default');
}