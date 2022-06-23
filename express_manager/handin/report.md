# 物流管理系统设计与实现

### 总体设计



### 关键类设计



### 重要问题&解决方案



### 心得体会

(想法,经验,教训)



### 题目一:物流业务管理子系统和用户管理系统子系统

功能需求:

1. ##### 用户注册登录

(guest)reg \<admin/user\>

nick name:

password:

repeat password:

real name:

(仅user)tele:

(仅user)address:

> 此外均有属性balance=0

(用户名$)log \<用户名\> 

password for 用户名:

(用户名$)logout - 退出登录

2. ##### 修改帐户密码

(用户名$)passwd <当前密码> <新密码>

3. ##### 余额查询

(用户名$)balance

(用户名$)balance charge <充值数>

4. ##### 发送快递

(用户名$)exp send

to user:

description:

5. ##### 接受快递

需要先查询快递得知快递编号



6. ##### 查询快递

(用户名$)exp ls

ID	| from	| to	| send time | status

1 		YOU		...		2020.1.1

...

(用户名$)exp ls ID/send time/from user <关键字>

...

7. ##### 物流业务管理

物流公司管理员可查看所有用户信息和所有的<u>历史快递</u>的详细信息,可根据用户,时间,快递单号进行查询.





- 快递信息

每个快递存放于一个单独文件中,这个文件位于${cwd}/express文件夹中.

快递信息文件(文件名:快递单号)格式:

```
寄件用户
收件用户
状态(0=待签收,1=已签收)
寄送时间
接收时间
物品描述
```



- 用户信息

每个用户(包括管理员)信息存放于一个单独文件中,这个文件位于${cwd}/user文件夹中.

用户信息文件(文件名:nickname)格式:

```
用户类型type=admin(0)/user(1)
密码
真实姓名realname
余额balance
--------类型特定-----------
1.user
电话tele
地址address
2.admin
公司company
```



### 题目二:快递员任务管理子系统

courier

1. 添加一条管理员独占指令,可管理快递员帐户

(admin#)coumgr rm \<courier nick\>

这将删除一个快递员帐户


2. 快递Express类构造函数内询问快递类型
3. 新增待揽收状态,需要修改发送快递(exp send)和接收快递(exp accept)
4. 管理员独占指令,分配快递 *注意,已分配和已揽收是两个状态.快递员帐户独占功能:揽收一个或多个
5. 已实现
6. 管理员分配快递,快递员揽收快递

(admin#)exp assign \<express ID\> \<courier nick\>

(courier$)exp dlry \<express ID\> 

注意:admin使用exp ls时无条件查看所有快递

courier使用exp ls时额外显示被分配给自己的快递(status: "pending delivery(YOU)")

7. 已实现(filter)

更改快递文件格式:

```
快递类型
快递重量
寄件用户
收件用户
状态(0=待签收,1=已签收)
寄送时间
接收时间
物品描述
```



### 题目三:物流管理系统(网络版)


TODO: 添加read_from_file()函数,每当用户查看自己的数据时必须先调用