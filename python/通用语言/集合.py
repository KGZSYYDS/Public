#集合是无键只有值的元素组合体，允许多种数据类型，但是元素唯一，具有无序性（print集合每次结果不同）
jihe={1,2,"a",4,1,2}
#由于字典和集合都是{}，所以定义时不同
kongzidian={}#这是空字典
kongjihe=set()#这是空集合

#此处打印每次结果不一定相同，并且重复元素会自动去除
print("jihe为",jihe)

#集合相关函数
#add函数，集合的成员函数，参数为要添加的元素，如果已经有该元素则无事发生
jihe.add(5)
print("add(5)之后jihe为",jihe)

#update函数，集合成员函数，参数为一个可迭代对象，会将参数拆分之后再加入到集合中
jihe.update([1,2,3])
print("update([1,2,3])之后jihe为",jihe)

#remove函数，集合成员函数，参数为集合里原有的函数，如果原本就不存在会报错
jihe.remove(5)
print("remove(5)之后jihe为",jihe)

#discard函数，集合成员函数，参数为集合里原有的函数，原本不存在无事发生
jihe.discard(2)
print("discard(2)之后jihe为",jihe)
jihe.discard("b")
print("discard(b)之后jihe为",jihe)

#交并集操作，同数学，交集(∩)为&，并集(∪)为|  （and和or）
shuji1={1,2,3}
shuji2={2,3,4}
print("shuji1为",shuji1,"shuji2为",shuji2)
#注：无交叉元素时返回空集
print("shuji1 & shuji2结果为",shuji1&shuji2)
print("shuji1 | shuji2结果为",shuji1|shuji2)