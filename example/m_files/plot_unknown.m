function plot_unknown(unknown_number, plot_surf, plot_contour, file_name)
%
%
% function plot_unknown(unknown_number,  plot_surf, plot_contour, file_name)
%
% Plot dell'incognita unknown_number. 
% plot_surf e plot_contour: eseguono il plot rispettivamente della superficie e/o del contorno se poste pari ad 1
% file_name = ../data/unknowns/unknowns per default

f='../data/domain/x';
x = load(f);
dx = x(2)-x(1);
size_x = length(x);

f='../data/domain/y';
y = load(f);
dy = y(2)-y(1);
size_y = length(y);

if nargin == 1
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

Z = NaN(length(y), length(x));

for i=1:np
    Z(wet_cells(i,2)+1, wet_cells(i,1)+1) = U(i,unknown_number);
end

if plot_surf == 1
plot_grid_edges();
h=surf (xc,yc,Z);
%set(h,'EdgeColor','flat')
caxis([0.8 2.2]);
axis([-1.1 1.1 -1.1 1.1 0.0 2.3]);
view([30,50]);

shading interp;
light('Position',[-2,2,20]);
lighting phong;
%set(h,'EdgeColor','flat')
%set(h,'FaceLighting','phong');

colorbar;
hold off;
end

if plot_contour == 1
plot_grid_edges();
hold on;
contourf(xc,yc,Z);
colorbar;
end








