#!/usr/bin/python
import os
import sys

c_test = 'echo '
c_send = './do_unfolding '
nscans = ' 130'
tau_value = ' -1'

datasets = ['pseudo1', 'pseudo1_sd', 'pseudo1_puppi', 'pseudo1_puppi_sd', 'pseudo2', 'pseudo2_sd', 'pseudo2_puppi', 'pseudo2_puppi_sd', 'pseudo3', 'pseudo3_sd', 'pseudo3_puppi', 'pseudo3_puppi_sd', 'scale_uu', 'scale_uu_sd', 'scale_uu_puppi', 'scale_uu_puppi_sd', 'scale_un', 'scale_un_sd', 'scale_un_puppi', 'scale_un_puppi_sd', 'scale_nu', 'scale_nu_sd', 'scale_nu_puppi', 'scale_nu_puppi_sd', 'scale_nd', 'scale_nd_sd', 'scale_nd_puppi', 'scale_nd_puppi_sd', 'scale_dn', 'scale_dn_sd', 'scale_dn_puppi', 'scale_dn_puppi_sd', 'scale_dd', 'scale_dd_sd', 'scale_dd_puppi', 'scale_dd_puppi_sd', 'data', 'data_sd', 'data_puppi', 'data_puppi_sd']

subtract_background = []

for file in datasets:
    if 'pseudo' in file:
        subtract_background.append(' false')
    elif 'scale' in file:
        subtract_background.append(' false')
    elif 'data' in file:
        subtract_background.append(' true')
    # os.system('echo %s ' %subtract_background)

for i in range(len(datasets)):
    os.system(c_send+datasets[i]+nscans+tau_value+subtract_background[i])
