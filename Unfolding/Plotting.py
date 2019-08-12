#!/usr/bin/python
import os
import sys

c_test = 'echo '
c_send = './Plotter '
prefix = 'Unfoldings/Unfolding_'
ending = '.root'
# datasets = [
#               'data', 'data_sd', 'data_puppi', 'data_puppi_sd'
#             , 'pseudo1', 'pseudo1_sd', 'pseudo1_puppi', 'pseudo1_puppi_sd'
#             , 'pseudo2', 'pseudo2_sd', 'pseudo2_puppi', 'pseudo2_puppi_sd'
#             , 'pseudo3', 'pseudo3_sd', 'pseudo3_puppi', 'pseudo3_puppi_sd'
#             , 'scale_uu', 'scale_uu_sd', 'scale_uu_puppi', 'scale_uu_puppi_sd'
#             , 'scale_un', 'scale_un_sd', 'scale_un_puppi', 'scale_un_puppi_sd'
#             , 'scale_nu', 'scale_nu_sd', 'scale_nu_puppi', 'scale_nu_puppi_sd'
#             , 'scale_nd', 'scale_nd_sd', 'scale_nd_puppi', 'scale_nd_puppi_sd'
#             , 'scale_dn', 'scale_dn_sd', 'scale_dn_puppi', 'scale_dn_puppi_sd'
#             , 'scale_dd', 'scale_dd_sd', 'scale_dd_puppi', 'scale_dd_puppi_sd'
#             , 'mtop1695', 'mtop1695_sd', 'mtop1695_puppi', 'mtop1695_puppi_sd'
#             , 'mtop1715', 'mtop1715_sd', 'mtop1715_puppi', 'mtop1715_puppi_sd'
#             , 'mtop1735', 'mtop1735_sd', 'mtop1735_puppi', 'mtop1735_puppi_sd'
#             , 'mtop1755', 'mtop1755_sd', 'mtop1755_puppi', 'mtop1755_puppi_sd'
#             , 'madgraph', 'madgraph_sd', 'madgraph_puppi', 'madgraph_puppi_sd'
#             ]

# channel = ['_mu','_ele','_comb']
channel = ['_comb']
datasets = [  'data'
            , 'pseudo1'
            , 'pseudo2'
            , 'pseudo3'
            ]
for chan in channel:
    for i in range(len(datasets)):
        os.system('echo '+c_send+prefix+datasets[i]+chan+ending)
        os.system(c_send+prefix+datasets[i]+chan+ending)
