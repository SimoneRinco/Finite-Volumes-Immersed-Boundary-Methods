function z_frames(N_frames)


output_file = '../data/z_frames/';
data = '../data/unknowns/frame_';

for i = 0:N_frames

i
is = int2str(i);
plot_unknown(1, 1, 0, strcat(data,is));
caxis([0.8 2.2]);
axis([-10.1 10.1 -5.1 5.1 0.0 2.3]);
view([30,50]);
colorbar;
iss = int2str(i+10000);
print ('-dpng', '-r0', strcat(output_file,iss));
close;

end