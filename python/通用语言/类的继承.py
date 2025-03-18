#类的继承，同c++，继承父类的属性和函数
class person:
    def eat(self):
        print("吃饭函数调用中")
    def sleep(self):
        print("睡觉函数调用中")

#单继承，可以兼容多重继承（即孙子类可以通过继承父类而继承爷爷类的对象和方法）
#语法：在类名后面加上（），括号内填父类
class man(person):
    def boki(self):
        print("man中boki函数调用中")
    def dajiao(self):
        print("man中dajiao函数调用中")
    def piao(self):
        print("man中piao函数调用中")
#子类不需要额外代码时，可以使用pass代替代码必要缩进或语法
class girl(person):
    pass

#在继承父类之后具有新的变量或者方法称为派生类
class son(man):
    #函数重写：在子类中重写父类同名函数，子类执行函数时优先执行已重写的函数
    def boki(self):
        print("son中boki函数调用中，boki函数已被重写")

    #函数扩展：
    #方法1，重写函数内加上原来的函数
    def dajiao(self):
        man.dajiao(self)
        print("son中dajiao函数调用中，dajiao函数已被扩展")

    #方法2，利用python中自带的super类调用父类函数（推荐）
    #super()创建了son的父类对象，可以在后面直接使用父类函数
    def piao(self):
        super().piao()
        super().eat()
        print("son中piao函数调用中，piao函数已被扩展")

#子类使用父类函数
m1=man()
m1.eat()
m1.boki()
s1=son()
s1.boki()
s1.dajiao()
s1.piao()



#object类是py中自带的顶级父类，在python3之后的版本中所有类都默认继承，可以使用object类自带的成员函数
class animal(object):
    name="animal"
    def func(self):
        print(f"animal的func函数调用中，name为{animal.name}")

#使用dir函数可以查看object类可使用的函数，包括init函数和del函数
print(dir(object))

class bnimal():
    name="bnimal"
    def func(self):
        print(f"bnimal的func函数调用中，name为{bnimal.name}")

#多继承可以继承两个父类全部的属性跟方法
#写法：把多个父类写进括号里用逗号隔开。（注：当父类有重名函数时，优先执行最前面的，具体看括号内谁排前面）（就近原则）
class newman(bnimal,animal):
    pass

n1=newman()
n1.func()