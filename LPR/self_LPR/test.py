import time

start = time.time()

import cv2
import numpy as np
import province
import digit


def get_img(img_1):
    img = cv2.resize(img_1, (300, 100))
    _, img_thr = cv2.threshold(img, 180, 255, cv2.THRESH_BINARY)

    pix_max = -1
    for j in reversed(range(img_thr.shape[1])):
        for i in range(img_thr.shape[0]):
            if(img_thr[i][j] == 255):
                pix_max = j
                break
        if(pix_max != -1):
            break

    pix_min = -1
    for j in range(img_thr.shape[1]):
        for i in range(img_thr.shape[0]):
            if(img_thr[i][j] == 255):
                pix_min = j
                break
        if(pix_min != -1):
            break

    img_ = img[:, pix_min:pix_max]

    image = cv2.resize(img_, (300, 100))
    return image

watch_cascade = cv2.CascadeClassifier('cascade.xml')
image = cv2.imread('timg.jpg')

resize_h = 1000
height = image.shape[0]
scale = image.shape[1]/float(image.shape[0])
image = cv2.resize(image, (int(scale*resize_h), resize_h))

image_gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
watches = watch_cascade.detectMultiScale(image_gray, 1.1, 3, minSize=(36,9), maxSize=(36*40,9*40))

for (x, y, w, h) in watches:
    img = image_gray[y:y+h, x:x+w]
    image = get_img(img)
    a = cv2.resize(image[:, :40], (32, 40))
    b = cv2.resize(image[:, 30:85], (32, 40))
    c = cv2.resize(image[:, 85:135], (32, 40))
    d = cv2.resize(image[:, 130:180], (32, 40))
    e = cv2.resize(image[:, 175:215], (32, 40))
    f = cv2.resize(image[:, 215:260], (32, 40))
    g = cv2.resize(image[:, 255:], (32, 40))
    cv2.imshow('img_1', a)
    cv2.imshow('img_2', b)
    cv2.imshow('img_3', c)
    cv2.imshow('img_4', d)
    cv2.imshow('img_5', e)
    cv2.imshow('img_6', f)
    cv2.imshow('img_7', g)
    cv2.waitKey()

    DIG = [b, c, d, e, f, g]

    pro = province.get_pro(a)
    dig = digit.get_dig(DIG)
    ans = pro[0] + dig
    acc = pro[1]
    print('车牌号码为：', ans)
    print('准确率为：%.3f%%' % acc)

print('时间为：%.3fs' % (time.time()-start))
