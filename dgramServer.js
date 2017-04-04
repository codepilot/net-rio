'use strict';

const dgram = require('dgram');
const rioDgram = require('net-rio').dgram;
const dtg = require('./dgramTestGlobals.js');
const Tracers = require('./Tracers.js');


function test_udp4_builtin() {
	const server = dgram.createSocket('udp4');
	new Tracers.DgramSocketTracer({ prefix: 'udp4_server', emitter: server });

	server.bind(dtg.serverAddress.port, dtg.serverAddress.ipv4);
}

function test_udp6_builtin() {
	const server = dgram.createSocket('udp6');
	new Tracers.DgramSocketTracer({ prefix: 'udp6_server', emitter: server });

	server.bind(dtg.serverAddress.port, dtg.serverAddress.ipv6);
}


async function test_udp4_iocp_events() {
	const server = rioDgram.createSocket('udp4');
	new Tracers.DgramSocketTracer({ prefix: 'udp4_server', emitter: server });

	console.log('bound', server.bind(dtg.serverAddress.port, dtg.serverAddress.ipv4));
}

async function test_udp4_iocp_promise() {
	const server = rioDgram.createSocket('udp4');
	new Tracers.DgramSocketTracer({ prefix: 'udp4_server', emitter: server });

	console.log('bound', await server.bindAsync(dtg.serverAddress.port, dtg.serverAddress.ipv4));
}

async function test_udp4_rio() {
	const server = rioDgram.createSocket('udp4');
	new Tracers.DgramSocketTracer({ prefix: 'udp4_server', emitter: server });

	console.log('bound', server.bind(dtg.serverAddress.port, dtg.serverAddress.ipv4));
//console.log('bound', await server.bindAsync(dtg.serverAddress.port, dtg.serverAddress.ipv4));
}


function runAllTests() {
//test_udp4_builtin();
//test_udp6_builtin();
	test_udp4_iocp_events();
	test_udp4_iocp_promise();
//test_udp4_rio();
//test_udp6_rio();
}

setImmediate(runAllTests);

//process.send({ foo: 'bar' });