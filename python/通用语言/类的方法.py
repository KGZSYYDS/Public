class example1:

    name="kg"

    #python中分为三种方法：静态方法，类方法，实例方法。
    #类方法在调用时自动将cls作为参数传递，实例方法在调用时自动将self作为参数传递
    #使用时一般不会在静态方法内写类属性和实例属性，因为静态方法在调用的时候不会自动传cls和self，但是硬要写也不会报错
    #同理类方法一般也不会使用实例属性，需要在类方法里面使用实例属性需要手动传self作为参数
    #在类名直接调用实例属性形如 example.shili_fangfa()时，同样需要手动传self（例见类的封装）

    #注意：构造函数中的参数是实例参数，即使跟类属性同名也是两个东西
    def __init__(self,namex):
        self.name=namex

    #静态函数：在函数体上面加上一个@staticmethod字段形成语法糖
    #静态函数不需要传self参数和cls参数，可以看成是一个定义在类里面的全局函数，更省空间，一般不访问实例属性和类属性
    @staticmethod
    def static_func(name):
        return f"静态函数调用中，参数为{name}"

    #类方法：无需创建实例也可以使用的方法，默认传参cls，代表类本身（self代表实例本身）
    #一般配合类属性（或私有属性）使用，一般不访问实例属性
    @classmethod
    def class_func(cls):
        print("类方法调用中，cls.name为",cls.name)

    #实例方法没有特殊字段，默认传参为self自身实例，一般用于调用实例方法
    def func(self):
        print("实例方法调用中，self.name为",self.name)


e1=example1("yyds")
#静态函数的调用可以直接用类名后面跟函数（即使没有具体实例化也可以使用），也可以实例化之后调用
print(example1.static_func("kg"))
print(e1.static_func("zs"))
example1.class_func()
e1.func()