function mcmall = dis_grade(mcmall,aaa)

xy =[
 [49.149394 -123.943134],
 [48.993892 -122.702242],
 [48.971949 -122.700941],
 [49.025831 -122.810653],
 [48.980994 -122.688503]
];
xy2=[
   [9.80969000000016   ,-109.21506784387132],
   [21.318769999999958 ,-108.94022031255818],
   [40.90459999999979  ,-109.41133507675173],
   [24.40625000000047  ,-107.27859183850549],
   [18.163750000000363 ,-109.84731041492198],
   [18.16914000000004  ,-109.84779092393053],
   [18.174419999999998 ,-109.84782563019218],
   [18.688669999999874 ,-109.80716107151578],
   [18.262089999999844 ,-109.79973403513048]
];
zx = sum(xy)/5;
line_column = size(mcmall);
line = line_column(1);
column = line_column(2);
grade_dis = (zeros(line,1)+1)*100; %距离初始分100分

mcmall_pos = cell2mat(mcmall(:,7:8));

line_column = size(mcmall_pos);
line2 = line_column(1);
column2 = line_column(2);
mcmall_pos_real = zeros(line2,column2+1);
for i = 1:line2
    for j = 1:column2
        if j == 1
%            x = mcmall_pos(i,j);
%            x=110*(x-49.149394);    
            mcmall_pos_real(i,j) = mcmall_pos(i,j);;
        else
%            y = mcmall_pos(i,j);
%            y = 110*cos(y-123.943134);
            mcmall_pos_real(i,j) = mcmall_pos(i,j);;
        end
    end
end
pos = zeros(line2, column2+1);
x = 0;
y = 0;
for i = 1:line2
    pos(i,:) = mcmall_pos_real(i,:);
    x = pos(i,1);
    y = pos(i,2);
%    pos(i,3) = sqrt( (x - zx(1))^2 + (y - zx(2))^2 );
    pos(i,3) = Spherediff(pos(i,1:2), zx);    
end
mcmall_pos_real(:,3) = pos(:,3);
c = 1.8133;
cont = randperm(15);
year = sort(cont)+2017;
area = zeros(length(year),3);
area(:,1) = year';
R = 6378137; %地球半径

for i = 1:length(year)
    km2 = 24710000./(1+(24710000/12.566 - 1)*exp(-c*(i - 1)));
    h = km2/(2*pi*R);
    h1 = R - h;
    r = acos(h1/R)*R;
    area(i,2) = km2;
    area(i,3) = r;
end

time_list = area(:,1);
for i = 1:line2
    time = strsplit(string(mcmall(i,6)), '-');
    if max(strcmp(string(time_list(:,1)), time(1)))
        wz = find(strcmp( string(time_list(:,1)), time(1) ));
        ar = area(wz,3);
        dis = mcmall_pos_real(i,3);
        if dis<ar
            mcmall(i, 12) = 100;
        else
            mcmall(i, 12) = {100- (dis - ar)/(aaa*1000)};
        end
    end
end

    
