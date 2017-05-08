# XXX: http://stackoverflow.com/questions/30890434/how-to-generate-random-pairs-of-numbers-in-python-including-pairs-with-one-entr
from random import randint

def gencoordinates(m, n):
    x = randint(m, n)
    y = randint(x, n)

    while True:
        yield (x, y)
        x = randint(m, n)
        y = randint(x, n)

if __name__ == "__main__":
    generados=0
    cacas=[]
    g=gencoordinates(0,99999)
    while (generados<10000):
        cacas.append(next(g))
        generados+=1
    for (x,y) in cacas:
        print("%u %u"%(x,y))
