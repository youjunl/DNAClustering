% gamma = 0.9
ref = [0.01, 0.58, 0.84, 0.94, 0.97, 0.99, 0.997, 1, 1, 1, 1, 1, 1];
dat = [0.00, 0.00, 0.0526316, 0.157895, 0.263158, 0.315789, 0.631579, 0.684211, 0.789474, ...
    0.842105, 0.947368, 0.947368, 0.947368];
dat_10 = [0, 0.1, 0.2, 0.4, 0.4, 0.4, 0.7, 0.8, 0.9, 1, 1, 1, 1];
round = 2:2:26;
% 
figure(1)
hold on
plot(round, dat, '-*');
plot(round, dat_10, '-s');
plot(round, ref, '-^');
legend('Reporduced - 510 Reads 20 Centers', 'Reporduced - 297 Reads 10 Centers','Paper - 5.3B reads, 10 Centers')
title('Distributed Convergence (\gamma = 0.9)')
xlabel('Communication Rounds')
ylabel('Accuracy')
xlim([2, 26]);
grid on

figure(2)
hold on

gamma = [0.7, 0.8, 0.9, 1.0];
result = [0.947368, 0.947368, 0.947368, 0.842105];

ref_axis = [0.6, 0.7, 0.8, 0.9, 1];
ref_alg = [1, 1, 0.98, 0.9, 0.75];
ref_scd8 = [1, 0.99, 0.96, 0.89, 0.61];
ref_scd6 = [0.98, 0.94, 0.84, 0.53, 0.31];

plot(gamma, result, '-*');
plot(ref_axis, ref_alg, '-s');
plot(ref_axis, ref_scd8, '-^');
plot(ref_axis, ref_scd6, '-o');

legend('Reproduced', 'Paper, proposed alg', 'Paper, Starcode(d=8)', 'Paper, Starcode(d=6)');
grid on
title('Accuracy Comparison')
xlabel('\gamma')
ylabel('Accuracy')