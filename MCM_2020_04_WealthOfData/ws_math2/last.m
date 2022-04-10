%%
%love like hate annoying terrible

word = "small";
n = 0;
m = 0;
q = zeros(1, 5);
%%
%
for k = 1 : 5
    if k == 1
        data = rmed_microwave_parent_star_headline_body1;
    elseif k == 2
        data = rmed_microwave_parent_star_headline_body2;
    elseif k == 3
        data = rmed_microwave_parent_star_headline_body3;
    elseif k == 4
        data = rmed_microwave_parent_star_headline_body4;
    elseif k == 5
        data = rmed_microwave_parent_star_headline_body5;
    end
    line_column = size(data);
    line = line_column(1);
    column = line_column(2);    
    for i = 1 : line
        for j = 1 : column
            if ~isempty(data{i, j})
                if string(data(i,j)) == word
                    m = m + 1;
                end
            end
        end
    end
    fit(1, k) = m / line;
    n = 0;
    m = 0;
end
x = 1 : 5;
img = bar(x,fit);

