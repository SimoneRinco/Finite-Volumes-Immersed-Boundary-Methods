function [u_E, BP_E, RP_E, n_E, M1_E, M2_E, u_W, BP_W, RP_W, n_W, M1_W, M2_W, u_N, BP_N, RP_N, n_N, M1_N, M2_N, u_S, BP_S, RP_S, n_S, M1_S, M2_S] = debug_get_stencil_values(xl, yl, n_unknown, number_matrices_all_wet, number_matrices_GPs, number_matrices_dry_no_GP)


%
% function [u_E, BP_E, RP_E, n_E, M1_E, M2_E, u_W, BP_W, RP_W, n_W, M1_W, M2_W, u_N, BP_N, RP_N, n_N, M1_N, M2_N, u_S, BP_S, RP_S, n_S, M1_S, M2_S] = debug_get_stencil_values(xl, yl, n_unknown, number_matrices_all_wet, number_matrices_GPs, number_matrices_dry_no_GP)
% 
% Funzione che ritorna tutte le quantità associate alle celle E,W,N,S adiacenti alla cella corrente (bagnata) xl,yl.
%
% PARAMETRI D'USCITA
%
% u_ = valore dell'incognita nella cella adiacente;
% BP_ = punto di bordo (nel caso di ghost cell). Se la cella è bagnata
% viene ritornato [];
% RP_ = punto riflesso;
% n_ = vettore [nx ny] versore normale uscente al bordo;
% M1_ = matrice 4x4 o 3x3 (a seconda del tipo di quadrato di interpolazione) associata alla ghost cell per il calcolo di Z. Se
% la cella è bagnata viene ritornato [];
% M2_ = matrice 4x4, 8x8 o 6x6 associata alla ghost cell per il calcolo di
% Qx e Qy;
%
% PARAMETRI D'INGRESSO
%
% xl, yl = cella centrale (bagnata): componenti x e y;
% n_unknown = numero dell'incognita da plottare;
% number_matrices_all_wet = numero di matrici di interpolazione nel caso di
% vertici di interpolazione tutti bagnati;
% number_matrices_GPs = numero  di matrici di interpolazione nel caso ci
% sia almeno un vertice di interpolazione che sia un GP e nessun vertice
% che sia asciutto ma non GP;
% number_matrices_dry_no_GP = numero di matrici di interpolazione nel caso
% ci sia un vertice di interpolazione asciutto e non GP.

% le matrici di interpolazione vengono fornite nel formato [M c] dove
% M è la matrice di interpolazione e c il termine noto

u_E = [];
BP_E = [];
RP_E = [];
n_E = [];
M1_E = [];
M2_E = [];

u_W = [];
BP_W = [];
RP_W = [];
n_W = [];
M1_W = [];
M2_W = [];

u_N = [];
BP_N = [];
RP_N = [];
n_N = [];
M1_N = [];
M2_N = [];

u_S = [];
BP_S = [];
RP_S = [];
n_S = [];
M1_S = [];
M2_S = [];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% caricamento di tutti i file necessari

f = '../data/domain/wet_cells';
wet_cells = load(f);

f = '../data/unknowns/unknowns';
unknowns = load(f);

f = '../data/domain/ghost_cells';
ghost_cells = load(f);

% Trovare se la cella (xl, yl) è bagnata

w = is_wet(xl, yl, wet_cells);

if w == 0
    disp('Cella centrale non bagnata');
else
    
    disp('cella EST:');
    [u_E, BP_E, RP_E, n_E, M1_E, M2_E] = analisi_cella(xl, yl, xl+1, yl, wet_cells, ghost_cells, unknowns, n_unknown, number_matrices_all_wet, number_matrices_GPs, number_matrices_dry_no_GP);
    disp('cella WEST:');
    [u_W, BP_W, RP_W, n_W, M1_W, M2_W] = analisi_cella(xl, yl, xl-1, yl, wet_cells, ghost_cells, unknowns, n_unknown, number_matrices_all_wet, number_matrices_GPs, number_matrices_dry_no_GP);
    disp('cella NORD:');
    [u_N, BP_N, RP_N, n_N, M1_N, M2_N] = analisi_cella(xl, yl, xl, yl+1, wet_cells, ghost_cells, unknowns, n_unknown, number_matrices_all_wet, number_matrices_GPs, number_matrices_dry_no_GP);
    disp('cella SUD:');
    [u_S, BP_S, RP_S, n_S, M1_S, M2_S] = analisi_cella(xl, yl, xl, yl-1, wet_cells, ghost_cells, unknowns, n_unknown, number_matrices_all_wet, number_matrices_GPs, number_matrices_dry_no_GP);
    
    u = get_unknown(xl, yl, wet_cells, unknowns, n_unknown);
    
    disp('# Stencil values for selected unknown:');
    s = sprintf('central: %d', u);
    disp(s);
    s = sprintf('E: %d', u_E);
    disp(s);
    s = sprintf('W: %d', u_W);
    disp(s);
    s = sprintf('N: %d', u_N);
    disp(s);
    s = sprintf('S: %d', u_S);
    
end

end
        
        
        
    
    
    
    
    