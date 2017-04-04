'use strict';

function loadBinding() {
	try {
		return require('./build/Release/binding.node');
	} catch (err) {
		return require('./build/Debug/binding.node');
	}
}

const binding = loadBinding();
//console.log('binding', binding);
const dgram = require('dgram');

const EventEmitter = require('events');

const wsConsts = {
	AF_INET: 2,
	AF_INET6: 23,
	SOCK_STREAM: 1,
	SOCK_DGRAM: 2,
	IPPROTO_TCP: 6,
	IPPROTO_UDP: 17,
	WSA_FLAG_OVERLAPPED: 0x01,
	WSA_FLAG_REGISTERED_IO: 0x100
};

const type_to_ws_af = { udp4: wsConsts.AF_INET, udp6: wsConsts.AF_INET6 };
const type_to_ws_type = { udp4: wsConsts.SOCK_DGRAM, udp6: wsConsts.SOCK_DGRAM };
const type_to_ws_protocol = { udp4: wsConsts.IPPROTO_UDP, udp6: wsConsts.IPPROTO_UDP };
const type_to_ws_flags = { iocp: wsConsts.WSA_FLAG_OVERLAPPED, rio: wsConsts.WSA_FLAG_REGISTERED_IO };

class dgramSocket extends EventEmitter {
	constructor(...args) {
		super();

		switch (typeof args[0]) {
			case 'string':
				this.socket = new binding.UdpSocket(
					type_to_ws_af[args[0]],
					type_to_ws_type[args[0]],
					type_to_ws_protocol[args[0]],
					type_to_ws_flags.iocp);

				break;

			case 'object':
				this.socket = new binding.UdpSocket(
					type_to_ws_af[args[0].type],
					type_to_ws_type[args[0].type],
					type_to_ws_protocol[args[0].type],
					type_to_ws_flags[args[0].io]);

				break;

			default:
				throw new TypeError('Expected a string or an object');
		}

	//console.log(this.socket);
	//console.log(Object.getPrototypeOf(this.socket));
	//console.log(this.socket.bind);

	//this.socket = dgram.createSocket(type);
		/*
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
		*/
	}

	addMembership(...args) { //multicastAddress, multicastInterface
		return this.socket.addMembership(...args);
	}
	address(...args) {
		return this.socket.address(...args);
	}
	bindSync(...args) {
		const port = args[0];
		const strAddr = args[1];
		const addrParts = strAddr.split('.').map((v, i, a)=> parseInt(v, 10));
		const sockAddr = Buffer.alloc(128);
		sockAddr.writeUInt16LE(wsConsts.AF_INET, 0);
		sockAddr.writeUInt16BE(args[0], 2); //port
		sockAddr[4] = addrParts[0];
		sockAddr[5] = addrParts[1];
		sockAddr[6] = addrParts[2];
		sockAddr[7] = addrParts[3];
		const ret = this.socket.bindSync(sockAddr);
		console.log('bind ret', ret);
		return ret;
	}	
	
	bindAsync(...args) {
		const port = args[0];
		const strAddr = args[1];
		const addrParts = strAddr.split('.').map((v, i, a)=> parseInt(v, 10));
		const sockAddr = Buffer.alloc(128);
		sockAddr.writeUInt16LE(wsConsts.AF_INET, 0);
		sockAddr.writeUInt16BE(args[0], 2); //port
		sockAddr[4] = addrParts[0];
		sockAddr[5] = addrParts[1];
		sockAddr[6] = addrParts[2];
		sockAddr[7] = addrParts[3];
		const ret = this.socket.bindIocp(sockAddr);
		console.log('bind ret', ret);
		return ret;
	}
	
	bind(...args) {
		const port = args[0];
		const strAddr = args[1];
		const addrParts = strAddr.split('.').map((v, i, a)=> parseInt(v, 10));
		const sockAddr = Buffer.alloc(128);
		sockAddr.writeUInt16LE(wsConsts.AF_INET, 0);
		sockAddr.writeUInt16BE(args[0], 2); //port
		sockAddr[4] = addrParts[0];
		sockAddr[5] = addrParts[1];
		sockAddr[6] = addrParts[2];
		sockAddr[7] = addrParts[3];
		const ret = this.socket.bindIocp(sockAddr).then((...args)=>{
			this.emit('listening');
		});
		console.log('bind ret', ret);
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