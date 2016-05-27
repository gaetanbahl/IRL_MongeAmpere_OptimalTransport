function y = plus (x)
    y = max(x,0);
endfunction

function y = module(x,y)
    y = sqrt(x^2 + y^2);
endfunction

function y = f(x,y)
    y = plus(1 - 1/module(x,y));
endfunction

function y = u(x,y)
    y = (1/2)*(plus(module(x,y) - 1))^2;
endfunction

function y = uthing(x)
    y = u(x(1),x(2));
endfunction

function y = membregauche(x,y)
    [J,H] = numderivative(uthing, [x;y],[],[],"blockmat");
    y = det(H);
endfunction

Npoints = 100;

dx = 1/Npoints;
dy = dx;

xmin = -2;
xmax = 2;
ymin = -2;
ymax = 2;
z=0;
x = linspace(xmin,xmax,Npoints);
y = linspace(ymin, ymax,Npoints);

for i = 1:Npoints
    for j = 1:Npoints
        z(i,j) = u(x(i),y(j));
    end
end

//plot3d(x,y,z);

res2d = 0
for i = 1:Npoints
    res2d(i) = u(x(i),0);
end

resf = 0
for i = 1:Npoints
    resf(i) = f(x(i),0);
end

//plot(x,res2d, col="red");

derivee2 = 0;

for i = 1:Npoints
    derivee2(i) =  membregauche(x(i),0);
end
//plot(x, resf, col="green");
//plot(x,derivee2, col="blue");
sub = resf - derivee2;
//plot(x, sub);


for i = 1:10
    dh = 10^(-i);
    disp(u(1+dh, 0)/dh);
end

for i = 1:10
    dh = 10^(-i);
    disp((u(1+2*dh, 0)-2*u(1+dh,0))/dh^2);
end


for i = 0:0.01:1
    disp(i - f(1,0));
end


