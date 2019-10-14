#!/usr/bin/python
import os
import sys

c_test = 'echo '
c_send = './do_unfolding '
nscans = ' 75'
tau_value = ' -1 '

# datasets = [
#               'Data',     'Data_sd',     'Data_puppi',     'Data_puppi_sd'
#             , 'Pseudo_1',  'Pseudo_1_sd',  'Pseudo_1_puppi',  'Pseudo_1_puppi_sd'
#             , 'Pseudo_2',  'Pseudo_2_sd',  'Pseudo_2_puppi',  'Pseudo_2_puppi_sd'
#             , 'Pseudo_3',  'Pseudo_3_sd',  'Pseudo_3_puppi',  'Pseudo_3_puppi_sd'
#             , 'SCALEupup', 'SCALEupup_sd', 'SCALEupup_puppi', 'SCALEupup_puppi_sd'
#             , 'SCALEupnone', 'SCALEupnone_sd', 'SCALEupnone_puppi', 'SCALEupnone_puppi_sd'
#             , 'SCALEnoneup', 'SCALEnoneup_sd', 'SCALEnoneup_puppi', 'SCALEnoneup_puppi_sd'
#             , 'SCALEnonedown', 'SCALEnonedown_sd', 'SCALEnonedown_puppi', 'SCALEnonedown_puppi_sd'
#             , 'SCALEdownnone', 'SCALEdownnone_sd', 'SCALEdownnone_puppi', 'SCALEdownnone_puppi_sd'
#             , 'SCALEdowndown', 'SCALEdowndown_sd', 'SCALEdowndown_puppi', 'SCALEdowndown_puppi_sd'
#             , 'mtop1715', 'mtop1715_sd', 'mtop1715_puppi', 'mtop1715_puppi_sd'
#             , 'mtop1735', 'mtop1735_sd', 'mtop1735_puppi', 'mtop1735_puppi_sd'
#             , 'madgraph', 'madgraph_sd', 'madgraph_puppi', 'madgraph_puppi_sd'
#             , 'isrup',    'isrup_sd',    'isrup_puppi',    'isrup_puppi_sd'
#             , 'isrdown',  'isrdown_sd',  'isrdown_puppi',  'isrdown_puppi_sd'
#             , 'fsrup',    'fsrup_sd',    'fsrup_puppi',    'fsrup_puppi_sd'
#             , 'fsrdown',  'fsrdown_sd',  'fsrdown_puppi',  'fsrdown_puppi_sd'
#             , 'hdampup',  'hdampup_sd',  'hdampup_puppi',  'hdampup_puppi_sd'
#             , 'hdampdown','hdampdown_sd','hdampdown_puppi','hdampdown_puppi_sd'
#             ]

# channel = ['mu','ele','comb']
channel = ['comb']
# datasets = [
#               # 'Data_sd'
#              'Pseudo_1_sd'
#             , 'Pseudo_2_sd'
#             , 'Pseudo_3_sd'
#             ]
datasets = [
              'Data'
            , 'Pseudo_1'
            , 'Pseudo_2'
            , 'Pseudo_3'
            , 'SCALEupup'
            , 'SCALEupnone'
            , 'SCALEnoneup'
            , 'SCALEnonedown'
            , 'SCALEdownnone'
            , 'SCALEdowndown'
            , 'mtop1715'
            , 'mtop1735'
            , 'madgraph'
            , 'isrup'
            , 'isrdown'
            , 'fsrup'
            , 'fsrdown'
            , 'hdampup'
            , 'hdampdown'
            ]
# datasets = [
#               'Data_sd'
#             , 'Pseudo_1_sd'
#             , 'Pseudo_2_sd'
#             , 'Pseudo_3_sd'
#             , 'SCALEupup_sd'
#             , 'SCALEupnone_sd'
#             , 'SCALEnoneup_sd'
#             , 'SCALEnonedown_sd'
#             , 'SCALEdownnone_sd'
#             , 'SCALEdowndown_sd'
#             , 'mtop1695_sd'
#             , 'mtop1715_sd'
#             , 'mtop1735_sd'
#             , 'mtop1755_sd'
#             , 'madgraph_sd'
#             , 'isrup_sd'
#             , 'isrdown_sd'
#             , 'fsrup_sd'
#             , 'fsrdown_sd'
#             , 'hdampup_sd'
#             , 'hdampdown_sd'
#             ]

for chan in channel:
    for i in range(len(datasets)):
        os.system(c_send+datasets[i]+nscans+tau_value+chan)
        os.system('echo '+c_send+datasets[i]+nscans+tau_value+chan);
        os.system('\n')
