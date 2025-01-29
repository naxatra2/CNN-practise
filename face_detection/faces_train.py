import cv2 as cv
import os 
import numpy as np

p = []
for i in os.listdir("Faces/train"):
    p.append(i)

print(p)