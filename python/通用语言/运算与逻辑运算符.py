#python单斜杠不是整除，会发生隐式类型转换
print("10/3=",10/3)
#python中//表示整除
print("10整除3=",10//3)
#**表示乘方，运算优先级最高（排在*和/前面）
print("2的3次方=",2**3)

a=10
b=20
print("a=",a,"  b=",b)
#python可以直接交换
a,b=b,a
print("交换后a=",a,"  b=",b)

#并且、或者的表达方式为
print("3>1 and 2<9结果为",3>1 and 2<9)
print("7<9 or 1>9结果为",7<9 or 1>9)
#python中不等于仍为！=，但对结果取反为not运算符
print("not(8!=7)结果为",not(8!=7))