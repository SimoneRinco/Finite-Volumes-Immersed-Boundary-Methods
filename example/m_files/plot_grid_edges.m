function plot_grid_edges()

f='../data/domain/x';
x = load(f);

f='../data/domain/y';
y = load(f);

f='../data/domain/edges';
edges = load(f);

f='../data/domain/cutted_cells';
cutted_cells = load (f);


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

% plot dominio    
s = size(edges);
for i=1:s(1)
    plot([edges(i,1) edges(i,3)],[edges(i,2) edges(i,4)],'r-','Linewidth',3);
end    
    
    
end
    