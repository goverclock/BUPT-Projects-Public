#!env python3
import socket
import random
import hashlib

# setup server
HOST = "127.0.0.1"
PORT = 5678

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((HOST, PORT))
sock.listen()
print("Server running")


def send(conn, data):
    data = str.encode(data) + b"\0" * (1024 - len(data))
    conn.sendall(data)
    print('(server) sent data "' + data.decode() + '"')


def read(conn):
    ret = conn.recv(1024).rstrip(b"\0").decode("utf-8")
    print('(server) received data "' + ret + '"')
    return ret


def simple_hash(s):
    s = "randomchar".join(s)
    return hashlib.sha256(str.encode(s)).hexdigest()[:16]


# 预先设置好的用户
db = {
    "Alice": simple_hash("bestpasswordever"),
    "Bob": simple_hash("c$!5ZxB8UPyMBf"),
}  # user name - hash(password)


def resolve(conn, data):
    sp = data.strip().split(" ")
    if sp[0] == "wantlogintoken":
        username = sp[1]
        token = simple_hash(str(random.random()))
        send(conn, token)
        result = read(conn)
        if simple_hash([db[username], token]) == result:
            send(conn, "login successfully")
        else:
            send(conn, "login failed")
    elif sp[0] == "wantchangetoken":
        username = sp[1]
        token = simple_hash(str(random.random()))
        send(conn, token)
        result = read(conn)
        if simple_hash([db[username], token]) == result:
            db[username] = sp[2]
            send(conn, "change successfully")
        else:
            send(conn, "change failed")


while True:
    conn, addr = sock.accept()
    with conn:
        while True:
            data = read(conn)
            if not data:
                break
            resolve(conn, data)
