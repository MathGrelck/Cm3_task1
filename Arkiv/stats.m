clear all
close all
clc


%% Left side run with modified code
clc
lm = csvread('left_mod.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 9:0.1:11;
y = pdf(pd,x);


figure(1);
subplot(2,2,1)
hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
title('Left side with modified code')
line([pd.mu pd.mu],[0 3],'LineStyle','--','Color','k');
xlim([9.5 11])
xlabel('time [s]')
ylabel('samples')
hold off

%% Left side run with original code
clc
lm = csvread('left.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 13:0.1:15;
y = pdf(pd,x);

subplot(2,2,2)
hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
line([pd.mu pd.mu],[0 3],'LineStyle','--','Color','k');
title('Left side with original code')
xlim([13 15])
xlabel('time [s]')
ylabel('samples')
hold off

%% Right side run with modified code
clc
lm = csvread('right_mod.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 8.5:0.1:10.5;
y = pdf(pd,x);

subplot(2,2,3)
hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
line([pd.mu pd.mu],[0 3],'LineStyle','--','Color','k');
title('Right side with modified code')
xlim([8.5 10.5])
xlabel('time [s]')
ylabel('samples')
hold off
%% Right side run with original code
clc
lm = csvread('right.dat');

%normplot(lm)

pd = fitdist(lm,'Normal')

x = 11:0.1:18;
y = pdf(pd,x);

subplot(2,2,4)
hist(lm,x)
hold on
plot(x,y,'LineWidth',2,'Color','r')
line([pd.mu pd.mu],[0 3],'LineStyle','--','Color','k');
title('Right side with original code')
xlim([11 16])
xlabel('time [s]')
ylabel('samples')
hold off
