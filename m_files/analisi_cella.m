function [u, BP, RP, n, M1, M2] = analisi_cella(clx, cly, adjlx, adjly, wet_cells, ghost_cells, unknowns, n_unknown, number_matrices_all_wet, number_matrices_GPs, number_matrices_dry_no_GP)

    u = [];
    BP = [];
    RP = [];
    n = [];
    M1 = [];
    M2 = [];

    w = is_wet(adjlx, adjly, wet_cells);
    if w == 1
        disp('wet');
        u = get_unknown(adjlx, adjly, wet_cells, unknowns, n_unknown);
    else
        disp('ghost');
        gcq = get_ghost_cell(adjlx, adjly, ghost_cells);
        
        if gcq(9) == clx && gcq(10) == cly
            SWc_x=gcq(7);
            SWc_y=gcq(8);
            BP=[gcq(1) gcq(2)];
            RP=[gcq(3) gcq(4)];
            n=[gcq(5) gcq(6)];
        else
            if gcq(21) == clx && gcq(22) == cly
                SWc_x=gcq(19);
                SWc_y=gcq(20);
                BP=[gcq(13) gcq(14)];
                RP=[gcq(15) gcq(16)];
                n=[gcq(17) gcq(18)];
            end
        end
        
        [type_interp_matrices, number_matrices] = get_interp_matrices_info(SWc_x, SWc_y, number_matrices_all_wet, number_matrices_GPs, number_matrices_dry_no_GP);
            
        if type_interp_matrices == 1
            str = int2str(10000+number_matrices);
            c4 = termine_noto4(SWc_x, SWc_y, unknowns, n_unknown, wet_cells);
            M1 = load(strcat('./interp_matrices/all_wet/M4x4/',str));
            M1 = [M1 c4];
            M2 = M1; % questa matrice non serve (caso all_wet)
        else
            if type_interp_matrices == 2
                str = int2str(10000+number_matrices);
                
                c4 = termine_noto4(SWc_x, SWc_y, unknowns, n_unknown, wet_cells);
                M1 = load(strcat('./interp_matrices/GPs/M4x4/',str));
                M1 = [M1 c4];
                
                c8 = termine_noto8(SWc_x, SWc_y, unknowns, wet_cells);
                M2 = load(strcat('./interp_matrices/GPs/M8x8/',str));
                M2 = [M2 c8];
                
            else
                if type_interp_matrices == 3
                str = int2str(10000+number_matrices);
                M1 = load(strcat('./interp_matrices/dry_no_GP/M3x3/',str));
                M2 = load(strcat('./interp_matrices/dry_no_GP/M6x6/',str)); 
                end
            end
        end
    end
    
end