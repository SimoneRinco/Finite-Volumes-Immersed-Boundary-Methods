function plot_domain(edge_number, ghost_cell, cell, plot_real_domain)

%
% function plot_domain(edge_number, ghost_cell, cell, plot_real_domain)
%
% edge_number = numero del lato da visualizzare (colorato in magenta)
% ghost_cell = numero della ghost cell da visualizzare (punti riflessi, di
% bordo e quadrati di interpolazione).
% cell = vettore riga [cell_x cell_y] che indica il centro cella da
% visualizzare (quadrato blu).
% plot_real_domain = variabile booleana da usare solo nel caso 'vertices'.
% Se uguale ad 1 viene plottato in blu il vero dominio dato per
% punti.
%

% Solo per il caso level set. 
plot_exact_ls = 0;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% variabili booleane che stabiliscono il plot della grandezza indicata
plot_edge = 1;
plot_ghost_cell = 1;
plot_cell = 1;

% Acquisizione dei dati

f='../data/domain/x';
x = load(f);

f='../data/domain/y';
y = load(f);

f='../data/domain/edges';
edges = load(f);

if plot_real_domain == 1
f='../data/domain/real_vertices';
real_vertices = load(f);
end

f='../data/domain/cutted_cells';
cutted_cells = load (f);

f='../data/domain/wet_cells';
wet_cells = load(f);

f='../data/domain/ghost_cells';
ghost_cells = load (f);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% plot della griglia, celle bagnate, ghost cells, lato indicato

x_min = min(x);
x_max = max(x);
y_min = min(y);
y_max = max(y);

figure()
axis([x_min x_max y_min y_max]);
hold on;

% plot griglia
for i=1:length(x)
    plot([x(i) x(i)], [y_min y_max], 'k-');
end
    
    for i=1:length(y)
        plot([x_min x_max], [y(i) y(i)], 'k-');
    end
% end plot griglia

% plot edges
s = size(edges);
for i=1:s(1)
    plot([edges(i,1) edges(i,3)],[edges(i,2) edges(i,4)],'g*-');
end
% end plot edges

% plot vero dominio
if plot_real_domain == 1
    s = size(real_vertices);
    for i=1:(s-1)
        plot([real_vertices(i,1) real_vertices(i+1,1)], [real_vertices(i,2) real_vertices(i+1,2)], 'b*-');
    end
end
% end plot vero dominio

% plot lato indicato (magenta)
if plot_edge == 1
    j=edge_number;
     
    cell_x = cutted_cells(j,1);
    cell_y = cutted_cells(j,2);

    plot(0.5*(x(cell_x+1)+x(cell_x+2)), 0.5*(y(cell_y+1)+y(cell_y+2)), 'm*');
    plot([edges(j,1) edges(j,3)],[edges(j,2) edges(j,4)],'m*-');
end
% end plot lato indicato 

% plot wet cells (centri cella in azzurro)
s = size(wet_cells);
    for i=1:s(1)
        plot(0.5*(x(wet_cells(i,1)+1)+x(wet_cells(i,1)+2)),0.5*(y(wet_cells(i,2)+1)+y(wet_cells(i,2)+2)),'c*');
    end
% end plot wet cells  
   
% plot ghost cells (centri cella in rosso)
s = size(ghost_cells);
    for i=1:s(1)
        plot(0.5*(x(ghost_cells(i,1)+1)+x(ghost_cells(i,1)+2)),0.5*(y(ghost_cells(i,2)+1)+y(ghost_cells(i,2)+2)),'r*');
    end
% end plot ghost cells

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
    
% plot exact level set.
% Definire il contorno (level set) da plottare

if plot_exact_ls == 1

n_points=2000;
xls=zeros(1,n_points);
yls=zeros(1,n_points);

lambda1 = 1.382;
lambda2 = 3.618;
r = 2;

theta = 2*pi/(n_points+1);

for i=1:(n_points+1)
%    r = sqrt(2+cos(10*i*theta));
%    xls(i) = r*cos(i*theta);
%    yls(i) = r*sin(i*theta);
    z1 = sqrt(r/lambda1)*cos(i*theta);
    z2 = sqrt(r/lambda2)*sin(i*theta);
    
    xls(i) = -0.8507*z1 + 0.5257*z2;
    yls(i) = 0.5257*z1 + 0.8507*z2;


end

plot(xls,yls,'b-', 'LineWidth', 1);

end % if plot_exact_ls == 1

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

% plot del centro cella indicato
if plot_cell == 1
plot(0.5*(x(cell(1)+1)+x(cell(1)+2)), 0.5*(y(cell(2)+1)+y(cell(2)+2)), 'bs');    
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% plot della ghost cell indicata
if plot_ghost_cell == 1      
    i = ghost_cell;
   
    col = ghost_cells(i,1);
    row = ghost_cells(i,2);
        
    xc = 0.5*(x(col+1)+x(col+2));
    yc = 0.5*(y(row+1)+y(row+2));
        
    n_BP = ghost_cells(i,4);
        
    for j = 1:n_BP
         BPx = ghost_cells(i, 5+(j-1)*12);
         BPy = ghost_cells(i, 6+(j-1)*12);
         RPx = ghost_cells(i, 7+(j-1)*12);
         RPy = ghost_cells(i, 8+(j-1)*12);
         SWCx = ghost_cells(i, 11+(j-1)*12);
         SWCy = ghost_cells(i, 12+(j-1)*12);
         assWCx = ghost_cells(i, 13+(j-1)*12);
         assWCy = ghost_cells(i, 14+(j-1)*12);
     
         plot([xc BPx RPx], [yc BPy RPy], 'y-*');
         Lx = 0.5*(x(SWCx+1)+x(SWCx+2)); 
         Rx = 0.5*(x(SWCx+2)+x(SWCx+3));
         By = 0.5*(y(SWCy+1)+y(SWCy+2)); 
         Ty = 0.5*(y(SWCy+2)+y(SWCy+3));
                         
         plot([Lx Rx Rx Lx Lx],[By By Ty Ty By],'y--');
         plot(0.5*(x(assWCx+1)+x(assWCx+2)),0.5*(y(assWCy+1)+y(assWCy+2)),'k*');
        end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

end % function 












