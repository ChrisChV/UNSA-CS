import matplotlib.pyplot as plt
import numpy as np
import math
x = np.meshgrid(np.linspace(-4,4,251));
y = np.meshgrid(np.linspace(-4,4,251));
r = math.sqrt(x * x + y *y);
z = somb(r);

