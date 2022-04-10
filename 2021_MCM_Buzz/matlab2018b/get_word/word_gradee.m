function grade = word_gradee(data,keyWord)
line_column = size(data);
line = line_column(1);
column = line_column(2);
grade = (zeros(line,1)+1)*50;
n = 0;
for i = 1 : line
    for j = 1 : column
        if  ~isempty(data{i, j})
            if max(strcmp(string(keyWord(:, 1)), string(data(i, j)) ))
                wz = find(strcmp(string(keyWord(:, 1)), string(data(i, j))));
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                keyWord(wz, :);
                grade(i,1) = grade(i,1) + cell2mat(keyWord(wz,2));              
            end
        end
    end
    n = n + 1;
    if ~mod(n,100)
        disp(n);
    end
end