#列表是一个不限类型的数组，可以存放不同类型的不限数量的数据
liebiao=[1,2,"a",4]
print("liebiao为",liebiao)
print("liebiao的数据类型为",type(liebiao))
print("liebiao的第二个数据为",liebiao[1])
print("liebiao的第1到3个数据为",liebiao[0:2])
liebiao[2]="b"
print("liebiao的'a'改成'b'之后为",liebiao)

#list是可迭代对象，可以放在循环结构的in里面
print("循环结构打印liebiao内元素")
for i in liebiao:
    print(i)

#list相关函数
#append函数，将参数尾插到列表中，注意如果是字符串会分解为单个字符，所以一般是用来插数字
liebiao.append(10)
print("liebiao在append（10）之后为",liebiao)

#insert函数，第一个参数为位置，第二个元素为要插入的元素，如果位置超出列表内所有元素自动改为尾插
liebiao.insert(4,"5")
print("liebiao在第4个元素后添加元素后为",liebiao)

#in函数，全局函数，查找某一元素是否在列表里，是则返回true，否则返回false，同前可以加not
print("'b' in liebiao结果为","b" in liebiao)

#del函数，全局函数，可以直接删除一整个列表，也可以指定删除某一特定元素
del liebiao[2]
print("删除liebiao[2]后为",liebiao)

#pop函数，列表的成员函数，默认删除最后一个元素，也可以删除指定位置的元素，但不能超出下标范围
liebiao.pop()
print("删除最后一个元素之后为",liebiao)

#remove函数，列表的成员函数，直接查找并删除列表内的元素，元素不存在时报错，有多个相同元素时默认只删除第一个
liebiao.remove(1)
print("删除列表中的元素‘1’之后为",liebiao)

shuziliebiao=[1,21,32,4,5,10]
print("shuziliebiao为",shuziliebiao)

#reverse函数，列表的成员函数，直接将整个列表中的元素颠倒
shuziliebiao.reverse()
print("shuziliebiao在reverse后为",shuziliebiao)

#sort函数，列表的成员函数，如果列表内是纯数字或者可比较字符，则进行升序排序，如果有不可比较的元素（如数字＋字符）则会报错
shuziliebiao.sort()
print("shuzuiliebiao在sort后为",shuziliebiao)

#列表推导式：一种简便的列表写法
#【操作 for i in 可迭代对象】
print("逐个输出shuziliebiao的元素")
[print(i) for i in shuziliebiao]
print("逐个插入元素1-3")#注意range是前闭后开
[shuziliebiao.append(i) for i in range(1,4)]
print(shuziliebiao)

#一个判断素数的函数
def panduansushu(n):
    for i in range(2,n):
        if n%i==0:
            return 0
    return 1

#如果需要加入条件判断也可以使用列表推导式，在末尾加入一个条件即可
lie=[]
[lie.append(i) for i in range(2,101) if panduansushu(i)]
print("添加100以内的素数进入列表为",lie)

#高维列表，等同于数组
erweiliebiao=[liebiao,shuziliebiao,lie]
print("一个二维列表为",erweiliebiao)
print("erweiliebiao[1]为",erweiliebiao[1])