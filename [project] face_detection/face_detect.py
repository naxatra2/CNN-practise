import cv2 as cv
img = cv.imread("Photos\group 1.jpg")
#cv.imshow("Lady", img)


# face detection doesn't involve colours and skin tones in an image
# it uses edges for face detection

gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
#cv.imshow("Gray image", gray)

haar_cascade = cv.CascadeClassifier("haar_face.xml")

#rectangular coordinates of faces 
faces_rect = haar_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors = 3)

print(f"Number of faces found = {len(faces_rect)}")

for (x,y,w,h) in faces_rect:
    cv.rectangle(img, (x,y), (x+w, y+h), (0,255,0), thickness = 2)

# detected faces
cv.imshow("detected images", img)
cv.waitKey(0)
cv.destroyAllWindows()
