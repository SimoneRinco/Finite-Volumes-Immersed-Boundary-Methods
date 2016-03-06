function [type_interp_matrices, number_matrices] = get_interp_matrices_info(SWc_x, SWc_y, number_matrices_all_wet, number_matrices_GPs, number_matrices_dry_no_GP)

for i=0 : (number_matrices_all_wet-1)
    
    count = int2str(10000+i);
	f = strcat('interp_matrices/all_wet/SW_labels/', count);
    SW_label = load(f);
    if SW_label(1)==SWc_x && SW_label(2)==SWc_y
        type_interp_matrices = 1;
        number_matrices = i;
    end
end

for i=0 : (number_matrices_GPs-1)
    
    count = int2str(10000+i);
	f = strcat('interp_matrices/GPs/SW_labels/', count);
    SW_label = load(f);
    if SW_label(1)==SWc_x && SW_label(2)==SWc_y
        type_interp_matrices = 2;
        number_matrices = i;
    end
end

for i=0 : (number_matrices_dry_no_GP-1)
    
    count = int2str(10000+i);
	f = strcat('interp_matrices/dry_no_GP/SW_labels/', count);
    SW_label = load(f);
    if SW_label(1)==SWc_x && SW_label(2)==SWc_y
        type_interp_matrices = 3;
        number_matrices = i;
    end
end


end