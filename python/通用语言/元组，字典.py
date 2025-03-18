#元组是一个类似列表的数据结构，但是存入数据之后不支持增删改操作，只能进行查询,所以定义之后就不能修改
#所以append和insert函数不能使用，但是count和index仍然可以使用（用法与字符串相同）
yuanzu=(1,2,"a",)
print("yuanzu为",yuanzu)
print("yuanzu[1]为",yuanzu[1])


#字典是一个键（字符串）与值（任意类型）一一对应的数据结构
#键与值之间用： 不同键之间用， 键只能有一个，值可以重复，可以看做是结构体
zidian={"name":"zidian","shuliang":18}
#注意：字典是没有下标的，索引是键名，使用的时候键名要带“”
print("zidian为",zidian)
print("zidian的name为",zidian["name"])
#字典键值可修改，这个操作如果当键名不存在时会直接添加新键名
zidian["shuliang"]=20
zidian["zhongliang"]=150
print("zidian修改后为",zidian)
#字典是可迭代对象，可以放在for里面，但迭代只能取出键名
print("以下是通过for来输出键名")
[print(i) for i in zidian]

#字典相关函数
#get函数是字典的成员函数，参数是键名，同样要加“”，返回值为键对应的值
#如果没有该键名则直接返回none，也可以设置默认值，如果没有该键名时返回默认值
print("zidian.get(’name‘)为",zidian.get("name"))
print("zidian.get('tel')为",zidian.get("tel","未输入"))

#del函数，全局函数，可以直接删除整个字典，也可以输入参数删除键名和对应的值
#删除整个字典之后这个变量会被移除不能再以字典为单位新增
del zidian["zhongliang"]
print("删除zhongliang后zidian为",zidian)

#clear函数，字典的成员函数，清空字典内所有的键名和键所对应的值
zidian.clear()
print("zidian在clear之后为",zidian)

zidian={"name":"zidian","shuliang":18}
#keys函数，字典的成员函数，返回字典的全部键名
print("zidian的键名有",zidian.keys())

#values函数，字典的成员函数，返回字典的全部值
print("zidian的值有",zidian.values())
#也可以作为迭代对象
print("for输出values")
[print(i) for i in zidian.values()]

#items函数，字典成员函数，以元组形式返回字典的全部键值对
print("zidian的键值对有",zidian.items())
#也可以作为迭代对象
print("for输出items")
[print(i) for i in zidian.items()]
