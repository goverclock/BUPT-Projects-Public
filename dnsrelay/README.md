# DNS中继服务器


Linu下修改DNS服务器的方法:

sudo vim /etc/resolv.conf

建议使用的DNS: 114.114.114.114

观察到qtype=28,在RFC1035中没有解释,查找资料后:
AAAA	28	RFC 3596	IPv6地址记录	传回一个128位的IPv6地址，最常用于映射主机名称到IP地址。

如何查看CNAME?使用`host -t cname gaomez.top`

TODO: malloc的内存忘了回收
