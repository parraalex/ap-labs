import sys
palabra = sys.argv[1]
max = 0
lista = []

for i in palabra:
    if i not in lista:
        lista.append(i)
    else:
        if len(lista) > max:
            max = len(lista)
        lista = []
        lista.append(i)

print(max)
