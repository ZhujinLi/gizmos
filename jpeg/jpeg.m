clear;
close all;
rawImg = imread('landscape.jpg');
figure(1);
imshow(rawImg);

grayImg = rgb2gray(rawImg);
figure(2);
imshow(grayImg);

imgSize = size(rawImg);

%% Divide the image into non-overlapping 8x8 blocks.
imgBlocks = zeros( imgSize(1)/8, imgSize(2)/8, 8, 8);
for i = 1:size(imgBlocks,1)
    for j = 1:size(imgBlocks,2)
        rangeI = 8*(i-1)+1 : 8*i;
        rangeJ = 8*(j-1)+1 : 8*j;
        imgBlocks(i,j,:,:) = grayImg(rangeI, rangeJ);
    end
end

%% DCT
dctBlocks = zeros(size(imgBlocks));
for i = 1:size(dctBlocks,1)
    for j = 1:size(dctBlocks,2)
        dctBlocks(i,j,:,:) = dct(imgBlocks(i,j,:,:));
    end
end

%% Quantization.
quanN = 16;
quanBlocks = floor(dctBlocks ./ quanN);
quanImg = mergeBlocks(quanBlocks);
figure(3);
imshow(uint8(quanImg));

%% Revert quantization
invQuanBlocks = quanBlocks .* quanN;

%% Revert DCT
for i = 1:size(invQuanBlocks,1)
    for j = 1:size(invQuanBlocks,2)
        invDCTBlocks(i,j,:,:) = idct(invQuanBlocks(i,j,:,:));
    end
end
invDCTImg = mergeBlocks(invDCTBlocks);
figure(4);
imshow(uint8(invDCTImg));

%% Functions
function Y = mergeBlocks(X)
    Y = zeros(8*size(X,1), 8*size(X,2));
    for i = 1:size(X,1)
        for j = 1:size(X,2)
            rangeI = 8*(i-1)+1 : 8*i;
            rangeJ = 8*(j-1)+1 : 8*j;
            Y(rangeI, rangeJ) = X(i,j,:,:);
        end
    end
end
