'use strict';

function loadBinding() {
	try {
		return require('./build/Release/binding.node');
	} catch (err) {
		return require('./build/Debug/binding.node');
	}
}

//const binding = loadBinding();
const dgram = require('dgram');

const EventEmitter = require('events');

class dgramSocket extends EventEmitter {
	constructor(...args) {
		super();
		this.socket = dgram.createSocket(...args);

		this.socket.on('close', (...args) => {
			console.log('passed close', args);
			this.emit('close', ...args);
		});

		this.socket.on('error', (...args)=> {
			console.log('passed error', args);
			this.emit('error', ...args);
		});

		this.socket.on('listening', (...args) => {
			console.log('passed listening', args);
			this.emit('listening', ...args);
		});

		this.socket.on('message', (...args) => {
			console.log('passed message', args);
			this.emit('message', ...args);
		});

		this.on('newListener', (...args) => {
			console.log('newListener', args);
			setImmediate(() => {
				console.log(this.eventNames());
			})
		});

		this.on('removeListener', (...args) => {
			console.log('removeListener', args);
			setImmediate(() => {
				console.log(this.eventNames());
			})
		});
	}

	addMembership(...args) { //multicastAddress, multicastInterface
		return this.socket.addMembership(...args);
	}
	address(...args) {
		return this.socket.address(...args);
	}
	bind(...args) {
		return this.socket.bind(...args);
		/*
		setImmediate(() => {
			this.emit('listening');
		});
		*/
	}
//bind(port, address, callback)
//bind(options, callback)
	close(...args) {//callback
		return this.socket.close(...args);

	}
	dropMembership(...args) {//multicastAddress, multicastInterface
		return this.socket.dropMembership(...args);

	}
	ref(...args) {
		return this.socket.ref(...args);

	}
	send(...args) {//msg, offset, length, port, address, callback
		return this.socket.send(...args);

	}
	setBroadcast(...args) {//flag
		return this.socket.setBroadcast(...args);

	}
	setMulticastLoopback(...args) {//flag
		return this.socket.setMulticastLoopback(...args);

	}
	setMulticastTTL(...args) {//ttl
		return this.socket.setMulticastTTL(...args);

	}
	setTTL(...args) {//ttl
		return this.socket.setTTL(...args);

	}
	unref(...args) {
		return this.socket.unref(...args);

	}
}

module.exports = {dgram: class dgram {
	static createSocket(...args) { return new dgramSocket(...args); }
}
};