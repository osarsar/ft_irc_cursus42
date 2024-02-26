import socket


client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('10.13.1.4', 12345)
client_socket.connect(server_address)

while True:
    data = input()
    client_socket.send(data.encode())
    recv,  = client_socket.recvfrom(1024).decode()
    print(recv)