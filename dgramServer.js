'use strict';

const dgram = require('dgram');
const dtg = require('./dgramTestGlobals.js');
const Tracers = require('./Tracers.js');


function test_udp4_builtin() {
	const server = dgram.createSocket('udp4');
	new Tracers.DgramSocketTracer({ prefix: 'udp4_server', emitter: server });

	server.on('error', (err) => {
		console.log(`server error:\n${err.stack}`);
		server.close();
	});

	server.on('message', (msg, rinfo) => {
		console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
	});

	server.on('listening', () => {
		var address = server.address();
		console.log(`server listening ${address.address}:${address.port}`);
	});

	server.bind(dtg.serverAddress.port, dtg.serverAddress.ipv4);
}

function test_udp6_builtin() {
	const server = dgram.createSocket('udp6');
	new Tracers.DgramSocketTracer({ prefix: 'udp6_server', emitter: server });

	server.on('error', (err) => {
		console.log(`server error:\n${err.stack}`);
		server.close();
	});

	server.on('message', (msg, rinfo) => {
		console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
	});

	server.on('listening', () => {
		var address = server.address();
		console.log(`server listening ${address.address}:${address.port}`);
	});

	server.bind(dtg.serverAddress.port, dtg.serverAddress.ipv6);
}

test_udp4_builtin();
test_udp6_builtin();

process.send({ foo: 'bar' });