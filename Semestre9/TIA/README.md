# Proyecto de Tópicos en Inteligencia Artificial

Reconocimiento del lenguaje de señas mediante una red convolucional.

Integrantes:

```
Chavez Carazas Christofer Fabian
Quispe Quicaño Julio Cesar
Torres Lima Ruben Jesus
Valencia Rivero Carlos David
```

En este proyecto se desarrollo el artículo *"Visual Hand Gesture Recognition with
Convolution Neural Network"* [Mengmeng Han et. al].

##Preprocesamiento

El código fuente del preprocesamiento se encuentra en camera/preprocesamiento.cpp. Se recibe como entrada una imagen en RGB. Para todo el proceso se utilizó la libreria CImg.h como apoyo para el manejo de imágenes. Se hace una conversión de la imagen de entrada a formato .bmp para utilizarla en la libreria. Se hace la conversión de la imagen de RGB a Lab. El espacio de color LAB es usado en el método por la facilidad en el manejo de los rango y la eliminación del fondo. Se itera toda la imagen y se encuentran los puntos que cumplan con los siguientes rangos:

* 17 <= L <= 90
* 26 <= a <= 85
* -30 <= b <= 15

El valor de dichos puntos se cambia a 255 y el resto a 0 en una nueva imagen binaria. Como salida del preprocesamiento se retorna la imagen binaria con la silueta de la seña de la mano en color blanco y el fondo en color negro.

##Red convolucional

La red convolucional utilizada tiene la siguiente estructura:

* Capa de convolución de 32 filtros con una ventana de (10,10)
* Capa de Batch Normalization
* Capa de Activación ReLU
* Capa de convolución de 32 filtros con una ventana de (10,10)
* Capa de Batch Normalization
* Capa de Activación ReLU
* Capa de Max Pooling con un tamaño de pool de (2,2)
* Capa de Dropout con 0.4 de proporción.
* Capa de convolución de 64 filtros con una ventana de (10,10)
* Capa de Batch Normalization
* Capa de Activación ReLU
* Capa de convolución de 64 filtros con una ventana de (10,10)
* Capa de Batch Normalization
* Capa de Activación ReLU
* Capa de Max Pooling con un tamaño de pool de (2,2)
* Capa de Dropout con 0.4 de proporción.
* Capa de convolución de 32 filtros con una ventana de (3,3)
* Capa de Batch Normalization
* Capa de Activación ReLU
* Capa de convolución de 32 filtros con una ventana de (3,3)
* Capa de Batch Normalization
* Capa de Activación ReLU
* Capa de Max Pooling con un tamaño de pool de (2,2)
* Capa Flatten
* Capa totalmente conectada de 256 neuronas
* Capa de Batch Normalization
* Capa de Activación ReLU
* Capa de Dropout con 0.4 de proporción.
* Capa totalmente conectada de 128 neuronas
* Capa de Batch Normalization
* Capa de Activación ReLU
* Capa de Dropout con 0.4 de proporción.
* Capa totalmente conectada de 27 neuronas con activación softmax


##Base de Datos
La base de datos que se usó la base de datos de máscaras de piel que se encuentra en [http://sun.aei.polsl.pl/~mkawulok/gestures/](http://sun.aei.polsl.pl/~mkawulok/gestures/). La base de datos cuenta con 899 imágenes de siluetas de de la seña de la mano en color negro y el fondo en color blanco. Se ivertió los colores de la base de datos para igualarnos a los datos utilizados en el artículo del cual nos basamos. Se dividió la base de datos de forma aleatoria en los conjuntos de entrenamiento (60%) y testing (40%).

##Entrenamiento
Se utilizó python 2.7 y Tensorflow 1.8. El código fuente del proceso de entrenamiento se encuentra en el archivo python/bestCnn.py. Se utilizó un tamaño de batch de 64 y se ejecutaron 120 epochs. El proceso de entrenamiento fue ejecutado en el Centro de Alto Rendimiento Computacional de la Amazonía Peruana del Instituto de Investigaciones de la Amazonía Peruana. Mayor información en: http://iiap.org.pe/manati

##Interfaz
La interfaz gráfica fue programada en C++ y en QT. Para las pruebas con la interfaz se utilizó un guante de latex color rosa para que el algoritmo pueda detectar la silueta y eliminar el fondo. 


