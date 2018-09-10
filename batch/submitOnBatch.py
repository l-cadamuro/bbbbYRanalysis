import os
import sys
import glob
import argparse
import getpass
import subprocess

def chunks(l, n):
    """Yield successive n-sized chunks from l."""
    for i in range(0, len(l), n):
        yield l[i:i+n]

def splitInBlocks (l, n):
    """split the list l in n blocks of equal size"""
    k = len(l) / n
    r = len(l) % n

    i = 0
    blocks = []
    while i < len(l):
        if len(blocks)<r:
            blocks.append(l[i:i+k+1])
            i += k+1
        else:
            blocks.append(l[i:i+k])
            i += k

    return blocks

def formOutName (outputDir, tag, outputName):
    oput = outputDir
    if oput[-1] != '/':
        oput += '/'
    oput += tag + '/' + outputName 
    return oput

def parseInputFileList (fileName) :
    filelist = []
    with open (fileName) as fIn:
        for line in fIn:
            line = (line.split("#")[0]).strip()
            if line:
                filelist.append(line)
    return filelist

def writeln(f, line):
    f.write(line + '\n')

###########

parser = argparse.ArgumentParser(description='Command line parser of skim options')
parser.add_argument('--input',      dest='input',  help='input filelist',   required = True)
parser.add_argument('--tag',        dest='tag',    help='production tag',   required = True)
parser.add_argument('--njobs',      dest='njobs',  help='njobs', type=int,  default=500)
##
############################################################
## all the following are for "expert" use, not meant to be used by default
##
parser.add_argument('--outputName', dest='oname',  help='the name of the output (if not given, auto from filelist)', default = None)
parser.add_argument('--outputDir',  dest='odir',   help='the base EOS output directory. Use a {0} for username placeholder, or give it explicitely', default = "/eos/cms/store/user/{0}/YR_bbbb_trees/")
## if leaving a {0} argument, odir will be formatted with the current username
## otherwise a manual fixed path can also be given
##
# parser.add_argument('--no-tar',         dest='tar',        help='do not tar the executable folder',       action='store_false', default=True)
# parser.add_argument('--no-xrdcp-tar',   dest='xrdcptar',   help='do not xrdcp the tar to EOS',       action='store_false', default=True)
# parser.add_argument('--no-xrdcp-flist', dest='xrdcpflist', help='do not xrdcp the filelist to EOS',  action='store_false', default=True)
parser.add_argument('--dry-run',        dest='dryrun',     help='dry run without launching',         action='store_true',  default=False)
parser.add_argument('--verbose',        dest='verbose',    help='set verbose mode',                  action='store_true',  default=False)
parser.add_argument('--force',          dest='force',  help='force submission (delete work folder if existing)', action='store_true', default=False)
##
# parser.add_argument('--cfg',       dest='skim_cfg',   help='the name of the skim cfg',          default='skim.cfg')

# args = parser.parse_args()
args, unknown = parser.parse_known_args()

#### the output is saved in
#### odir / tag / oname
##### e.g.
#### root://cmseos.fnal.gov//store/user/lcadamur/bbbb_ntuples/' + TAG + '/' + ONAME

##################
executable = 'test/skimNtuple'
execute_in = os.getcwd() + '/..'
##################

queue = "1nh" # give bsub queue -- 8nm (8 minutes), 1nh (1 hour), 8nh, 1nd (1day), 2nd, 1nw (1 week), 2nw

username = getpass.getuser()
print "... Welcome", username

## oname: used for the jobs dir
# oname = args.oname
# if not oname:
    # print "A:" , args.input
    # print "B:" , args.input.rsplit(r'/', 1)[-1]
    # print "C:" , args.input.rsplit(r'/', 1)[0].rsplit('.', 1)
#     oname = args.input.rsplit(r'/', 1)[-1].rsplit('.', 1)[0]
# oname = 'SKIM_' + oname

## the dir where to save the output
# odir = args.odir.format(username)
# oLFN_base = formOutName(odir, args.tag, oname)
# if oLFN_base[-1] == '/':
#     oLFN_base = oLFN_base[:-1]

## where to run jobs
jobsDir                = 'jobs_' + args.tag
outScriptNameBareProto = 'job_{0}.sh'
outScriptNameProto     = (jobsDir + '/' + outScriptNameBareProto)
log_proto = 'job_{0}.log'
err_proto = 'job_{0}.err'

## where to store output
outDir                 = args.odir.format(username) + '/' + args.tag
outFileNameProto       = outDir + '/' + 'bbbbNtuple_{0}.root' ## full LFN

## where to fetch input lists from
inFileListBareProto    = 'filelist_{0}.txt'
inFileListProto        = jobsDir + '/' + inFileListBareProto

# jobsDir                = 'jobs_' + args.tag + '/' + oname
# outListNameBareProto   = 'filelist_{0}.txt'
# outListNameProto       = (jobsDir + '/' + outListNameBareProto)
# outScriptNameProto     = (jobsDir + '/' + outScriptNameBareProto)
# EOSfilelistBase        = oLFN_base + '/filelist'
# EOSfilelistProto       = EOSfilelistBase + '/' + outListNameBareProto
# outFileNameProto       = 'bbbbNtuple_{0}.root'

cmssw_base    = os.environ['CMSSW_BASE']
cmssw_version = os.environ['CMSSW_VERSION']
scram_arch    = os.environ['SCRAM_ARCH']

### make output dir
os.system('mkdir %s' % outDir)


# tarName      = 'bbbbAnalysis.tar.gz' #%s_tar.tgz' % cmssw_version
# bbbbWorkDir  = os.getcwd()
# tarLFN       = bbbbWorkDir + '/tars/' + tarName

# tarEOSdestLFN         = ('root://cmseos.fnal.gov//store/user/{0}/bbbb_analysis_tar/' + tarName).format(username)
# filelistEOSdestLFNdir = ('root://cmseos.fnal.gov//store/user/{0}/bbbb_analysis_tar/' + tarName).format(username)

inputfiles = parseInputFileList (args.input)    ## parse input list
njobs      = args.njobs if args.njobs <= len (inputfiles) else len (inputfiles)
fileblocks = splitInBlocks (inputfiles, njobs)
if njobs != len(fileblocks):
    print "** ERROR: length of file lists and njobs do not match, something went wrong"
    sys.exit()

## build the full command to launch
## all to format on the job number

## come commands MUST NOT be forwarded - so check and throw in case

# full_command = ' '.join(commands)
## now forward all the other commands to full_command
# full_command += ' ' + ' '.join(unknown)

full_command = executable + ' ' + ('batch/'+inFileListProto) + ' ' + outFileNameProto

#######

print "** Skim of filelist      :", args.input
print "** Production tag        :", args.tag
print "** Njobs                 :", njobs
print "** Working in            :", jobsDir
print "** Saving output to      :", outDir
print "** The script will execute :", full_command

if os.path.isdir(jobsDir):
    if not args.force:
        print "... working folder", jobsDir, " already exists, exit"
        sys.exit()
    else:
        print "... working folder", jobsDir, " already exists, FORCING ITS DELETION "
        cmd = 'rm -r ' + jobsDir
        print ">>> ", cmd
        os.system(cmd)        
os.system('mkdir -p ' + jobsDir)

## filelist
for n in range(0, njobs):
    inFileList = inFileListProto.format(n)
    jobfilelist = open(inFileList, 'w')
    for f in fileblocks[n]: jobfilelist.write(f+"\n")
    jobfilelist.close()

################################################################
## prepare the tarball of the executables
# if args.tar:
#     # tar -zcvf <Tar output name> <folder to tar>
#     # note -v is verbose option
#     ## tp exlcude something:
#     ### command = 'tar --exclude="{0}" --exclude="{1}" -zcf {2} -C {3} {4}'.format(excludePath, excludePath2, tarLFN, cmsswWorkDir, cmssw_version)

#     ### NOTE: I must be in bbbb
#     to_include = [
#         'bin/',
#         'lib/',
#         'config/',
#         'data/',
#     ]

#     command = 'tar -zcf {0} '.format(tarLFN)
#     for ti in to_include:
#         command += ti + ' '
#     # command += ' -zcf {0} -C {1} {2}'.format(tarLFN, cmsswWorkDir, cmssw_version)

#     print '** INFO: Going to tar executable folder into', tarName
#     if args.verbose: print "** INFO: executing:", command
#     os.system(command)
#     print '** INFO: tar finished and saved in:', tarLFN
# else:
#     print '** INFO: Not going to tar executable folder, using', tarLFN


##############################################################
# launch he jobs

proxynamebase = 'x509up_u63159'
homebase = '/afs/cern.ch/user/l/lcadamur/' ## keep trailing /
proxyname = '/tmp/'+proxynamebase
myproxyname = homebase + proxynamebase

regenerate_proxy = False
if not os.path.isfile(myproxyname): ## proxy file does not exist
    print "... proxy file does not exist"
    regenerate_proxy = True
else:
    lifetime = subprocess.check_output(['voms-proxy-info', '--file', myproxyname, '--timeleft']) ## in seconds
    lifetime = float(lifetime)
    lifetime = lifetime / (60*60)
    print "... proxy lifetime is ", lifetime, "hours"
    if lifetime < 10.0: ## at least 10 hours
        print "... proxy has expired"
        regenerate_proxy = True
if regenerate_proxy:
    print "... regenerating proxy"
    redone_proxy = False
    while not redone_proxy:
        status = os.system('voms-proxy-init -voms cms')
        if os.WEXITSTATUS(status) == 0:
            redone_proxy = True
        else:
            print "... something when wrong with proxy regeneration, please try again"
    print "... copying proxy" , proxyname, "to ", homebase
    os.system('cp %s %s' % (proxyname, homebase))  


#### copy the proxy to the home area, so that it can be used by the job to run xrootd
#### se lancio piu volte questo script, e un job sta gia girando, puo fallire se sostituisco il file di proxy?
#### per essere sicuro, dovrei copiare il proxy solo se piu vecchio di XXXX tempo
#### voms-proxy-info --file /afs/cern.ch/user/l/lcadamur/x509up_u63159 --timeleft ==> stampa il tempo rimanente del proxy in secondi
#### si puo usare nelllo script e nel caso copiare il file (e.g. se il tempo e' 0 o troppo corto)
#### da capire qual e' il valore di ritorno se il proxy e' scaduto
#### + aggiungi check se il proxy proprio non esiste
# os.system('cp %s %s' % (proxyname, homebase))



# script
for n in range(0, njobs):
    this_full_command = full_command.format(n)
    outScriptName  = outScriptNameProto.format(n)
    outScript      = open(outScriptName, 'w')
    outScript.write("#!/bin/sh\n")
    outScript.write("echo\n")
    outScript.write("echo\n")
    outScript.write("export X509_USER_PROXY=%s\n" % myproxyname) ### no need for proxy since I access local files
    outScript.write("echo 'START---------------'\n")
    outScript.write('echo "... start job at" `date "+%Y-%m-%d %H:%M:%S"`\n')
    outScript.write("echo 'WORKDIR ' ${PWD}\n")
    outScript.write("source /afs/cern.ch/cms/cmsset_default.sh\n")
    outScript.write("cd "+str(execute_in)+"\n")
    outScript.write("eval `scramv1 runtime -sh`\n") ## cmsenv
    # outScript.write("cd ..\n")
    outScript.write("pwd\n")
    outScript.write('echo "launching the skims ..."\n')
    outScript.write(this_full_command+'\n')
    outScript.write('echo "... job ended with status $?"\n')
    outScript.write("echo 'STOP---------------'\n")
    outScript.write('echo "... end job at" `date "+%Y-%m-%d %H:%M:%S"`\n')
    outScript.write("echo\n")
    outScript.write("echo\n")
    outScript.close()
    os.system("chmod 755 %s" % outScriptName)

## set directory to job directory, so that logs will be saved there
os.chdir(jobsDir)

# for n in range(0, njobs):
#     command = "%s/scripts/t3submit %s" % (bbbbWorkDir, outScriptNameBareProto.format(n))
#     if not args.dryrun:
#         if args.verbose: print "** INFO: submit job with command", command
#         os.system(command)
###### sends bjobs ######
if not args.dryrun:
    for i in range(njobs):
        os.system("bsub -q "+queue+" -o %s -e %s %s" % (log_proto.format(i), err_proto.format(i), outScriptNameBareProto.format(i)))
        print "job nr " + str(i) + " submitted"
