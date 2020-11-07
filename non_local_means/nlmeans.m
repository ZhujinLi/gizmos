% reference paper: <http://www.ipol.im/pub/art/2011/bcm_nlm/article.pdf>
% patchwise implementation is adopted here

clear;
close all;
tic;

u = imread('alley.png');
shrink_k = 5; % do not use full image for it's too slow
u = u(1:int32(size(u, 1)/shrink_k), 1:int32(size(u, 2)/shrink_k), :);
figure;
imshow(u);

sigma = 10;
var = (sigma/255).^2;
% 0.001 * sigma
u = imnoise(u, 'gaussian', 0, var);
figure;
imshow(u);

r = choose_r(sigma);
f = choose_f(sigma);
h = choose_h(sigma);

uo = zeros(size(u));
for i = 1:3
    ui = double(u(:,:,i));
    
    for x = r+1:size(ui, 2)-r % will produce black borders
        fprintf('progress: %d:%f\n', i, x / (size(ui, 2)-r));
        for y = r+1:size(ui, 1)-r
            uo_tmp = zeros(size(ui));
            C = 0;
            B_range_y = y-f:y+f;
            B_range_x = x-f:x+f;
            B = u(B_range_y, B_range_x,:);
            for xx = x-r+f : x+r-f
                for yy = y-r+f : y+r-f
                    Q_range_y = yy-f:yy+f;
                    Q_range_x = xx-f:xx+f;
                    Q = u(Q_range_y, Q_range_x,:);
                    w = calc_w(B, Q, f, h, sigma); % note all three channels are used
                    C = C + w;
                    uo_tmp(B_range_y, B_range_x) = uo_tmp(B_range_y, B_range_x) + ...
                        w * ui(Q_range_y, Q_range_x);
                end
            end
            uo(B_range_y, B_range_x,i) = uo(B_range_y, B_range_x,i) + ...
                uo_tmp(B_range_y, B_range_x) / C;
        end
    end
    
end

uo = uo / ((2*f+1)^2); % will have something wrong near borders
uo = uint8(uo);
figure;
imshow(uo);

toc;



%% utilities

function r = choose_r(sigma)
if sigma <= 25
    r = 10;
else
    r = 17;
end
end

function f = choose_f(sigma)
if sigma <= 25
    f = 1;
else
    if sigma <= 55
        f = 2;
    else
        f = 3;
    end
end
end

function h = choose_h(sigma)
if sigma <= 25
    h = 0.55*sigma;
else
    if sigma <= 55
        h = 0.40*sigma;
    else
        h = 0.35*sigma;
    end
end
end

function w = calc_w(B, Q, f, h, sigma)
    B = double(B);
    Q = double(Q);
    d2 = 1 / (3*((2*f+1).^2)) * sum(sum(sum(((B-Q).^2))));
    if false%d2 < 2*(sigma.^2)
        fprintf('%f vs %f\n', d2, 2*(sigma.^2));
    end
    w = exp(-max(d2 - 2*(sigma.^2), 0) / h / h);
end
