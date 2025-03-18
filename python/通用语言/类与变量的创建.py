#注：py中类成员对象必须定义一个具体的数值才能使用，仅有变量不给值无法判断数据类型
class person:
    height=10
    weight=20

    # __new__函数是py中object（顶级父类）的静态函数，先于__init__函数调用，用于分配内存
    #并且在执行后会将自己返回传递给__init__函数，所以此处的扩展函数需要将父类函数作为返回值返回
    #此处需要传cls作为参数的原因是：object中__new__函数是一个静态函数，不会自动传cls参数，需要手动传参
    def __new__(cls, *args, **kwargs):
        print("__new__函数已扩展，实例函数__new__调用中")
        res=super().__new__(cls)
        return res

    #py中构造函数的写法如下，该函数在实例化对象时会自动调用
    #self储存的是该类的地址，在实例化对象时会单独分配一个给这个实例，代表自身
    #使用成员变量时，即使是类内使用也要带上self.或者类名.，其中self.引用的变量都是实例对象，不能被person.调用，
    #没有self时这个变量只是一个局部变量
    #使用构造函数传参的方式构造实例对象可以使每一个对象带上不同值的同类属性
    def __init__(self,x):
        self.age=1
        self.money=x

    #类中的成员函数必须包含一个self的参数，无论是否用到，称为实例方法
    def func1(self):
        print("该对象height为",self.height)

    #析构函数写法如下，该实例被释放或者到代码结尾时会自动执行
    def __del__(self):
        print("析构函数调用中")

#py中由于没有类型定义法，所以实例化对象时必须使用构造函数定义
p1=person(12)
p2=person(100)
#py中的成员变量分为类属性和实例属性，都可以在类外更改
#其中类属性为全局变量，一个改了全都会变；实例属性可以在类外进行定义和修改，但是不能被其他类成员访问
p1.name="kg"
p2.name="zs"

print("p1.name为",p1.name,"p2.name为",p2.name)
print("p1.height为",p1.height,"person.height为",person.height)
#这里相当于定义了一个新的实例属性，所以后续改变类属性不会对p1的实例属性做出更改
p1.height=11
print("更改p1.height后为",p1.height,"person.height为",person.height,"p2.height为",p2.height)
person.height=111
print("更改person.height后p1.height为",p1.height,"person.height为",person.height,"p2.height为",p2.height)

print("p1.money为",p1.money,"p2.money为",p2.money)
p1.func1()
#即使所有实例都带有一个同名实例属性，但是person仍然不能调用
try:
    print(person.money)
except:
    print("person.money无法调用")

#del函数是py中的释放函数
del p2
print("p2已经被释放")

