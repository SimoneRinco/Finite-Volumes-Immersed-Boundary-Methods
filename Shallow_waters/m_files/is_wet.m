function w = is_wet(xl, yl, wet_cells)

w=0;

s = size(wet_cells);
s = s(1);

for i=1:s
    
    if wet_cells(i,1) == xl && wet_cells(i,2) == yl
        w=1;
    end
end
