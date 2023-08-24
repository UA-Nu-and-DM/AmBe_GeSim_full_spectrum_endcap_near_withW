#ifndef gesimHisto_h
#define gesimHisto_h 1

//---------------------------------------------------------------------------
//
// ClassName:   gesimHisto
//
// Description: Utility class to hold and manipulate histograms/nTuples
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "globals.hh"
#include <vector>

#ifdef G4ANALYSIS_USE
namespace AIDA {
 class IAnalysisFactory;
 class ITree;
 class ITuple;
 class IHistogram1D;
}
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifdef G4ANALYSIS_USE_ROOT
// Root classes
class TFile;
class TH1D;
class TNtuple;
#endif

class gesimHistoMessenger;

class gesimHisto
{

public:
  gesimHisto();

  ~gesimHisto();

  void Book();
  // Book predefined histogramms 

  void Save();
  // Save histogramms to file

  void Add1D(const G4String&, const G4String&, G4int nb=100, G4double x1=0., 
                                               G4double x2=1., G4double u=1.);
  // In this method histogramms are predefined

  void SetHisto1D(G4int, G4int, G4double, G4double, G4double);
  // It change fBins and boundaries

  void FillHisto(G4int, G4double, G4double);
  // gesimHistogramms are filled

  void ScaleHisto(G4int, G4double);

  void AddTuple(const G4String&, const G4String&, const G4String&);
  // In this method fNTuple is booked

  void FillTuple(G4int, const G4String&, G4double);
  // Fill fNTuple parameter with a double

  void FillTuple(G4int, G4int, G4double);
  // Fill fNTuple at a given col with a double
  void FillTuple(G4int, const G4String&, G4String&);
  // Fill fNTuple parameter with a string

  void FillTuple(G4int, const G4String&, G4bool);
  // Fill fNTuple parameter with a bool

  void AddRow(G4int);
  // Save tuple event 

  void SetFileName(const G4String&);
  const G4String& GetFileName() const;  

  void SetFileType(const G4String&);
  const G4String& FileType() const;

private:

  G4String fHistName;
  G4String fHistType;

  G4int    fNHisto;
  G4int    fNTuple;
  G4int    fVerbose;
  G4int    fDefaultAct;
#ifdef G4ANALYSIS_USE
  std::vector<AIDA::IHistogram1D*> fHisto;
  std::vector<AIDA::ITuple*>   fNtup;
  AIDA::IAnalysisFactory* fAida;
  AIDA::ITree*    fTree;
#endif

#ifdef G4ANALYSIS_USE_ROOT
  TFile* fHfileROOT; 
  std::vector<TH1D*> fROOThisto;
  std::vector<TNtuple*>   fROOTntup;
  std::vector< std::vector<float> > fRarray;
  std::vector<G4int> fRcol;
#endif

  gesimHistoMessenger* fMessenger;

  std::vector<G4int>     fActive;
  std::vector<G4int>     fBins;
  std::vector<G4double>  fXmin;
  std::vector<G4double>  fXmax;
  std::vector<G4double>  fUnit;
  std::vector<G4String>  fIds;
  std::vector<G4String>  fTitles;
  std::vector<G4String>  fTupleName;
  std::vector<G4String>  fTupleId;
  std::vector<G4String>  fTupleList;
  std::vector<G4String>  fTupleListROOT; 
};

#endif
