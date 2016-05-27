function y = uf(x)
    
    y = 1 - abs(x)
    
endfunction

x = -1:0.1:1

plot(x,uf(x))
