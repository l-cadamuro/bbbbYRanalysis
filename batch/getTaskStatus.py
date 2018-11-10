import os
import sys
import argparse
import datetime
import glob
import re

def getLog (proto, ID, silenceWarning=False):
    logs = glob.glob(proto.format(ID, '*'))
    if len(logs) == 0:
        if not silenceWarning: print ">>> No log found for job", ID
        return None
    if len(logs) > 1:
        # print ">>> Too many logs found for job", ID, ' : ', logs, 'returning last'
        if not silenceWarning: print ">>> Too many logs found for job", ID, ' (resubmitted?) , returning last'
    return logs[-1]

def getExitCode(fname, text='job ended with status'):
    f = open(fname)
    code = -888
    for line in f:
        # if not '... cmsRun finished with status' in line:
        if not '... %s' % text in line:
            continue
        # code = int(re.search('... cmsRun finished with status (\d+)', line).group(1))
        code = int(re.search('... %s (\d+)' % text, line).group(1))
    return code

def getOutFileName(fname):
    f = open(fname)
    ofn = None
    for line in f:
        if not '.. output name : ' in line:
            continue
        ofn = (re.search('.. output name : (\S+)', line).group(1)).strip()

    ## euristic, quick fix : replace file path for those files that were produced on pool and then transferred
    if ofn and '/pool' in ofn:
        ofn = '/eos/cms/store/user/lcadamur/YR_bbbb_trees/' + '/'.join(ofn.split('/')[4:])
    return ofn

def checkErrorLog(fname, checks):
    f = open(fname)
    failed = False
    for line in f:
        for el in checks:
            if el in line:
                failed = True
                break
    return failed


##############################
##### CMD line options
parser = argparse.ArgumentParser(description='Command line parser of plotting options')

parser.add_argument('--dir',          dest='folder',      help='process folder',         default=None)
parser.add_argument('--resubCmd',     dest='resubCmd',    help='print resubmit cmd',  action='store_true',   default=False)
parser.add_argument('--issueCmd',     dest='issueCmd',    help='issue resubmit cmd',  action='store_true',   default=False)
parser.add_argument('--long',         dest='short',       help='want long summary',   action='store_false',  default=True)
parser.add_argument('--flist',        dest='flist',       help='dump good files in flist', default=None)
parser.add_argument('--checkErr',     dest='checkErr',    help='also check the .err file', action='store_true', default=False)

args = parser.parse_args()

if not args.folder:
    print "Please set job+log folder name"

##############################

job_proto = args.folder + '/job_{0}.sh'
# log_proto = args.folder + '/job_{0}.sh_{1}.stdout'
log_proto = args.folder + '/job_{0}.log'
err_proto = args.folder + '/job_{0}.err'

jobs_sh = glob.glob(job_proto.format('*'))
jobs_ID = [int(re.search(job_proto.format('(\d+)'), x).group(1)) for x in jobs_sh]
# print jobs_ID
# print "** Found", len(jobs_ID), 'jobs'

if len(jobs_ID) == 0:
    print "** No jobs found"
    sys.exit()

logs_txt = {ID: getLog(log_proto, ID) for ID in jobs_ID if getLog(log_proto, ID, silenceWarning=True)}
# print logs_txt

#############################

exitCodes = []
copyCodes   = [] ## extra ops done by the tasks
deleteCodes = [] ## extra ops done by the tasks
## code -999 means no log yet (unfinished?)
## code -888 means no CMSSW string found (unfinished? / crash?)
for ID in jobs_ID:
    if not ID in logs_txt:
        exitCodes.append(-999)
        copyCodes.append(-999)
        deleteCodes.append(-999)
    else:
        exitCodes.append(getExitCode(logs_txt[ID], text = 'job ended with status'))
        copyCodes.append(getExitCode(logs_txt[ID], text = 'xrdcp copy ended with status'))
        deleteCodes.append(getExitCode(logs_txt[ID], text = 'deletion done with status'))

###########################
missing    = []
unfinished = []
failed     = []
success    = []

for idx, ID in enumerate(jobs_ID):
    code = exitCodes[idx]
    cpcode = copyCodes [idx]
    delcode = deleteCodes [idx]

    # thisisgood = (code == 0)
    thisisgood = (code == 0 and cpcode == 0 and delcode == 0)
    if args.checkErr:
        err_found = checkErrorLog(err_proto.format(ID), ['Error'])
        if thisisgood and err_found:
            print ">> job", ID, "had good status but errors found in err file", err_proto.format(ID)
        if err_found:
            thisisgood = False


    if   code == -999: missing.append((ID, code))
    elif code == -888: unfinished.append((ID, code))    
    # elif code == 0:    success.append((ID, code))
    # #elif code == 1:    success.append((ID, code)) ## #for the danalysis succes == 1.. mah
    # else:              failed.append((ID, code))
    elif thisisgood :  success.append((ID, code))
    else:              failed.append((ID, code))

## also fetch output names if required
if args.flist:
    goodfiles = []
    for ID, code in success:
        ofn = getOutFileName(logs_txt[ID])
        if not ofn:
            print "*** something went wrong in finding the out name in file", logs_txt[ID]
            continue
        goodfiles.append(ofn)

# print exitCodes
print "\n***********************************************************"
print "** TOTAL JOB FOUND: ", len(jobs_ID)
print "** Success        : ", len(success) , "(%.1f%%)" % (100.*len(success)/len(jobs_ID))
print "** Failed         : ", len(failed) , "(%.1f%%)" % (100.*len(failed)/len(jobs_ID))
print "** Unfinished     : ", len(unfinished) , "(%.1f%%)" % (100.*len(unfinished)/len(jobs_ID))
print "** Missing logs   : ", len(missing) , "(%.1f%%)" % (100.*len(missing)/len(jobs_ID))
print "***********************************************************"

if not args.short:
   print '\n'
   print '** Failed jobs ID + CODE, logfile'
   for val in failed:
       print '-', val[0], '+', val[1], ' , ', logs_txt[val[0]]
   print '** Unfinished jobs ID + CODE, logfile'
   for val in unfinished:
       print '-', val[0], '+', val[1], ' , ', logs_txt[val[0]]
   print '** Missing log jobs ID'
   for val in missing:
       print '-', val[0]

#######################
if args.resubCmd or args.issueCmd:
    print "\n** Resubmit commands\n"
    print "cd %s" % args.folder
    resubCmds = []
    for val in failed:
        jobscript = job_proto.format(val[0]).replace(args.folder + '/', '')
        command   = "../t3submit %s" % jobscript
        resubCmds.append(command)
    for cmd in resubCmds:
        print cmd

if args.issueCmd:
    print "\n** Issuing resub\n"
    os.chdir(args.folder)
    for cmd in resubCmds:
        os.system(cmd)

if args.flist:
    print " >> creating a list of good files at", args.flist
    fout = open(args.flist, 'w')
    for f in goodfiles:
        fout.write(f + '\n')
    fout.close()


