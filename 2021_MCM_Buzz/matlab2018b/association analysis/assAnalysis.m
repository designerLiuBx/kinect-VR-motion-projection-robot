%% 使用Apriori算法挖掘关联规则
function assAnalysis(flagL, star)
if flagL == 1
    inputfile = 'G:\workspace\ws_matlab\ws_math2\txt\hairdryer_txt\hairdryer'+string(star)+'.txt'; % 销量及其他属性数据
    outputfile='G:\workspace\ws_matlab\ws_math2\txt\hairdryer_txt\as.txt';% 输出转换后0,1矩阵文件
    rulefile = 'G:\workspace\ws_matlab\ws_math2\rule\hairdryer\hairdryerRule'+string(star)+'.txt'; % 规则输出文件
elseif flagL == 2
    inputfile = 'G:\workspace\ws_matlab\ws_math2\txt\microwave_txt\microwave'+string(star)+'.txt'; % 销量及其他属性数据
    outputfile='G:\workspace\ws_matlab\ws_math2\txt\microwave_txt\as.txt';% 输出转换后0,1矩阵文件
    rulefile = 'G:\workspace\ws_matlab\ws_math2\rule\microwave\microwaveRule'+string(star)+'.txt'; % 规则输出文件
elseif flagL == 3
    inputfile = 'G:\workspace\ws_matlab\ws_math2\txt\pacifier_txt\pacifier'+string(star)+'.txt'; % 销量及其他属性数据
    outputfile='G:\workspace\ws_matlab\ws_math2\txt\pacifier_txt\as.txt';% 输出转换后0,1矩阵文件
    rulefile = 'G:\workspace\ws_matlab\ws_math2\rule\pacifier\pacifierRule'+string(star)+'.txt'; % 规则输出文件
elseif flagL == 4
    inputfile = 'G:\workspace\ws_matlab\ws_math2\txt\lunwen'+string(star)+'.txt'; % 销量及其他属性数据
    outputfile='G:\workspace\ws_matlab\ws_math2\txt\as.txt';% 输出转换后0,1矩阵文件
    rulefile = 'G:\workspace\ws_matlab\ws_math2\rule\lunwenRule'+string(star)+'.txt'; % 规则输出文件

end
minSup = 0.01; % 最小支持度
minConf = 0.8;% 最小置信度
nRules = 800;% 输出最大规则数
sortFlag = 1;% 按照支持度排序

%% 调用转换程序 ，把数据转换为0,1矩阵，自定义函数
[transactions,code] = trans2matrix(inputfile,outputfile,','); 

%% 调用Apriori关联规则算法，自定义函数
[Rules,FreqItemsets] = findRules(transactions, minSup, minConf, nRules, sortFlag, code, rulefile);

disp('Apriori算法挖掘菜品订单关联规则完成！');
