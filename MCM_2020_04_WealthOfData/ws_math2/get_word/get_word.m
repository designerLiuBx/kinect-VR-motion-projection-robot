function bobySimpReal_pacifier = get_word(data,q)
line_column = size(data);
line = line_column(1);
column = line_column(2);
n = 0;
for i = 1 : line
    cell_z = data(i,q);
    simp_real_pacifier(i,1) = cell_z;
    string_z = string(cell_z);
    strsplit_z = strsplit(string_z);
    n = size(strsplit_z);
    n = n(2);
    for m = 1 : n
        simp_real_pacifier(i,m+1) = {strsplit_z(m)};
    end
end

bobySimpReal_pacifier = get_word2(simp_real_pacifier);
%word_pacifier =word_histogram(simp_real_pacifier);

        