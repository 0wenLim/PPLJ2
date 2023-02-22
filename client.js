const net = require('net');
const fs = require('fs');

const HOST = '127.0.0.1';
const PORT = 12345;
const FILENAME = 'file_final.pdf';

const client = new net.Socket();
let timeout;
const prompt = require('prompt-sync')();

client.connect(PORT, HOST, () => {
  console.log('Connected to server.');

  // Kirim pesan "download" ke server
  const command = prompt('What is your command?');
  client.write(command);
});

// Tangani data yang diterima dari server
client.on('data', (data) => {
//   console.log(data.toString())
  if (data.toString().includes('belum')) {
    console.log(data.toString());
    process.exit();
  }

  // Tulis data ke file PDF
  fs.appendFileSync(FILENAME, data);

  console.log('Data received:', data.length, 'bytes.');
  
  // Tutup koneksi setelah selesai menerima file
  
  // Set the new timer
  clearTimeout(timeout);
  timeout = setTimeout(function() {
    console.log('No data received from server for 2.5 seconds. Closing the connection.');
    process.exit();
  }, 2500); // 10 seconds timeout
});

client.on('end', () => {
    console.log('Disconnected from server');
});