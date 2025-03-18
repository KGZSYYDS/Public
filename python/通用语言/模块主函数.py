#import相当于#include头文件，可以使用模块包里的函数和已经命名的变量，要带上文件名.变量/函数
#可以直接给文件起别名，注意，起别名之后原名就不能用了，但是在另一个地方再次使用import时仍需使用原名
import 模块包 as mo
print("模块包函数中name为",mo.name)
mo.funca()

#from 模块名 import 功能 相当于using 库::命名空间，用这种方法可以具体导入某个函数，之后使用不用加模块名
#给某个函数起别名也是可以的
from 模块包 import funcb as b
b()

#from 模块名 import * 相当于导入所有命名空间，使用这个操作之后所有的函数不需要加模块名也可以使用
from 模块包 import  *
funcc()
funcd()

#自编一个包文件，优先执行这个包的_init_文件中的代码
import test
#从包里面导入这个文件夹下的模块，这个.py文件里的函数就可以被使用，不导入到文件，该文件内的函数无法使用
from test import test_mokuai1
test_mokuai1.func1()

#如果想使用import *，必须保证_init_文件下包含对该模块的导入或者all列表对该模块的包含
from test import *
test_mokuai2.func1()
test_mokuai3.func1()