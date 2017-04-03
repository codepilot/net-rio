'use strict';

const child_process = require('child_process');

const Tracers = {};
module.exports = Tracers;

Tracers.StreamReadableTracer = class StreamReadableTracer {
	constructor(options = {prefix: 'process.stdin', emitter: process.stdin, encoding: null} ) {
		this.options = options;
		this.options.emitter.setEncoding(this.options.encoding);

		this.options.emitter.on('close', this.on_close.bind(this));
		this.options.emitter.on('data', this.on_data.bind(this));
		this.options.emitter.on('end', this.on_end.bind(this));
		this.options.emitter.on('on_error', this.on_error.bind(this));
		this.options.emitter.on('readable', this.on_readable.bind(this));
	}

	on_close() {
		console.log(`${this.options.prefix}.on_close: `);
	}

	on_data(chunk) {
		if(chunk instanceof Buffer) {
			process.stdout.write(`${this.options.prefix}.on_data: `);
			console.log(chunk);
		} else {
			process.stdout.write(`${this.options.prefix}.on_data: ${chunk}`);
		}
	}

	on_end() {
		console.log(`${this.options.prefix}.on_end: `);
	}

	on_error(err) {
		console.log(`${this.options.prefix}.on_err: `, err);
	}

	on_readable() {
		console.log(`${this.options.prefix}.on_readable: `);
	}

}


Tracers.DgramSocketTracer = class DgramSocketTracer {
	constructor(options = {prefix: 'process.stdin', emitter: process.stdin} ) {
		this.options = options;

		this.options.emitter.on('close', this.on_close.bind(this));
		this.options.emitter.on('message', this.on_message.bind(this));
		this.options.emitter.on('listening', this.on_listening.bind(this));
		this.options.emitter.on('error', this.on_error.bind(this));
	}

	on_close() {
		console.log(`${this.options.prefix}.on_close: `);
	}

	on_message(msg, rinfo) {
		process.stdout.write(`${this.options.prefix}.on_message: `);
		console.log(msg);
		console.log(rinfo);
	}

	on_error(err) {
		console.log(`${this.options.prefix}.on_err: `, err);
	}

	on_listening() {
		console.log(`${this.options.prefix}.on_listening: `);
	}

}


Tracers.StreamWritableTracer = class StreamWritableTracer {
	constructor(options = {prefix: 'process.stdin', emitter: process.stdin, encoding: null} ) {
		this.options = options;
		this.options.emitter.setDefaultEncoding(this.options.encoding);

		this.options.emitter.on('close', this.on_close.bind(this));
		this.options.emitter.on('drain', this.on_drain.bind(this));
		this.options.emitter.on('finish', this.on_finish.bind(this));
		this.options.emitter.on('error', this.on_error.bind(this));
	}

	on_close() {
		console.log(`${this.options.prefix}.on_close: `);
	}

	on_drain() {
		console.log(`${this.options.prefix}.on_drain: `);
	}

	on_finish() {
		console.log(`${this.options.prefix}.on_finish: `);
	}

	on_error(err) {
		console.log(`${this.options.prefix}.on_err: `, err);
	}

}




Tracers.ChildProcessTracer = class ChildProcessTracer {
	constructor(options = {prefix: 'process.stdin', emitter: process.stdin, encoding: null} ) {
		this.options = options;

		this.options.emitter.on('close', this.on_close.bind(this));
		this.options.emitter.on('disconnect', this.on_disconnect.bind(this));
		this.options.emitter.on('error', this.on_error.bind(this));
		this.options.emitter.on('exit', this.on_exit.bind(this));
		this.options.emitter.on('message', this.on_message.bind(this));

		new Tracers.StreamWritableTracer({ prefix: options.prefix + '.stdin', emitter: this.options.emitter.stdin, encoding: 'utf8' });
		new Tracers.StreamReadableTracer({ prefix: options.prefix + '.stdout', emitter: this.options.emitter.stdout, encoding: 'utf8' });
		new Tracers.StreamReadableTracer({ prefix: options.prefix + '.stderr', emitter: this.options.emitter.stderr, encoding: 'utf8' });

	}

	on_close(code, signal) {
		console.log(`${this.options.prefix}.on_close: `, code, signal);
	}

	on_disconnect() {
		console.log(`${this.options.prefix}.on_disconnect: `);
	}

	on_error(err) {
		console.log(`${this.options.prefix}.on_err: `, err);
	}

	on_exit(code, signal) {
		console.log(`${this.options.prefix}.on_exit: `, code, signal);
	}

	on_message(message, sendHandle) {
		console.log(`${this.options.prefix}.on_message: `, message, sendHandle);
	}


}


Tracers.ForkTracer = function ForkTracer(path, prefix) {
	const cp = child_process.fork(path, [], { stdio: ['pipe', 'pipe', 'pipe', 'ipc'] });
	new Tracers.ChildProcessTracer({ prefix: prefix, emitter: cp });
	return cp;
}
