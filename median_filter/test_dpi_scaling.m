
clear; close all; clc;

I = imread('screenshot.png');
I = I(1:100,1:200);
I = imresize(I, 5, 'nearest');
figure;
imshow(I);

h = fspecial('average', [5 5]);
J = imfilter(I, h);
figure;
imshow(J);

K = medfilt2(I, [5 5]);
figure;
imshow(K);