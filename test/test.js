#!/usr/bin/env node
/**
 * Testovací skript DNS Resolveru
 * 
 * Pro funkčnost je třeba mít na systému nainstalovanou rozumně novou verzi node.js
 */
//"use strict";


const path = require(`path`);
const child_process = require(`child_process`);
const UdpServer = require("./udp_server");
const write = (...args) => process.stdout.write(...args);

const communicationTests = require("./communication_tests");
const shouldFailArgs = require("./argumentfail_tests");

const termclr = {
	HEADER	: '\033[95m',
	GRAY	: '\033[30;1m',
	OKBLUE	: '\033[94m',
	PASS	: '\033[92m',
	WARNING	: '\033[93m',
	FAIL	: '\033[91m',
	RESET	: '\033[0m',
	BOLD	: '\033[1m'
}

/** Cesta ke spustitelnému souboru DNS resolveru */
const DNSRES_BIN = path.resolve(__dirname, `..`, `dns`);

function TestWrongArgs(){
	write(`${termclr.HEADER}Argument testing (program should exit with return 1)${termclr.RESET}\n`);
	
	let passed = 0;
	for(const args of shouldFailArgs){
		write(`\nRunning with ${JSON.stringify(args)}\n${termclr.GRAY}`);
		let status = 0;
		try {
			child_process.execFileSync(DNSRES_BIN, args);
		} catch (error) {
			status = error.status;
		}
		
		write(`${termclr.RESET}Exit code: ${status} ... ${termclr.BOLD}`);
		if(status === 1){
			write(`${termclr.PASS}PASS${termclr.RESET}\n`);
			passed++;
		}
		else write(`${termclr.FAIL}FAIL${termclr.RESET}\n`);
	}
	
	write(passed == shouldFailArgs.length ? termclr.PASS : termclr.FAIL);
	write(`\nArgument testing complete! Passed ${passed}/${shouldFailArgs.length}${termclr.RESET}\n\n`);

	return passed;
}

async function RunDnsResolver(args){
	return new Promise((res) => {
		try{
			proc = child_process.execFile(
				DNSRES_BIN, args,
				(err, sout, serr) => res({
					error: err ? err : {code: 0},
					stdout: sout,
					stderr: serr
				}));
		}catch(e){
			console.error(e);
		}
	});
}

async function TestCommunication(){
	write(`${termclr.HEADER}-------------------------------------------------\n`);
	write(`Communication testing (using a simple UDP server)\n`);
	let server = new UdpServer();
	await server.Bind();
	write(`${termclr.GRAY}UDP server listening locally on port ${server.port}${termclr.RESET}\n\n`);

	let passed = 0;
	for(const test of communicationTests){
		let pass = true;
		let modargs = [`-slocalhost`, `-p${server.port}`, ...test.args];
		write(`Running with ${JSON.stringify(modargs)}\n${termclr.GRAY}`);
		server.response = Buffer.from(new Uint8Array(test.serverResponse).buffer);
		
		let proc = await RunDnsResolver(modargs);
		write(proc.stderr);

		write(`${termclr.RESET}Exit code: ${proc.error.code} ... ${termclr.BOLD}`);
		if(proc.error.code == 0) write(`${termclr.PASS}PASS\n`);
		else{
			write(`${termclr.FAIL}FAIL\n\n${termclr.RESET}`);
			break;
		}

		/** @type {string} */
		let debugData = proc.stdout.split("==DEBUG==")[1];
		let outRequest = debugData.split("REQUEST")[1].trim();
		let outResponse = debugData.split("RESPONSE")[1].trim();
		let outResponseConvBack = debugData.split("RESPONSE_CONV_BACK")[1].trim();

		let serverRequest = "";
		server.request.forEach(byte => {
			serverRequest += `${byte.toString(16).padStart(2, `0`)} `;
		});

		let serverResponse = "";
		server.response.forEach(byte => {
			serverResponse += `${byte.toString(16).padStart(2, `0`)} `;
		});

		let expectedRequest = "";
		test.expectedRequest.forEach(byte => {
			expectedRequest += `${byte.toString(16).padStart(2, `0`)} `;
		});

		write(`${termclr.RESET}Request is correct ... ${termclr.BOLD}`);
		if(serverRequest.substr(6) === expectedRequest) write(`${termclr.PASS}PASS\n`);
		else{
			write(`${termclr.FAIL}FAIL\n`);
			pass = false;
		}

		write(`${termclr.RESET}Request server match ... ${termclr.BOLD}`);
		if(outRequest.indexOf(serverRequest) === 0) write(`${termclr.PASS}PASS\n`);
		else{
			write(`${termclr.FAIL}FAIL\n`);
			pass = false;
		}

		write(`${termclr.RESET}Response server match ... ${termclr.BOLD}`);
		if(outResponseConvBack.substr(6) === serverResponse.trim()) write(`${termclr.PASS}PASS\n`);
		else{
			write(`${termclr.FAIL}FAIL\n`);
			pass = false;
		}
		
		write(`${termclr.RESET}Response parsing ... ${termclr.BOLD}`);
		if(outResponse.indexOf(outResponseConvBack) === 0) write(`${termclr.PASS}PASS\n`);
		else{
			write(`${termclr.FAIL}FAIL\n`);
			pass = false;
		}
		
		write(`\n${termclr.RESET}`);

		if(pass) passed++;
	}

	write(`${termclr.GRAY}Stopping UDP server...\n`);
	server.Close();
	write(passed == communicationTests.length ? termclr.PASS : termclr.FAIL);
	write(`Communication testing complete! Passed ${passed}/${communicationTests.length}${termclr.RESET}\n\n`);

	return passed;
}

async function RunAllTests(){
	let passed = TestWrongArgs();
	passed += await TestCommunication();

	write(`${termclr.HEADER}+-----------------------------------------------+\n`);
	write(`| ${termclr.BOLD}Testing complete!\t`);
	if(passed < shouldFailArgs.length + communicationTests.length)
	write(`${termclr.FAIL}Some tests failed 😥`);
	else 
	write(`${termclr.PASS}All tests passed 🥂`);
	
	write(`${termclr.RESET}\t${termclr.HEADER}|\n`)
	write(`+-----------------------------------------------+\n${termclr.RESET}`);
}

RunAllTests();
