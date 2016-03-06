function c4 = termine_noto4(SWc_x, SWc_y, unknowns, n_unknown, wet_cells)

c4 = zeros(4,1);
if is_wet(SWc_x, SWc_y, wet_cells)
    c4(1,1) = get_unknown(SWc_x, SWc_y, unknowns, n_unknown);
else
    c4(1,1) = 0.0; % questo caso riguarda solo l'altezza totale nel caso GPs
end

if is_wet(SWc_x+1, SWc_y, wet_cells)
    c4(2,1) = get_unknown(SWc_x+1, SWc_y, unknowns, n_unknown);
else
    c4(2,1) = 0.0; % questo caso riguarda solo l'altezza totale nel caso GPs
end

if is_wet(SWc_x+1, SWc_y+1, wet_cells)
    c4(3,1) = get_unknown(SWc_x+1, SWc_y+1, unknowns, n_unknown);
else
    c4(3,1) = 0.0; % questo caso riguarda solo l'altezza totale nel caso GPs
end

if is_wet(SWc_x, SWc_y+1, wet_cells)
    c4(4,1) = get_unknown(SWc_x, SWc_y+1, unknowns, n_unknown);
else
    c4(4,1) = 0.0; % questo caso riguarda solo l'altezza totale nel caso GPs
end


end