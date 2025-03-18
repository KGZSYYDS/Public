#字符串的序列和一些操作
print("字符串的序列和操作")
str4="1234567"
print("字符串为"+str4)
#正向下标与c++相同，反向下标用负数表示，并且是从-1开始算，所以绝对值等于字符串长度
print("下标为1："+str4[1],"下标为-1："+str4[-1])
#[]内使用：可以截取字符串,不允许反向输出，例如【-2：-6是无效的】
print("下标为【1:5】"+str4[1:5],"下标为【-6:-2】"+str4[-6:-2])
print("3个这个字符串："+3*str4)

#字符串相关函数

#st1 in str2的用法（同理not in）：这是一个bool类型的函数，用于检测str1是否在str2里面，并且bool类型的返回值print时直接是true和false而不是0和1
if"1234" in str4:
    print("1234在str4里面,\"1234\"in str4结果为",("1234" in str4))
else:
    print("1234不在str4里面,\"1234\"not in str4结果为",("1234" not in str4))

#find和index函数，字符串的成员函数，find的第一个参数为要查找的字符串，【第二个参数是起始下标，第三个参数是步长】
#【】内有默认值，若有成功匹配字符串返回第一个元素首地址，无则返回-1.index与find相同，但无匹配时报错不返回
name="helloworld"
print("helloworld中hello用find的结果为：",name.find("hello"))
print("helloworld中hello用index的结果为：",name.index("hello"))

#len函数，全局函数，求字符串的长度，也可以对列表、元组、字典使用，返回数据结构中的元素个数
print("helloworld长度为",len(name))

#count函数，字符串的成员函数，第一个参数为要查找的字符串，【第二个参数是起始下标，第三个参数是步长】
print("helloworld中l的出现：",name.count("l"))

#replace函数，字符串成员函数，第一个参数：需修改的字符，第二个参数：修改后的字符，【第三个参数：替换次数，默认值为全部替换】
print("name修改前为",name)
print("name修改2个l为i后为",name.replace("l","i",2))

#split函数，字符串成员函数，第一个参数为字符串中的一段字符，将这个字符所在位置删除，前后两段分开，找不到该字符不分割
#【第二个参数：替换次数，默认值为全部替换】
name="helloworld"
print("name分割l后结果为",name.split("l"))

#大小写函数
#首字母大写函数
print("name首字母大写为",name.capitalize())
#全部转小写函数
print("name lower后为",name.lower())
#全部转大写函数
print("name lower后为",name.upper())