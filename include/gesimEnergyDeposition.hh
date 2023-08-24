#ifndef gesimEnergyDeposition_h
#define gesimEnergyDeposition_h 1

#include "globals.hh"

class gesimEnergyDeposition
{
  public:   // with description

    gesimEnergyDeposition();
    gesimEnergyDeposition( const gesimEnergyDeposition &right );
    gesimEnergyDeposition( G4double, G4double, G4double );
    virtual ~gesimEnergyDeposition();
         // Constructor and virtual destructor

    G4bool operator==(const gesimEnergyDeposition &right) const ;
    G4bool operator< (const gesimEnergyDeposition &right) const ;
    G4bool operator<=(const gesimEnergyDeposition &right) const ;
  // Operators  

  G4double GetEnergy() {return fEnergy;};
  G4double GetTime() {return fTime;};
  G4double GetWeight() {return fWeight;};
  // Accessors

  private:

    G4double fEnergy;  
    G4double fTime;    
    G4double fWeight;
};
#endif



