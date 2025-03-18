#下列表示进制的字母可以用大写字母表示
print("2进制、8进制、16进制：")
a=0b101
b=0o123
c=0xf21
#这里不加函数默认输出int类型
#bin（x）是转化为二进制字符串，oct（x）是转换为八进制字符串，hex（x）是转换为十六进制字符串
print("二进制的",bin(a),"=",a,"八进制的",oct(b),"=",b,"十六进制的",hex(c),"=",c,"\n")
#如果要转换int为二进制等，可以用以下函数


#python中可以表示虚数
print("虚数的表示：")
x=12+23j
print(x,"x的实部为:",x.real,"x的虚部为:",x.imag,"\n")

#允许使用‘‘‘’’’三引号(python中双引号和单引号意义相同)，注意换行字符串中是会读空行的，所以以下字符串中字符串第一行是空行
print("换行字符串")
str1='''
位置：深圳市
名称：深圳大学
身份：老总
姓名：马化腾
'''
print(str1,"\n")

#转义字符
#\n表示换行，\'或者\"可以输出单引号或者双引号，如果输出字符的双引号外包含r或者R，则这个串的转义字符失效（这个字符串仍然合法）
#这两行代码表示输出换行符，反斜杠，单引号和双引号
print("转义字符的输出")
str2="\n\t\'\"\\"
print(str2)
str3=r"\n\t\'\"\\"
print(str3,"\n")

#ascell码显示
print("\nascell码显示：")
#ord函数表示将（）内的字符转化为其对应的ascell码
print("a=",ord("a"))

#数据类型转换
shuzichuan1="123"
shu1=int(shuzichuan1)
#强制类型转换同c++，但是功能更广，其中纯数字组成的字符串可以强制类型转换为int或者float
print("shu1由‘123’强制类型转换而来，shu1类型为",type(shu1),"shu1=",shu1)

#eval函数，全局函数。可以去掉字符串的“”，但如果输入本来就是字符串，则会报错
shuzichuan2="1232"
shu2=eval(shuzichuan2)
print("shu2由‘1232’强制类型转换而来，shu2类型为",type(shu2),"shu2=",shu2)

#list函数，全局函数，将一个可迭代对象转化为列表（注：整数不是可迭代对象）
zifuchuan1="abcd"
list1=list(zifuchuan1)
print("list1由list(‘abcd’)而来，list1类型为",type(list1),"list1=",list1)
#但是字典转化为列表的时候只会取键名为元素
zidian1={"name":"kg","age":18}
print("zidian1为",zidian1)
list2=list(zidian1)
print("list2由zidian1转化而来,list2类型为",type(list2),"list2=",list2)

