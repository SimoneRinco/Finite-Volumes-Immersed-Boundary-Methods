function Q_frames(N_frames)


output_file1 = '../data/Qx_frames/';
output_file2 = '../data/Qy_frames/';
data = '../data/unknowns/';

for i = 0:N_frames

is = int2str(i+10000);
plot_unknown(2, 1, 0, strcat(data,is));
caxis([-1.1 1.1]);
axis([-1.1 1.1 -1.1 1.1 -1.1 1.1]);
view([30,50]);
colorbar;
print ('-dpng', '-r0', strcat(output_file1,is));
close;

plot_unknown(3, 1, 0, strcat(data,is));
caxis([-1.1 1.1]);
axis([-1.1 1.1 -1.1 1.1 -1.1 1.1]);
view([30,50]);
colorbar;
print ('-dpng', '-r0', strcat(output_file2,is));
close;


end