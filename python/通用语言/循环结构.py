#循环结构
#for in及for-else结构

print("for语句相关")

sumi=0
#for语句在python中专门用于遍历，所以是for-in语句，让循环变量在in后面的数组等空间中遍历
#range是前闭后开，表示从【1,101）的整数，range里面只有一个数n的时候默认从0开始，到n-1结束
for i in range(1,101):
    sumi+=i
print("0到100的累加和为",sumi)

#for语句的遍历对象可以是一个字符串,可以做到逐行输出
print("逐行输出irobot:")
for i in "irobot":
    print(i)

#第三个参数是步长，有默认值1
sumi=0
for i in range(1,101,2):
    sumi+=i
print("0到100的所有奇数累加和为",sumi)

for i in range(2,101):
    #for-else语句是python中由于for专用于进行数组遍历而衍生的语句，在全部遍历后会执行else中的语句（注意else与其所对应的for同级）
    #注意：for-else同级之间不能有别的语句，例如在else上面与else同级缩进下写个print是不行的，但是可以在else结束后同级写，这样算结束了for-else的顺序结构
    for j in range(2,i):
        if i%j==0:
            #针对break情况的语句不能跟在else结束后同级之下，因为即使全部遍历完也会正常执行，所以针对break情况的语句要与break同级写
            #print(i,"不是质数")
            break
    else:
        print(i,"是质数")


#while语句
print("while语句")
#while的条件变量在循环外初始化为全局变量
i=100
#while基本相同，只需多加一个：，注意缩进即可，同样也具有else分支，规则同for-else
while i<1000:
    if (i%10)**3+(i//10%10)**3+(i//100)**3==i:
        print("水仙花数",i)
    #python中没有i++的表达方式
    i+=1

