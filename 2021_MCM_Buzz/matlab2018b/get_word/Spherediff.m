function distance = Spherediff(x,y)
R = 6378137; %地球半径
x = deg2rad(x); %度转弧度
y = deg2rad(y);
DeltaS = acos(cos(x(2))*cos(y(2))*cos(x(1)-y(1))+sin(x(2))*sin(y(2)));
distance = R * DeltaS; %米
DeltaA = atan((x(1)-y(1))*cos(y(2))/(y(2)-y(1)));
%deg = mod(DeltaA * 180/pi+360,360); %度
