#ifdef G4ANALYSIS_USE
#include <AIDA/AIDA.h>
#endif
//
#ifdef G4ANALYSIS_USE_ROOT
#include "TROOT.h"
#include "TApplication.h"
#include "TGClient.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TH1D.h"
#include "TNtuple.h"
#endif

#include "gesimHisto.hh"
#include "gesimHistoMessenger.hh"
#include "G4ParticleTable.hh"

#include "G4Tokenizer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
gesimHisto::gesimHisto()
:fHistName("gesim"), fHistType("root"),
 fNHisto(0), fNTuple(0), fVerbose(0),
 fDefaultAct(1)
{
#ifdef G4ANALYSIS_USE
  fAida = 0;
  fTree = 0;
#endif

#ifdef G4ANALYSIS_USE_ROOT
  fROOThisto.clear();
  fROOTntup.clear();
  fRarray.clear();
  fRcol.clear();
#endif

  fActive.clear();
  fBins.clear();
  fXmin.clear();
  fXmax.clear();
  fUnit.clear();
  fIds.clear();
  fTitles.clear();
  fTupleName.clear();
  fTupleId.clear();
  fTupleList.clear();
  fTupleListROOT.clear();

  fMessenger = new gesimHistoMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimHisto::~gesimHisto()
{
#ifdef G4ANALYSIS_USE
  fHisto.clear();
  fNtup.clear();
#endif
#ifdef G4ANALYSIS_USE_ROOT
  //FIXME : G.Barrand : the below is crashy.
  //        In principle the TH are deleted
  //        when doing the TFile::Close !
  //         In fact the fHfileROOT should 
  //        be deleted in Save(). And I am pretty
  //        sure that the TApplication is not needed.
  //
  // removed by F.Lei
  //  for(G4int i=0; i<fNHisto; i++) {
  //   if(fROOThisto[i]) delete fROOThisto[i];
  // }
#endif
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::Book()
{
#ifdef G4ANALYSIS_USE
  G4cout << "### gesimHisto books " << fNHisto << " histograms " << G4endl; 
  // Creating the analysis factory
  fAida = AIDA_createAnalysisFactory();
  if(!fAida) {
    G4cout << "ERROR: can't get AIDA." << G4endl; 
    return;
  }
  // Creating the fTree factory
 {AIDA::ITreeFactory* tf = fAida->createTreeFactory(); 
  // Creating a fTree mapped to a new fAida file.
  G4String fileName = fHistName + "." + fHistType;
  if (fHistType == "root") fileName = fHistName + "_aida." + fHistType;
  fTree = tf->create(fileName,fHistType,false,true,"");
  delete tf;
  if(!fTree) { 
    G4cout << "ERROR: Tree store " << fHistName  << " is not created!" << G4endl; 
    return;
  }
  G4cout << "Tree store  : " << fTree->storeName() << G4endl;}
  // Creating a histogram factory, whose histograms will be handled by the fTree
 {AIDA::IHistogramFactory* hf = fAida->createHistogramFactory(*fTree);
  // Creating an 1-dimensional histograms in the root directory of the fTree
  for(G4int i=0; i<fNHisto; i++) {
    if(fActive[i]) {
      if(fVerbose>1)
        G4cout<<"Book: histogram "<< i << " id= " << fIds[i] <<G4endl;
      G4String tit = fIds[i];
      if(fHistType == "root") tit = "h" + fIds[i];
      fHisto[i] = hf->createHistogram1D(tit, fTitles[i], fBins[i], fXmin[i],
                                        fXmax[i]);
    }
  }
  delete hf;
  G4cout << "AIDA histograms are booked" << G4endl;}

  // Creating a tuple factory, whose tuples will be handled by the fTree  
 {AIDA::ITupleFactory* tpf =  fAida->createTupleFactory( *fTree );
  G4cout << "AIDA will Book " << fNTuple << " ntuples" << G4endl;
  for(G4int i=0; i<fNTuple; i++) {
    if(fTupleList[i] != "") {
      G4cout << "Creating Ntuple: " << fTupleName[i] <<":" <<fTupleList[i]
             << G4endl;
      fNtup[i] = tpf->create(fTupleId[i], fTupleName[i], fTupleList[i],"");
    }
  }
  delete tpf;
  G4cout << "AIDA ntuples are booked" << G4endl;}
#endif

#ifdef G4ANALYSIS_USE_ROOT
//  new TApplication("App", ((int *)0), ((char **)0));
  G4String fileNameROOT = fHistName + G4String(".root");
  fHfileROOT = new TFile(fileNameROOT.c_str() ,"RECREATE","ROOT file for gesim");
  G4cout << "Root file: " << fileNameROOT << G4endl;
  // Creating an 1-dimensional histograms in the root directory of the fTree
  for(G4int i=0; i<fNHisto; i++) {
    if(fActive[i]) {
      G4String id = G4String("h")+fIds[i];
      fROOThisto[i] = new TH1D(id, fTitles[i], fBins[i], fXmin[i], fXmax[i]);
      G4cout << "ROOT Histo " << fIds[i] << " " << fTitles[i] << " booked "
             << G4endl;
    }
  }
  // Now the ntuples  
  for(G4int i=0; i<fNTuple; i++) {
    if(fTupleListROOT[i] != "") {
      G4String id = G4String("t")+fTupleId[i];
      G4cout << "Creating Ntuple "<<fTupleId[i] << " in ROOT file: " 
             << fTupleName[i] << G4endl;
      fROOTntup[i] = new TNtuple(id, fTupleName[i], fTupleListROOT[i]);
      G4cout << "ROOT Ntuple " << id << " " << fTupleName[i] <<" "
             << fTupleListROOT[i]<< " booked " << G4endl;
    }
  }
#endif

} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::Save()
{
#ifdef G4ANALYSIS_USE
  // Write histogram file
  fTree->commit();
  G4cout << "Closing the AIDA fTree..." << G4endl;
  fTree->close();
  G4cout << "Histograms and Ntuples are saved" << G4endl;
  delete fTree;
  fTree = 0;
  delete fAida;
  fAida = 0;
  {for(G4int i=0; i<fNHisto; i++) fHisto[i] = 0;}
  {for(G4int i=0; i<fNTuple; i++) fNtup[i] = 0;}
#endif
#ifdef G4ANALYSIS_USE_ROOT
  G4cout << "ROOT: files writing..." << G4endl;
  fHfileROOT->Write();
  G4cout << "ROOT: files closing..." << G4endl;
  fHfileROOT->Close();
  //
  // F.Lei added following Guy's suggestion!
  delete fHfileROOT;

#endif
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::Add1D(const G4String& id, const G4String& name, G4int nb, 
                       G4double x1, G4double x2, G4double u)
{
  if(fVerbose > 0) {
    G4cout << "New histogram will be booked: #" << id << "  <" << name 
           << "  " << nb << "  " << x1 << "  " << x2 << "  " << u 
           << G4endl;
  }
  fNHisto++;
  x1 /= u;
  x2 /= u;
  fActive.push_back(fDefaultAct);
  fBins.push_back(nb);
  fXmin.push_back(x1);
  fXmax.push_back(x2);
  fUnit.push_back(u);
  fIds.push_back(id);
  fTitles.push_back(name);
#ifdef G4ANALYSIS_USE
  fHisto.push_back(0);
#endif
#ifdef G4ANALYSIS_USE_ROOT
  fROOThisto.push_back(0);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::SetHisto1D(G4int i, G4int nb, G4double x1, G4double x2, G4double u)
{
  if(i>=0 && i<fNHisto) {
    if(fVerbose > 0) {
      G4cout << "Update histogram: #" << i  
             << "  " << nb << "  " << x1 << "  " << x2 << "  " << u 
             << G4endl;
    }
    fBins[i] = nb;
    fXmin[i] = x1;
    fXmax[i] = x2;
    fUnit[i] = u;
  } else {
    G4cout << "gesimHisto::setgesimHisto1D: WARNING! wrong histogram index "
           << i << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::FillHisto(G4int i, G4double x, G4double w)
{
  if(fVerbose > 1) {
    G4cout << "fill histogram: #" << i << " at x= " << x 
           << "  weight= " << w
           << G4endl;   
  }
#ifdef G4ANALYSIS_USE
  if(i>=0 && i<fNHisto) {
    fHisto[i]->fill(x/fUnit[i], w);
  } else {
    G4cout << "gesimHisto::fill: WARNING! wrong AIDA histogram index "
           << i << G4endl;
  }
#endif
#ifdef G4ANALYSIS_USE_ROOT  
  if(i>=0 && i<fNHisto) {
    fROOThisto[i]->Fill(x/fUnit[i],w);
  } else {
    G4cout << "gesimHisto::fill: WARNING! wrong ROOT histogram index "
           << i << G4endl;
  }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::ScaleHisto(G4int i, G4double x)
{
  if(fVerbose > 0) {
    G4cout << "Scale histogram: #" << i << " by factor " << x << G4endl;   
  }
#ifdef G4ANALYSIS_USE
  if(i>=0 && i<fNHisto) {
    fHisto[i]->scale(x);
    G4cout << "gesimHisto::scale: WARNING! wrong AIDA histogram index "
           << i << G4endl;
  }
#endif
#ifdef G4ANALYSIS_USE_ROOT  
  if(i>=0 && i<fNHisto) {
    fROOThisto[i]->Scale(x);
  } else {
    G4cout << "gesimHisto::scale: WARNING! wrong ROOT histogram index "
           << i << G4endl;
  }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifdef G4ANALYSIS_USE
void gesimHisto::AddTuple(const G4String& w1, const G4String& w2,
                          const G4String& w3 )
#else
#ifdef G4ANALYSIS_USE_ROOT
void gesimHisto::AddTuple(const G4String& w1, const G4String& w2,
                          const G4String& w3 )
#else
void gesimHisto::AddTuple(const G4String& w1, const G4String& w2,
                          const G4String& )
#endif
#endif

{
  //G4cout << w1 << " " << w2 << " " << w3 << G4endl;
  fNTuple++;
  fTupleId.push_back(w1);
  fTupleName.push_back(w2) ;
#ifdef G4ANALYSIS_USE
  fTupleList.push_back(w3);
  fNtup.push_back(0);
#endif

#ifdef G4ANALYSIS_USE_ROOT
  std::vector<float> ar;
  ar.clear();
  for (size_t i = 0; i < 20; i++) ar.push_back(0.);
  fRarray.push_back(ar);
  // convert AIDA header to ROOT header for ntuple
  G4Tokenizer next(w3);
  G4String token = next();
  G4String ROOTList1 = "" ;
  G4int col = 0;
  while ( token != "") {
   token = next();
   if (token == ",") token = next();
   if (token.contains(",")) token.remove(token.size()-1);
   ROOTList1 = ROOTList1 + token + G4String(":");
   col++;
  }
  G4String ROOTList = ROOTList1.substr(0,ROOTList1.length()-2);
//  G4cout << ROOTList << G4endl;
  fTupleListROOT.push_back(ROOTList);
  fROOTntup.push_back(0);
  fRcol.push_back(col-1);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::FillTuple(G4int i, const G4String& parname, G4double x)
{
  if(fVerbose > 1) 
    G4cout << "fill tuple # " << i 
           <<" with  parameter <" << parname << "> = " << x << G4endl; 
#ifdef G4ANALYSIS_USE
  if(fNtup[i]) fNtup[i]->fill(fNtup[i]->findColumn(parname), x);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::FillTuple(G4int i, G4int col, G4double x)
{
  if(fVerbose > 1) {
    G4cout << "fill tuple # " << i 
           <<" in column < " << col << "> = " << x << G4endl; 
  }
#ifdef G4ANALYSIS_USE
  if(fNtup[i]) fNtup[i]->fill(col,double(x));
#endif

#ifdef G4ANALYSIS_USE_ROOT  
  if(fROOTntup[i]) (fRarray[i])[col] = float(x);
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::FillTuple(G4int i, const G4String& parname, G4String& x)
{
  if(fVerbose > 1) {
    G4cout << "fill tuple # " << i 
           <<" with  parameter <" << parname << "> = " << x << G4endl; 
  }
#ifdef G4ANALYSIS_USE
  if(fNtup[i]) fNtup[i]->fill(fNtup[i]->findColumn(parname), x);
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::AddRow(G4int i)
{
  if(fVerbose > 1) G4cout << "Added a row #" << i << " to tuple" << G4endl; 
#ifdef G4ANALYSIS_USE
  if(fNtup[i]) fNtup[i]->addRow();
#endif

#ifdef G4ANALYSIS_USE_ROOT
  float ar[10];
  for (G4int j=0; j < fRcol[i]; j++) {
//      G4cout << i << " " << fRarray[i][j] << G4endl;
      ar[j] = fRarray[i][j];       
  }  
  if(fROOTntup[i]) fROOTntup[i]->Fill(ar);
#endif

} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::SetFileName(const G4String& nam) 
{
  fHistName = nam;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const G4String& gesimHisto::GetFileName() const
{
  return fHistName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gesimHisto::SetFileType(const G4String& nam) 
{
  fHistType = nam;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const G4String& gesimHisto::FileType() const
{
  return fHistType;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

