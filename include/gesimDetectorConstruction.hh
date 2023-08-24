#ifndef gesimDetectorConstruction_h
#define gesimDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include <vector>
//#include "gesimMagneticField.hh"

using namespace std;

class G4Tubs;
class G4Cons;
class G4Box;
class G4Orb;
class G4UnionSolid;
class G4SubtractionSolid;
class G4IntersectionSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;
class G4Colour;
class gesimDetectorMessenger;
class gesimMaterial;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gesimDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
     gesimDetectorConstruction();
     virtual ~gesimDetectorConstruction();

  public:
  
     virtual G4VPhysicalVolume* Construct();
     void SelectDetector(G4String);
     void SetSourcePosition(G4String);
     void SetDeadLayerTweak(G4double value);
     void SetMisalignment(G4double value);

     G4VPhysicalVolume* GetNitrateLayerPV() const; //added for AmLi
     G4VPhysicalVolume* GetSourcePV() const; //added for AmLi
     G4VPhysicalVolume* GetMiddleCapsulePV() const; //added for AmLi
     G4VPhysicalVolume* GetOuterCapsulePV() const; //added for AmLi

     G4VPhysicalVolume* GetSSShieldPV() const; //added for LZ AmBe source
     G4VPhysicalVolume* GetWShieldPV() const; //added for LZ AmBe source
     G4VPhysicalVolume* GetSrcCorePV() const; //added for LZ AmBe source
     G4VPhysicalVolume* GetwCapsuleBasePV() const; //added for LZ AmBe source 
     G4VPhysicalVolume* GetwCapsuleCapPV() const; //added for LZ AmBe source 
     
//     const 
//     G4VPhysicalVolume* GetDetector() {return fPhysiDetector;};
//
//     G4double GetDetectoFullLength() {return fDetectorLength;};
//     G4double GetTargetFullLength()  {return fTargetLength;};
//     G4double GetWorldFullLength()   {return fWorldLength;}; 
//     G4double GetDetectorThickness() {return fDetectorThickness;};
//     G4double GetTargetRadius()  {return fTargetRadius;};
//     G4double GetWorldRadius()   {return fWorldRadius;}; 
     
//     void SetTargetMaterial (G4String);
//     void SetDetectorMaterial(G4String);
//
//     void SetTargetRadius (G4double value) { fTargetRadius = value; };
//     void SetDetectorThickness(G4double value){ fDetectorThickness = value;};  
//     void SetTargetLength (G4double value) { fTargetLength = value; };
//     void SetDetectorLength(G4double value){ fDetectorLength = value;};  
     
  private:
     void DefineMaterials();


     
     
  private: 

     //G4Orb*             fSolidWorld;    // pointer to the solid envelope 
     G4Box*             fSolidWorld;    // pointer to the solid envelope 
     G4LogicalVolume*   fLogicWorld;    // pointer to the logical envelope
     G4VPhysicalVolume* fPhysiWorld;    // pointer to the physical envelope
     
     G4Box*             fSolidOuterShielding;   // pointer to the solid OuterShielding
     G4LogicalVolume*   fLogicOuterShielding;   // pointer to the logical OuterShielding
     G4VPhysicalVolume* fPhysiOuterShielding;   // pointer to the physical OuterShielding
     G4SubtractionSolid* fSolidOuterShieldingreplica;
               
     G4Box*             fSolidInnerShielding;   // pointer to the solid InnerShielding
     G4LogicalVolume*   fLogicInnerShielding;   // pointer to the logical InnerShielding
     G4VPhysicalVolume* fPhysiInnerShielding;   // pointer to the physical InnerShielding
     G4SubtractionSolid* fSolidInnerShieldingreplica;

     G4Box*             fSolidOuterCavity;   // pointer to the solid OuterCavity
     G4LogicalVolume*   fLogicOuterCavity;   // pointer to the logical OuterCavity
     G4VPhysicalVolume* fPhysiOuterCavity;   // pointer to the physical OuterCavity
               
     G4Box*             fSolidInnerCavity;   // pointer to the solid InnerCavity
     G4LogicalVolume*   fLogicInnerCavity;   // pointer to the logical InnerCavity
     G4VPhysicalVolume* fPhysiInnerCavity;   // pointer to the physical InnerCavity



  
     G4Tubs*             fCopperBoxCavity;  // pointer to the cavity in Copper box sample

     G4Box*	         fCopperBox;
     G4SubtractionSolid* fCopperBoxSample;
  
     G4Tubs*             fSolidDetectorMain;  // pointer to the solid DetectorMain
     G4Tubs*             fSolidDetectorWell;  // pointer to the solid DetectorWell
     G4Tubs*             fSolidDetectorGroove1;  // pointer to the solid DetectorWell
     G4Tubs*             fSolidDetectorGroove2;  // pointer to the solid DetectorWell
     G4SubtractionSolid* fSolidDetector1; // intermediate
     G4UnionSolid*       fSolidDetector2; // intermediate
     G4SubtractionSolid* fSolidDetector; // pointer to the solid Detector
     G4LogicalVolume*    fLogicDetector;  // pointer to the logical Detector


     G4LogicalVolume*    fLogicCopperBoxSample;
     G4LogicalVolume*    logicLZMarenilli;


     G4VPhysicalVolume*  fPhysiDetector;  // pointer to the physical Detector


     G4VPhysicalVolume*  physiLZMarenilli;
       

     G4Tubs*              fSolidActiveDetectorMain;  // pointer to the solid ActiveDetectorMain
     G4IntersectionSolid* fSolidActiveDetector; // pointer to the solid ActiveDetector
     G4LogicalVolume*     fLogicActiveDetector;  // pointer to the logical ActiveDetector
     G4VPhysicalVolume*   fPhysiActiveDetector;  // pointer to the physical ActiveDetector
     G4VPhysicalVolume*   fPhysiCopperBoxSample;  

     G4Tubs*            fSolidDetectorEnclosure; // pointer to the solid DetectorEnclosure
     G4SubtractionSolid * fSolidDetectorEnclosureGe2;
     G4LogicalVolume*   fLogicDetectorEnclosure;  // pointer to the logical DetectorEnclosure
     G4VPhysicalVolume* fPhysiDetectorEnclosure;  // pointer to the physical DetectorEnclosure

     G4Tubs*            fSolidAirPocket; // pointer to the solid AirPocket
     G4LogicalVolume*   fLogicAirPocket;  // pointer to the logical AirPocket
     G4VPhysicalVolume* fPhysiAirPocket;  // pointer to the physical AirPocket

     G4Tubs*            fSolidEndCapBody;  // pointer to the solid EndCap
     G4Tubs*            fSolidEndCapRim;  // pointer to the solid EndCap
     G4UnionSolid*            fSolidEndCapMain;  // pointer to the solid EndCap
     G4Tubs*            fSolidEndCapCavity;  // pointer to the solid EndCapCavity
     G4SubtractionSolid* fSolidEndCap;  // pointer to the solid EndCap
     G4LogicalVolume*   fLogicEndCap;  // pointer to the logical EndCap
     G4VPhysicalVolume* fPhysiEndCap;  // pointer to the physical EndCap

     G4Tubs* fSolidHeadBody;
     G4Tubs* fSolidHeadCavity;
     G4Tubs* fSolidHeadOpening;
     G4Tubs* fSolidThroat;
     G4SubtractionSolid* fSolidHead1;
     G4SubtractionSolid* fSolidHead2;
     G4SubtractionSolid* fSolidHead;
     G4LogicalVolume* fLogicHead;
     G4VPhysicalVolume* fPhysiHead;

     G4Tubs* fSolidSnout1;
     G4Tubs* fSolidSnout2;
     G4Tubs* fSolidSnout3;
     G4Cons* fSolidSnout4;
     G4UnionSolid* fSolidSnoutBody12;
     G4UnionSolid* fSolidSnoutBody123;
     G4UnionSolid* fSolidSnoutBody;
     G4Tubs* fSolidSnoutCavity;
     G4SubtractionSolid* fSolidSnout;
     G4LogicalVolume* fLogicSnout;
     G4VPhysicalVolume* fPhysiSnout;


     G4Tubs* fSolidNeckBody;
     G4SubtractionSolid* fSolidNeckinCopper;
     G4SubtractionSolid* fSolidNeckinLead;
     G4SubtractionSolid* fSolidNeck;
     G4LogicalVolume* fLogicNeck;
     G4LogicalVolume* fLogicNeckinCopper;
     G4LogicalVolume* fLogicNeckinLead;
     G4VPhysicalVolume* fPhysiNeck;
     G4VPhysicalVolume* fPhysiNeckinCopper;
     G4VPhysicalVolume* fPhysiNeckinLead;
     G4Tubs* fSolidNeckShieldHole;     // to subtract from the outer shield 
     G4Tubs* fSolidNeckInnerShieldHole;// to subtract from the inner shield
  
     G4Tubs* fSolidNeckCavityBody;
     G4Tubs* fSolidNeckinCopperShielding;
     G4Tubs* fSolidNeckinLeadShielding;
     G4Tubs* fSolidNeckCavityBodyinCopper;
     G4Tubs* fSolidNeckCavityBodyinLead;
     G4SubtractionSolid* fSolidNeckCavity;
     G4LogicalVolume*    fLogicNeckCavity;
     G4VPhysicalVolume*  fPhysiNeckCavity;
     G4LogicalVolume*    fLogicNeckCavityinCopper;
     G4VPhysicalVolume*  fPhysiNeckCavityinCopper;
     G4LogicalVolume*    fLogicNeckCavityinLead;
     G4VPhysicalVolume*  fPhysiNeckCavityinLead;

     G4Tubs* fSolidColdFingerRod;
     G4Tubs* fSolidColdFingerRodinCopper;
     G4Tubs* fSolidColdFingerRodinLead;
     G4Tubs* fSolidColdFingerCavity;
     G4Tubs* fSolidColdFingerCavityinCopper;
     G4Tubs* fSolidColdFingerCavityinLead;
     G4SubtractionSolid* fSolidColdFingerHollowRod;
     G4SubtractionSolid* fSolidColdFingerHollowRodinCopper;
     G4SubtractionSolid* fSolidColdFingerHollowRodinLead;
     G4SubtractionSolid* fSolidColdFingerHollowRodSideGrooved;
     G4SubtractionSolid* fSolidColdFingerHollowRodSideGroovedinCopper;
     G4SubtractionSolid* fSolidColdFingerHollowRodSideGroovedinLead;
     G4SubtractionSolid* fSolidColdFingerHollowRod2SidesGrooved;
     G4SubtractionSolid* fSolidColdFinger;
     G4SubtractionSolid* fSolidColdFingerinCopper;
     G4SubtractionSolid* fSolidColdFingerinLead;
     G4SubtractionSolid* fSolidColdFingerCavityShort;
     G4Box* fSolidGroove;
     G4Box* fSolidGrooveinCopper;
     G4Box* fSolidGrooveinLead;
     G4LogicalVolume* fLogicColdFinger;
     G4LogicalVolume* fLogicColdFingerinCopper;
     G4LogicalVolume* fLogicColdFingerinLead;
     G4LogicalVolume* fLogicColdFingerCavity;
     G4LogicalVolume* fLogicColdFingerCavityinCopper;
     G4LogicalVolume* fLogicColdFingerCavityinLead;
     G4VPhysicalVolume* fPhysiColdFinger;
     G4VPhysicalVolume* fPhysiColdFingerinCopper;
     G4VPhysicalVolume* fPhysiColdFingerinLead;
     G4VPhysicalVolume* fPhysiColdFingerCavity;
     G4VPhysicalVolume* fPhysiColdFingerCavityinCopper;
     G4VPhysicalVolume* fPhysiColdFingerCavityinLead;

     G4Tubs* fSolidHolderTopBody;
     G4Tubs* fSolidHolderRim;
     G4Tubs* fSolidHolderRidge1;
     G4Tubs* fSolidHolderRidge2;
     G4UnionSolid* fSolidHolderTopMain1;
     G4UnionSolid* fSolidHolderTopMain2;
     G4UnionSolid* fSolidHolderTopMain;
     G4Tubs*            fSolidHolderBottomMain;
     G4UnionSolid*      fSolidHolderMain;
     G4Tubs*            fSolidHolderCavity1;
     G4Tubs*            fSolidHolderCavity2;
     G4Tubs*            fSolidHolderCavity3;
     G4UnionSolid*      fSolidHolderCavity12;
     G4UnionSolid*      fSolidHolderCavity;
     G4SubtractionSolid* fSolidHolderIntermediate; // pointer to the solid Holder
     G4SubtractionSolid* fSolidHolder; // pointer to the solid Holder
     G4LogicalVolume*   fLogicHolder;  // pointer to the logical Holder
     G4VPhysicalVolume* fPhysiHolder;  // pointer to the physical Holder
     
     G4Tubs*             fSolidSheathMain;  // pointer to the solid SheathMain
     G4Tubs*             fSolidSheathCavity;  // pointer to the solid SheathCavity
     G4SubtractionSolid* fSolidSheath; // pointer to the solid Sheath
     G4LogicalVolume*   fLogicSheath;  // pointer to the logical Sheath
     G4VPhysicalVolume* fPhysiSheath;  // pointer to the physical Sheath

     G4Tubs* fSolidSignalPinMain;
     G4Cons* fSolidSignalPinHead;
     G4Tubs* fSolidSignalPinNeck;
     G4Tubs* fSolidSignalPinWell;
     G4UnionSolid* fSolidSignalPin1;
     G4UnionSolid* fSolidSignalPin2;
     G4SubtractionSolid* fSolidSignalPin ;
     G4LogicalVolume*   fLogicSignalPin;
     G4VPhysicalVolume* fPhysiSignalPin;

     G4Tubs* fSolidSignalPinSupportMain1;
     G4Tubs* fSolidSignalPinSupportMain2;
     G4Tubs* fSolidSignalPinSupportHole1; 
     G4Tubs* fSolidSignalPinSupportHole2;
     G4UnionSolid* fSolidSignalPinSupportMain ;
     G4UnionSolid* fSolidSignalPinSupportHole ;
     G4SubtractionSolid* fSolidSignalPinSupport ;
     G4LogicalVolume* fLogicSignalPinSupport;
     G4VPhysicalVolume* fPhysiSignalPinSupport;
     
     G4Tubs* fSolidPlasticMountBody;
     G4Tubs* fSolidPlasticMountCavity;
     G4LogicalVolume*   fLogicPlasticMount;
     G4VPhysicalVolume* fPhysiPlasticMount;

     G4Tubs* fSolidButton;
     G4LogicalVolume* fLogicButton;
     G4VPhysicalVolume* fPhysiButton;

     G4Box* fSolidCopperBlock;
     G4LogicalVolume* fLogicCopperBlock;
     G4VPhysicalVolume* fPhysiCopperBlock;

     G4Tubs* fSolidCopperTube;
     G4LogicalVolume* fLogicCopperTube;
     G4VPhysicalVolume* fPhysiCopperTube;

     G4Tubs * solidBottleLiquidPre;
     G4Tubs * solidBottleNeckCavity;
     G4Tubs * solidBottleBodyCavity;
     G4Tubs * solidBottleCapCavity;
     G4Tubs * solidBottleNeck;
     G4Tubs * solidBottleBody;
     G4Tubs * solidBottleCap;
     G4Tubs * solidBottleFlap;

     G4IntersectionSolid * solidBottleLiquid;
     G4UnionSolid * solidBottleNeckBodyCavity;
     G4UnionSolid * solidBottleInterior;
     G4UnionSolid * solidBottleNeckBody;
     G4UnionSolid * solidBottleNeckBodyCap;
     G4UnionSolid * solidBottleExterior;

     G4LogicalVolume * logicBottleLiquid;
     G4LogicalVolume * logicBottleInterior;
     G4LogicalVolume * logicBottleExterior;

     G4VPhysicalVolume * physiBottleLiquid;
     G4VPhysicalVolume * physiBottleInterior;
     G4VPhysicalVolume * physiBottleExterior;

     G4VPhysicalVolume * source_phys;//added for LZ AmLi src
     G4VPhysicalVolume * middlecapsule_phys;//added for LZ AmLi src
     G4VPhysicalVolume * outercapsule_phys;//added for LZ AmLi src
     G4VPhysicalVolume * nitrateLayer_phys;//added for LZ AmLi src

     G4VPhysicalVolume * ssShield_phys;//added for LZ AmBe src
     G4VPhysicalVolume * wShield_phys;//added for LZ AmBe src
     G4VPhysicalVolume * ssShieldLid_phys;//added for LZ AmBe src
     G4VPhysicalVolume * wShieldLid_phys;//added for LZ AmBe src
     G4VPhysicalVolume * srcCore_phys;//added for LZ AmBe src
     G4VPhysicalVolume * wCapsuleBase_phys;//added for LZ AmBe src
     G4VPhysicalVolume * wCapsuleCap_phys;//added for LZ AmBe src
     

     gesimDetectorMessenger* fDetectorMessenger;  // pointer to the Messenger
     gesimMaterial* fMaterialsManager;         // material manager
      
     G4Material* fDefaultMater;           // Default material
     G4Material* fOuterShieldingMater;    // Outer shielding material
     G4Material* fInnerShieldingMater;    // Inner shielding material
     G4Material* fSolderMater;            // Solder wire material
     G4Material* fGe2EndCapMater;         // Detector material
     G4Material* fGe3EndCapMater;         // Detector material
     G4Material* fHeadMater;              // Detector material
     G4Material* fNeckMater;              // Detector material
     G4Material* fNeckCavityMater;        // Detector material
     G4Material* fColdFingerMater;        // Detector material
     G4Material* fColdFingerCavityMater;  // Detector material
     G4Material* fHolderMater;            // Detector material
     G4Material* fSheathMater;            // Detector material
     G4Material* fSignalPinMater;         // Detector material
     G4Material* fSignalPinSupportMater;  // Detector material
     G4Material* fPlasticMountMater;      // Detector material
     G4Material* fDetectorMater;          // Detector material
     G4Material* fDetectorEnclosureMater; // Detector material
     G4Material* fButtonMater;            // Detector material
     G4Material* fButtonFillerMater;      // Detector material
     G4Material* fCopperBlockMater;       // Detector material
     G4Material* fCopperTubeMater;        // Detector material
     G4Material* fCarbonFiberMater;       // Detector material
     G4Material* fCarbonFiberMater_M17_2[4];// Detector material
     G4Material* fExo200CapsuleMater;
     G4Material* fExo200PlugMater;
     G4Material* fExo200SourceMater;
     G4Material* fExo200SourceHolderMater;
     G4Material* fBottleMater;
     G4Material* fBottleLiquidMater;
     G4Material* fGerdaD6Mater;
     G4Material* fXLPEMater;
     G4Material* fCeilingMater;
     G4Material* fResinMater_M16[5];
     G4Material* fWippSaltMater[17]; 
     G4Material* fZiplocMater;
     G4Material* fWippSalt5DetailedMater;
     G4Material* fR028Mater;
     G4Material* fR029Mater;
     G4Material* fR030Mater;
     G4Material* fCalibBottleMater;
     G4Material* fCalibSilicaMater;
 
     // Misc. Detector components
     G4Material* fIrShieldMater1;
     G4Material* fIrShieldMater2;

     G4double fWorldX;            // Length of the world volume in X-dimension
     G4double fWorldY;            // Length of the world volume in Y-dimension
     G4double fWorldZ;            // Length of the world volume in Z-dimension
     G4double fWorldR;            // Radius of the world volume

     G4double fOuterShieldingX;            // Length of the outer shielding volume in X-dimension
     G4double fOuterShieldingY;            // Length of the outer shielding volume in Y-dimension
     G4double fOuterShieldingZ;            // Length of the outer shielding volume in Z-dimension
     G4double fOuterShieldingThickness;    // Thickness of the outer shielding 
     G4double fOuterCavityX;            // Length of the outer cavity volume in X-dimension
     G4double fOuterCavityY;            // Length of the outer cavity volume in Y-dimension
     G4double fOuterCavityZ;            // Length of the outer cavity volume in Z-dimension

     G4double fInnerShieldingX;            // Length of the inner shielding volume in X-dimension
     G4double fInnerShieldingY;            // Length of the inner shielding volume in Y-dimension
     G4double fInnerShieldingZ;            // Length of the inner shielding volume in Z-dimension
     G4double fInnerShieldingThickness;    // Thickness of the inner shielding 
     G4double fInnerCavityX;            // Length of the inner cavity volume in X-dimension
     G4double fInnerCavityY;            // Length of the inner cavity volume in Y-dimension
     G4double fInnerCavityZ;            // Length of the inner cavity volume in Z-dimension

     G4double fDetectorEnclosureLength;         // Full length of the Detector
     G4double fDetectorEnclosureRadius;         // Radius of the Detector

     G4double fEndCapRadius;
     G4double fEndCapLength;
     G4double fEndCapEntranceWindowThickness;
     G4double fEndCapSideThickness;
     G4double fEndCapCavityRadius;
     G4double fEndCapCavityLength;
     G4double fEndCapRimWidth;
     G4double fEndCapRimThickness;

     G4double fHeadRadius;
     G4double fHeadLength;
     G4double fHeadWallThickness;
     G4double fHeadOpeningRadius;
     G4double fHeadOpeningLength;

     G4double fSnout1Length;
     G4double fSnout1Radius;
     G4double fSnout2Length;
     G4double fSnout2Radius;
     G4double fSnout3Length;
     G4double fSnout3Radius;
     G4double fSnout4Length;
     G4double fSnout4RadiusTop;
     G4double fSnout4RadiusBottom;
     G4double fSnoutCavityRadius;
     G4double fSnoutCavityLength;

     G4double fNeckRadius;
     G4double fNeckThickness;
     G4double fNeckOffset; // distance from neck center to "back" of head
     G4double fNeckLocation; //distance from inner cavity center to neck center.

     G4double fColdFingerRadius;
     G4double fColdFingerCavityRadius;
     G4double fColdFingerSideGrooveDepth;

     G4double fHolderTopLength;
     G4double fHolderTopRadius;
     G4double fHolderBottomLength;
     G4double fHolderBottomRadius;
     G4double fHolderCavity1Length;
     G4double fHolderCavity1Radius;
     G4double fHolderCavity2Length;
     G4double fHolderCavity2Radius;
     G4double fHolderCavity3Length;
     G4double fHolderCavity3Radius;
     G4double fHolderCavityOffset;
     G4double fHolderRimWidth;
     G4double fHolderRimThickness;
     G4double fHolderRidge1Width;
     G4double fHolderRidge1Thickness;
     G4double fHolderRidge1Position; //distance between top of ridge and top of holder.
     G4double fHolderRidge2Width;
     G4double fHolderRidge2Thickness;
     G4double fHolderRidge2Position; //distance between top of ridge and top of holder.
     G4double fHolderOffset;
  
     G4double fSheathLength;         // Full length of the Sheath
     G4double fSheathRadius;         // Radius of the Sheath
     G4double fSheathSideThickness;
     G4double fSheathFrontThickness;
     G4double fSheathGap; 
     G4double fSheathBaseHeight;

     G4double fSignalPinLength;
     G4double fSignalPinRadius;
     G4double fSignalPinHeadLength;
     G4double fSignalPinHeadUpperRadius;
     G4double fSignalPinHeadLowerRadius;
     G4double fSignalPinNeckLength;
     G4double fSignalPinNeckRadius;
     G4double fSignalPinWellDepth;
     G4double fSignalPinWellRadius;

     G4double fSignalPinSupportLength1;
     G4double fSignalPinSupportRadius1;
     G4double fSignalPinSupportLength2;
     G4double fSignalPinSupportRadius2;
     G4double fSignalPinSupportHoleRadius1;
     G4double fSignalPinSupportHoleLength1;
     G4double fSignalPinSupportHoleRadius2;
     G4double fSignalPinSupportHoleLength2;
    
     G4double fPlasticMountLength;     // Full length of the plastic mounting piece
     G4double fPlasticMountRadius; 
     G4double fPlasticMountCavityLength; 
     G4double fPlasticMountCavityRadius;
  
     G4double fDetectorLength;         // Full length of the detector
     G4double fDetectorRadius;         // Radius of the detector
     G4double fDetectorWellLength;     // Full length of the detector
     G4double fDetectorWellRadius;     // Radius of the detector well
     G4double fDetectorBaseHeight;
     G4double fDetectorGrooveInnerRadius;
     G4double fDetectorGrooveOuterRadius;
     G4double fDetectorGrooveDepth;
     G4double fDeadLayerSideThickness;
     G4double fDeadLayerTopThickness;
     G4double fFrontFaceGap;

     G4double fButtonRadius;
     G4double fButtonLength;
     G4double fCopperBlockX;
     G4double fCopperBlockY;
     G4double fCopperBlockZ;
     G4double fCopperTubeRadius;
     G4double fCopperTubeLength;
     G4double fCopperTubeThickness;

     G4double fDeadLayerTweak;
     G4double fMisalignment;

     G4Colour MakeColor(G4double hue, G4double a);
     std::vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems);
     std::vector<std::string> split(const std::string &s, char delim);
     void PrintVolumeInfo(G4LogicalVolume * lv);

     G4String fDetId;
     G4String fSourcePosition;

     G4NistManager* nistMan; 

     //G4Region*   fTargetRegion;
     //G4Region*   fDetectorRegion;
};

inline G4VPhysicalVolume* gesimDetectorConstruction::GetNitrateLayerPV() const {
     return nitrateLayer_phys;
}                               //added for AmLi
inline G4VPhysicalVolume* gesimDetectorConstruction::GetSourcePV() const {
     return source_phys;                               //added for AmLi
}

inline G4VPhysicalVolume* gesimDetectorConstruction::GetMiddleCapsulePV() const {
     return middlecapsule_phys;                               //added for AmLi
}

inline G4VPhysicalVolume* gesimDetectorConstruction::GetOuterCapsulePV() const {
     return outercapsule_phys;                               //added for AmLi
}


inline G4VPhysicalVolume* gesimDetectorConstruction::GetSSShieldPV() const {
     return ssShield_phys;     //added for AmBe
}

inline G4VPhysicalVolume* gesimDetectorConstruction::GetWShieldPV() const {
     return wShield_phys;     //added for AmBe
}

inline G4VPhysicalVolume* gesimDetectorConstruction::GetSrcCorePV() const {
     return srcCore_phys;     //added for AmBe
}

inline G4VPhysicalVolume* gesimDetectorConstruction::GetwCapsuleBasePV() const {
     return wCapsuleBase_phys;     //added for AmBe

}

inline G4VPhysicalVolume* gesimDetectorConstruction::GetwCapsuleCapPV() const {
     return wCapsuleCap_phys;     //added for AmBe

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
