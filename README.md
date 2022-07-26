# TDA-HASH
Implementación de una tabla de hash en C.

# 1. Introducción:
  Para este trabajo, se pidió la implementación de una Tabla de Hash abierta(direccionamiento cerrado) 
  en lenguaje C, partiendo de la firmas de las funciones públicas a implementar. Adicionalmente se 
  crearon estructuras y funciones auxiliares privadas del TDA cuyas firmas se encuentran en el archivo 
  estructuras_y_auxiliares.h. Para esta implementación se pide que las claves admitidas por la tabla 
  sean solamente strings. Adicionalmente se pide la creación de un iterador interno que recorra todas 
  las claves almacenadas en la tabla.
  Para esta tarea se implementó una metodología de desarrollo orientada a pruebas, realizando una 
  serie de pruebas estándares de caja negra según el funcionamiento esperado de cada primitiva del 
  TDA y agregando nuevas ante cada bug encontrado durante el testeo.
  
 # 2. Hash (Teoría)
 Una Tabla de Hash es una estructura de datos que asocia claves con valores. La operación principal 
  que soporta de manera eficiente es la búsqueda: permite el acceso a los elementos almacenados a 
  partir de la clave única con la que se los guarda (con complejidad promedio O(1)) . Funciona 
  transformando la clave con una función hash en un código numérico, que, aplicándole la operación de 
  resto de una división entera (%) por la capacidad de la tabla, identifica la posición donde se almacena
  el valor deseado en dicha tabla.
  Existen dos tipos de implementaciones posibles según su método de resolución de colisiones de pares 
  clave – valor a los que les corresponde la misma posición en la tabla.
  
 # 2.1 Tabla de hash abierta:
  Las tablas de hash abiertas tienen por característica su direccionamiento cerrado, esto se debe a la 
  estrategia que se adopta en este tipo de hashes para las colisiones, que es el encadenamiento o 
  chaining. El chaining consiste en encadenar pares de clave valor a los que les corresponde la misma 
  posición, por fuera de la tabla de hash y manteniendo la dirección de esta cadena desde la posición 
  mencionada. Por lo tanto, el direccionamiento cerrado hace referencia a que los pares clave valor 
  siempre se almacenarán relacionados a la posición calculada que les toca inicialmente, como se 
  muestra en la figura:
  
  ![image](https://user-images.githubusercontent.com/92110097/181110242-612f2bc6-a9c5-413f-a07b-c5d0b2a8df96.png)
  
  A continuación, con diagramas, se ilustra la operación de inserción en este tipo de hash:
  
  ![image](https://user-images.githubusercontent.com/92110097/181110305-6c756362-9d3c-419e-8bff-93636ca783a8.png)
  
  Y el otro caso:
  
  ![image](https://user-images.githubusercontent.com/92110097/181110351-5f999d7b-9b8f-48d8-9bef-a153f4054f5d.png)
  
  Además de los pasos ilustrados, también debemos incluir una operación para mantener la 
  complejidad promedio de las operaciones en O(1): el rehash. El rehash es una operación que se 
  encarga de aumentar el tamaño de la tabla y redistribuir los pares ya creados en el hash 
  calculándoles sus nuevas posiciones. De esta manera se evita el encadenamiento de varios pares 
  en una sola posición de la tabla:
 
![image](https://user-images.githubusercontent.com/92110097/181110437-4f1956d7-7a58-4b48-9a0b-8c34b69955d0.png)

  Esta operación se realiza con un criterio que involucra un factor de carga de la estructura. Es decir, 
  si con una inserción nueva se sobrepasaría ese factor de carga se aplica la operación de rehash.
  Este factor de carga se calcula como:

![image](https://user-images.githubusercontent.com/92110097/181110507-9ae28008-70f7-449b-bf79-c493d66058dd.png)

El valor a sobrepasar es seteado por el diseñador del TDA según las necesidades de uso del mismo. 
(80% → 0,8; 70% → 0,7; etc.). El aumento de tamaño de la tabla de hash en esta tarea también 
puede variar dependiendo las necesidades de uso.
El resultado de aumentar la capacidad de la tabla es que al calcular la posición real de la tabla de 
un código de hash de una clave y el código de otra clave que dé una posición real igual en el hash 
anterior, se obtengan posiciones reales diferentes en la nueva tabla.
Avanzamos sobre la operación de Eliminación de pares clave – valor de un hash abierto:

![image](https://user-images.githubusercontent.com/92110097/181110561-87474aa2-619e-4d4e-9f68-a718b1f6c0da.png)

Vemos ahora la operación de Búsqueda:

![image](https://user-images.githubusercontent.com/92110097/181110649-1108119b-68df-4be2-b5f0-b86984549e6d.png)

#2.2 Tabla de hash cerrada:
  Las tablas de hash cerradas tienen por característica su direccionamiento abierto, esto se debe a la 
  estrategia que se adopta en este tipo de hashes para las colisiones y que los pares se guardan por 
  dentro de la tabla de hash, es decir que si se inserta una clave cuya posición calculada ya está 
  ocupada en la tabla de hash, se itera sobre la misma hasta encontrar una posición vacía (Probing
  lineal). Por lo tanto, el direccionamiento abierto hace referencia a que los pares clave valor pueden 
  variar de la posición calculada que les toca inicialmente, como se muestra en la figura:
  
  ![image](https://user-images.githubusercontent.com/92110097/181110744-2c6bf5e2-22ef-4add-9f5c-7b9e47c24143.png)
  
  A continuación, con diagramas se ilustra la operación de inserción con diagramas en este tipo de 
  hash:
  
  ![image](https://user-images.githubusercontent.com/92110097/181110818-8b566992-6d3c-4c5a-a83e-cf2e52752d3d.png)
  
  Si la clave ya existe en la tabla de hash, simplemente se reemplaza el valor por el nuevo:
  
  ![image](https://user-images.githubusercontent.com/92110097/181110877-2bc01d0c-de6d-4da7-9ed3-aa4e54d2336b.png)
  
  De la misma manera que con el hash abierto, para mantener la eficiencia deseada, se realiza la 
  operación de rehash una vez que al querer insertar un par clave – valor se sobrepase el factor de 
  carga establecido. Como vimos anteriormente, se aumenta el tamaño de la tabla y se reubican los 
  pares existentes en las ubicaciones nuevas que les corresponden de acuerdo al nuevo tamaño y 
  nuevo cálculo de posición.
  Vemos ahora la operación de Búsqueda:
  
  ![image](https://user-images.githubusercontent.com/92110097/181111028-de9ddff6-c743-40be-ba8b-4938ab65e6ca.png)

  Ahora busquemos “Jota”:
  
  ![image](https://user-images.githubusercontent.com/92110097/181111126-bd3d917c-c7ef-483f-883b-31251a3ac557.png)
  
  Si buscamos la clave “Jabón” y el calculo de posición de esta nos da 7, se iterará hasta la posición 9 
  (que es un espacio vacío) por lo tanto la búsqueda de una clave inexistente en el hash será 
  infructuosa.
  Avanzamos sobre la operación de Eliminación de pares clave – valor de un hash cerrado.
  Para eliminar pares sin generar problemas en las operaciones posteriores del hash se tienen en 
  cuenta dos métodos diferentes:
    - Reemplazar el espacio liberado con otro par ya insertado en el hash. Este par reemplazante
  debe estar en posiciones contiguas a la posición liberada, sin posiciones vacías entre medio y
  que le “correspondiera” estar en esa posición liberada (mediante el cálculo de posición). Si 
  efectivamente hay un reemplazante, se genera otro espacio vacío nuevo, por lo que se realiza 
  el mismo procedimiento hasta que se encuentre un espacio vacío al buscar reemplazantes. Si 
  el calculo de posición del posible reemplazante da un resultado que correspondiera a 
  posiciones mayores que las de la posición primeramente liberada se lo deja en el lugar.
    - Utilizar un flag para indicar que se borró algo. De esta manera si está vacía la posición 
  igualmente se sigue iterando en cualquier operación. Al ser ocupada por un nuevo elemento 
  en una inserción, el flag se setea nuevamente a su valor original.
  Utilizando flags, saco la clave “palabra”:

![image](https://user-images.githubusercontent.com/92110097/181111259-cfc1aff2-6568-461c-b01b-27af07abb05d.png)

  Ahora si quiero buscar (o quitar) “pelota” tengo que hacer el mismo procedimiento que antes, 
  saltando la posición vacía por que el flag está activado. Con el siguiente esquema se muestra como 
  eliminar la clave “Pelota”:
  
  ![image](https://user-images.githubusercontent.com/92110097/181111884-1858c702-e1e6-4e33-87ad-af66d8a80aa9.png)

#3 Detalles de la implementacion
En primer lugar, además de los archivos provistos para la implementación, se creó un archivo .h adicional 
llamado “estructuras_y_auxiliares.h” que contiene las estructuras utilizadas y las firmas de las funciones 
auxiliares desarrolladas (junto con una breve explicación de su objetivo).
Se tomó la decisión de no utilizar implementaciones de TDAs previamente desarrollados como la Lista o 
el Árbol binario de búsqueda para simplificar el código y por otro lado para mantener las complejidades 
algorítmicas se necesitarían modificar estos TDAs con primitivas nuevas.
Entonces para guardar los pares clave – valor se utilizó una estructura llamada “par” que contiene un 
puntero a char para la clave, un puntero void para el valor (ya que puede ser cualquier cosa) y un puntero 
hacia otro par “siguiente” que se inicializa en null.
La estructura hash se definió utilizando un doble puntero a la estructura par previamente mencionada, 
es decir un vector de punteros a par. Adicionalmente se agregaron una variable numérica para la 
capacidad del vector y otra para la cantidad de pares insertados.
Al principio se implementó una función de hashing que sumaba los valores ascii de cada uno de los 
caracteres de la clave (string) a insertar y el resultado era ese código de hash. Ante el testeo realizado 
junto con las pruebas de chanutron’t quedó en evidencia que esta generaba una distribución de los pares 
mediocre. 

Como consecuencia se modificó la función de tal manera que al empezar a calcular el código de una clave 
se empiece con un numero elevado (5381). Por cada carácter de la string, se realizaba la siguiente 
operación:

![image](https://user-images.githubusercontent.com/92110097/181112014-0c083963-c678-4235-9546-3fd01f9db20c.png)

De esta manera el valor de un carácter ascii, aunque no sea muy elevado si lo será el valor elevado a la 
quinta potencia al que previamente se le sumó ese carácter. Entonces al calcular la posición real en la 
tabla, la distribución es superior y el tiempo de ejecución mejoró notablemente.
Cabe destacar que antes de descubrir lo anterior, y con el motivo de mejorar el tiempo de ejecución, se 
cambiaron varias funciones auxiliares recursivas a su equivalente iterativo, logrando algunos segundos 
de mejoría en el testeo oficial de la cátedra.
Como criterio para el rehash se eligió un factor de carga máximo de 0,7 (70% ) junto con una duplicación 
del tamaño de la tabla de Hash cada vez que se ejecuta esta operación.
Para evitar hacer reservas de memoria innecesarias, la metodología de rehash adoptada fue de trasladar
los pares ya existentes a sus nuevas posiciones, solamente haciendo reserva de memoria dinámica para 
aumentar el tamaño de la tabla.
Para lo anterior se reserva, durante la ejecución de la función de rehash, el vector de punteros hacia las 
cadenas de pares en el Stack para que al modificar el tamaño de la tabla con la nueva reserva de memoria 
dinámica no se pierdan los pares ya insertados.







