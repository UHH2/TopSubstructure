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




Ndone = None # keep track of finished jobs by each worker
def main():
    if len(sys.argv) < 2:
        print 'Usage: ./Multicore_hadd.py <Number of Jobs>'
        exit(0)

    path_list = [  '/nfs/dust/cms/user/skottkej/TopSubstructure/PreSelection/workdir_Presel/']
    # path_list2 = [
    #               '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECup/Electron/'
    #         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECdown/Electron/'
    #         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERup/Electron/'
    #         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERdown/Electron/'
    #         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/Electron/'
    #         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECup/Muon/'
    #         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JECdown/Muon/'
    #         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERup/Muon/'
    #         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/JERdown/Muon/'
    #         ,     '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/kin_cut_TDC40_btm_cmssw10_data/Muon/'
    #         ]
    path_list2 = [
                 # '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Test_Sel/Electron/'
                 '/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Test_Sel/Muon/'
            ]
    data_names = ['DATA.DATA_2016v3_SingleMuonB', 'DATA.DATA_2016v3_SingleMuonC', 'DATA.DATA_2016v3_SingleMuonD', 'DATA.DATA_2016v3_SingleMuonE', 'DATA.DATA_2016v3_SingleMuonF', 'DATA.DATA_2016v3_SingleMuonG', 'DATA.DATA_2016v3_SingleMuonH', 'DATA.DATA_2016v3_SingleElectronB', 'DATA.DATA_2016v3_SingleElectronC', 'DATA.DATA_2016v3_SingleElectronD', 'DATA.DATA_2016v3_SingleElectronE', 'DATA.DATA_2016v3_SingleElectronF', 'DATA.DATA_2016v3_SingleElectronG', 'DATA.DATA_2016v3_SingleElectronH', 'MC.DYJetsToLL_2016v3_LO_M50_HT0070to0100', 'MC.DYJetsToLL_2016v3_LO_M50_HT0100to0200', 'MC.DYJetsToLL_2016v3_LO_M50_HT0200to0400', 'MC.DYJetsToLL_2016v3_LO_M50_HT0400to0600', 'MC.DYJetsToLL_2016v3_LO_M50_HT0600to0800', 'MC.DYJetsToLL_2016v3_LO_M50_HT0800to1200', 'MC.DYJetsToLL_2016v3_LO_M50_HT1200to2500', 'MC.DYJetsToLL_2016v3_LO_M50_HT2500toInft', 'MC.Diboson_WW_2016v3', 'MC.Diboson_WZ_2016v3', 'MC.Diboson_ZZ_2016v3', 'MC.ST_2016v3_s-channel', 'MC.ST_2016v3_t-channel_antitop', 'MC.ST_2016v3_t-channel_top', 'MC.ST_2016v3_tW_antitop', 'MC.ST_2016v3_tW_top', 'MC.TTbar_2016v3_Mtt0000to0700', 'MC.TTbar_2016v3_Mtt0700to1000', 'MC.TTbar_2016v3_Mtt1000toInft', 'MC.TTbar_2016v3_madgraph', 'MC.TTbar_2016v3_mtop1695', 'MC.TTbar_2016v3_mtop1715', 'MC.TTbar_2016v3_mtop1735', 'MC.TTbar_2016v3_mtop1755', 'MC.TTbar_2016v3_fsrdown', 'MC.TTbar_2016v3_fsrup', 'MC.TTbar_2016v3_hdampdown', 'MC.TTbar_2016v3_hdampup', 'MC.TTbar_2016v3_isrdown', 'MC.TTbar_2016v3_isrup', 'MC.WJetsToLNu_2016v3_Pt0100to0250', 'MC.WJetsToLNu_2016v3_Pt0250to0400', 'MC.WJetsToLNu_2016v3_Pt0400to0600', 'MC.WJetsToLNu_2016v3_Pt0600toInft']

    nworkers = int(sys.argv[len(sys.argv)-1])
    # data_list = create_hadd_list(path_list, data_names)
    data_list2 = create_hadd_list_KinSelection(path_list2, data_names)

    print '================================'
    print '====== Hadd/rm Multi Core ======'
    print '================================'

    # for entry in data_list:
    #     print '  -- hadd + rm: ' + entry
    print '  -- Number of Jobs:',nworkers


    # get number of xmls and calculate xmls per job
    nxmls = len(data_list2)
    print '  -- Number of files:',nxmls

    if nworkers > nxmls :
        print '[ERROR] you selected more workers than number of xmls'
        exit(0)

    # setup global counter
    Ndone = Value('i', 0)

    # start workers
    pool = Pool(processes = nworkers, initializer = init, initargs = (Ndone, ))
    # create lists for every worker including the xmls that should be processed
    # also pair it with index of worker (for keeping track of done jobs)
    job_lists = []
    for i in range(nworkers):
        job_lists.append( create_job_list(i, nworkers, data_list2) )

    # print job_lists
    result = pool.map_async(submit_job,job_lists,chunksize=1)

    while not result.ready():
        with Ndone.get_lock():
            percent = Ndone.value*100/nxmls
            missing = nxmls - Ndone.value
        sys.stdout.write("\r" + str(missing) + " jobs left - " +loading_bar(percent))
        sys.stdout.flush()
        time.sleep(1)

    pool.close()
    pool.join()

    print ' '
    print '  -- finished!'


# ------------------------------------------------------------------------------
# needed to setup global counter
def init(args):
    global Ndone
    Ndone = args
# ------------------------------------------------------------------------------
# this function submits the job and increments the global counter
# (global counter is not that easy with multiprocessing)
def submit_job(job_list):
    #print len(job_list)
    for xml in job_list:
        # subprocess.call(['echo',xml], stdout=open("/dev/null", "w"), stderr=subprocess.STDOUT)
        os.system(xml)
        # process = subprocess.Popen(['echo',xml], stdout=PIPE, stderr=subprocess.STDOUT)
        # output = process.stdout.read()
        # print output
        global Ndone
        with Ndone.get_lock():
            Ndone.value += 1

    return 1

# ------------------------------------------------------------------------------
# for every worker an array containing different xmls is created
# it is taken care of distributing the job as equally as possible
def create_job_list(i, nworkers, xmls):
    integer_div = len(xmls) // nworkers
    rest = len(xmls) - (integer_div * nworkers)
    new_xml_list = []
    # the rest should not just be added to the last worker but distributed equally
    # therefore every worker with i+1 gets one additional job
    if i+1 <= rest:
        min_index = i*integer_div + i
        max_index = (i+1)*integer_div + 1 + i
    else:
        min_index = i*integer_div + rest
        max_index = (i+1)*integer_div + rest

    # loop over total list and only keep element if index is between min and max
    for index, val in enumerate(xmls):
        if index >= min_index and index < max_index:
            new_xml_list.append( xmls[index] )

    return new_xml_list
# ------------------------------------------------------------------------------
def create_hadd_list(path_list, data_names):
    limit = 91268055040
    size =  0
    data_list = []
    to_remove = []

    i = 0
    text = ''
    text2 = ''

    final_file = ''
    path2 = ''

    for path in path_list:
        for dirpath, dirname, filename in os.walk(path):
            filename.sort()
            for root in data_names:
                for name in filename:
                    if root in name:
                        if text == '':
                            if 'workdir' in path:
                                words = path.split('/')
                                path2 = '/'
                                for patch in words[1:len(words)-1]:
                                    if 'workdir' not in patch:
                                        path2 += patch
                                        path2 += '/'
                                text += 'hadd -f '+path2+'uhh2.AnalysisModuleRunner.'+root+'.root'
                                text2 += 'rm '
                                final_file = path2+'uhh2.AnalysisModuleRunner.'+root+'.root'
                            else:
                                text += path+'uhh2.AnalysisModuleRunner.'+root+'.root'
                                final_file = path+'uhh2.AnalysisModuleRunner.'+root+'.root'

                        size += os.stat(path+name).st_size
                        if size <= limit:
                            add_file = os.path.join(dirpath,name)
                            if final_file != add_file:
                                text += ' ' + add_file
                                text2 += add_file + ' '
                        else:
                            text += ';'
                            text += text2
                            data_list.append(text)
                            i += 1
                            if 'workdir' in path:
                                text = 'hadd -f '+path2+'uhh2.AnalysisModuleRunner.'+root+'_part'+str(i)+'.root'
                            else:
                                text = 'hadd -f '+path+'uhh2.AnalysisModuleRunner.'+root+'_part'+str(i)+'.root'

                            text += ' ' + os.path.join(dirpath,name)
                            text2 = 'rm '+os.path.join(dirpath,name) + ' '
                            size = 0
                text += ';'
                text += text2
                data_list.append(text)
                i = 0
                size = 0
                text = ''
                text2 = ''
    # print data_list

    data_list_result = []
    for test in data_list:
        if ';' != test:
            data_list_result.append(test)

    # print data_list_result
    return data_list_result


def create_hadd_list_KinSelection(path_list, data_names):
    limit = 91268055040
    size =  0
    data_list = []

    i = 0
    text = ''
    text2 = ''
    final_file = ''

    for path in path_list:
        for dirpath, dirname, filename in os.walk(path):
            if 'workdir' not in dirpath:
                filename.sort()
                for root in data_names:
                    for name in filename:
                        if root in name:
                            final_file = path+'uhh2.AnalysisModuleRunner.'+root+'.root'
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
                                text = 'hadd -f '+path+'uhh2.AnalysisModuleRunner.'+root+'_part'+str(i)+'.root'

                                text += ' ' + os.path.join(dirpath,name)
                                text2 = 'rm '+os.path.join(dirpath,name) + ' '
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

    # print data_list_result
    return data_list_result

# this function deletes the temporary output of sframe_main
def delete_temp_dirs():
    path = os.getcwd()
    del_paths = glob.glob(os.path.join(path, 'jobTempOutput_*'))
    for del_path in del_paths:
        shutil.rmtree(del_path)
# ------------------------------------------------------------------------------
# this converts percent into a loading bar and returns a string containing the bar
def loading_bar(percent):
    p = float("{0:.2f}".format(percent)) # round to two digits
    bar = 'Progress: '+str(p)+'% ['
    levels = [0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95]
    for level in levels:
        if percent > level:
            bar += '=='
        else:
            bar += '  '
    bar += ']'
    return bar

# ------------------------------------------------------------------------------
# main function
if __name__ == "__main__":
    main()
