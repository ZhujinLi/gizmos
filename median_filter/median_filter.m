function y = median_filter(x, r)

y = x;
for i = r+1:size(x, 1)-r
    for j = r+1:size(x, 2)-r
        vals = x(i-r:i+r,j-r:j+r);
        vals = vals(:);
        vals = sort(vals);
        y(i, j) = vals(2*r*r+2*r+1);
    end
end

end