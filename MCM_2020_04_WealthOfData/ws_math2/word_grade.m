function wordGrade = word_grade(data,keyWord)
line_column = size(data);
line = line_column(1);
column = line_column(2);
wordGrade = data(:, 1);
not = 3; like = 2; very = 4;
z = {1};
z1 = {1};
flag = -1;
for i = 1 : line
    n = 0;
    for j = 1 : column
        if  ~isempty(data{i, j})
            if max(strcmp(string(keyWord(:, 1)), string(data(i, j)) ))
                wz = find(strcmp(string(keyWord(:, 1)), string(data(i, j))));
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                keyWord(wz, :);
                if flag ==  3 %已经遇到very
                    if string(keyWord(wz, like)) ~= string(100)  %very like  
                        z = {cell2mat(z) * cell2mat(keyWord(wz, like))};
                        flag = 0;
                        z2 = {cell2mat(z)};
                        n = n + 1;
                    elseif string(keyWord(wz, not)) ~= string(100) % very not
                        z = {cell2mat(z)  * (-1)};
                        flag = 2;
                        z2 = {cell2mat(z)};
                        n = n + 1;         %           else
          %              flag = -1;
                    end
                elseif flag == 2 %遇到very后遇到 not
                    if string(keyWord(wz, like)) ~= string(100) %very not like 
                        z = {cell2mat(z) * cell2mat(keyWord(wz, like))};
                        flag = 0;
                        z2 = {cell2mat(z)};
                        n = n + 1;           %         else
            %            flag = -1;
                    end
                elseif flag == 1 %遇到like后
                    if string(keyWord(wz, very)) ~= string(100) % 又遇到 very
                        z = {cell2mat(z) * cell2mat(keyWord(wz, very))};
                        flag = 0;
                        z2 = {cell2mat(z)};
                        n = n + 1;        %            else
        %                flag = -1;
                    end
                elseif flag == 4 %not
                    if string(keyWord(wz, like)) ~= string(100)
                        z = {cell2mat(z) * cell2mat(keyWord(wz, like))};
                        flag = 5;
                        z2 = {cell2mat(z)};
                        n = n + 1;
                    elseif string(keyWord(wz, very))~= string(100)
                        z = {cell2mat(z) * cell2mat(keyWord(wz, very))};
                        flag = 6;
                        z2 = {cell2mat(z)};
                        n = n + 1;
                        %              else
       %                 flag = -1;
                    end
                elseif flag == 5
                    if string(keyWord(wz, very))~= string(100)
                        z = {cell2mat(z) * cell2mat(keyWord(wz, very))};
                        flag = 0;
                        z2 = {cell2mat(z)};
                        n = n + 1;
                    end
                elseif flag ==6
                    if string(keyWord(wz, like))~= string(100)
                        z = {cell2mat(z) * cell2mat(keyWord(wz, like))};
                        flag = 0;
                        z2 = {cell2mat(z)};
                        n = n + 1;
                    end
  %                  else
   %                     flag = -1;
                end
                if flag == -1
                    if string(keyWord(wz, like)) ~= string(100) 
                        z = {cell2mat(z) * cell2mat(keyWord(wz, like))};
                        flag = 1;
                        z2 = {cell2mat(z)};
                        n = n + 1;
                    elseif string(keyWord(wz, not)) ~= string(100)
                        z = {cell2mat(z)  * (-1)};
                        flag = 4;
                        z2 = {cell2mat(z)};
                        n = n + 1;
                    elseif string(keyWord(wz, very)) ~= string(100)
                        z = {cell2mat(z) * cell2mat(keyWord(wz, very))};
                        flag = 3;
                        z2 = {cell2mat(z)};
                        n = n + 1;
 %                   else
   %                     flag = -1;
                    end
                end
                
                if flag == 0
                    z1 = {cell2mat(z) + cell2mat(z1)};
                    z = {1};
                    flag = -1;
                end
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            end
        end
    end
    if n == 1
        z1 = z2;
        z = {1};
    end
    wordGrade(i,2) = {cell2mat(z1)/n};
    z1 = {1};
    z2 = {1};
    z = {1};
    flag = -1;
end