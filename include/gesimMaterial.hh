#ifndef gesimMaterial_HH
#define gesimMaterial_HH
////////////////////////////////////////////////////////////////////////////////
#include "G4Material.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include <vector>

class gesimMaterialMessenger;
////////////////////////////////////////////////////////////////////////////////
//
class gesimMaterial
{
public:

  gesimMaterial ();
  ~gesimMaterial ();

public:

  void  AddMaterial (G4String, G4String, G4double, G4String, 
                     G4double tem = CLHEP::STP_Temperature, 
                     G4double pres = CLHEP::STP_Pressure);
  void  AddMaterial (G4Material * aMaterial);
  G4Material* GetMaterial (G4int i)  {return fMaterial[i];};
  G4Material* GetMaterial (G4String name)
    {return G4Material::GetMaterial(name);} ;
  G4int GetMaterialIndex (G4String);
  G4int GetNbOfMaterial () {return fMaterial.size();};
  void  DeleteMaterial (G4int);
  void  DeleteMaterial (G4String);

  void  ListMaterial();

private:

  gesimMaterialMessenger         *fMaterialMessenger;

  std::vector<G4Material*>   fMaterial;
  std::vector<G4Element*>    fElement;
  std::vector<G4Isotope*>    fIsotope;

private:
  static const G4String        fELU[110];
  static const G4String        fELL[110];
  static const G4String        fEUU[110];
  static const G4double        fA[110];
       
};
////////////////////////////////////////////////////////////////////////////////
#endif
