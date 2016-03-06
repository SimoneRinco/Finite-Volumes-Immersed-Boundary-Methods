function c8 = termine_noto8(SWc_x, SWc_y, unknowns, wet_cells)

c8 = zeros(8,1);

if is_wet(SWc_x, SWc_y, wet_cells)
    c8(1,1) = get_unknown(SWc_x, SWc_y, unknowns, 2);
    c8(2,1) = get_unknown(SWc_x, SWc_y, unknowns, 3);
else
    c8(1,1) = 0.0;
    c8(2,1) = 0.0;
end

if is_wet(SWc_x+1, SWc_y, wet_cells)
    c8(3,1) = get_unknown(SWc_x+1, SWc_y, unknowns, 2);
    c8(4,1) = get_unknown(SWc_x+1, SWc_y, unknowns, 3);
else
    c8(3,1) = 0.0;
    c8(4,1) = 0.0;
end

if is_wet(SWc_x+1, SWc_y+1, wet_cells)
    c8(5,1) = get_unknown(SWc_x+1, SWc_y+1, unknowns, 2);
    c8(6,1) = get_unknown(SWc_x+1, SWc_y+1, unknowns, 3);
else
    c8(5,1) = 0.0;
    c8(6,1) = 0.0;
end

if is_wet(SWc_x, SWc_y+1, wet_cells)
    c8(7,1) = get_unknown(SWc_x, SWc_y+1, unknowns, 2);
    c8(8,1) = get_unknown(SWc_x, SWc_y+1, unknowns, 3);
else
    c8(7,1) = 0.0;
    c8(8,1) = 0.0;
end

