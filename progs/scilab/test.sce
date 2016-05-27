function z = rhoX(x,y)
    //z = 1
    z =  (Npoints^2)/66.051535  * exp(- norm([x,y]) ^2 / 0.1) +1;
endfunction


function z = rhoY(x,y)
    z = 9 * rhoX(x^3,y^3) * x^2 * y^2 
endfunction

function z = u(x,y)
    z = (x^4 + y^4)/4
endfunction

function z = graduX(x)
    z = x^3
endfunction

Npoints = 30

x = linspace(-1,1,Npoints)
y = x

matgraduX = 0
matgraduY = 0
gradidX = 0
gradidY = 0
u = 0
for i = 1:Npoints
    for j = 1:Npoints
        matgraduX(i,j) = graduX(x(i))
        matgraduY(i,j) = graduX(y(j))
        gradidX(i,j) = x(i);
        gradidY(i,j) = y(j);
    end
end
clf()
champ1(x,y,matgraduX,matgraduY)

clf()

fplot3d(x,y,rhoY)

temps = linspace(0,1,100)
res = 0
for t = temps
    drawlater
    for i = 1:Npoints
    for j = 1:Npoints
        res(i,j) = ((1-t) + t*9)* rhoX((1-t) *gradidX(i,j) + t*matgraduX(i,j), (1-t) *gradidY(i,j) + t*matgraduY(i,j)) *((1-t) + t * x(i)^2) *((1-t) + t *  y(j)^2)
    end
    
end
    clf()
    plot3d(x,y,res)
    s=gca()
    
    s.data_bounds = [-1 1 ; -1 1 ; 0 24]'
    drawnow
    sleep(100)
end






