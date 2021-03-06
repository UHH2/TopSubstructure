#!/usr/bin/python
import os

c_send = 'rm '
option = '-rf '
path = '/nfs/dust/cms/user/skottkej/TopSubstructure/'

# remove all workdirs from 'path'
for dirpath, dirname, filename in os.walk(path):
    for name in dirname:
        if 'workdir' in name:
            os.system('rm -rf %s' %os.path.join(dirpath,name))
            os.system('echo removed: %s \n' %os.path.join(dirpath,name))

# remove all workdir from the directory from which this script is running
for dirpath, dirname, filename in os.walk(os.getcwd()):
    for name in dirname:
        if 'workdir' in name:
            os.system('rm -rf %s' %os.path.join(dirpath,name))
            os.system('echo removed: %s \n' %os.path.join(dirpath,name))
