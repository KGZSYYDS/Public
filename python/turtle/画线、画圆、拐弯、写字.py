import turtle as t

#设置运行速度为（从1到10）档位，默认为5
t.speed(7)
#向当前方向（初始为x轴正方向）移动指定大小长度，可以简写为fd
t.forward(300)
#向反方向移动指定大小长度，注意这些都是向量移动
t.backward(300)

#向顺时针方向转（度数）
t.right(60)
t.forward(300)
#向逆时针方向转（度数）
t.left(300)
t.backward(300)
#画圆函数，第一个参数是半径，然后以当前位置为圆上的点逆时针画圆,半径是负数表示顺时针画圆
#第二个参数是画的角度，此处表示只画半个圆
t.circle(-100,180)

#抬笔与落笔
#通过这个方式可以让画笔去到任意一个点并且不会留下痕迹
t.penup()
t.goto(-200,200)
t.pendown()

#write函数用于输出文本，形参分别为:
#arg：文本内容
#move：是否移动，写完字之后海龟仍停留在原地，可以填true来让其移动到文本最右端
#align：对齐方式，以起点为准，left就是向右输出，right就是输出完移动到左边，center就是居中
#font：字体样式，包括“字体”，字号，“样式”（可填normal——正常，bold——加粗，italic——斜体，bold italic——加粗+斜体
t.write("testing",True,"center",font=("Arial",30,"normal"))


t.done()