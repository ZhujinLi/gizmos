clear; close all; clc;

x = imread('peppers.png');
x = rgb2gray(x);
figure;
imshow(x);

x = imnoise(x, 'gaussian');
figure;
imshow(x);

r = 1;

y = median_filter(x, r);

figure;
imshow(y);

figure;
z = medfilt2(x, [2*r+1,2*r+1]);
imshow(z);
