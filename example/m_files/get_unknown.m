function u = get_unknown(xl, yl, wet_cells, unknowns, n_unknown)

s=size(wet_cells);
s=s(1);
for i=1:s
    if (wet_cells(i,1)==xl && wet_cells(i,2)==yl)
        u = unknowns(i, n_unknown);
    end
end
end
