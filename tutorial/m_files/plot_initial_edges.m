function plot_initial_edges(cell)

%
% Funzione utile solo nel caso 'level set'. Vengono plottati i lati
% originali creati a partire dal level set, che andranno successivamente
% uniti per formare il dominio.
%
% function plot_initial_edges(cell)
%
% cell = vettore [cell_x cell_y] per l'evidenzazione in rosso della cella
% selezionata
%


f='../data/domain/initial_edges';
initial_edges = load (f);

f='../data/domain/x';
x = load(f);

f='../data/domain/y';
y = load(f);


x_min = min(x);
x_max = max(x);
y_min = min(y);
y_max = max(y);


n_initial_edges = size(initial_edges);
n_initial_edges = n_initial_edges(1);


figure()
axis([x_min x_max y_min y_max]);
hold on;
for i=1:length(x)
    plot([x(i) x(i)], [y_min y_max], 'k-');
end
    
    for i=1:length(y)
        plot([x_min x_max], [y(i) y(i)], 'k-');
    end
    
    dx = x(2)-x(1);
    dy = y(2)-y(1);
    
    plot(x(cell(1)+1)+0.5*dx, y(cell(2)+1)+0.5*dy,'rs');
    
for i=1:n_initial_edges

    lx = initial_edges(i,1);
    ly = initial_edges(i,2);
    v1x = initial_edges(i,3);
    v1y = initial_edges(i,4);
    v2x = initial_edges(i,5);
    v2y = initial_edges(i,6);
    
    plot([v1x v2x],[v1y v2y],'b-*');
    
end
end
