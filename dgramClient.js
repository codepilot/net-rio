'use strict';

const dgram = require('dgram');
const dtg = require('./dgramTestGlobals.js');
const Tracers = require('./Tracers.js');


function addAsyncFuncs() {
	const client = dgram.createSocket('udp4');
	const dgramSocketPrototype = Object.getPrototypeOf(client);
	client.close();

	dgramSocketPrototype.sendAsync = function(...callArgs) {
		return new Promise((resolve, reject) => {
			this.send(...callArgs, (err, ...cbArgs) => {
				if (err) { reject(err); } else { resolve(...cbArgs); }
			});
		});
	}

	dgramSocketPrototype.bindAsync = function(...callArgs) {
		return new Promise((resolve, reject) => {
			this.bind(...callArgs, (err, ...cbArgs) => {
				if (err) { reject(err); } else { resolve(...cbArgs); }
			});
		});
	}

	dgramSocketPrototype.closeAsync = function(...callArgs) {
		return new Promise((resolve, reject) => {
			this.close(...callArgs, (err, ...cbArgs) => {
				if (err) { reject(err); } else { resolve(...cbArgs); }
			});
		});
	}
}

addAsyncFuncs()

function test_udp4_builtin() {
	const client = dgram.createSocket('udp4');
	new Tracers.DgramSocketTracer({prefix: 'udp4_builtin_client', emitter: client});
	client.on('error', (err) => {
		console.log(`client error:\n${err.stack}`);
		client.close();
	});

	client.on('message', (msg, rinfo) => {
		console.log(`client got: ${msg} from ${rinfo.address}:${rinfo.port}`);
	});

	client.on('listening', () => {
		var address = client.address();
		console.log(`client listening ${address.address}:${address.port}`);
	});

	client.send('test_udp4_builtin', dtg.serverAddress.port, dtg.serverAddress.ipv4, (err, ...args) => {
		if(err) {
			console.log('send err', err);
		} else {
			console.log('sent', args);
		}
	});
}

function test_udp6_builtin() {
	const client = dgram.createSocket('udp6');
	new Tracers.DgramSocketTracer({prefix: 'udp6_builtin_client', emitter: client});

	client.on('error', (err) => {
		console.log(`client error:\n${err.stack}`);
		client.close();
	});

	client.on('message', (msg, rinfo) => {
		console.log(`client got: ${msg} from ${rinfo.address}:${rinfo.port}`);
	});

	client.on('listening', () => {
		var address = client.address();
		console.log(`client listening ${address.address}:${address.port}`);
	});

	client.send('test_udp6_builtin', dtg.serverAddress.port, dtg.serverAddress.ipv6, (err, ...args) => {
		if(err) {
			console.log('send err', err);
		} else {
			console.log('sent', args);
		}
	});
}

async function test_udp4_builtin_async() {
	const client = dgram.createSocket('udp4');
	new Tracers.DgramSocketTracer({prefix: 'udp4_builtin_async_client', emitter: client});

	client.on('error', (err) => {
		console.log(`server error:\n${err.stack}`);
		client.close();
	});

	client.on('message', (msg, rinfo) => {
		console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
	});

	console.log('bindAsync', await client.bindAsync(dtg.clientAddress.port, dtg.clientAddress.ipv4));
	console.log('sendAsync', await client.sendAsync('test_udp4_builtin_async', dtg.serverAddress.port, dtg.serverAddress.ipv4));
}

async function test_udp6_builtin_async() {
	const client = dgram.createSocket('udp6');
	new Tracers.DgramSocketTracer({prefix: 'udp6_builtin_async_client', emitter: client});

	client.on('error', (err) => {
		console.log(`server error:\n${err.stack}`);
		client.close();
	});

	client.on('message', (msg, rinfo) => {
		console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
	});

	console.log('bindAsync', await client.bindAsync(dtg.clientAddress.port, dtg.clientAddress.ipv6));
	console.log('sendAsync', await client.sendAsync('test_udp6_builtin_async', dtg.serverAddress.port, dtg.serverAddress.ipv6));
}

function runAllTests() {
	test_udp4_builtin();
	test_udp6_builtin();
	test_udp4_builtin_async();
	test_udp6_builtin_async();
}

setTimeout(runAllTests, 100);
