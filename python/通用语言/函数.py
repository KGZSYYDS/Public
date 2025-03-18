#python函数返回多个参数在返回值后面加“，”就行，以元组的形式返回，可迭代对象
def func1():
    return "123",1

print(func1())

#形参不用加数据类型，编译器会自动转化，默认参数的用法和c++一样
def func2(name1,name2='kg'):
    print("实参1",name1)
    print("默认参数",name2)

func2("kgg")

#python特有的可变参数，前面必须加“*”，后面有一个参数，起名随意，一般写args。
#这种写法可以将传入的所有参数以元组的形式返回，并且可以传的个数不限（不给参数或者给多个参数均可）
def func3(*args):
    print("可变函数调用中，参数为",args)

func3()
func3(1)
func3("参数1","参数2","参数3","参数4")

#关键字参数，与可变参数类似，也可以传多个参数，但需要以成对的形式传入，以字典的形式返回
def func4(**kwargs):
    print("关键字函数调用中，参数为",kwargs)

#以赋值的方式传入，此处name和shuliang是一个变量，传到函数里面之后成为键名，等号右边是值
func4(name="kg",shuliang=20)

#全局变量跟局部变量重名时优先调用局部变量，且互相不覆盖
#注：由于python不需要在定义时添加数据类型，所以“canshu=10”这个操作不是修改全局变量，而是定义了一个新变量
#所以全局变量的使用不像c++一样可以在外部定义之后在函数里面直接用，只有在该函数内无重名变量时才可以直接用全局变量
canshu=1
def func5():
    canshu=10
    print("局部变量canshu=",canshu)
func5()
print("全局变量canshu=",canshu)

#添加global关键字之后将会定义一个新的名为canshu的全局变量，地址不同，这个canshu在函数外也可以使用，已经是一个全局变量了
def func6():
    global canshu
    canshu=100
func6()
print("修改全局变量后canshu=",canshu)

#匿名函数：一种简便的函数写法，也可以把参数写成*或者**作为不定变量的函数
#格式： 函数名=lambda 形参1，形参2（可以有默认值）.......：返回值（可以是一个表达式）
#这个式子的意思是，func7函数的参数有两个，返回这两个参数相加的结果
func7=lambda x1,x2:x1+x2
print("参数1为2，参数2为3，func7结果为",func7(2,3))

#匿名函数带简单判断的写法，同if的简单语句
#格式：函数名=lambda 形参1，形参2......：条件1下的返回值或表达式 if（条件）【else 其他条件下的表达式或返回值】
func8=lambda a,b:print("a=",a,"b=",b,"a>b") if a>b else print("a=",a,"b=",b,"a<b")
func8(1,2)

#函数可以将另一个函数的名称作为参数，并在后续的代码中调用
def func9(fun):
    print("func9调用中，调用了其他函数")
    fun()

func9(func3)

