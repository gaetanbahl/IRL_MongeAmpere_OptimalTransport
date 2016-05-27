u = fscanfMat("/home/blacky/Documents/2A/IRL/progs/ClImplem/rescl.txt")

[Npoints,mhh] = size(u)
matDy = zeros(Npoints, Npoints)
matDx = zeros(Npoints, Npoints)
matDxx = zeros(Npoints, Npoints)
matDyy = zeros(Npoints, Npoints)
matDxy = zeros(Npoints, Npoints)
rhoYmat = zeros(Npoints, Npoints)
detmat = zeros(Npoints, Npoints)

xmin = -1
xmax = 1
ymin = -1
ymax = 1
h = (xmax - xmin)/Npoints
x = linspace(xmin, xmax, Npoints)
y = linspace(ymin, ymax, Npoints)

matDxx(2:(Npoints-1), 2:(Npoints-1))  = (1/h^2) * (u(3:Npoints,2:(Npoints-1)) + u(1:(Npoints-2),2:(Npoints-1)) - 2*u(2:(Npoints-1),2:(Npoints-1)));
matDyy(2:(Npoints-1), 2:(Npoints-1)) = (1/h^2) * (u(2:(Npoints-1),3:Npoints) + u(2:(Npoints-1),1:(Npoints-2)) - 2*u(2:(Npoints-1),2:(Npoints-1)));
matDxy(2:(Npoints-1), 2:(Npoints-1)) = (1/(4 *h^2)) * (u(3:Npoints,3:Npoints) + u(1:(Npoints-2),1:(Npoints-2)) - u(1:(Npoints-2),3:Npoints) - u(3:Npoints,1:(Npoints-2)));
matDx(2:(Npoints-1), 2:(Npoints-1)) = (1/(2*h)) * (u(3:Npoints,2:(Npoints-1)) - u(1:(Npoints-2),2:(Npoints-1)));
matDy(2:(Npoints-1), 2:(Npoints-1)) = (1/(2*h)) * (u(2:(Npoints-1),3:Npoints) - u(2:(Npoints-1),1:(Npoints-2)));
    

function z = rhoY(x,y)
//    z = 1
    z =  exp(- norm([x+0.3,y-0.3]) ^2 / 0.1) +0.1;
    //z =  (Npoints^2)/66.051535  * exp(- norm([x,y]) ^2 / 0.1) +1;
    
    //z =  0.1 + exp(- norm([x,y]) ^2 / 0.1);
//    
//    if sqrt((x+0.2)^2 + (y-0.2)^2) < 0.3 then
//        z = 2
//    else
//        z = 1
//    end
//    
endfunction

for i = 2:(Npoints-1)
        for j = 2:(Npoints -1)
            rhoYmat(i,j) = rhoY(matDx(i,j), matDy(i,j)) ;
            detmat(i,j) = det([Dxx(u,i,j,h) Dxy(u,i,j,h) ; Dxy(u,i,j,h) Dyy(u,i,j,h)]);
        end
    end
clf()
res = rhoYmat .* detmat
plot3d(x(2:(Npoints-1)),y(2:(Npoints-1)),res((2:(Npoints-1)),(2:(Npoints-1))))
//clf()
//champ1(x,y,matDx, matDy)
