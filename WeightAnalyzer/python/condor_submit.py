#!/usr/bin/python

import os
import re
import sys
import fileinput
import getopt
import subprocess
import socket
files_per_job = 50

#Absolute path that precedes '/store...'
# The script checks whether it is an absolute path or not

print "Submitting jobs at FNAL"
#sePath='\'root://cmseos.fnal.gov/'
sePath='\'root://cmsxrootd.fnal.gov/'
#sePath='\'root://eoscms.cern.ch/'
storePath=''
setupString=''

#Configuration options parsed from arguments
#Switching to getopt for compatibility with older python
try:
    opts, args = getopt.getopt(sys.argv[1:], "", ["sample=","fileList=","outDir=","submit=","isX53=","isWJ="])
except getopt.GetoptError as err:
    print str(err)
    sys.exit(1)

prefix   = str('None')
fileList = str('None')
outDir   = str('None')
submit   = bool(False)
isX53    = bool(False)
isWJ     = bool(False)

for o, a in opts: 
    print o, a

    if   o == "--sample":   prefix   = str(a)
    elif o == "--fileList": fileList = str(a)
    elif o == "--outDir":   outDir   = str(a)
    elif o == "--submit":
        if a == 'True':     submit = True

relBase = os.environ['CMSSW_BASE']

files = fileList 

if (not os.path.isfile(files)):
    print 'File with input root files '+ fileList +' not found. Please give absolute path'
    sys.exit(1)

outputdir = os.path.abspath('.')
if str(outDir) != 'None': outputdir = outDir
else:                     print 'Output dir not specified. Setting it to current directory.'

if submit: print 'Will submit jobs to Condor'
else:      print 'Will not submit jobs to Condor'

dir = outputdir+'/'+prefix

if os.path.exists(dir):
    print 'Output directory already exists!'
    sys.exit(1)
else: os.makedirs(dir)

def get_input(num, list):
    result = ''
    file_list = open(files)
    file_count = 0
    for line in file_list:
        if line.find('root')>0:
            file_count=file_count+1
            if file_count>(num-1) and file_count<(num+files_per_job):
                result=result+sePath
                if (line.strip()[:6]=='/store'):
                    result=result+storePath
                line=line[line.find("/store"): ]                    
                result=result+line.strip()+'\',\n'
    file_list.close()
    return result

j = 1
nfiles = 1

py_file = open(dir+"/"+prefix+"_"+str(j)+".py","w")

print 'CONDOR work dir: '+dir

count = 0
file_list = open(files)
for line in file_list:
    if line.find('.root')>0:
        count = count + 1

file_list.close()
        
while ( nfiles <= count ):    

    # ADD YOUR CONFIG FILE HERE!!
    py_templ_file = open(relBase+'/src/MonteCarloInfo/WeightAnalyzer/python/runWeightAnalyzer_template_cfg.py')
    
    py_file = open(dir+'/'+prefix+'_'+str(j)+'.py','w')

    #Avoid calling the get_input function once per line in template
    singleFileList = get_input(nfiles, files)
    
    for line in py_templ_file:
        line=line.replace('CONDOR_RELBASE',  relBase)
        line=line.replace('CONDOR_FILELIST', singleFileList)
        line=line.replace('CONDOR_OUTFILE',  prefix+'_'+str(j))
        py_file.write(line)
    py_file.close()

    jdl_templ_file = open(relBase+'/src/MonteCarloInfo/WeightAnalyzer/python/template.jdl')
    jdl_file       = open(dir+'/'+prefix+'_'+str(j)+'.jdl','w')
    
    for line in jdl_templ_file:
        line=line.replace('PREFIX',     prefix)
        line=line.replace('JOBID',      str(j))
        line=line.replace('OUTPUT_DIR', dir)
        line=line.replace('CMSSWBASE',  relBase)
        jdl_file.write(line)
        
    jdl_file.close()

        
    j = j + 1
    nfiles = nfiles + files_per_job
    py_templ_file.close()

njobs = j - 1

os.system('sed -e \'s/SETUP/'+setupString+'/g\' template.csh > '+dir+'/'+prefix+'.csh')

if (submit):
    savedPath = os.getcwd()
    os.chdir(dir)
    print njobs
    proc = subprocess.Popen(['condor_submit', prefix+'_1.jdl'], stdout=subprocess.PIPE) 
    (out, err) = proc.communicate()
    print "condor output:", out

    for k in range(2,njobs+1):
        os.system('condor_submit '+prefix+'_'+str(k)+'.jdl')
        os.system('sleep 0.25')

    os.chdir(savedPath)
