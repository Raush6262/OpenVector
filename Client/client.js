const net = require('net');

console.log("Starting Node.js Client...");

const client = new net.Socket();

client.connect(8080, '127.0.0.1', () => {
    console.log('✅ Connected to C++ COT Server!');
});

client.on('data', (data) => {
    console.log('\n🎉 SERVER RESPONSE:');
    console.log(data.toString());
    client.destroy(); // Close connection after receiving the message
});

client.on('error', (err) => {
    console.log('❌ Connection Error: ' + err.message);
});

client.on('close', () => {
    console.log('Connection closed. YOU ARE DONE!');
});