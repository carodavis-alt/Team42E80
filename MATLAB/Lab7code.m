%Author: Ali El Seblani
%Date: Mar 24, 2026
%Objective: 
%Includes plot of depth and depth_des, overlaid on the same y-axis vs. time on the x-axis. 
%Includes plot of uV vs. time with appropriate labels and units.
%Includes appropriate labels, units and legends.

% logreader.m
% Use this script to read data from your micro SD card

clear;
%clf;

filenum = '009'; % file number for the data you want to read
infofile = strcat('INF', filenum, '.TXT');
datafile = strcat('LOG', filenum, '.BIN');

%% map from datatype to length in bytes
dataSizes.('float') = 4;
dataSizes.('ulong') = 4;
dataSizes.('int') = 4;
dataSizes.('int32') = 4;
dataSizes.('uint8') = 1;
dataSizes.('uint16') = 2;
dataSizes.('char') = 1;
dataSizes.('bool') = 1;

%% read from info file to get log file structure
fileID = fopen(infofile);
items = textscan(fileID,'%s','Delimiter',',','EndOfLine','\r\n');
fclose(fileID);
[ncols,~] = size(items{1});
ncols = ncols/2;
varNames = items{1}(1:ncols)';
varTypes = items{1}(ncols+1:end)';
varLengths = zeros(size(varTypes));
colLength = 256;
for i = 1:numel(varTypes)
    varLengths(i) = dataSizes.(varTypes{i});
end
R = cell(1,numel(varNames));

%% read column-by-column from datafile
fid = fopen(datafile,'rb');
for i=1:numel(varTypes)
    %# seek to the first field of the first record
    fseek(fid, sum(varLengths(1:i-1)), 'bof');
    
    %# % read column with specified format, skipping required number of bytes
    R{i} = fread(fid, Inf, ['*' varTypes{i}], colLength-varLengths(i));
    eval(strcat(varNames{i},'=','R{',num2str(i),'};'));
end
fclose(fid);

figure(1)
subplot(2,1,2)
plot(depth,'m x')
hold on
plot(depth_des, 'r o')
ylabel("Depth [m]")
xlabel("time [s]")
title("Depth vs Depth desired")
subplot(2,2,2)
plot(uV,"g x")
ylabel("Verticle Motor Control Effort")
xlabel("time[s]")
title("uV vs. time")
