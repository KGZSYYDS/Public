#这是一个包的标识文件，有这个文件代表这个文件夹（该文件夹此次命名为test）是一个包，可以导入其他模块到这个文件夹底下
#包的作用是将各种模块或者子包打包放在一起，便于管理
print("这是test包下的_init.py文件")

#有以下两种语句，可以使文件导入包时能够使用import*来导入mokuai1，但是两种写法仅引用一个文件时只能选一种，
#不然另一个方式的引用会报错
#写法一
from test import test_mokuai2,test_mokuai3
#写法二
__all__=['test_mokuai2','test_mokuai3']