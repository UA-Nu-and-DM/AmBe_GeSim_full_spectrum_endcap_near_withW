#include "gesimEnergyDeposition.hh"


//
// Default constructor
//
gesimEnergyDeposition::gesimEnergyDeposition()
: fEnergy(0.), fTime(0.),fWeight(1.)
{;}
//
// Specific constructor
//
gesimEnergyDeposition::gesimEnergyDeposition( G4double energy,
                                    G4double time,
                                    G4double weight )
  : fEnergy(energy),
    fTime(time),
    fWeight(weight)
{;}


//
// Copy constructor
//
gesimEnergyDeposition::gesimEnergyDeposition(
                                                   const gesimEnergyDeposition &right )
  : fEnergy(right.fEnergy),
    fTime(right.fTime),
    fWeight(right.fWeight)
{;}

//
// Destructor
//
gesimEnergyDeposition::~gesimEnergyDeposition() {;}

//
// Equivalence operator
//
G4bool gesimEnergyDeposition::operator==
                                          ( const gesimEnergyDeposition &right ) const
{
  return fTime == right.fTime;
}

//
// Order operators
//
G4bool gesimEnergyDeposition::operator<
                                  ( const gesimEnergyDeposition &right ) const
{
  return fTime < right.fTime;
}

G4bool gesimEnergyDeposition::operator<=
                                  ( const gesimEnergyDeposition &right ) const
{
  return fTime <= right.fTime;
}

