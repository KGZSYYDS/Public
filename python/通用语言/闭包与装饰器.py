#a本质上是对一个储存位置的指针，指向一个储存特定值的地址，修改a是创建一个新的值和地址，指向新的值所在的位置
#并且这个值是公用的，可以同时成为多个指针指向的对象
a=10
b=10
print("a=",a,"id(a)=",id(a),"id(10)=",id(10))
a=20
print("更改后a=",a,"id(a)=",id(a),"id(10)=",id(10),"id(20)=",id(20))

#闭包
#闭包是一种特殊的函数结构，满足以下三个条件即为闭包
#1，有至少2层的嵌套函数
#2，内层函数使用了外层函数的变量
#3，外层函数返回的是内层函数的函数名（一般不加括号）
#满足这三种条件的函数叫做闭包，闭包中内函数使用的变量不会随着函数结束而销毁，即内函数的引用同时保存了他自己和外层的变量
#函数名本身是一个指针（引用），加上（）才是调用

def outter(x):
    y=10
    def inner(z):
        print("x=",x,"y=",y,"z=",z,"inner结果为",x+y+z)
    return inner

#使用闭包有两种方式
#第一种为类似解类的方式，outter（）结束后返回的是inner这个名称，所以加（）可以直接执行inner函数
outter(10)(20)
#第二种是先用一个变量储存outter的返回（即inner），再继续调用inner，道理相同
#在用变量接收outter之后，outter及所使用的x不会销毁，同一个outter变量的x值是一样的
print("这是ot1")
ot1=outter(1)
ot2=outter(10)
ot1(2)
ot1(3)
print("这是ot2")
ot2(2)
ot2(3)

#装饰器
#装饰器本质是一个闭包，它可以在不更改原函数代码的基础上添加功能，并且可以根据不同情况调参数实现不同的功能

#标准装饰器
#以下是两个补丁新函数
def func1(x,y):
    print("func1调用中，x=",x,"y=",y)
def func2(x,y):
    print("func2调用中，x=",x,"y",y)

#这个函数中funin用于调用被装饰函数，之后的使用中只需要在新函数的基础上修改即可
def funa(fun,x):
    print("funa内x=",x)
    def funin(y):
        fun(x,y)
    return funin

fu1=funa(func1,10)
fu1(1)
fu1(11)
fu2=funa(func2,20)
fu2(2)
fu2(22)

#语法糖：
#在被装饰函数的头顶加上@装饰器名称（不加括号，返回函数名），可以自动将被装饰函数添加进装饰器里面，
#相当于外层函数的参数已经被fn占掉了，后续调用fun3和fun4的时候加（）其实是给inner传参
#此处新函数相当于是外函数的参数fn，inner里面的fn传参用法跟被装饰函数（fun3和fun4）相同
#注意，使用语法糖的时候外函数不能有除了fn以外的别的参数
def funb(fn):
    def inner(x):
        print("内函数调用中，参数为",x)
        fn()
    return inner


@funb
def func3():
    print("func3调用中")

@funb
def func4():
    print("func4调用中")

func3(10)
func4(11)

#多级语法糖嵌套时，优先内层的语法糖返回值先执行，并且将内层语法糖打包成一个新函数作为外层语法糖的fn
#最终的返回值是inner的return，但是外层的其他语句会先执行
def funoutter(fn):
    def inner():
        print("funoutter的inner函数执行中")
        return "funo "+fn()+" outter"
    return inner

def funinner(fn):
    def inner():
        print("funinner的inner函数执行中")
        return "funi "+fn()+" inner"
    return inner

@funoutter
@funinner
def func5():
    print("func5执行中")
    return "func5"

print(func5())

