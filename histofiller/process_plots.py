import ROOT
import argparse
import collections
import fnmatch

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--dir', dest='dir', help='dir name', required=True)
args = parser.parse_args()

print "... running on folder", args.dir

fIn_name  = args.dir + '/outPlotter.root'
fOut_name = args.dir + '/analysedOutPlotter.root'
fIn  = ROOT.TFile(fIn_name)
fOut = ROOT.TFile(fOut_name, 'recreate')

histos   = collections.OrderedDict()
histos2D = collections.OrderedDict()

keys = fIn.GetListOfKeys()
print "... file contains", len(keys), 'objects'

for key in keys:
    kname = key.GetName()
    # print key, kname
    obj = fIn.Get(kname)

    # 1D plots
    if isinstance(obj, ROOT.TH1):
        obj.SetDirectory(0)
        histos[kname] = obj

    ## 2D plots
    if isinstance(obj, ROOT.TH2):
        obj.SetDirectory(0)
        self.histos2D[kname] = obj

    ### create stat unc QCD histos
    # if fnmatch.fnmatch(kname, 'QCD_*_BDTG_6_masscut'):
    if fnmatch.fnmatch(kname, 'QCD_*_BDTG*'):
        print "... computing systs on", kname
        print "... tot bins", obj.GetNbinsX()
        
        ### find the last N filled bins
        Ntofind = 2
        shift   = 0.01
        found   = []
        for ibin in reversed(range(1, obj.GetNbinsX() +1)):
            cont = obj.GetBinContent(ibin)
            if cont != 0:
                print ' >> bin', ibin, 'has', cont, 'yield'
                found.append(ibin)
                if len(found) >= Ntofind: ## I am done
                    break
        print '... found bins for systs:', found
        if len(found) != Ntofind:
            print ' @@@@ sizes do not match, skipping'
            continue
        h_up = obj.Clone(kname+"_QCDbinUp")
        h_do = obj.Clone(kname+"_QCDbinDown")
        h_up.SetDirectory(0)
        h_do.SetDirectory(0)
        for ibin in found:
            cont = obj.GetBinContent(ibin)
            h_up.SetBinContent(ibin, (1.0+shift)*cont)
            h_do.SetBinContent(ibin, (1.0-shift)*cont)
        histos[h_up.GetName()] = h_up
        histos[h_do.GetName()] = h_do

print "... will stageout", len(histos), "1D histos"
print "... will stageout", len(histos2D), "2D histos"

#### copy all to the output file
fOut.cd()
for h in histos:
    histos[h].SetLineColor(ROOT.kRed)
    histos[h].Write()
for h in histos2D:
    histos2D[h].Write()
