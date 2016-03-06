function gcq = get_ghost_cell(xl, yl, ghost_cells)

s=size(ghost_cells);
s=s(1);

for i=1:s
    if ghost_cells(i,1) == xl && ghost_cells(i,2) == yl
        gcq = ghost_cells(i, 5:40);
    end
end
