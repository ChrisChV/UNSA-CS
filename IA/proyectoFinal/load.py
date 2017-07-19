import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.utils import np_utils
from keras.datasets import mnist
from matplotlib import pyplot as plt
from keras import backend as K
from keras.models import load_model

np.random.seed(123)

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

model = load_model('my_model.h5')

#score = model.evaluate(X_test, y_test, verbose=1) #[loss,acuracy]
print y_test[0]
print y_test[1]
pre = model.predict(X_test[:2], batch_size= 1, verbose = 1)
print pre