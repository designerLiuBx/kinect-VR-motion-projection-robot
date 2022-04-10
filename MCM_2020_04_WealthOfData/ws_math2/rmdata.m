function data = rmdata(data, needRmData)

line_column = size(data);
line = line_column(1);
column = line_column(2);

for i = 1 : line
    for j = 2 : column
        if  ~isempty(data{i, j})
            if max(strcmp(string(needRmData), string(data(i, j)) ))
                data(i, j) = {[]};
            end
        end
    end
end
