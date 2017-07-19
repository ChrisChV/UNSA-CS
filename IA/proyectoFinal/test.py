import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.utils import np_utils
from keras.datasets import mnist
from matplotlib import pyplot as plt
from keras import backend as K


###
	#TODO:
	# los X_train/test son ndArrays con las imagenes en forma de matrices. Los shapes : (numero de img,dimension,tam,tam)
		# Hacer el Load de las imagenes.
		# Generar las matrices.
		# Crear el X_train y el X_test
	# los Y_train/test son las clases de las imagenes. Deben estar en formato binario. Los shapes :(numero de img, numero de clases)
		# Hacer el Load de las clases.
		# Convertir las clases en su forma binaria (si es que ya no lo estan)
		# Crear el Y_train y el Y_test
	# Armar una arquitectura adecuada
###

np.random.seed(123)

# load (downloaded if needed) the MNIST dataset
(X_train, y_train), (X_test, y_test) = mnist.load_data() # X:imagenes y Y:clases 

X_train = X_train.reshape(X_train.shape[0],28, 28,1)
X_test = X_test.reshape(X_test.shape[0],28, 28,1)
input_shape = (28,28,1)

X_train = X_train.astype('float32')
X_test = X_test.astype('float32')
X_train /= 255
X_test /= 255

y_train = np_utils.to_categorical(y_train, 10)
y_test = np_utils.to_categorical(y_test, 10)

model = Sequential()

model.add(Conv2D(32, (3,3), activation='relu', input_shape=input_shape))  # (filtros,tamVentana,tamVentana)
model.add(Conv2D(32, (3,3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2,2)))
model.add(Dropout(0.25)) #porcentaje de neuronas a excluir (regularizacion)
model.add(Flatten())
model.add(Dense(128, activation='relu')) # Fully-conected con 128 neuronas
model.add(Dropout(0.5))
model.add(Dense(10, activation='softmax')) #Capa de salida con el numero de clases (10)
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
model.fit(X_train, y_train, batch_size=128, epochs=1, verbose=1)
#pre = model.predict(X_test[:1], batch_size= 1, verbose = 1)
score = model.evaluate(X_test, y_test, verbose=1) #[loss,acuracy]
print score
score = model.evaluate(X_test, y_test, verbose=1) #[loss,acuracy]
print score
model.save('my_model.h5')
