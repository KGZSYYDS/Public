#sum求和函数
#sum的参数可以是多个不同的变量，也可以是可迭代对象，可以将参数全部累加求和，支持浮点数，但不支持字符串相加
l1=[1,2,3.14,4,5.20,0,-10]
print("l1为",l1,"l1求和为",sum(l1))

#min与max函数
#py里支持多个值比较，也可以接收可迭代对象
print("l1中的最大值为",max(l1),"最小值为",min(l1),"加入abs关键字后最大值为",max(l1,key=abs),"最小值为",min(l1,key=abs))

#zip函数
#zip函数的参数是两个可迭代对象，会将两个可迭代对象中的所有元素一一对应然后以多个元组的形式返回，这个返回值也是可迭代对象
#如果两个长度不同，会按短的那个返回
l2=[1,2,3]
l3=[4,5,6,7]
print("l2为",l2,"l3为",l3)
print("l2和l3打包之后输出结果为")
for i in zip(l2,l3):
    print(i)

#map函数
#map函数可以将可迭代对象中的每一个元素都放到自定义的函数或者内置函数里面然后执行，并以一个特殊的数据类型打包返回（可迭代对象）
#注：map不会执行里面的语句，只会记录返回值
def func1(x):
    return x*3
mp=map(func1,l1)
print("l1所有元素*3的结果为")
for i in mp:
    print(i,end=" ")

print(end="\n")
#reduce函数
#reduce函数是functool库里的一个函数，使用前必须有“from functools import reduce”
#reduce函数可以将一个可迭代对象里每一个对象都在一个函数里面执行相同的操作，每次操作的结果保留，并将多次执行的结果返回
#参数：第一个参数是一个函数，这个函数必须是两个参数，其中一个参数是多次执行的结果，
#另一个参数是从可迭代对象中取出的下一个元素；第二个参数是可迭代对象
from functools import reduce
def func2(x1,x2):
    return x1*x2
res=reduce(func2,l2)
print("l2全部元素累乘的结果为",res)