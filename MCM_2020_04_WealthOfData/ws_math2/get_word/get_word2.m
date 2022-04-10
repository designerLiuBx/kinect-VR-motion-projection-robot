function simp_real_pacifier = get_word2(simp_real_pacifier)
line_column = size(simp_real_pacifier);
line = line_column(1);
column = line_column(2);
%ismissing_pacifier = ismissing(simp_real_pacifier);
for i = 1 : line
    for j = 2 : column
        %if ismissing_pacifier(i, j) == 0
        if  ~isempty(simp_real_pacifier{i, j})
            char_simp_real_pacifier = char(string(simp_real_pacifier(i, j)));
            n = size(char_simp_real_pacifier);
            n = n(2);
            for n = 1 : n
                if  abs(char_simp_real_pacifier(1,n)) >= 97 && abs(char_simp_real_pacifier(1,n))<= 122                    %abs(char_simp_real_pacifier(1,n));
                elseif  abs(char_simp_real_pacifier(1,n))>= 65 && abs(char_simp_real_pacifier(1,n))<= 90
                    char_simp_real_pacifier(1, n) = lower(char_simp_real_pacifier(1,n));
                else
                    char_simp_real_pacifier(1, n) = NaN;
                    char_simp_real_pacifier = deblank( char_simp_real_pacifier);
                end
                simp_real_pacifier(i, j) = {char_simp_real_pacifier};
            end
        end
    end
end
