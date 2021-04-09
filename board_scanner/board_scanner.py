#!/usr/local/bin/python3

# A naive board scanner for this very image.

import numpy as np
import cv2 as cv
import math


def line_intersect(rhoA, thetaA, rhoB, thetaB):
    Ax1, Ay1, Ax2, Ay2 = line_polar_to_xy(rhoA, thetaA)
    Bx1, By1, Bx2, By2 = line_polar_to_xy(rhoB, thetaB)

    d = (By2 - By1) * (Ax2 - Ax1) - (Bx2 - Bx1) * (Ay2 - Ay1)
    if d:
        uA = ((Bx2 - Bx1) * (Ay1 - By1) - (By2 - By1) * (Ax1 - Bx1)) / d
        uB = ((Ax2 - Ax1) * (Ay1 - By1) - (Ay2 - Ay1) * (Ax1 - Bx1)) / d
    else:
        return
    if not(0 <= uA <= 1 and 0 <= uB <= 1):
        return
    x = Ax1 + uA * (Ax2 - Ax1)
    y = Ay1 + uA * (Ay2 - Ay1)
 
    return x, y


def line_polar_to_xy(rho, theta):
    a = math.cos(theta)
    b = math.sin(theta)
    x0 = a * rho
    y0 = b * rho
    return (int(x0 + 1000*(-b)), int(y0 + 1000*(a)), int(x0 - 1000*(-b)), int(y0 - 1000*(a)))


def four_point_transform(image, rect):
    (tl, tr, br, bl) = rect

    widthA = np.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2))
    widthB = np.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2))
    maxWidth = max(int(widthA), int(widthB))

    heightA = np.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2))
    heightB = np.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2))
    maxHeight = max(int(heightA), int(heightB))

    rect = np.array([
        [rect[0][0], rect[0][1]],
        [rect[1][0], rect[1][1]],
        [rect[2][0], rect[2][1]],
        [rect[3][0], rect[3][1]]], dtype = "float32")

    dst = np.array([
        [0, 0],
        [maxWidth - 1, 0],
        [maxWidth - 1, maxHeight - 1],
        [0, maxHeight - 1]], dtype = "float32")

    M = cv.getPerspectiveTransform(rect, dst)
    return cv.warpPerspective(image, M, (maxWidth, maxHeight))


###


# Edge detection
rgb = cv.imread('./src.png')
gray = cv.cvtColor(rgb, cv.COLOR_BGR2GRAY)
blured = cv.GaussianBlur(gray, (7, 7), 0)
edged = cv.Canny(blured, 75, 180)

# Identify lines using Hough transform
lines = cv.HoughLines(edged, 1, np.pi / 60, 50, None, 0, 0)

# Find four outermost lines
left = [math.inf, 0]
top = [math.inf, 0]
right = [0, 0]
bottom = [0, 0]
for line in lines:
    rho = line[0][0]
    theta = line[0][1]
    if np.pi / 4 < theta < np.pi / 4 * 3:   # Horizontal line
        if abs(rho) < abs(top[0]):
            top[0] = rho
            top[1] = theta
        if abs(rho) > abs(bottom[0]):
            bottom[0] = rho
            bottom[1] = theta
    else:                                   # Vertical line
        if abs(rho) < abs(left[0]):
            left[0] = rho
            left[1] = theta
        if abs(rho) > abs(right[0]):
            right[0] = rho
            right[1] = theta

# Solve four corners
corners = [
    line_intersect(left[0], left[1], top[0], top[1]),
    line_intersect(right[0], right[1], top[0], top[1]),
    line_intersect(right[0], right[1], bottom[0], bottom[1]),
    line_intersect(left[0], left[1], bottom[0], bottom[1])
]

# Transform and save
warped = four_point_transform(rgb, corners)
cv.imwrite('./dst.png', warped)
