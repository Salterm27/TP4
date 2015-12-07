# TP4

Se han eliminado operadores else para simplificar la lectura.
Segun nuestras pruebas, el programa se ejecuta con menor velocidad, pero
la legibilidad es más sencilla.(que es uno de los objetivos de la materia)

El programa se ejecuta bajo el nombre "main.exe" por comodidad.

superCalc: entra al modo de super calculadora.

simpleCalc: entra al modo del tp2.

-p define la precision, que debe ser un numero entero positivo

-i define un archivo de entrada

-o define un archivo de salida

ejemplos:

./main.exe simpleCalc

./main.exe superCalc

./main.exe superCalc -p 200

./main.exe superCalc -i operaciones

./main.exe superCalc -o resultados.txt
