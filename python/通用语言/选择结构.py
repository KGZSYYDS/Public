#选择结构
from re import match

print("选择结构：if语句")
a=eval(input("输入一个1到100的数"))
#python中用缩进控制判断语句，即不在缩进下的语句就是正常顺序语句，多级嵌套就是多级缩进
#记得&&是and，||是or
#1、条件后面有： 2、如果判断后只有一条语句需要执行，可以将这行语句直接写在:后面 3、else if 写成elif
if a>50:
    print("这个数大于50")
elif a>30:
    print("这个数大于30")
else:
    print("这个数小于等于30")

#等效三目运算符 “运行结果1 if 条件 else 运行结果2”：
#如果一定要只有一个判断有输出，else可以用pass代替语句块2，来保证语法结构完整,但pass不能代替数据进行赋值等操作
#赋值结果1 if 判断条件 else 赋值结果2
x="这个数大于50" if a>50 else "这个数小于等于50"
print(x)
#或写成
print("这个数大于50") if a>50 else print("这个数小于等于50")

#相当于switch-case
print("选择结构：match-case语句")
#注意：这里就是要判断字符，不要用eval
al=input("请输入一个字母")
match al:
    case "a":
        print("是a")
    case "b":
        print("是b")
    case "c":
        print("是c")
    case "d":
        print("是d")
    case "e":
        print("是e")
    #这行语句相当于default
    case _:
        print("不是abcde")
