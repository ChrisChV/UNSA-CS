import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.utils import np_utils
from keras.datasets import mnist
from matplotlib import pyplot as plt
from keras import backend as K
import ImgOp as Io

FILE_NAMES_TRAIN = "namesTrain.dat"
FILE_NAMES_TEST = "namesTest.dat"
FILE_CLASS_TRAIN = "clasesTrain.dat"
FILE_CLASS_TEST = "claesTest.dat"
DIR_TRAIN = "skin_train"
DIR_TEST = "skin_test"

HEIGHT_IMGS = 300
WEIGHT_IMGS = 300
N_TRAIN = 551
N_TEST = 348


X_train = dirToArray(DIR_TRAIN,FILE_NAMES_TRAIN,N_TRAIN,HEIGHT_IMGS,WEIGHT_IMGS)
X_test = dirToArray(DIR_TEST,FILE_CLASS_TEST,N_TEST,HEIGHT_IMGS,WEIGHT_IMGS)

Y_train = getClassFromFile(FILE_NAMES_TRAIN)
Y_test = getClassFromFile(FILE_CLASS_TEST)


(X2_train, y2_train), (X2_test, y2_test) = mnist.load_data()
