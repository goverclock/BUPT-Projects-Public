#!env python3
import socket
import hashlib

# setup client
usage = """\
Usage:
    login <user name> <password>
    change <user name> <old password> <new password>\
"""
HOST = "127.0.0.1"
PORT = 5678

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))
print("Client running")


def send(data):
    data = str.encode(data) + b"\0" * (1024 - len(data))
    sock.sendall(data)


def read():
    return sock.recv(1024).rstrip(b"\0").decode("utf-8")


def simple_hash(s):
    s = "randomchar".join(s)
    return hashlib.sha256(str.encode(s)).hexdigest()[:16]


# 用户输入处理
def resolve(line):
    sp = line.strip().split(" ")
    if len(sp) != 3 and len(sp) != 4:
        print(usage)
        return
    if sp[0] == "login" and len(sp) == 3:  # 用户尝试登陆
        sp[2] = simple_hash(sp[2])  # 登录密码的哈希值
        # 向服务器请求一次性令牌
        send(" ".join(["wantlogintoken", sp[1]]))
        token = read()
        # 使用令牌对登录密码进行加密
        result = simple_hash([sp[2], token])
        send(result)
        # 服务器返回登陆成功/失败
        print(read())
    elif sp[0] == "change" and len(sp) == 4:  # 用户尝试修改密码
        sp[2] = simple_hash(sp[2])  # 原密码的哈希值
        sp[3] = simple_hash(sp[3])  # 新密码的哈希值
        # 向服务器请求一次性令牌
        send(" ".join(["wantchangetoken", sp[1], sp[3]]))
        token = read()
        # 使用令牌对原密码进行加密
        result = simple_hash([sp[2], token])
        send(result)
        # 服务器返回修改成功/失败
        print(read())
    else:
        print(usage)


print(usage)
while True:
    line = input("(client)$ ")
    resolve(line)
