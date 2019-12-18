#!/usr/bin/python
import os
from subprocess import Popen, PIPE
from pprint import pprint
from datetime import datetime
import sys, time, subprocess
# from ROOT import *
from multiprocessing import Pool, Value
import random
import shutil
import glob
import os

# path_list = [  '/nfs/dust/cms/user/skottkej/TopSubstructure/PreSelection/workdir_Presel/']
path_list = [
              # '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECdown/Muon/'
        #      '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECup/Electron/'
        # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECdown/Electron/'
        # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERup/Electron/'
        # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERdown/Electron/'
            '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/Muon/'
        # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/Electron/'
        # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECup/Muon/'
        # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERup/Muon/'
        # ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERdown/Muon/'
        ]
data_names = ['DATA.DATA_2016v3_SingleMuonB', 'DATA.DATA_2016v3_SingleMuonC', 'DATA.DATA_2016v3_SingleMuonD', 'DATA.DATA_2016v3_SingleMuonE', 'DATA.DATA_2016v3_SingleMuonF', 'DATA.DATA_2016v3_SingleMuonG', 'DATA.DATA_2016v3_SingleMuonH', 'DATA.DATA_2016v3_SingleElectronB', 'DATA.DATA_2016v3_SingleElectronC', 'DATA.DATA_2016v3_SingleElectronD', 'DATA.DATA_2016v3_SingleElectronE', 'DATA.DATA_2016v3_SingleElectronF', 'DATA.DATA_2016v3_SingleElectronG', 'DATA.DATA_2016v3_SingleElectronH', 'MC.DYJetsToLL_2016v3_LO_M50_HT0070to0100', 'MC.DYJetsToLL_2016v3_LO_M50_HT0100to0200', 'MC.DYJetsToLL_2016v3_LO_M50_HT0200to0400', 'MC.DYJetsToLL_2016v3_LO_M50_HT0400to0600', 'MC.DYJetsToLL_2016v3_LO_M50_HT0600to0800', 'MC.DYJetsToLL_2016v3_LO_M50_HT0800to1200', 'MC.DYJetsToLL_2016v3_LO_M50_HT1200to2500', 'MC.DYJetsToLL_2016v3_LO_M50_HT2500toInft', 'MC.Diboson_WW_2016v3', 'MC.Diboson_WZ_2016v3', 'MC.Diboson_ZZ_2016v3', 'MC.ST_2016v3_s-channel', 'MC.ST_2016v3_t-channel_antitop', 'MC.ST_2016v3_t-channel_top', 'MC.ST_2016v3_tW_antitop', 'MC.ST_2016v3_tW_top', 'MC.TTbar_2016v3_Mtt0000to0700', 'MC.TTbar_2016v3_Mtt0700to1000', 'MC.TTbar_2016v3_Mtt1000toInft', 'MC.TTbar_2016v3_madgraph', 'MC.TTbar_2016v3_mtop1695', 'MC.TTbar_2016v3_mtop1715', 'MC.TTbar_2016v3_mtop1735', 'MC.TTbar_2016v3_mtop1755', 'MC.TTbar_2016v3_fsrdown', 'MC.TTbar_2016v3_fsrup', 'MC.TTbar_2016v3_hdampdown', 'MC.TTbar_2016v3_hdampup', 'MC.TTbar_2016v3_isrdown', 'MC.TTbar_2016v3_isrup', 'MC.WJetsToLNu_2016v3_Pt0100to0250', 'MC.WJetsToLNu_2016v3_Pt0250to0400', 'MC.WJetsToLNu_2016v3_Pt0400to0600', 'MC.WJetsToLNu_2016v3_Pt0600toInft']
# data_names = ['MC.TTbar_2016v3_mtop1755_']

limit = 91268055040
size =  0
data_list = []

i = 0
text = ''
text2 = ''
final_file = ''

for path in path_list:
    for dirpath, dirname, filename in os.walk(path):
        filename.sort()
        for root in data_names:
            for name in filename:
                if root in name:
                    final_file =path+'uhh2.AnalysisModuleRunner.'+root+'.root'
                    if text == '':
                        text += 'hadd -f '+path+'uhh2.AnalysisModuleRunner.'+root+'.root'
                        text2 += 'rm'

                    size += os.stat(path+name).st_size
                    if size <= limit:
                        add_file = os.path.join(dirpath,name)
                        if final_file != add_file:
                            text += ' ' + add_file
                            text2 += ' '+ add_file
                    else:
                        text += ';'
                        text += text2
                        data_list.append(text)
                        i += 1
                        text = 'echo hadd -f '+path+'uhh2.AnalysisModuleRunner.'+root+'_part'+str(i)+'.root'

                        text += ' ' + os.path.join(dirpath,name)
                        text2 = 'echo rm '+os.path.join(dirpath,name) + ' '
                        size = 0
            text += ';'
            text += text2
            data_list.append(text)
            i = 0
            size = 0
            text = ''
            text2 = ''

data_list_result = []
for test in data_list:
    if ';' != test and 'part' in test:
        data_list_result.append(test)

for test in data_list_result:
    print test+'\n'
