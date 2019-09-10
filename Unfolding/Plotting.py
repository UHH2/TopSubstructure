#!/usr/bin/python
import os
import sys

c_test = 'echo '
c_send = './Plotter '
prefix = 'Unfoldings/Unfolding_'
ending = '.root'
datasets = [
            #   'Data',     'Data_sd',     'Data_puppi',     'Data_puppi_sd'
            # , 'Pseudo_1',  'Pseudo_1_sd',  'Pseudo_1_puppi',  'Pseudo_1_puppi_sd'
            # , 'Pseudo_2',  'Pseudo_2_sd',  'Pseudo_2_puppi',  'Pseudo_2_puppi_sd'
            # , 'Pseudo_3',  'Pseudo_3_sd',  'Pseudo_3_puppi',  'Pseudo_3_puppi_sd'
            # , 'SCALEupup', 'SCALEupup_sd', 'SCALEupup_puppi', 'SCALEupup_puppi_sd'
            # , 'SCALEupnone', 'SCALEupnone_sd', 'SCALEupnone_puppi', 'SCALEupnone_puppi_sd'
            # , 'SCALEnoneup', 'SCALEnoneup_sd', 'SCALEnoneup_puppi', 'SCALEnoneup_puppi_sd'
             'SCALEnonedown', 'SCALEnonedown_sd', 'SCALEnonedown_puppi', 'SCALEnonedown_puppi_sd'
            , 'SCALEdownnone', 'SCALEdownnone_sd', 'SCALEdownnone_puppi', 'SCALEdownnone_puppi_sd'
            , 'SCALEdowndown', 'SCALEdowndown_sd', 'SCALEdowndown_puppi', 'SCALEdowndown_puppi_sd'
            , 'mtop1695', 'mtop1695_sd', 'mtop1695_puppi', 'mtop1695_puppi_sd'
            , 'mtop1715', 'mtop1715_sd', 'mtop1715_puppi', 'mtop1715_puppi_sd'
            , 'mtop1735', 'mtop1735_sd', 'mtop1735_puppi', 'mtop1735_puppi_sd'
            , 'mtop1755', 'mtop1755_sd', 'mtop1755_puppi', 'mtop1755_puppi_sd'
            , 'madgraph', 'madgraph_sd', 'madgraph_puppi', 'madgraph_puppi_sd'
            , 'isrup',    'isrup_sd',    'isrup_puppi',    'isrup_puppi_sd'
            , 'isrdown',  'isrdown_sd',  'isrdown_puppi',  'isrdown_puppi_sd'
            , 'fsrup',    'fsrup_sd',    'fsrup_puppi',    'fsrup_puppi_sd'
            , 'fsrdown',  'fsrdown_sd',  'fsrdown_puppi',  'fsrdown_puppi_sd'
            , 'hdampup',  'hdampup_sd',  'hdampup_puppi',  'hdampup_puppi_sd'
            , 'hdampdown','hdampdown_sd','hdampdown_puppi','hdampdown_puppi_sd'
            ]
# datasets = [
#               'data',     'data_sd'
              # 'pseudo1',  'pseudo1_sd'
            # , 'pseudo2',  'pseudo2_sd'
            # , 'pseudo3',  'pseudo3_sd'
             # 'scale_uu', 'scale_uu_sd'
            # , 'scale_un', 'scale_un_sd'
            # , 'scale_nu', 'scale_nu_sd'
            # , 'scale_nd', 'scale_nd_sd'
            # , 'scale_dn', 'scale_dn_sd'
            # , 'scale_dd', 'scale_dd_sd'
            # , 'mtop1695', 'mtop1695_sd'
            # , 'mtop1715', 'mtop1715_sd'
            # , 'mtop1735', 'mtop1735_sd'
            # , 'mtop1755', 'mtop1755_sd'
             # 'madgraph', 'madgraph_sd'
            # , 'isrup',    'isrup_sd'
            # , 'isrdown',  'isrdown_sd'
            # 'fsrup',    'fsrup_sd'
            # , 'fsrdown',  'fsrdown_sd'
            # , 'hdampup',  'hdampup_sd'
            # , 'hdampdown','hdampdown_sd'
            # ]

# channel = ['_mu','_ele','_comb']
channel = ['_ele']
# channel = ['_comb']
# datasets = [
#               'data_sd'
#             , 'pseudo1_sd'
#             , 'pseudo2_sd'
#             , 'pseudo3_sd'
#             ]
# datasets = [
#               'data'
#             , 'pseudo1'
#             , 'pseudo2'
#             , 'pseudo3'
#             , 'scale_uu'
#             , 'scale_un'
#             , 'scale_nu'
#             , 'scale_nd'
#             , 'scale_dn'
#             , 'scale_dd'
#             , 'mtop1695'
#             , 'mtop1715'
#             , 'mtop1735'
#             , 'mtop1755'
#             , 'madgraph'
#             ]
# datasets = [  'pseudo1'            ]
#             ]
# datasets = [
#               'data_sd'
#             , 'pseudo1_sd'
#             , 'pseudo2_sd'
#             , 'pseudo3_sd'
#             , 'scale_uu_sd'
#             , 'scale_un_sd'
#             , 'scale_nu_sd'
#             , 'scale_nd_sd'
#             , 'scale_dn_sd'
#             , 'scale_dd_sd'
#             , 'mtop1695_sd'
#             , 'mtop1715_sd'
#             , 'mtop1735_sd'
#             , 'mtop1755_sd'
#             , 'madgraph_sd'
#             ]
for chan in channel:
    for i in range(len(datasets)):
        os.system('echo '+c_send+prefix+datasets[i]+chan+ending)
        os.system(c_send+prefix+datasets[i]+chan+ending)
