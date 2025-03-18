class animal:
    def eat(self):
        print("animal吃饭")

class dog(animal):
    def eat(self):
        print("dog吃饭")

class cat(animal):
    def eat(self):
        print("cat吃饭")

#多态同c++，优先执行子类的重写函数
def test(ani):
    ani.eat()

c1=cat()
d1=dog()
test(c1)
test(d1)
