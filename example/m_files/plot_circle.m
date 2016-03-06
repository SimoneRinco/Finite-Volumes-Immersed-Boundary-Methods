function plot_circle(r)

N = 1000;

x=[];
y=[];

for i=1:N
    
    x(i)= r*cos(2*pi*(i-1)/(N-1));
    y(i)= r*sin(2*pi*(i-1)/(N-1));
    
end

plot(x,y,'r-');