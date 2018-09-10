import argparse
import os
import sys
import subprocess

parser = argparse.ArgumentParser('list options')
parser.add_argument('--path', dest='path', help='directory path starting from /store/..', required=True)
parser.add_argument('--site', dest='site', help='where is the prod', required=True)
parser.add_argument('--out',  dest='out', help='out file name', required=True)
args = parser.parse_args()

lines = []
path = args.path
if args.path[-1] == '/':
    args.path = args.path[:-1]

if args.site.lower() == 'cern':
    # serv = ''
    # out = subprocess.check_output([])
    print '... TODO!'
    pass

elif args.site.lower() == 'desy':
    srvr = 'srm://dcache-se-cms.desy.de/pnfs/desy.de/cms/tier2'
    fpath  = srvr + path
    osrvr = 'root://dcache-cms-xrootd.desy.de//pnfs/desy.de/cms/tier2'
    out = subprocess.check_output(['gfal-ls', fpath]).split()
    # print out
    for o in out:
        l = osrvr + path + '/' + o
        lines.append(l)

elif args.site.lower() == 'fnal':
    out = subprocess.check_output(['eos', 'root://cmseos.fnal.gov', 'ls', path]).split()
    osrvr = 'root://cmseos.fnal.gov/'
    # print out
    for o in out:
        l = osrvr + path + '/' + o
        lines.append(l)

else:
    print "INVALID SITE!"
    sys.exit()

print "... got", len(lines), 'files'
ofile = open(args.out, 'w')
for l in lines:
    ofile.write(l+'\n')
ofile.close()