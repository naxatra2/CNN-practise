import cv2 as cv

haar_cascade = cv.CascadeClassifier('haar_face.xml')
webcam = cv.VideoCapture(0) # 0 is the default webcam

while True:
    _, img = webcam.read()
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    faces = haar_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=4)
    for (x,y,w,h) in faces:
        cv.rectangle(img, (x,y), (x+w,y+h), (0,255,0), thickness=2)
    cv.imshow('Webcam', img)
    key = cv.waitKey(10)
    if key == 27:
        break
webcam.release()
cv.destroyAllWindows()


