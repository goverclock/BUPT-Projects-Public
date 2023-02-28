# BUPT-Projects-Public

本人为了学校作业而写的部分代码,隐去了私人信息.

**禁止作为作业参考或者提交其中的代码,否则后果自负.**

---

**[ dnsrelay ](/dnsrelay/)**

> 本科二年级,<<计算机网络课程设计>>题目. 
> 
> 设计并实现一个DNS服务器程序,读入"域名-IP地址"对照表,当客户端查询域名对应的IP地址时,用域名检索该对照表,三种检索结果: 
> 
> - 检索结果为ip地址0.0.0.0,则向客户端返回“域名不存在”的报错消息,而不是返回IP地址为0.0.0.0（不良网站拦截功能）
> - 检索结果为普通IP地址,则向客户返回这个地址（服务器功能）
> - 表中未检到该域名,则向因特网DNS服务器发出查询,并将结果返给客户端（中继功能）
> - 考虑多个计算机上的客户端会同时查询,需要进行消息ID的转换

实现了上述所有功能,同时支持Linux和Windows平台,并且能够很好的应对多个客户端发来相同消息ID的情况.


**[ datalink ](/datalink/)**

> 本科二年级,<<计算机网络>>课程实验. 
> 
> 利用所学数据链路层原理,自己设计一个滑动窗口协议,在仿真环境下编程实现有噪音信道环境下两站点之间无差错双工通信.
> 
> 信道模型为8000bps全双工卫星信道,信道传播时延270毫秒,信道误码率为1e-5,信道提供字节流传输服务,网络层分组长度固定为256字节.

我实现的协议是选择重传(SR),高误码率情况下性能超越参考值,其它情况下略低(3%~5%)于参考值.协议能够长时间稳定运行而无错误.

小组项目,Go-Back-N协议由[zsc-124](https://github.com/zsc-124)实现.

**[ student_assistant ](/student_assistant/)**

> 本科二年级,<<数据结构课程设计>>题目. 
> 
> 设计与实现学生线下课程辅助系统.
> 
> 线下课程辅助系统可以帮助学生管理自己的课程和课外活动,具备课程导航功能、课程信息管理和查询功能,以及课外信息管理和查询功能等.每天晚上系统会提醒学生第二天上的课,每门课需要交的作业和需要带的资料,以及考试的信息；快要上课时系统根据该课程的上课地点设计一条最佳线路并输出；学生可以通过系统管理每门课的学习资料、作业和考试信息;在课外,学生可以管理自己的个人活动和集体活动信息,可以进行活动时间的冲突检测和闹钟提醒.

使用Qt实现了带GUI的学生辅助系统,满足所有的主要功能需求,贡献了主要的代码部分,并负责合并代码等项目管理工作.

小组项目,部分代码由[zsc-124](https://github.com/zsc-124)贡献,[lilinton](https://github.com/lilinton)提供了测试数据.


**[ express_manager ](/express_manager/)**

> 本科二年级,<<面向对象程序设计(C++)>>课程实验.
> 
> 使用C++语言,基于面向对象的程序设计方法,设计并实现一个简单的物流管理平台,提供物流管理,用户管理,员工管理等功能.

CS架构,多进程.
