function word_pacifier = word_histogram(simp_real_pacifier)
line_column = size(simp_real_pacifier);
line = line_column(1);
column = line_column(2);
m = 0;
word_pacifier = cell(1,1);
word_pacifier(1,1) = {"START"};
word_pacifier(2,1) = {0};
for i = 1: line
    for j = 2 : column
    
        if ~isempty(simp_real_pacifier{i, j})
            z = simp_real_pacifier(i, j);
            n = size(word_pacifier);
            n = n(2);
            if string(word_pacifier(1, n))=="START" && n==1
                m = 1;
                word_pacifier(1, 1) = {simp_real_pacifier(i, j)};
                word_pacifier(2, m) = {1};
            else
                y = 0;

                for q = 1 : n
                    if string(word_pacifier(1, q)) == string(simp_real_pacifier(i, j))
                        word_pacifier(2,q) = {str2num(string(word_pacifier(2,q))) + 1};
                        y = 1;
                        break
                    end
                end
                if y==0
                    m = m + 1;
                    word_pacifier(1, m) = {simp_real_pacifier(i, j)};
                    word_pacifier(2, m) = {1};
                    
                end
            end
        end
    end
end
