/**
 * Jednoduchý UDP server pro testování DNS komunikace
 */
"use strict";

const dgram = require(`dgram`);
const UDP_PORT = 32323;

module.exports = class UdpServer{
	constructor(){
		this.response = Buffer.alloc(0);
		this.request = Buffer.alloc(0);
		this.port = 0;
		
		this.server = dgram.createSocket(`udp4`);
		this.server.on("message", (msg, remote) => {
			if(msg.length >= 2);
			this.request = msg;

			this.server.send(Buffer.concat([msg.slice(0, 2), this.response]), remote.port, remote.address);
		});

		this.server.on("error", (err) => {
			console.error(err);
		});
	}

	async Bind(port = UDP_PORT){
		this.port = port;
		return new Promise((res) => {
			this.server.bind(port, "127.0.0.1", () => res());
		})
	}

	Close(){
		this.server.close();
	}
}
