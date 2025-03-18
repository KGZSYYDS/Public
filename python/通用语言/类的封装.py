class person:
    name="kg"
    #私有属性，同一个文件中都可以访问，可以被子类继承，但是在使用from xxx import *时无法导入私有属性
    #（与protected类型不完全相同），一般这种类型是为了与下面的隐藏属性做区分
    _weight=150
    #隐藏属性，类内可以使用而类外无法使用，也无法被继承，使用import导入该文件时不会导入该变量
    #相当于c++中被private定义的私有变量，但是这类变量一般具有特殊含义或功能，不会随便定义
    #在类属性名前加上__，则该变量定义为私有变量，类外无法直接访问
    __age=10

    #隐藏函数同隐藏属性
    def __func1(self):
        print("这是隐藏方法")

    def func2(self):
        print("这是普通函数")
        #类内可以正常访问类的隐藏函数，但是一般不推荐使用，只有在使用这种函数的情况下需要把self作为参数填入
        #注：因为__func1是一个实例隐藏方法，而使用类名的调用是类方法，导致自动传参传的是cls，所以需要手动传self
        person.__func1(self)
        #这种方法一般推荐使用
        self.__func1()

p1=person()
print("p1.name为",p1.name)
print("p1.weight为",p1._weight)
#私有变量外部直接访问报错
try:
    print("p1.age为",p1.age)
except Exception as e:
    print("p1.age无法访问，原因为",e)
#将私有变量带上_类名和__可以在类外访问和修改（名称改编）
print("p1._person__age为",p1._person__age)

try:
    #类外不能直接使用隐藏函数
    print(p1.__func1())
except Exception as e:
    print("p1.func1无法访问，原因为",e)

#普通函数包含隐藏函数时，只需要在普通函数内正确传参
p1.func2()