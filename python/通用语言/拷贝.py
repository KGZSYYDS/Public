import copy
li=[1,2,[3]]
li_nocopy=li
li_qiancopy=copy.copy(li)
print("li为",li,"li_nocopy为",li_nocopy,"li_qiancopy为",li_qiancopy)

#拷贝：python中的部分数据结构，其值可以修改但内存不变（如列表、字典、集合），称为可变对象
#但是也有不可变对象（如整数、字符串、元组），如果改变这些类型的变量的值，那么会直接开一个新空间来储存
#所以拷贝操作仅针对可变对象，不可变对象仅支持赋值

#半浅拷贝
li.append(4)
#python中赋值是浅拷贝，指向同一个地址，深拷贝可以使用copy包
#copy.copy()参数为一个变量，返回这个变量的半浅拷贝，只有第一层是深拷贝，多级嵌套时内部的列表仍然是浅拷贝
print("li外层改变后为",li,"li_nocopy为",li_nocopy,"li_yes为",li_qiancopy)
li[2].append(5)
print("li内的列表添加元素后li为",li,"li_nocopy为",li_nocopy,"li_yes为",li_qiancopy)

#但是如果是改变元素则没有影响，因为已经是一个新的对象，地址也是新的，但是li_yes仍然指向原来的地址
li[2]=[1]
print("li[3]改变后li为",li,"li_nocopy为",li_nocopy,"li_yes为",li_qiancopy)


#深拷贝
#copy.deepcopy()参数为一个变量，返回这个对象的深拷贝，其中不管套多少层都是新地址
li=[1,[2,[3]]]
li_deepcopy=copy.deepcopy(li)
print("当前li为",li,"li_deepcopy为",li_deepcopy)
li[1].append('a')
li[1][1].append('b')
print("在内层列表里加元素后li为",li,"li_deepcopy为",li_deepcopy)