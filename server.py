import socket

# Inisialisasi socket server
HOST = '127.0.0.1'  # Bind ke alamat IP localhost
PORT = 12345  # Gunakan port yang sama dengan yang digunakan di client.py
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(2)
bool_upload = 0
bool_download = 0
filename = ''
print(f"Server berjalan di {HOST}:{PORT}")

# client_socket, client_address = server_socket.accept()
# print(f"Koneksi diterima dari {client_address}")

# Terima file PDF dari client
while(True):
    client_socket, client_address = server_socket.accept()
    print(f"Koneksi diterima dari {client_address}")
    data = client_socket.recv(1024).decode()
    # pdf_data = bytearray()
    # print(f"Pesan diterima: {data}")
    if(data == "upload"):
        print("File akan disimpan")
        pdf_data = bytearray()
        while True:
            data = client_socket.recv(1024)
            if not data:
                break
            pdf_data += data
            bool_upload = 1
        # # Tulis file PDF ke disk
        filename = 'file_server.pdf'  # Nama file PDF yang ingin Anda simpan
        with open(filename, 'wb') as f:
            f.write(pdf_data)
        print(f"File PDF berhasil diterima dan disimpan ke {filename}")
    elif(data[:8] == "download"):
        filename = data[9:]
        print(data[9:])
        print(f"mencari {filename}")
        # client_socket.send("sending")
        # client_socket.sendall(pdf_data)
        try:
            with open(filename, 'rb') as file:
                data = file.read()
                # print(type(data))
                client_socket.sendall(data)
                print('File', filename, 'sent successfully')
                bool_download = 1
        except FileNotFoundError:
            client_socket.sendall(b'file belum tersedia')
            print("file belum tersedia")
            print("file gagal dikirim")
        # break
    else:
        client_socket.sendall("pesan tidak dikenal".encode())
        print("pesan tidak dikenal")
    
    if(bool_upload and bool_download):
        break

# Tutup koneksi
client_socket.close()
server_socket.close()



# print(f"File PDF berhasil diterima dan disimpan ke {filename}")
# file = open(filename, 'rb')
# pdf_reader = PyPDF2.PdfReader(file)

# # Cetak jumlah halaman PDF
# print('Jumlah halaman:', len(pdf_reader.pages))

# # Dapatkan teks dari halaman pertama PDF
# page = pdf_reader.pages[0]
# text = page.extract_text()

# # Cetak teks dari halaman pertama PDF
# print(text)

# # Tutup file
# file.close()