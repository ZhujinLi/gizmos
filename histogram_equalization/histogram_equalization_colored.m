clear;
close all;
x = imread('overexposed.jpg');
L = 256;
subplot(2,3,1);
imshow(x);

subplot(2,3,2);
imhist(x);

p = zeros(256, 3);
for i = 0:255
    for j = 1:3
        p(i+1, j) = sum(sum(x(:,:,j) == i)) / length(x(:)) * 3;
    end
end

s = zeros(256, 3);
for i = 1:256
    for k = 1:3
        for j = 1:i
            s(i,k) = s(i,k) + p(j,k);
        end
        s(i,k) = s(i,k) * (L-1);
    end
end

s = uint8(s);

y = zeros(size(x), 'uint8');
for i = 1:size(x,1)
    for j = 1:size(x,2)
        for k = 1:3
            y(i,j,k) = s(x(i,j,k) + 1,k);
        end
    end
end

subplot(2,3,4);
imshow(y);

subplot(2,3,5);
imhist(y);