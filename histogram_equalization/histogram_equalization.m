clear;
close all;

%% Original image
x = imread('img3.png');
x = rgb2gray(x);
L = 256;
imshow(x);

figure;
imhist(x);

%% My implementation

p = zeros(256, 1);
for i = 0:255
    p(i+1) = sum(sum(x == i)) / length(x(:));
end

s = zeros(256, 1);
for i = 1:256
    for j = 1:i
        s(i) = s(i) + p(j);
    end
    s(i) = s(i) * (L-1);
end

s = uint8(s);
figure;
plot(0:255, s);

y = zeros(size(x), 'uint8');
for i = 1:size(x,1)
    for j = 1:size(x,2)
        y(i,j) = s(x(i,j)+1);
    end
end

figure;
imshow(y);

figure;
imhist(y);

%% Built-in implementation

figure;
gt = histeq(x);
imshow(gt);

figure;
imhist(gt);
