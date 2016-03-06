function plot_Q(file_name)

%
%
% function plot_Q(file_name)
% 
% plot della portata in ternimi vettoriali.
% file_name = '../data/unknowns/unknowns' per default
%
%

f='../data/domain/x';
x = load(f);
dx = x(2)-x(1);

f='../data/domain/y';
y = load(f);
dy = y(2)-y(1);

if nargin == 0
file_name ='../data/unknowns/unknowns';
end
U = load(file_name);

f='../data/domain/wet_cells';
wet_cells = load(f);

np = size(U);
np = np(1);


%%% versione con i nan

xc = x+0.5*dx;
yc = y+0.5*dy;

% display variabiles

xQ = [];
yQ = [];
u = [];
v = [];


for i=1:np
    xQ(i) = xc(wet_cells(i,1)+1);
    yQ(i) = yc(wet_cells(i,2)+1);
    u(i) = U(i,2);
    v(i) = U(i,3);   
end

plot_grid_edges();
quiver(xQ, yQ, u, v);
