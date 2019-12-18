#!/usr/bin/python
import os
from subprocess import Popen, PIPE
from pprint import pprint


path_list = [  '/nfs/dust/cms/user/skottkej/TopSubstructure/PreSelection/workdir_Presel/']
# path_list = [
#               # '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECdown/Muon/'
#              '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECup/Electron/'
#         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECdown/Electron/'
#         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERup/Electron/'
#         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERdown/Electron/'
#         # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/Muon/'
#         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/Electron/'
#         # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECup/Muon/'
#         # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERup/Muon/'
#         # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERdown/Muon/'
#         ]

limit = 91268055040
size =  0
data_list = []
to_remove = []

i = 0
text = ''
text2 = ''
data_names = ['DATA.DATA_2016v3_SingleMuonB', 'DATA.DATA_2016v3_SingleMuonC', 'DATA.DATA_2016v3_SingleMuonD', 'DATA.DATA_2016v3_SingleMuonE', 'DATA.DATA_2016v3_SingleMuonF', 'DATA.DATA_2016v3_SingleMuonG', 'DATA.DATA_2016v3_SingleMuonH', 'DATA.DATA_2016v3_SingleElectronB', 'DATA.DATA_2016v3_SingleElectronC', 'DATA.DATA_2016v3_SingleElectronD', 'DATA.DATA_2016v3_SingleElectronE', 'DATA.DATA_2016v3_SingleElectronF', 'DATA.DATA_2016v3_SingleElectronG', 'DATA.DATA_2016v3_SingleElectronH', 'MC.DYJetsToLL_2016v3_LO_M50_HT0070to0100', 'MC.DYJetsToLL_2016v3_LO_M50_HT0100to0200', 'MC.DYJetsToLL_2016v3_LO_M50_HT0200to0400', 'MC.DYJetsToLL_2016v3_LO_M50_HT0400to0600', 'MC.DYJetsToLL_2016v3_LO_M50_HT0600to0800', 'MC.DYJetsToLL_2016v3_LO_M50_HT0800to1200', 'MC.DYJetsToLL_2016v3_LO_M50_HT1200to2500', 'MC.DYJetsToLL_2016v3_LO_M50_HT2500toInft', 'MC.Diboson_WW_2016v3', 'MC.Diboson_WZ_2016v3', 'MC.Diboson_ZZ_2016v3', 'MC.ST_2016v3_s-channel', 'MC.ST_2016v3_t-channel_antitop', 'MC.ST_2016v3_t-channel_top', 'MC.ST_2016v3_tW_antitop', 'MC.ST_2016v3_tW_top', 'MC.TTbar_2016v3_Mtt0000to0700', 'MC.TTbar_2016v3_Mtt0700to1000', 'MC.TTbar_2016v3_Mtt1000toInft', 'MC.TTbar_2016v3_madgraph', 'MC.TTbar_2016v3_mtop1695', 'MC.TTbar_2016v3_mtop1715', 'MC.TTbar_2016v3_mtop1735', 'MC.TTbar_2016v3_mtop1755', 'MC.WJetsToLNu_2016v3_Pt0100to0250', 'MC.WJetsToLNu_2016v3_Pt0250to0400', 'MC.WJetsToLNu_2016v3_Pt0400to0600', 'MC.WJetsToLNu_2016v3_Pt0600toInft']
# data_names = ['MC.TTbar_2016v3_mtop1755_']

final_file = ''
path2 = ''

for path in path_list:
    # os.system('echo path: %s' %path)
    # print('\n')
    # print('\n')
    # os.system('ls -lh %s' %path)
    for dirpath, dirname, filename in os.walk(path):
        filename.sort()
        # os.system('echo filename: %s \n' %filename)
        # print('\n')
        # print('\n')
        # if 'workdir' not in dirpath:

        # os.system('echo dirname: %s \n' %dirname)
        # os.system('echo dirpath: %s \n' %dirpath)
        for root in data_names:
            for name in filename:
                if root in name:
                    # print(len(data_list))
                    # print(text)
                    # print('\n')
                    if text == '':
                        if 'workdir' in path:
                            words = path.split('/')
                            path2 = '/'
                            # print(words[1:len(words)-1])
                            for patch in words[1:len(words)-1]:
                                # print(patch)
                                if 'workdir' not in patch:
                                    path2 += patch
                                    path2 += '/'
                            # path2 = path.replace('workdir*', '')
                            # print('Hello World!')
                            # print(path2)
                            # print('\n')
                            text += path2+'uhh2.AnalysisModuleRunner.'+root+'.root'
                            final_file = path2+'uhh2.AnalysisModuleRunner.'+root+'.root'
                        else:
                            text += path+'uhh2.AnalysisModuleRunner.'+root+'.root'
                            final_file = path+'uhh2.AnalysisModuleRunner.'+root+'.root'

                    size += os.stat(path+name).st_size
                    # os.system('echo file: %s \n' %os.path.join(dirpath,name))
                    # os.system('echo size: %s \n' %size)
                    if size <= limit:
                        add_file = os.path.join(dirpath,name)
                        if final_file != add_file:
                            text += ' ' + add_file
                            text2 += add_file + ' '
                            # print(size)
                    else:
                        data_list.append(text)
                        to_remove.append(text2)
                        i += 1
                        if 'workdir' in path:
                            text = path2+'uhh2.AnalysisModuleRunner.'+root+'_part'+str(i)+'.root'
                        else:
                            text = path+'uhh2.AnalysisModuleRunner.'+root+'_part'+str(i)+'.root'

                        text += ' ' + os.path.join(dirpath,name)
                        text2 = os.path.join(dirpath,name) + ' '
                        size = 0
            data_list.append(text)
            to_remove.append(text2)
            for test in data_list:
                if test != '' and test != final_file:
                    os.system('hadd -f %s \n' %test)
                    # print('\n')
                    # print('\n')
            del data_list[:]
            for test2 in to_remove:
                if test2 != '' and test2 != final_file:
                    os.system('rm %s \n' %test2)
                    # print('\n')
                    # print('\n')
            del to_remove[:]
            i = 0
            size = 0
            text = ''
            text2 = ''
