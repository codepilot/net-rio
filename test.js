'use strict';

const netRio = require('net-rio');
const Tracers = require('./Tracers.js');

const cp_dgramServer = Tracers.ForkTracer('./dgramServer.js', 'dgramServer');
const cp_dgramClient = Tracers.ForkTracer('./dgramClient.js', 'dgramClient');
