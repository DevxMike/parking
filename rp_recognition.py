import cv2
import imutils
import pytesseract
from PIL import Image

pytesseract.pytesseract.tesseract_cmd = 'D:\\tesseract_ocr\\tesseract'



class registration_plates_recognition:
    def __init__(this, image):
        this.original_image = image

    def recognise(this)->str:
        image = this.original_image
        image = imutils.resize(image, width=300 )

        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        gray_image = cv2.bilateralFilter(gray_image, 11, 17, 17) 

        edged = cv2.Canny(gray_image, 30, 200) 

        cnts,new = cv2.findContours(edged.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        image1=image.copy()

        cv2.drawContours(image1,cnts,-1,(0,255,0),1)

        cnts = sorted(cnts, key = cv2.contourArea, reverse = True) [:30]
        screenCnt = None

        for c in cnts:
                perimeter = cv2.arcLength(c, True)
                approx = cv2.approxPolyDP(c, 0.018 * perimeter, True)
                if len(approx) == 4: 
                        screenCnt = approx
                        x,y,w,h = cv2.boundingRect(c) 
                        new_img=image[y:y+h,x:x+w]
                        cv2.imwrite('cropped'+'.png',new_img)
                        #i+=1
                        break

        cv2.drawContours(image, screenCnt, -1, (0, 255, 0),5)

        Cropped_loc = 'cropped.png'
        plate = Image.open(Cropped_loc)
        text = pytesseract.image_to_string(plate, lang = 'eng', config='--psm 13 --oem 1 -c tessedit_char_whitelist=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789')
        if(len(text) > 5):
            return text
        else:
            return None