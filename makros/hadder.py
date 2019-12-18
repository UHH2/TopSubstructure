#!/usr/bin/python
import os
import sys

prefix = 'uhh2.AnalysisModuleRunner.'
path = '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection/'

for dirpath, dirname, filename in os.walk(path):
    filename.sort()
    if 'Plots' not in dirpath and filename:
        if 'workdir' not in dirpath and filename:
            ele = False
            mu = False
            tt = False
            drell = False
            top = False
            dib = False
            wj_ht = False
            wj_pt = False
            q = False
            mtop95 = False
            mtop15 = False
            mtop35 = False
            mtop55 = False
            mad = False
            iu = False
            id = False
            fu = False
            fd = False
            hu = False
            hd = False
            SingleEle = dirpath + '/' + prefix + 'DATA.2016v3_DATA.root'
            SingleMu = dirpath + '/' + prefix + 'DATA.2016v3_DATA.root'
            ttbar = dirpath + '/' + prefix + 'MC.2016v3_TTbar.root'
            dy = dirpath + '/' + prefix + 'MC.2016v3_DYJets.root'
            st = dirpath + '/' + prefix + 'MC.2016v3_ST.root'
            diboson = dirpath + '/' + prefix + 'MC.2016v3_Diboson.root'
            wjets_ht = dirpath + '/' + prefix + 'MC.WJets_HT.root'
            wjets_pt = dirpath + '/' + prefix + 'MC.WJets_Pt.root'
            qcd = dirpath + '/' + prefix + 'MC.2016v3_QCD.root'
            mtop1695 = dirpath + '/' + prefix + 'MC.TTbar_mtop1695.root'
            mtop1715 = dirpath + '/' + prefix + 'MC.TTbar_mtop1715.root'
            mtop1735 = dirpath + '/' + prefix + 'MC.TTbar_mtop1735.root'
            mtop1755 = dirpath + '/' + prefix + 'MC.TTbar_mtop1755.root'
            madgraph = dirpath + '/' + prefix + 'MC.TTbar_madgraph.root'
            isrup = dirpath + '/' + prefix + 'MC.TTbar_isrup.root'
            isrdown = dirpath + '/' + prefix + 'MC.TTbar_isrdown.root'
            fsrup = dirpath + '/' + prefix + 'MC.TTbar_fsrup.root'
            fsrdown = dirpath + '/' + prefix + 'MC.TTbar_fsrdown.root'
            hdampup = dirpath + '/' + prefix + 'MC.TTbar_hdampup.root'
            hdampdown = dirpath + '/' + prefix + 'MC.TTbar_hdampdown.root'
            for file in filename:
                if 'SingleElectron' in file:
                    SingleEle += ' ' + dirpath + '/' + file
                    ele = True
                if 'SingleMuon' in file:
                    SingleMu += ' ' + dirpath + '/' + file
                    mu = True
                if 'Mtt' in file:
                    ttbar += ' ' + dirpath + '/' + file
                    tt = True
                if 'DYJetsToLL_' in file:
                    dy += ' ' + dirpath + '/' + file
                    drell = True
                if 'ST_2016' in file:
                    st += ' ' + dirpath + '/' + file
                    top = True
                if 'Diboson_' in file:
                    diboson += ' ' + dirpath + '/' + file
                    dib = True
                if 'WJetsToLNu_2016v3_HT_' in file:
                    wjets_ht += ' ' + dirpath + '/' + file
                    wj_ht = True
                if 'WJetsToLNu_2016v3_Pt0' in file:
                    wjets_pt += ' ' + dirpath + '/' + file
                    wj_pt = True
                if 'QCD_2016v3' in file:
                    qcd += ' ' + dirpath + '/' + file
                    q = True
                if 'TTbar_2016v3_mtop1695' in file:
                    mtop1695 += ' ' + dirpath + '/' + file
                    mtop95 = True
                if 'TTbar_2016v3_mtop1715' in file:
                    mtop1715 += ' ' + dirpath + '/' + file
                    mtop15 = True
                if 'TTbar_2016v3_mtop1735' in file:
                    mtop1735 += ' ' + dirpath + '/' + file
                    mtop35 = True
                if 'TTbar_2016v3_mtop1755' in file:
                    mtop1755 += ' ' + dirpath + '/' + file
                    mtop55 = True
                if 'TTbar_2016v3_madgraph' in file:
                    madgraph += ' ' + dirpath + '/' + file
                    mad = True
                if 'TTbar_2016v3_isrup' in file:
                    isrup += ' ' + dirpath + '/' + file
                    iu = True
                if 'TTbar_2016v3_isrdown' in file:
                    isrdown += ' ' + dirpath + '/' + file
                    id = True
                if 'TTbar_2016v3_fsrup' in file:
                    fsrup += ' ' + dirpath + '/' + file
                    fu = True
                if 'TTbar_2016v3_fsrdown' in file:
                    fsrdown += ' ' + dirpath + '/' + file
                    fd = True
                if 'TTbar_2016v3_hdampup' in file:
                    hdampup += ' ' + dirpath + '/' + file
                    hu = True
                if 'TTbar_2016v3_hdampdown' in file:
                    hdampdown += ' ' + dirpath + '/' + file
                    hd = True
            # if ele:
            #     os.system('hadd -f %s' %SingleEle)
            # if mu:
            #     os.system('hadd -f %s' %SingleMu)
            # if tt:
            #     os.system('hadd -f %s' %ttbar)
            if drell:
                os.system('hadd -f %s' %dy)
            # if top:
            #     os.system('hadd -f %s' %st)
            # if dib:
            #     os.system('hadd -f %s' %diboson)
            if wj_ht:
                os.system('hadd -f %s' %wjets_ht)
            # if wj_pt:
            #     os.system('hadd -f %s' %wjets_pt)
            if q:
                os.system('hadd -f %s' %qcd)
            # if mtop95:
            #     os.system('hadd -f %s' %mtop1695)
            # if mtop15:
            #     os.system('hadd -f %s' %mtop1715)
            # if mtop35:
            #     os.system('hadd -f %s' %mtop1735)
            # if mtop55:
            #     os.system('hadd -f %s' %mtop1755)
            # if mad:
            #     os.system('hadd -f %s' %madgraph)
            # if iu:
            #     os.system('hadd -f %s' %isrup)
            # if id:
            #     os.system('hadd -f %s' %isrdown)
            # if fu:
            #     os.system('hadd -f %s' %fsrup)
            # if fd:
            #     os.system('hadd -f %s' %fsrdown)
            # if hu:
            #     os.system('hadd -f %s' %hdampup)
            # if hd:
            #     os.system('hadd -f %s' %hdampdown)
            # os.system('echo hadd -f %s/' %dirpath)
            # os.system('echo filename: %s' %filename)
            # os.system('echo \n')
