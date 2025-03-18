#try......except语法：在不确定代码段哪里有问题时，可以使用这个语句，规避报错
#try语句缩进下表示不确定是否正确的代码，在这段代码报错时会不执行错误的代码以及后续代码，然后执行except下的语句
#except语句缩进下表示出bug时的处理代码，如果try不报错则不执行
from logging import exception

a=10
b=20
print("初始a=",a,"b=",b,"try内修改a的值，except内修改b的值")
try:
    a=100
    buy(a)
    print("try语句内a，b为",a,b)
except:
    b=40
    print("a=",a,"b=",b)

#这段语句可以指定在特定报错条件下才执行except，并且可以输出错误原因，此处取错误原因为别名e
#except兼容多分支，可以匹配多种错误原因，也可以直接使用Exception关键字来包含所有错误原因来输出错误原因
try:
    print(x)
except NameError as e:
    print("错误原因为",e)
except IndexError as e:
    print("错误原因为",e)
#这个语句可以捕获所有的错误类型
except Exception as e:
    print("错误原因为", e)

#try......except......else语句：else缩进下的代码段只有在try没有报错是才会触发，走except不触发
try:
    print("try语句正常执行中")
except:
    print("except语句正常执行中")
else:
    print("else语句正常执行中，代表try内没有报错")

#try......except......finally语句：finally语句是在try或者except语句执行后必定执行的代码段
#有finally时可以没有except，也可以没有else，无论try是否报错finally都会执行
try:
    print("try语句正常执行中")
    bought(la)
except Exception as e:
    print("except语句正常执行中，错误原因为",e)
else:
    print("else语句正常执行中，代表try内没有报错")
finally:
    print("finally语句正常执行中")

#在日常使用中，会使用一个函数包含可能出bug的代码，使代码段简洁，try语句可以识别函数内报错
def func():
    return int(input("请输入一个数"))

try:
    print("这个数/10=",func()/10)
except:
    print("请输入正确的数")

#抛出异常：通过raise语句来特别指定一种异常，使在特定情况下能嵌合try——except语句，可视作特殊的if——else
try:
    m=int(input("请输入一个纯数字密码"))
    if(len(m)<6):
        raise "请输入长度大于6的纯数字密码"
except Exception as e:
    print("密码输入失败，原因为",e)

