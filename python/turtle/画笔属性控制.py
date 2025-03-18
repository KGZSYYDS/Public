import turtle as t

#color函数表示更改画笔颜色，第一个参数是画笔画出来的线的颜色
#第二个是画笔的填充颜色（会在完成封闭的时候填充内部为这个颜色）
t.color("red","blue")
#这个函数可以调整画笔粗细，默认为1
t.pensize(8)
t.speed(10)

#表示开始填充
t.begin_fill()
t.circle(100)
t.forward(100)
t.left(90)
t.forward(200)
t.left(90)
t.forward(100)
t.end_fill()
#注意：若起终点不一致，会直接连接起终点然后填充
#如果有重合区域，则重合区域变成白色，这个区域再被覆盖则会变成填充颜色

t.done()