#include "gesimDetectorConstruction.hh"
#include "gesimDetectorMessenger.hh"
//#include "gesimDetectorSD.hh"
#include "G4UImanager.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4Torus.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
//#include "G4SDManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "gesimMaterial.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4ios.hh"
#include <sstream>

static const G4double inch = 2.54*cm;
static const G4double mil = 0.001*inch;
static const G4double lb = 0.453592*kg;
static const G4double ft = 12.*inch;
static const G4double ft3 = ft*ft*ft;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
gesimDetectorConstruction::gesimDetectorConstruction()
:fSolidWorld(0),  fLogicWorld(0),  fPhysiWorld(0),
 fSolidOuterShielding(0), fLogicOuterShielding(0), fPhysiOuterShielding(0), 
 fSolidInnerShielding(0), fLogicInnerShielding(0), fPhysiInnerShielding(0), 
 fSolidOuterCavity(0), fLogicOuterCavity(0), fPhysiOuterCavity(0), 
 fSolidInnerCavity(0), fLogicInnerCavity(0), fPhysiInnerCavity(0), 
 fSolidDetector(0),fLogicDetector(0),fPhysiDetector(0),
 fMaterialsManager(0),
 fDefaultMater(0),fOuterShieldingMater(0),fInnerShieldingMater(0), fDetectorMater(0),
 fDetectorLength(0), fDetectorRadius(0),
 fWorldX (2.*m), fWorldY (2.*m), fWorldZ (2.*m), 
 fOuterShieldingX (0), fOuterShieldingY (0), fOuterShieldingZ (0), 
 fOuterShieldingThickness (0), 
 fOuterCavityX (0), fOuterCavityY (0), fOuterCavityZ (0), 
 fInnerShieldingX (0), fInnerShieldingY (0), fInnerShieldingZ (0), 
 fInnerShieldingThickness (0), 
 fInnerCavityX (0), fInnerCavityY (0), fInnerCavityZ (0)
{
  fDetectorMessenger = new gesimDetectorMessenger(this);
  DefineMaterials();
  fDetId = "3";
  fSourcePosition = "1_Down";
  //fSourcePosition = "M16.3";
  fMisalignment = 0;
  fDeadLayerTweak = 0;
  nistMan = G4NistManager::Instance();
  nistMan->SetVerbose(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
gesimDetectorConstruction::~gesimDetectorConstruction()
{
  delete fDetectorMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimDetectorConstruction::SetSourcePosition(G4String s){
  fSourcePosition = s;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gesimDetectorConstruction::SelectDetector(G4String s){
  fDetId = s;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void gesimDetectorConstruction::SetDeadLayerTweak(G4double value){
  fDeadLayerTweak = value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void gesimDetectorConstruction::SetMisalignment(G4double value){
  fMisalignment = value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void gesimDetectorConstruction::DefineMaterials()
{
//--------- Material definition ---------


  fMaterialsManager = new gesimMaterial();
  // Lead
  fMaterialsManager->AddMaterial("Lead","Pb",11.3*g/cm3,"");
  //Germanium detector
  fMaterialsManager->AddMaterial("Germanium","Ge",5.323*g/cm3,""); 
  //Copper
  fMaterialsManager->AddMaterial("Copper","Cu",8.96*g/cm3,"");
  //Aluminum
  fMaterialsManager->AddMaterial("Aluminum","Al",2.70*g/cm3,"");
  //Polyethylene
  fMaterialsManager->AddMaterial("Polyethylene","C1-H2",0.93*g/cm3,"");
  //Polypropylene
  fMaterialsManager->AddMaterial("Polypropylene","C1-H2",0.855*g/cm3,"");
  //Carbon Fiber
  fMaterialsManager->AddMaterial("CarbonFiber","C",0.33*g/cm3,"");
  fMaterialsManager->AddMaterial("CarbonFiber_M17.2_1x2","C",0.329731*g/cm3,"");
  fMaterialsManager->AddMaterial("CarbonFiber_M17.2_3","C",0.462829*g/cm3,"");
  fMaterialsManager->AddMaterial("CarbonFiber_M17.2_4","C", 0.567113*g/cm3,"");
  fMaterialsManager->AddMaterial("CarbonFiber_M17.2_5","C",0.527175*g/cm3,"");

  //Mylar
  //fMaterialsManager->AddMaterial("Mylar","C10-H8-O4",1.38*g/cm3,"");
  // Stainless Steel
  // Steel (SS304L)
  //fMaterialsManager->AddMaterial("SS304L","Fe69.2-Cr19.4-Ni9.4-Mn1.9",8.0*g/cm3,"");
  // Steel (SS304L)
  // Acciaierie Valbruna (https://confluence.slac.stanford.edu/pages/viewpage.action?pageId=86246731)
  // SS304L	Mn	Cr 	Ni 	Fe
  // byWeight	1.91	18.23	10	69.86
  //fMaterialsManager->AddMaterial("SS304L","Fe6649-Cr1998-Ni932-Mn199-Si195-C14-P8-S5",8.0*g/cm3,"");
  // Steel (SS304)
  //fMaterialsManager->AddMaterial("SS304","Fe6703-Cr1994-Ni860-Mn199-Si194-C36-P8-S5",8.0*g/cm3,"");
  // http://hghouston.com/resources/material-property-data/stainless-steel-data/stainless-steel-compositions.aspx
  // Alloy	C 	Mn	P  	S  	Si 	Cr 	Ni 	Mo	Fe
  // 304	0.08	2.00	0.045	0.030	1.00	19.00	9.25	-	68.595
  // 304L	0.03	2.00	0.045	0.03	1.00	19.00	10  	-	67.895
  // 316L	0.03	2.00	0.045	0.030	1.00	17.00	12.0	2.5	65.395
  // Steel (SS304L)
  fMaterialsManager->AddMaterial("SS304L","C18-Si600-Mn1910-Cr18230-Mo52-Cu490-Po30-S29-N87-Ni8160-Co100-Fe69826",8.0*g/cm3,""); //from oldmc

  // Steel (SS316L)
  fMaterialsManager->AddMaterial("SS316L","C30-Mn2000-P45-S30-Si1000-Cr17000-Ni12000-Mo2500-Fe65395",8.0*g/cm3,""); 

  // Vespel SP-1
  fMaterialsManager->AddMaterial("VespelSp1","C22-H10-O5-N2",1.43*g/cm3,""); //density is from oldmc
  // http://en.wikipedia.org/wiki/Vespel

  // Resin (isoprene)
  fMaterialsManager->AddMaterial("Resin","C39-H26-O8",1.17*g/cm3,""); //density is from oldmc
  // http://en.wikipedia.org/wiki/Isoprene

  // Acrylic (PMMA: (C5O2H8)n) 
  fMaterialsManager->AddMaterial("Acrylic","C5-H8-O2",1.18*g/cm3,""); //density is from oldmc
  //http://en.wikipedia.org/wiki/Pmma
  
  // Liquid Scintillator (80/20 mixture of Dodecane (C12H26) and Pseudocumene(C9H12), equiv to C57H116) 
  //fMaterialsManager->AddMaterial("LiquidScintillator","C57-H116",0.78*g/cm3,""); 
  // Liquid Scintillator (70/30 mixture of Dodecane (C12H26) and Pseudocumene(C9H12), equiv to C111H218) 
  fMaterialsManager->AddMaterial("LiquidScintillator","C111-H218",0.78*g/cm3,""); 
   
  // Water
  fMaterialsManager->AddMaterial("Water","H2-O1",1.00*g/cm3,"");

  // densities are just nominal values from: http://physics.nist.gov/PhysRefData/XrayMassCoef/tab2.html

  // 1M nitric acid
  fMaterialsManager->AddMaterial("NitricAcid","H2018-O1054-N18",1.00*g/cm3,"");

  // 0.3M nitric acid
  fMaterialsManager->AddMaterial("NitricAcidLowConc","H1115-O565-N3",1.00*g/cm3,"");

  // Alumina - Teflon Resin: a homogeneous mixture of activated alumina (~10 g), water (~5 g captured in the alumina), and teflon resin (~77 g)
  fMaterialsManager->AddMaterial("AluminaTeflon","F3080-C1540-O572-H556-Al196",0.763*g/cm3,"");
  // Alumina - Teflon Resin: a homogeneous mixture of activated alumina (11.3% ), water (4.7% captured in the alumina), and teflon resin (84.0%)
  //fMaterialsManager->AddMaterial("AluminaTeflon","F3080-C1540-O572-H556-Al196",0.773*g/cm3,"");
  //fMaterialsManager->AddMaterial("AluminaTeflon","F4-C2",0.77*g/cm3,"");
  
  // Cross-linked Polyethylene (XLPE)
  fMaterialsManager->AddMaterial("XLPE","C1-H2",2.0*lb/ft3,"");

  // Gemac film (Polyethylene)
  fMaterialsManager->AddMaterial("polyethylene_film","C2-H4",0.9688*g/cm3,"");

  // Kovar for the Pb-210 cleaning test
  fMaterialsManager->AddMaterial("kovar","Fe5900-Bi1624-Co952",8.35*g/cm3,"");
  
  // 4M Hydrochloric Acid (HCl)
  // HCl = 4 mol HCl =: X
  // 1 liter water (H2O) = 1000 g -> 55.6 mol  =: Y
  // H:O:Cl = X*2Y : Y : X = 115.11 : 55.56 : 4 = 11511 : 5556 : 40
  //
  // HCl = 14.58% by mass -> density = 1.07043 g/cm3
  // "Perry's Chemical Engineers' Handbook" by Robert H. Perry, Don Green, Sixth Edition with supplemental data provided by the Solvay company. 
  // http://www.handymath.com/cgi-bin/hcltble3.cgi
  fMaterialsManager->AddMaterial("HCl","H11511-O5556-Cl40",1.07043*g/cm3,"");

  //1M HCl
  //fMaterialsManager->AddMaterial("HCl","H11511-O5556-Cl100",1.017*g/cm3,"")

  // Potassium hydroxide (KOH)
  // Mass of KOH = 21.6447 g -> 0.3865 mol =: X
  // Total mass of solution = 144.8843 g
  // Mass of water (H2O) = 123.24 g -> 6.8466 mol =: Y
  // K:O:H = X : X+Y : X+2*Y  = 0.3865 : 7.23 : 14.08 = 39 : 723 : 1408
  //
  // KOH = 14.94% by mass -> density = 1.13353 g/cm3
  // "Perry's Chemical Engineers' Handbook" by Robert H. Perry, Don Green, Sixth Edition with supplemental data provided by the Solvay company. 
  // http://www.handymath.com/cgi-bin/spcfgrvkoh.cgi?submit=Entry
  fMaterialsManager->AddMaterial("KOH","K39-O723-H1408",1.13353*g/cm3,"");

  fMaterialsManager->AddMaterial("Concrete","Al60-Ca101-Fe26-K17-Mg12-Mn4-Na23-Si277-O480",2.32*g/cm3,"");
  // M16.[123] Resin
  /*
    nEXO MD#16.1: Cured-ESM611+ mix of 1 part M-1 catalyst to 15,000 parts DCPD (mole ratio).
    This is Ultrene 99 DCPD manufactured by Cymetech.com. The M-1 catalyst is manufactured
    by Umicore.com, the data sheet of the catalyst can be found in attachment 9. This resin 
    further contains toluene to dissolve the Ru catalyst.
    // 15000 part: dicyclopentadiene (DCPD): C10H12
    // 1 part: M-1 catalyst (ignored)
    // Bottomline: Implemented as pure DCPD
    
    nEXO MD#16.2: Cured-ESM70 contains 1 part M-1 catalyst and to 26,830 parts DCPD and 1 part 
    M-73 catalyst (see attachment 11) to 372,900 parts DCPD (mole ratio). The M-1 and M-73
    catalysts are manufactured by Umicore.com. This mix further contains Toluene, used to dissolve 
    the Ru catalyst.
    // 26830 part: dicyclopentadiene (DCPD): C10H12   
    // 1 part: M-1 catalyst (ignored)
    // 372900 part: dicyclopentadiene (DCPD): C10H12   
    // 1 part: M-73: catalyst (ignored)
    // Bottomline: Implemented as pure DCPD
    
    nEXO MD#16.3: Cured-862/81k is 100 g EPON Resin 862 (Diglycidyl Ether of Bisphenol F) 
    is a low viscosity, liquid epoxy resin manufactured from epichlorohydrin and Bisphenol-F. A
    description can be found in attachment 8. The EPON resin is manufactured by Momentive.com 
    The mix further contains 96 g LS-81k curing agent (see attachment 10) manufactured by Lindau 
    Chemicals (Lindau.net).
    // 100 g EPON Resin 862 (Diglycidyl Ether of Bisphenol F): C19 H20 O4
    //   Number of moles: 100/312 = 0.32 mol
    // 96 g LS-81k curing agent: Proprietary. Only specified as "Modified Carboxylic Acid Anhydride": R-COOCO-R where R = C_n H_(2n+1)
    //   density of this material: 1.19
    //   n = 0: Formic anhydride, gas
    //   n = 1: Acetic anhydride, density = 1.082
    //   n = 2: Propionic anhydride, density = 1.015
    //   n = 3: Butanoic andride, density = 0.967
    //   n = 4: Pentanoic anhydride, density = 0.9223
    //   Assume n = 1, CH3COOCOCH3 -> C4 H6 O3
    //   Number of moles: 96/102 = 0.94 mol
    // Mole ratio ~ 1:3
    // Bottomline: (C19 H20 O4) + 3*(C4 H6 O3) = C31 H38 O13
  */
  fMaterialsManager->AddMaterial("Resin_M16.1","C10-H12",1.026*g/cm3,"");
  fMaterialsManager->AddMaterial("Resin_M16.2","C10-H12",0.987*g/cm3,"");
  fMaterialsManager->AddMaterial("Resin_M16.3","C31-H38-O13",1.196*g/cm3,"");
  
  // WIPP samples
  // http://130.160.100.75/Laboratory_log/133
  //
  // Sample #1 - Salt/Soot from Condenser ground, WIPP Sample
  // Dimensions=7cm X 7.5cm X 1cm
  // Mass=60.7 g
  // Density = 1.156 g/cm3    (c.f. NaCl crystal density = 2.16 g/cm3)
  fMaterialsManager->AddMaterial("Wipp_Salt_1","Na1-Cl1",1.156*g/cm3,"");

  // Sample #2 - Salt/Soot Entrance to Gowning, WIPP Sample
  // Mass=130.2 g
  fMaterialsManager->AddMaterial("Wipp_Salt_2","Na1-Cl1",1.644*g/cm3,"");
  // 
  // Sample #3 - "Clean" Salt, WIPP Sample
  // Mass=119.0 g
  fMaterialsManager->AddMaterial("Wipp_Salt_3","Na1-Cl1",1.259*g/cm3,"");
  // 
  // Sample #4 - Salt/Soot Entrance to U/G office, WIPP Sample
  // Mass=43.5 g
  fMaterialsManager->AddMaterial("Wipp_Salt_4","Na1-Cl1",1.244*g/cm3,""); 
  // 
  // Sample #5 - Soot from Vacuum after cleaning coils on HVAC Condenser, WIPP Sample
  // Mass=17.8 g
  fMaterialsManager->AddMaterial("Wipp_Salt_5","C",0.5467*g/cm3,"");  // c.f. Density of amorphous carbon: 1.8 - 2.1 g/cm3
  fMaterialsManager->AddMaterial("Wipp_Salt_5_Detailed","C",2.0*g/cm3,"");  //phony
  

  // For Wipp salt gamma emanation studies
  // Salt density is 90% of intact NaCl crystal [ref]. Here I assume 100% density. 
  // ref: http://www.wipp.energy.gov/library/Information_Repository_A/Supplemental_Information/Brodsky,%20Hansen,%20Pfeifle%201996.pdf
  fMaterialsManager->AddMaterial("Wipp_Salt","Na1-Cl1",2.16*g/cm3,"");  // Many source

  // SS mortar and pestle pulverized Si wafer
  fMaterialsManager->AddMaterial("LL134_SiWafer","Si",1.547539*g/cm3,"");  // density adjusted to match the average of measured sample weight

  // R-028: Silver Cable (Part no: SL8802-/08-201N5_CUT)
  // Implement as homogeneous material.
  // 10 wires of 30 AWG Ag-coated Cu (diameter = 0.254 mm)
  // 8 wires are insulated with Polyolefin  (Alkene, CnH2n)
  // Diameter with insulation = 0.88 mm
  // Ignored: Ag-coating, Aluminum ribbon, Polyester jacket
  // Cu mass: 8.96 x 0.254^2 x 10 = 5.7806336  =>  Cu atomic fraction = 5.78/63.5 = 0.091
  // CH2 mass: 0.9 x (0.88^2 - 0.254^2) x 8 = 5.1111648 => C atomic fraction = 5.11/14 = 0.365
  // Formula: Cu91-C365-H730
  // Total Density = 297.8 / (150.23/2)^2 / 17.54 / Pi = 0.96 g/cm3
  fMaterialsManager->AddMaterial("R-028","Cu91-C365-H730",0.96*g/cm3,""); 

  // R-029: Twisted Ethernet Cable (Part no: 73-8891-25)
  // Implement as homogeneous material.
  // 8 wires of 24 AWG Cu (diameter = 0.511 mm)
  // PE divider of unspecified thickness. (Assume 0.5 mm)
  // Cu mass: 8.96 x 0.511^2 x 8 = 18.717   => Cu atomic frcation = 18.717/63.5 = 0.295
  // PE mass: 0.9 x (1.511^2 - 0.511^2) x 8 = 14.5584 => C atomic fraction = 14.5584/14 = 1.040
  // Formula: Cu30-C104-H208
  // Total Density = 312.1 / (133.4 x 85.4 x 45.5) = 0.602 g/cm3
  fMaterialsManager->AddMaterial("R-029","Cu30-C104-H208",0.602*g/cm3,"");

  // Generic Teflon
  fMaterialsManager->AddMaterial("Teflon","C1-F2",2.2*g/cm3,"");

  // Silica for cross-calibration
  fMaterialsManager->AddMaterial("Silica","Si1-O2",1.31*g/cm3,"");


  // Loose silica for interlab comparison 2020
  fMaterialsManager->AddMaterial("LooseSilica","Si1-O2",1.*g/cm3,"");

  // Magnesium Fluoride (MgF2)
  // http://130.160.10.13/Laboratory_log/275
  fMaterialsManager->AddMaterial("MgF2","Mg1-F2",1.67*g/cm3,"");  // density adjusted to account for packing density  (Nominal density = 3.15 g/cm3)

  // Aluminum pellets
  // Density adjusted to account for packing density (nominal density = 2.7 g/cm^3)
  // Mass of aluminum = 204.8g + 68.9g = 273.7g
  // Volume of aluminum = pi(50.2mm/2 - 2mm)^( 55+/-3mm - 2mm ) = 88.85cm^3
  fMaterialsManager->AddMaterial( "AlPellets", "Al", 1.5402637685091811*g/cm3, "" );

  // Xenon gas at 760 psi (= 51.7 atm)
  fMaterialsManager->AddMaterial("GXe","Xe",51.7*5.894e-3*g/cm3,"");

  // SAES getter pellets (St707) from Peter Rowson and Brian Mong at SLAC
  // The SAES pellets that I assume you got from Peter or Brian are presumably SAES St707 media, that is by weight 70% Zr, 24.6% V, 5.4% Fe.
  // http://130.160.10.13/Laboratory_log/315
  // 334.3 g, 71 mm, Density = 334.3/(2.3*2.3*pi*7.1) = 2.83 g/cm3
  // 145.9 g, 28.0 mm, Density = 145.9/(2.3*2.3*pi*2.8) = 3.14 g/cm3
  // Assume density = 3.0 g/cm3.
  auto elZr = new G4Element("Zirconium","Zr", 40., 91.224*g/mole);
  auto elV  = new G4Element("Vanadium","V", 23., 50.942*g/mole);
  auto elFe = new G4Element("Iron","Fe", 26., 55.845*g/mole);
  auto Be = new G4Element("Beryllium","Be", 4., 9.1218*g/mole);
  auto SAESSt707 = new G4Material("SAESSt707", 3.0*g/cm3, 3);
  SAESSt707->AddElement(elZr, 0.7);
  SAESSt707->AddElement(elV,  0.246);
  SAESSt707->AddElement(elFe, 0.054);
  fMaterialsManager->AddMaterial(SAESSt707);
  // HP Zr pellets from Giorgio 
  // http://130.160.10.13/Laboratory_log/331
  // Zr+Hf>99.95%, Hf<300ppm
  // https://www.albmaterials.com/high-purity-zirconium-zr-crystal-bar-low-hafnium.html
  // 533.0 g, 73 mm, Density = 533/(2.3*2.3*pi*7.3) = 4.39 g/cm3
  // 468.1 g, 62 mm, Density = 468.1/(2.3*2.3*pi*6.2) = 4.54 g/cm3
  // Assume density = 4.46 g/cm3 (weighted average of the above two)
  auto HPZrPellets = new G4Material("HPZrPellets", 4.46*g/cm3, 1);
  HPZrPellets->AddElement(elZr, 1.0);

  // Assembled PCBs from Eric Raguzin BNL. The PCBs are assumed to be 50% Cu and 50% CH2. 
  // With an effective density of 3.448 g/cm3.
  // Note: (379.5/(245.0 x 200.0 x 2.2)*1e3 + 179.5/(181.5 x 146.5 x 2.0)*1e3)/2 = 3.448.

  auto elC = new G4Element("Carbon","C", 6., 12.0111*g/mole);
  auto elH  = new G4Element("Hydrogen","H", 1., 1.007947*g/mole);
  auto elCu = new G4Element("Copper","Cu", 29., 63.5463*g/mole);
  auto R157 = new G4Material("R-157", 3.448*g/cm3, 3);
  double ch2molarmass = 12.0111 + 2.*1.007947;
  R157->AddElement(elC,  0.5*(12.0111/ch2molarmass));
  R157->AddElement(elH,  0.5*(2.*1.007947/ch2molarmass));
  R157->AddElement(elCu, 0.500);
  fMaterialsManager->AddMaterial(R157);

  //////////////////////////////////////////////////////////////////////////////
  // Block from Omar's code:
  // rhyolite for LZ experiment
  /*
  G4Material* SiO2     = new G4Material( "SiO2",  "Si1_O2", 1.5  *g/cm3          );
  G4Material* Al2O3    = new G4Material( "Al2O3", "Al2_O3", 4.0  *g/cm3          );
  G4Material* Iron     = new G4Material( "Iron",  "Fe",     7.874*g/cm3          );
  G4Material* Rhyolite = new G4Material( "Rhyolite",        7.874*g/cm3, ncomp=3 );
  Rhyolite->AddMaterial( SiO2,  fracMass=80*perCent );
  Rhyolite->AddMaterial( Al2O3, fracMass=15*perCent );
  Rhyolite->AddMaterial( Iron,  fracMass= 3*perCent );
  */
  // weight by mass: SiO2 80%, Al2O3 15%, Fe 3%
  // weight by atom number: 1333 SiO2 + 147 Al2O3 + 54 Fe
  // density adjusted to match the average of measured sample weight
  fMaterialsManager->AddMaterial("Rhyolite", "Si1333_O3107_Al294_Fe54", 2.385*g/cm3, "");

  // Teflon (PTFE)
  // general teflon density
  fMaterialsManager->AddMaterial("PTFE", "C2F4", 2.20*g/cm3, "");

  // Ti
  fMaterialsManager->AddMaterial("Titanium", "Ti",4.43*g/cm3,"");

  // Tin
  // density of tin is calculated using the net weight of tin in the samples divided by the volume of the two bottles: 439.9g/250cc
  //fMaterialsManager->AddMaterial("Tin", "Sn", 1.7596*g/cm3, "");
  // the density of tin is calculated using the net weight of tin in the Kester with flux core samples divided by the volume of the two bottles: 359.8g/250cc
  //fMaterialsManager->AddMaterial("Tin", "Sn", 1.4392*g/cm3, "");
  // the density of tin is calculated using the net weight of tin in the Alpha no flux core samples divided by the volume of the two bottles: 338.94g/250cc
  //fMaterialsManager->AddMaterial("Tin", "Sn", 1.35576*g/cm3, "");
  // the density of tin is calculated using the net weight of tin in the Kester no flux core samples divided by the volume of the two bottles: 439.9g/250cc
  //fMaterialsManager->AddMaterial("Tin", "Sn", 1.7596*g/cm3, "");
  // the density of tin is calculated using the net weight of tin in the LZ sample divided by the volume of the Marenilli beaker= 2000g/2177cc
  fMaterialsManager->AddMaterial("Tin", "Sn", 0.9187*g/cm3, "");

  // CCD camera
  // average density, assume most of the parts are SS, change the density after weight and volume calculation
  fMaterialsManager->AddMaterial("CCD", "Fe", 1.124*g/cm3,"");

  //CCD camera lens
  // average density, assume most of the parts are SS, change the density after weight and volume calculation
  fMaterialsManager->AddMaterial("Lens_Glass", "Si", 2.067*g/cm3, "");

  // glass for radon purge
  // average density, assume most of the parts are SS, change the density after weight and volume calculation
  fMaterialsManager->AddMaterial("Bare_Glass", "Si2_O2", 2.48*g/cm3, "");
  
  // IAEA385 powder
  //fMaterialsManager->AddMaterial("IAEA_Powder", "Si", 2.56* g/cm3, "");
  fMaterialsManager->AddMaterial("IAEA_Powder", "Si", 0.965*g/cm3, "");
  //fMaterialsManager->AddMaterial("IAEA_Powder", "Si", 0.820*g/cm3, "");

  // rubber sample, use natural rubber structure and composition
  fMaterialsManager->AddMaterial("Rubber", "C5_H8", 1.259*g/cm3, "");

  // radon gas at standard temperature and pressure
  fMaterialsManager->AddMaterial("Radon", "Rn", 0.00973*g/cm3, "");

  // dust at standard temperature and pressure
  //fMaterialsManager->AddMaterial("Dust", "Si105_O409_Al22_Fe9_Ca14_Mg5_S3_C15", 0.5304*g/cm3, "");

  // 125mL PE bottle dust at standard temperature and pressure
  //fMaterialsManager->AddMaterial("Dust", "Si105_O409_Al22_Fe9_Ca14_Mg5_S3_C15", 0.4413*g/cm3, "");
 
  // 125mL PE bottle dust at standard temperature and pressure
  //fMaterialsManager->AddMaterial("Dust", "Si105_O409_Al22_Fe9_Ca14_Mg5_S3_C15", 0.209* g/cm3, "");

  // 125mL PE bottle dust at standard temperature and pressure
  // A4 coarse dust
  //fMaterialsManager->AddMaterial("Dust", "Si105_O409_Al22_Fe9_Ca14_Mg5_S3_C15", 1.20*  g/cm3, "");
  // A1 fine dust
  fMaterialsManager->AddMaterial("Dust", "Si105_O409_Al22_Fe9_Ca14_Mg5_S3_C15", 0.6955*g/cm3, "");

  // 1000mL LDPE bottle of Aluminium Oxide
  // The density of Al2O3 is 3.95 g/cm^3:
  // https://www.microgrit.com/pdfjs/web/viewer.html?file=/pdf/MICROGRIT%20WCA.pdf
  // The dry powder of Al2O3 density is 1.282 g/cm^3:
  // https://www.powderhandling.com.au/bulk-density-chart
  // The calculated average density is 1.19 g/cm^3, need to measure the empty bottle mass and re-calculate
  //fMaterialsManager->AddMaterial("AluminiumOxide", "Al2_O3", 1.282*  g/cm3, "");
  //fMaterialsManager->AddMaterial("AluminiumOxide", "Al2_O3", 3.95*   g/cm3, "");
  // average density by measurement, subtract the bottle mass
  //fMaterialsManager->AddMaterial("AluminiumOxide", "Al2_O3", 1.13944*g/cm3, "");
  
  // sandpaper 7-in bundle average density
  fMaterialsManager->AddMaterial("AluminiumOxide", "Al2_O3", 0.716*g/cm3, "");

  // sandpaper 5-in bundle average density
  //fMaterialsManager->AddMaterial("AluminiumOxide", "Al2_O3", 0.863*g/cm3, "");

  // 3M Fineesse II polishing material
  // 70% H20, 15% C32H60, 10% Al2O3, 5% C10H30O5Si5i, the average density is 1.01 g/cm3
  fMaterialsManager->AddMaterial("WaterAluminiumOxide", "H674_O1961_C12134_Al42504_Si102046", 1.01*g/cm3, "");
  // End of block from Omar's code.
  //////////////////////////////////////////////////////////////////////////////


  //Tungsten alloy for LZ AmLi source capsule

  auto W = new G4Element("Tungsten","W",74,183.84*CLHEP::g/CLHEP::mole);
  auto Fe = new G4Element("Iron","Fe",26,55.845*CLHEP::g/CLHEP::mole);
  auto Ni = new G4Element("Nickel","Ni",28,58.693*CLHEP::g/CLHEP::mole);
  auto Walloy = new G4Material("TungstenAlloy",18.51*CLHEP::g/CLHEP::cm3,3);
  Walloy->AddElement(W,97.*CLHEP::perCent);
  Walloy->AddElement(Ni,2.1*CLHEP::perCent);
  Walloy->AddElement(Fe,0.9*CLHEP::perCent);
  fMaterialsManager->AddMaterial(Walloy);


  //Gold-Lithium sandwich (defined as mixture), Au thicknedd is 0.1mm, Li thickness is 0.75mm
  auto Au = new G4Element("Gold","Au",79,196.666*CLHEP::g/CLHEP::mole);
  auto Li = new G4Element("Lithium","Li",3,6.94*CLHEP::g/CLHEP::mole);
  auto AuLi = new G4Material("Gold-Lithium-Sandwich",1.38*CLHEP::g/CLHEP::cm3,2); //actual density
  AuLi->AddElement(Au,71.0*CLHEP::perCent); //actual fraction
  AuLi->AddElement(Li,29.0*CLHEP::perCent); //actual fraction
  fMaterialsManager->AddMaterial(AuLi);

 

  //LiNO3 definition
  auto N = new G4Element("Nitrogen","N",7,14.007*CLHEP::g/CLHEP::mole);
  auto O = new G4Element("Oxygen","O",8,16.00*CLHEP::g/CLHEP::mole);
  auto LiNO3 = new G4Material("Lithium-Nitrate",2.38*CLHEP::g/CLHEP::cm3,3);//2.38 g/cm3
  LiNO3->AddElement(Li,10.0*CLHEP::perCent);
  LiNO3->AddElement(N, 20.0*CLHEP::perCent);
  LiNO3->AddElement(O, 70.0*CLHEP::perCent);
  fMaterialsManager->AddMaterial(LiNO3); 

  //just Aluminium sandwich to chek if Geant knows about 871 keV oxygen neutron absorption gammas 
  auto Al = new G4Element("Aluminium","Al",13,26.9815*CLHEP::g/CLHEP::mole);
  auto AlSandwich = new G4Material("Aluminium-sandwich",200.7*CLHEP::g/CLHEP::cm3,1);
  AlSandwich->AddElement(Al, 100.0*CLHEP::perCent);
  fMaterialsManager->AddMaterial(AlSandwich); 

  auto OSandwich = new G4Material("Oxygen-Sandwich",1.e6*CLHEP::g/CLHEP::cm3,1);
  OSandwich->AddElement(O, 100.0*CLHEP::perCent);
  fMaterialsManager->AddMaterial(OSandwich); 


  //auto AuLiNO3 = new G4Material("Gold-Lithium-Nitrate-Sandwich",10.0*CLHEP::g/CLHEP::cm3,2);
  //AuLiNO3->AddMaterial(LiNO3, 90.0*CLHEP::perCent);
  //AuLiNO3->AddElement(Au,10.0*CLHEP::perCent);
  //fMaterialsManager->AddMaterial(AuLiNO3);

  //Li2O definition
  auto Li2O = new G4Material("Lithium-Oxide",2.01*CLHEP::g/CLHEP::cm3,3);

  //Gold-Lithium-Oxygen sandwich (defined as mixture), Au thicknedd is 0.1mm, Li thickness is 0.75mm
  //auto Au = new G4Element("Gold","Au",79,196.666*CLHEP::g/CLHEP::mole);
  //auto Li = new G4Element("Lithium","Li",3,6.94*CLHEP::g/CLHEP::mole);
  auto AuLiO = new G4Material("Gold-Lithium-Oxygen-Sandwich",1.75*CLHEP::g/CLHEP::cm3,3); //actual density
  AuLiO->AddElement(Au,18.13*CLHEP::perCent); //actual fraction
  AuLiO->AddElement(Li,7.4*CLHEP::perCent); //actual fraction
  AuLiO->AddElement(O,74.47*CLHEP::perCent);
  fMaterialsManager->AddMaterial(AuLiO);

  //Fe sandwich
  //auto Fe = new G4Element("Iron","Fe",26,57.0*CLHEP::g/CLHEP::mole);
  auto Fe2O3 = new G4Material("Iron-Oxide-Sandwich",5.24*CLHEP::g/CLHEP::cm3,2);
  Fe2O3->AddElement(Fe,54.*CLHEP::perCent); 
  Fe2O3->AddElement(O,46.*CLHEP::perCent);

  auto Am241 = new G4Element("Americium","Am",95,241.057*CLHEP::g/CLHEP::mole);
  auto americium_nitrate = new G4Material("Americium_nitrate",11.68*CLHEP::g/CLHEP::cm3,3);//the density of nitrate is not known hence used the desinity of americium oxide
  americium_nitrate->AddElement(Am241,56.45*CLHEP::perCent);
  americium_nitrate->AddElement(N,9.84*CLHEP::perCent);
  americium_nitrate->AddElement(O,33.71*CLHEP::perCent);
  fMaterialsManager->AddMaterial(americium_nitrate);


  //americum di-oxide
  //
  auto AmO2 = new G4Material("Americium_Di-Oxide",11.68*CLHEP::g/CLHEP::cm3,2);
  AmO2->AddElement(Am241,88.4*CLHEP::perCent);
  AmO2->AddElement(O,11.6*CLHEP::perCent);
  fMaterialsManager->AddMaterial(AmO2);
  // add new material here


  //americum di-oxide with beryllium mixture (based om AmBe1 description in thesis of I. Ostrovsky)
  //density: 0.199 g/cm3: 0.127 mg mixture mass pressed into a cylindrical volume with d=0.57 mm and h=2.5 mm
  //atomic ratio rescaled from AmO2 for 140:10 Am to Be ratio in the micture
  auto AmO2_Be = new G4Material("Americium_Di-Oxide_Beryllium_mixture",0.199*CLHEP::g/CLHEP::cm3,3);
  AmO2_Be->AddElement(Am241,83.15*CLHEP::perCent);
  AmO2_Be->AddElement(O,10.91*CLHEP::perCent);
  AmO2_Be->AddElement(Be,5.94*CLHEP::perCent);
  fMaterialsManager->AddMaterial(AmO2_Be);
  // add new material here
  
 



  // G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    
  fDefaultMater           = fMaterialsManager->GetMaterial("Air");
  fOuterShieldingMater    = fMaterialsManager->GetMaterial("Lead");
  fInnerShieldingMater    = fMaterialsManager->GetMaterial("Copper");
  //fInnerShieldingMater  = fMaterialsManager->GetMaterial("Germanium"); // Temporary
  fGe2EndCapMater         = fMaterialsManager->GetMaterial("Copper");
  //fGe3EndCapMater       = fMaterialsManager->GetMaterial("Copper");
  fGe3EndCapMater         = fMaterialsManager->GetMaterial("Aluminum");
  fHeadMater              = fMaterialsManager->GetMaterial("Copper");
  fNeckMater              = fMaterialsManager->GetMaterial("Copper");
  fNeckCavityMater        = fMaterialsManager->GetMaterial("Vacuum");
  fColdFingerCavityMater  = fMaterialsManager->GetMaterial("Vacuum");
  fColdFingerMater        = fMaterialsManager->GetMaterial("Copper");
  fHolderMater            = fMaterialsManager->GetMaterial("Copper");
  fSheathMater            = fMaterialsManager->GetMaterial("Polyethylene");
  fSignalPinMater         = fMaterialsManager->GetMaterial("Copper");
  fSignalPinSupportMater  = fMaterialsManager->GetMaterial("Polyethylene");
  fPlasticMountMater      = fMaterialsManager->GetMaterial("Polyethylene");
  fDetectorEnclosureMater = fMaterialsManager->GetMaterial("Vacuum");
  //fDetectorEnclosureMater = fMaterialsManager->GetMaterial("Air");
  fDetectorMater          = fMaterialsManager->GetMaterial("Germanium");
  //fDetectorMater        = fMaterialsManager->GetMaterial("Copper"); //Temporary
  fCopperTubeMater        = fMaterialsManager->GetMaterial("Copper");
  fCopperBlockMater       = fMaterialsManager->GetMaterial("Copper");
  //fButtonMater          = fMaterialsManager->GetMaterial("Polyethylene");
  fButtonMater            = fMaterialsManager->GetMaterial("Acrylic");
  fButtonFillerMater      = fMaterialsManager->GetMaterial("Resin");
  //fButtonDecalMater     = fMaterialsManager->GetMaterial("Acrylic");
  fCarbonFiberMater       = fMaterialsManager->GetMaterial("CarbonFiber");
  fCarbonFiberMater_M17_2[0] = fMaterialsManager->GetMaterial("CarbonFiber_M17.2_1x2");
  fCarbonFiberMater_M17_2[1] = fMaterialsManager->GetMaterial("CarbonFiber_M17.2_3");
  fCarbonFiberMater_M17_2[2] = fMaterialsManager->GetMaterial("CarbonFiber_M17.2_4");
  fCarbonFiberMater_M17_2[3] = fMaterialsManager->GetMaterial("CarbonFiber_M17.2_5");
  fExo200CapsuleMater     = fMaterialsManager->GetMaterial("SS304L");
  fExo200PlugMater        = fMaterialsManager->GetMaterial("VespelSp1");
  fExo200SourceMater      = fMaterialsManager->GetMaterial("Acrylic");
  fExo200SourceHolderMater= fMaterialsManager->GetMaterial("Polyethylene");
  fXLPEMater              = fMaterialsManager->GetMaterial("XLPE");

  fBottleMater            = fMaterialsManager->GetMaterial("Polyethylene");
  fBottleLiquidMater      = fMaterialsManager->GetMaterial("Air");
  //fBottleLiquidMater    = fMaterialsManager->GetMaterial("LiquidScintillator");
  
  fGerdaD6Mater           = fMaterialsManager->GetMaterial("SS304L");
  // For Muon
  fCeilingMater           = fMaterialsManager->GetMaterial("Concrete");
  // For Wipp Am241 gamma emanation study
  //fCeilingMater         = fMaterialsManager->GetMaterial("Wipp_Salt");

  fResinMater_M16[1] = fMaterialsManager->GetMaterial("Resin_M16.1");
  fResinMater_M16[2] = fMaterialsManager->GetMaterial("Resin_M16.2");
  fResinMater_M16[3] = fMaterialsManager->GetMaterial("Resin_M16.3");
  fResinMater_M16[4] = fMaterialsManager->GetMaterial("Copper"); //phony. For SCSCu.

  fWippSaltMater[1] = fMaterialsManager->GetMaterial("Wipp_Salt_1");
  fWippSaltMater[2] = fMaterialsManager->GetMaterial("Wipp_Salt_2");
  fWippSaltMater[3] = fMaterialsManager->GetMaterial("Wipp_Salt_3");
  fWippSaltMater[4] = fMaterialsManager->GetMaterial("Wipp_Salt_4");
  fWippSaltMater[5] = fMaterialsManager->GetMaterial("Wipp_Salt_5");
  fWippSaltMater[11] = fMaterialsManager->GetMaterial("Air");
  fWippSaltMater[12] = fMaterialsManager->GetMaterial("Air");
  fWippSaltMater[13] = fMaterialsManager->GetMaterial("Air");
  fWippSaltMater[14] = fMaterialsManager->GetMaterial("Air");
  fWippSaltMater[15] = fMaterialsManager->GetMaterial("Air");
  fWippSaltMater[16] = fMaterialsManager->GetMaterial("Air");

  fZiplocMater = fMaterialsManager->GetMaterial("Polyethylene");
  fWippSalt5DetailedMater = fMaterialsManager->GetMaterial("Wipp_Salt_5_Detailed");

  fR028Mater = fMaterialsManager->GetMaterial("R-028");
  fR029Mater = fMaterialsManager->GetMaterial("R-029");
  fR030Mater = fMaterialsManager->GetMaterial("Acrylic");

  fCalibBottleMater = fMaterialsManager->GetMaterial("Teflon");
  fCalibSilicaMater = fMaterialsManager->GetMaterial("Silica");

  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Colour gesimDetectorConstruction::MakeColor(G4double hue, G4double a=1){
  G4double r = 0;
  G4double g = 0;
  G4double b = 0;
  hue = fmod(hue,360);
  if (hue >= 0 && hue < 120){
    g = hue;
    r = 120-hue;
    if (r > g){
      g = g/r; r = 1;
    }else{
      r = r/g; g = 1;
    }
  }else if(hue >= 120 && hue < 240){
    b = hue-120;
    g = 240-hue;
    if (b > g){
      g = g/b; b = 1;
    }else{
      b = b/g; g = 1;
    }
  }else if(hue >= 240 && hue < 360){
    r = hue-240;
    b = 360-hue;
    if (r > b){
      b = b/r; r = 1;
    }else{
      r = r/b; b = 1;
    }
  }else{
    r = 1; 
    g = 1;
    b = 1;
  }
  //G4cout << "MakeColor: " << r << ", " << g << ", " << b << G4endl;
  return G4Colour(r,g,b,a);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<std::string> & gesimDetectorConstruction::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> gesimDetectorConstruction::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void gesimDetectorConstruction::PrintVolumeInfo(G4LogicalVolume * lv){
  G4String name = lv->GetName();
  G4double mass = lv->GetMass(true,false);
  G4double volume = lv->GetSolid()->GetCubicVolume();
  G4double density = mass/volume;
  G4double matdensity = lv->GetMaterial()->GetDensity();
  G4cout << name << ": " << mass / g << " g, " << volume /cm3 << " cm3, " << density / g*cm3 << "g/cm3. (c.f. " << matdensity/g*cm3 << " g/cm3 as defined in its G4Material)\n";
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* gesimDetectorConstruction::Construct()
{
//--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  //--------- Sizes of the principal geometrical components (solids)  ---------

  G4double epsilon = 1.0*um;
  G4String detName = "";
  if(fDetId == "2"){
    detName = "GeII";
  }else if(fDetId == "3"){
    detName = "GeIII"; 
  }else if(fDetId == "4"){
    detName = "GeIV"; 
  }else{
    detName = "<Unknown>";
  }
  G4cout << "Constructing Detector: " << detName << "...\n";

  // Colors
  G4VisAttributes* RedVisAtt         = new G4VisAttributes(MakeColor(0));
  G4VisAttributes* OrangeVisAtt      = new G4VisAttributes(MakeColor(30));
  G4VisAttributes* YellowVisAtt      = new G4VisAttributes(MakeColor(60));
  G4VisAttributes* YellowGreenVisAtt = new G4VisAttributes(MakeColor(90));
  G4VisAttributes* GreenVisAtt       = new G4VisAttributes(MakeColor(120));
  G4VisAttributes* SpringGreenVisAtt = new G4VisAttributes(MakeColor(150));
  G4VisAttributes* CyanVisAtt        = new G4VisAttributes(MakeColor(180));
  G4VisAttributes* AzureVisAtt       = new G4VisAttributes(MakeColor(210));
  G4VisAttributes* AzureTransVisAtt  = new G4VisAttributes(MakeColor(210,0.5));
  G4VisAttributes* BlueVisAtt        = new G4VisAttributes(MakeColor(240));
  G4VisAttributes* VioletVisAtt      = new G4VisAttributes(MakeColor(270));
  G4VisAttributes* MagentaVisAtt     = new G4VisAttributes(MakeColor(300));
  G4VisAttributes* RoseVisAtt        = new G4VisAttributes(MakeColor(330));
  G4VisAttributes* GreyVisAtt        = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
  G4VisAttributes* CopperVisAtt      = new G4VisAttributes(G4Colour(0.8,0.5,0.1));

  
  //cout << "Split: " << split( split(fSourcePosition, '_')[0], '.' )[0] << endl;

  // Source
  //fSourcePosition = "3";  //0-3
  fButtonRadius = 0.5*inch;
  //fButtonLength = 7./32.*inch; //my own measurement
  fButtonLength = 1./4.*inch;  //conform with oldmc.  

  // Remote2 position
  double remoteOffset = 23.2*cm;
  /*// Extra bits for source position 0
  fCopperBlockX = 50.*mm;
  fCopperBlockY = 100.*mm;
  fCopperBlockZ = 25.*mm;
  fCopperTubeRadius = 25.*mm;
  fCopperTubeLength = 152.4*mm - fCopperBlockZ;
  fCopperTubeThickness = 2.*mm; // */
  // Made-up dimensions

  if(fDetId == "2"){
    //double deadLayerTweak = 0.0 *mm; // no tweak
    //double deadLayerTweak = -0.3129 * mm; // tweak_v1
    //double deadLayerTweak = 0.523 * mm; // tweak_v2
    fDeadLayerTweak = 0.523 * mm; // tweak_v2
    // Detector Dimensions
    fDetectorLength = 68.0*mm;
    fDetectorRadius = 70.5/2.0*mm;
    fDetectorWellLength = 55.0*mm;
    fDetectorWellRadius = 10.0/2.0*mm;
    //fDetectorBaseHeight = 69.0*mm; // Not used anymore.
    fDetectorGrooveInnerRadius = 21.0/2.0*mm;
    fDetectorGrooveOuterRadius = 27.0/2.0*mm;
    fDetectorGrooveDepth = 1.5*mm;
    fDeadLayerSideThickness = 0.9*mm + fDeadLayerTweak;
    fDeadLayerTopThickness = 0.9*mm + fDeadLayerTweak;
    fFrontFaceGap = 4.5*mm; // Distance between detector front face and inside of end cap

    G4cout << "Dead Layer = " << fDeadLayerSideThickness << ", " <<  fDeadLayerTopThickness << G4endl;
   
    fSheathLength = 85.0*mm;
    fSheathRadius = 71.5/2.0*mm;
    fSheathSideThickness = 0.5*mm;
    fSheathFrontThickness = 1.0*mm;
    fSheathGap = 0*um;  // Gap between Ge Crystal and sheath for ir shield
    //fSheathBaseHeight = 53.0*mm; // Not used anymore
  
    // Holder Dimensions
    // Holder is constructed as a union of 2 cylinders: HolderTop and HolderBottom, 
    // from which 3 cylinders are substracted: Cavity1, Cavity2 and Cavity3 (from top to bottom)
    fHolderTopLength = 92.0+15.0*mm;
    fHolderTopRadius = 75.0/2.0*mm;
    fHolderBottomLength = 31.0*mm;
    fHolderBottomRadius = 11.0*mm;
    fHolderCavity1Length = 92.0*mm;
    fHolderCavity1Radius = (75.0-2.0)/2.0*mm;
    fHolderCavity2Length = 30.0*mm;
    fHolderCavity2Radius = 7.9*mm;
    fHolderCavity3Length = 16.0*mm;
    fHolderCavity3Radius = 1.5*mm;
    fHolderCavityOffset = 7.5*mm; // XXX Wrong! Doesn't consider holder cover on front face (Correct. holder has no cover on front face)
    //Holder rim and two ridges. 
    fHolderRimWidth = 1.5*mm;
    fHolderRimThickness = 2.5*mm;
    fHolderRidge1Width = 7.0*mm;
    fHolderRidge1Thickness = 2.5*mm;
    fHolderRidge1Position = 18.0*mm; //distance between top of ridge and top of holder.
    fHolderRidge2Width = 7.0*mm;
    fHolderRidge2Thickness = 2.5*mm;
    fHolderRidge2Position = 45.0*mm; //distance between top of ridge and top of holder.
    fHolderOffset = 0*mm;
  
    fSignalPinLength = 35.0*mm;
    fSignalPinRadius = 2.5/2.0*mm;
    fSignalPinHeadLength = 2.0*mm;
    fSignalPinHeadUpperRadius = 9.0/2.0*mm;
    fSignalPinHeadLowerRadius = 16.0/2.0*mm;
    fSignalPinNeckLength = 10.0*mm;
    fSignalPinNeckRadius = 6.7/2.0*mm;
    fSignalPinWellDepth = 10.0*mm;
    fSignalPinWellRadius = 1.2/2.0*mm;
  
    // Signal Pin support = 2 cylinders - 2 cylinders.
    fSignalPinSupportLength1 = 9.5*mm;
    fSignalPinSupportRadius1 = 19.5/2.0*mm;
    fSignalPinSupportLength2 = 10.5*mm;
    fSignalPinSupportRadius2 = 16.0/2.0*mm;
    fSignalPinSupportHoleRadius1 = 7.0/2.0*mm;
    fSignalPinSupportHoleLength1 = 17.0*mm;
    fSignalPinSupportHoleRadius2 = 3.8/2.0*mm;
    fSignalPinSupportHoleLength2 = 3.0*mm;
  
    // End Cap, Head and Neck Dimensions
    // Mount = head, pipe = neck
    fEndCapLength = 140.0*mm;
    fEndCapRadius = 89.0/2.0*mm;
    fEndCapEntranceWindowThickness = 1.0*mm;
    fEndCapSideThickness = 1.5*mm;
    fEndCapCavityLength = fEndCapLength - fEndCapEntranceWindowThickness;
    fEndCapCavityRadius = fEndCapRadius - fEndCapSideThickness;
    fEndCapRimWidth = 12.6/2.*mm; // radial direction
    fEndCapRimThickness = 6.4*mm; // axial direction
  
    fHeadRadius = 101.6/2.0*mm;
    fHeadLength = 70.*mm;
    fHeadWallThickness = 5.0*mm;
    fHeadOpeningRadius = 64.0/2.0*mm;
    fHeadOpeningLength = 15.0*mm; 
  
    fNeckRadius = 32.2/2.*mm;
    fNeckThickness = 2.*mm; // XXX made-up number: actual thickness not known.
    fNeckOffset = 25.6*mm; // distance from neck center to "back" of head
    fNeckLocation = 8.125*inch; //distance from inner cavity center to neck center.
  
    fDetectorEnclosureRadius = fEndCapRadius + fEndCapRimWidth;
    fDetectorEnclosureLength = fEndCapLength + fHeadLength;
  }else if(fDetId == "3"){
    // Detector Dimensions
    fDetectorLength = 82.0*mm;
    fDetectorRadius = 80.0/2.0*mm;
    fDetectorWellLength = 70.0*mm;
    fDetectorWellRadius = 10.0/2.0*mm;
    //fDetectorBaseHeight = 69.0*mm; // Not used anymore.
    fDetectorGrooveInnerRadius = 26.0/2.0*mm;
    fDetectorGrooveOuterRadius = 34.0/2.0*mm;
    fDetectorGrooveDepth = 2.0*mm;
    fDeadLayerSideThickness = 0.7*mm;// + fDeadLayerTweak;
    fDeadLayerTopThickness = 0.7*mm + fDeadLayerTweak;
    fFrontFaceGap = 5.5*mm; // Distance between detector front face and inside of end cap
   
    fSheathLength = 88.0*mm;
    fSheathRadius = 81.7/2.0*mm;
    fSheathSideThickness = 0.5*mm;
    fSheathFrontThickness = 1.0*mm;
    fSheathGap = 15.*um;  // Gap between Ge Crystal and sheath for ir shield
    //fSheathBaseHeight = 53.0*mm; // Not used anymore
  
    // Holder Dimensions
    // Holder is constructed as a union of 2 cylinders: HolderTop and HolderBottom, 
    // from which 3 cylinders are substracted: Cavity1, Cavity2 and Cavity3 (from top to bottom)
    fHolderTopLength = 105.0*mm;
    fHolderTopRadius = 83.9/2.0*mm;
    fHolderBottomLength = 35.0*mm;
    fHolderBottomRadius = 25.4/2.0*mm;
    fHolderCavity1Length = 90.0*mm;
    fHolderCavity1Radius = 82.3/2.0*mm;
    fHolderCavity2Length = 30.0*mm;
    fHolderCavity2Radius = 8.0*mm;
    fHolderCavity3Length = 15.0*mm;
    //fHolderCavity3Radius = 1.5*mm;  //original
    fHolderCavity3Radius = 3*mm; // for Ge4 M6 screw
    fHolderCavityOffset = 7.5*mm; 
    //Holder rim and two ridges. 
    fHolderRimWidth = 1.3*mm;
    fHolderRimThickness = (87.7-83.9)/2*mm;
    fHolderRidge1Width = (22.6-14.0)*mm;
    fHolderRidge1Thickness = fHolderRimThickness;
    fHolderRidge1Position = 14.0*mm; //distance between top of ridge and top of holder.
    fHolderRidge2Width = (64.4-55.8)*mm;
    fHolderRidge2Thickness = fHolderRimThickness;
    fHolderRidge2Position = 55.8*mm; //distance between top of ridge and top of holder.
    fHolderOffset = 0*mm;
  
    fSignalPinLength = 30.0*mm;
    fSignalPinRadius = 3.0/2.0*mm;
    fSignalPinHeadLength = 2.0*mm;
    fSignalPinHeadUpperRadius = 9.0/2.0*mm;
    fSignalPinHeadLowerRadius = 19.0/2.0*mm; // "max19" on tech drawing, cf "16" for GeII
    fSignalPinNeckLength = 11.0*mm;
    fSignalPinNeckRadius = 6.7/2.0*mm;
    fSignalPinWellDepth = 6.0*mm;
    fSignalPinWellRadius = 1.1/2.0*mm;
  
    // Signal Pin support = 2 cylinders - 2 cylinders.
    fSignalPinSupportLength1 = 4.0*mm;
    fSignalPinSupportRadius1 = 26.0/2.0*mm;
    fSignalPinSupportLength2 = (21.0-4.0)*mm;
    fSignalPinSupportRadius2 = 16.0/2.0*mm;
    fSignalPinSupportHoleRadius1 = 7.0/2.0*mm;
    fSignalPinSupportHoleLength1 = 17.0*mm;
    fSignalPinSupportHoleRadius2 = 3.3/2.0*mm;
    fSignalPinSupportHoleLength2 = (21.0-17.0)*mm;
  
    // End Cap, Head and Neck Dimensions
    // Mount = head, pipe = neck
    fEndCapLength = 159.0*mm;
    fEndCapRadius = 95.25/2.0*mm; // 3-3/4 inch
    fEndCapEntranceWindowThickness = 1.5*mm;
    fEndCapSideThickness = 1.5*mm;
    fEndCapCavityLength = fEndCapLength - fEndCapEntranceWindowThickness;
    fEndCapCavityRadius = fEndCapRadius - fEndCapSideThickness;
    // No Rim
    fEndCapRimWidth = 0.*mm; // radial direction
    fEndCapRimThickness = 0.*mm; // axial direction
  
    fHeadRadius = 95.25/2.0*mm;
    fHeadLength = (81.5-13.0)*mm; 
    fHeadWallThickness = 3.0*mm;
    fHeadOpeningRadius = 58.0/2.0*mm;
    fHeadOpeningLength = 6.0*mm; // XXX made-up number. Actual should be between 0 mm and 11.5 mm.

    // Part of the head that goes inside end gap
    fSnout1Length = (13.0-5.3)*mm;
    fSnout1Radius = 91.25/2.*mm;
    fSnout2Length = (5.3-2.52)*mm;
    fSnout2Radius = 87.95/2.*mm;
    fSnout3Length = (2.52-1.0)*mm;
    fSnout3Radius = 91.25/2.*mm;
    fSnout4Length = 1.0*mm;
    fSnout4RadiusBottom = 91.25/2.*mm;
    fSnout4RadiusTop = fSnout4RadiusBottom - fSnout4Length; // XXX made-up number. assume 45 degree bevel.
    fSnoutCavityRadius = 58.0/2.*mm;
    fSnoutCavityLength = fSnout1Length + fSnout2Length + fSnout3Length + fSnout4Length;
  
    fNeckRadius = 32.0/2.*mm;
    fNeckThickness = 3.*mm; // XXX Simplified. Actual thickness: 3.5 mm near head, 1.5 mm elsewhere.
    fNeckOffset = 30.0*mm; // distance from neck center to "back" of head
    fNeckLocation = 8.125*inch; //distance from inner cavity center to neck center.
  
    fDetectorEnclosureRadius = fEndCapRadius + fEndCapRimWidth;
    fDetectorEnclosureLength = fEndCapLength + fHeadLength;
  }else if(fDetId == "4"){
    // Detector Dimensions
    // Spec sheet: http://130.160.10.13/GeIV/160816_092646/Canberra_GeIV_data_sheet_07052016.pdf
    fDetectorLength = 80.5*mm; // from spec sheet
    fDetectorRadius = 80.4/2.0*mm; // from spec sheet
    fDetectorWellLength = 55.0*mm; // from spec sheet
    fDetectorWellRadius = 9.5/2.0*mm;  // from spec sheet 
    fDetectorGrooveInnerRadius = 26.0/2.0*mm;
    fDetectorGrooveOuterRadius = 34.0/2.0*mm;
    fDetectorGrooveDepth = 2.0*mm;
    fDeadLayerSideThickness = 0.9*mm;// + fDeadLayerTweak;
    fDeadLayerTopThickness = 0.0004*mm;// + fDeadLayerTweak;
    fFrontFaceGap = 5.0*mm; // Distance between detector front face and inside of end cap
   
    fSheathLength = 86.5*mm;// HDPE sheath
    fSheathRadius = 82.9/2*mm;
    fSheathSideThickness = (82.9-81.5)/2*mm;
    fSheathFrontThickness = 0*mm;
    //fSheathGap is not set in Omar's code
    //fSheathGap = 15.*um;  // Gap between Ge Crystal and sheath for ir shield
    //fSheathBaseHeight = 53.0*mm; // Not used anymore
  
    // Holder Dimensions
    // Holder is constructed as a union of 2 cylinders: HolderTop and HolderBottom, 
    // from which 2 cylinders are substracted: Cavity1 and Cavity2 (from top to bottom)
    fHolderTopLength = 110.0*mm;
    fHolderTopRadius = 85.1/2*mm;
    fHolderBottomLength = 35.0*mm;  
    fHolderBottomRadius = 24.03/2*mm;
    fHolderCavity1Length = 85.0*mm;
    fHolderCavity1Radius = 83.5/2*mm;
    fHolderCavity2Length = 50.0*mm;
    fHolderCavity2Radius = 15.9/2*mm;
    fHolderCavityOffset = 10.0*mm; 
    //Holder rim and two ridges. 
    fHolderRimWidth = 1.3*mm;
    fHolderRimThickness = (89.5-85.1)/2*mm;
    fHolderRidge1Width = 8.6*mm;
    fHolderRidge1Thickness = fHolderRimThickness;
    fHolderRidge1Position = 14.0*mm; //distance between top of ridge and top of holder.
    fHolderRidge2Width = 8.6*mm;
    fHolderRidge2Thickness = fHolderRimThickness;
    fHolderRidge2Position = 46.4*mm; //distance between top of ridge and top of holder.
    fHolderOffset = -1.5*mm; //distance holder protrudes beyond ge crystal (negative means ge crystal protrudes beyond holder)
  
    fSignalPinLength = 35.0*mm;
    fSignalPinRadius = 3.3/2*mm;
    fSignalPinHeadLength = 2.0*mm;
    fSignalPinHeadLowerRadius = 16.0/2*mm; // "max19" on tech drawing, cf "16" for GeII
    fSignalPinHeadUpperRadius = 9.0/2*mm;
    fSignalPinNeckLength = 11.0*mm;
    fSignalPinNeckRadius = 7.0/2*mm;
    fSignalPinWellDepth = 6.0*mm;     //unknown to me (Omar)
    fSignalPinWellRadius = 1.1/2.0*mm;//unknown to me (Omar)
  
    // Signal Pin support = 2 cylinders - 2 cylinders.
    fSignalPinSupportLength1 = 2.0*mm;
    fSignalPinSupportRadius1 = 26.0/2*mm;
    fSignalPinSupportLength2 = (24.0-2.0)*mm;
    fSignalPinSupportRadius2 = 15.9/2*mm;
    fSignalPinSupportHoleRadius1 = 7.0/2*mm;
    fSignalPinSupportHoleLength1 = 22.0*mm;
    fSignalPinSupportHoleRadius2 = 3.3/2*mm;
    fSignalPinSupportHoleLength2 = (24.0-22.0)*mm;

    // Plastic piece that goes underneathe the holder
    fPlasticMountLength= 12.7*mm;         // Full length of the plastic mounting piece
    fPlasticMountRadius= 60.33/2*mm;
    fPlasticMountCavityLength= 12.7*mm; 
    fPlasticMountCavityRadius= 24.04/2*mm;
    
    // End Cap, Head and Neck Dimensions
    // Mount = head, pipe = neck
    fEndCapLength = 159.0*mm;
    fEndCapRadius = 95.3/2*mm;
    fEndCapEntranceWindowThickness = 1.5*mm;
    fEndCapSideThickness = 1.5*mm;
    fEndCapCavityLength = fEndCapLength - fEndCapEntranceWindowThickness;
    fEndCapCavityRadius = fEndCapRadius - fEndCapSideThickness;
    // No Rim
    fEndCapRimWidth = 0*mm; // radial direction
    fEndCapRimThickness = 0*mm; // axial direction
  
    fHeadRadius = 95.25/2*mm;
    fHeadLength = (80.0-13.0)*mm; 
    fHeadWallThickness = (95.25-89.25)/2*mm;
    fHeadOpeningRadius = 58.0/2.0*mm;
    fHeadOpeningLength = 5.0*mm;

    // Part of the head that goes inside end gap
    fSnout1Length = 7.7*mm;
    fSnout1Radius = fHeadRadius - fEndCapSideThickness;
    fSnout2Length = (10.48 - 7.7)*mm;
    double OringThickness= 2.62*mm;
    fSnout2Radius = (fHeadRadius-2.0*fEndCapSideThickness-2.0*OringThickness)/2.0*mm;
    fSnout3Length = (13.0 - 10.48 - 1.0)*mm;// 1*mm is unknown to me (Omar)
    fSnout3Radius = fSnout1Radius;
    fSnout4Length = 1.0*mm;
    fSnout4RadiusBottom = fSnout1Radius;
    fSnout4RadiusTop = fSnout4RadiusBottom - fSnout4Length; // XXX made-up number. assume 45 degree bevel.
    fSnoutCavityRadius = 58.0/2*mm;
    fSnoutCavityLength = fSnout1Length + fSnout2Length + fSnout3Length + fSnout4Length;
  
    fNeckRadius = 32.0/2*mm;
    fNeckThickness = (32.0-27.0)/2*mm;
    fNeckOffset = 28.11*mm; // distance from neck center to "back" of head (measured from the drawing)
    fNeckLocation = 8.125*inch; //distance from inner cavity center to neck center. Unknown to Omar.

    fColdFingerRadius = 19.0/2*mm;
    fColdFingerCavityRadius = 3.0/2*mm;
    fColdFingerSideGrooveDepth = (19.0-11.0)/2*mm;
    
    fDetectorEnclosureRadius = fEndCapRadius + fEndCapRimWidth;
    fDetectorEnclosureLength = fEndCapLength + fHeadLength;
  }else{
    G4cout << "Unknown detector!\n";
    return 0;
  }// */

  // X-position of the end cap external front surface.
  double endCapFace = fDetectorEnclosureLength - fNeckOffset - fNeckLocation;

  // Shielding 
  fInnerShieldingX = 24.0*inch;
  fInnerShieldingY = 16.0*inch;
  fInnerShieldingZ = 16.0*inch;
  fInnerShieldingThickness = 2.0*inch;
  if(fDetId == "2"){
    fInnerCavityX = fInnerShieldingX - 2.0*fInnerShieldingThickness;
    fInnerCavityY = fInnerShieldingY - 2.0*fInnerShieldingThickness;
    fInnerCavityZ = fInnerShieldingZ - 2.0*fInnerShieldingThickness;
  }else if(fDetId == "3" || fDetId == "4"){
    double chamberSizeTweak = -1./8*inch;
    fInnerCavityX = fInnerShieldingX - 2.0*fInnerShieldingThickness;
    fInnerCavityY = fInnerShieldingY - 2.0*fInnerShieldingThickness + chamberSizeTweak;
    fInnerCavityZ = fInnerShieldingZ - 2.0*fInnerShieldingThickness + chamberSizeTweak;
  }

  fOuterShieldingX = 40.0*inch;
  fOuterShieldingY = 32.0*inch;
  fOuterShieldingZ = 32.0*inch;
  fOuterShieldingThickness = (fOuterShieldingZ - fInnerShieldingZ)/2;
  //fOuterShieldingThickness = 5.0*inch;
  //fOuterCavityX = fOuterShieldingX - 2.0*fOuterShieldingThickness;
  //fOuterCavityY = fOuterShieldingY - 2.0*fOuterShieldingThickness;
  fOuterCavityZ = fOuterShieldingZ - 2.0*fOuterShieldingThickness;

  // Ceiling and World dimensions
  // Actual
  double ceilingThickness = 7.*inch + epsilon;
  // For wipp am241 gamma emanation study
  //double ceilingThickness = 1.*m;
  double ceilingDistance = 1.*m;
  double exteriorX = 0.5*(3.*m - fOuterShieldingX);
  double exteriorY = 0.5*(3.*m - fOuterShieldingY);
  double exteriorZ = ceilingDistance + ceilingThickness;
  fWorldX = fOuterShieldingX + 2.*exteriorX;
  fWorldY = fOuterShieldingY + 2.*exteriorY;
  fWorldZ = fOuterShieldingZ + 2.*exteriorZ;
  fWorldR = sqrt(fWorldX*fWorldX+fWorldY*fWorldY+fWorldZ*fWorldZ)/2;
  G4cout << "World Radius = " << fWorldR/mm << " mm\n";
  G4cout << "World X = " << fWorldX/mm << " mm\n";
  G4cout << "World Y = " << fWorldY/mm << " mm\n";
  G4cout << "World Z = " << fWorldZ/mm << " mm\n";
              
  //------------------------------ 
  // World
  //------------------------------ 

  // Cuboid World
  fSolidWorld= new G4Box("world",fWorldX/2.,fWorldY/2.,fWorldZ/2.);
  // Spherical World
  //fSolidWorld = new G4Orb("world",fWorldR);
  fLogicWorld = new G4LogicalVolume(fSolidWorld, fDefaultMater, "World", 0, 0, 0);
  //  Must place the World Physical volume unrotated at (0,0,0).
  fPhysiWorld = new G4PVPlacement(0,              // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 fLogicWorld,     // its logical volume
                                 "World",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0,true);         // no field specific to volume
  
  G4Box * solidCeiling = new G4Box("ceiling",fWorldX/2.,fWorldY/2.,ceilingThickness/2.);
  G4LogicalVolume * logicCeiling = new G4LogicalVolume(solidCeiling,fCeilingMater,"Ceiling",0,0,0);
  G4VPhysicalVolume * fPhysiCeiling = new G4PVPlacement(0,              // no rotation
                                 G4ThreeVector(0,0,fWorldZ/2.-ceilingThickness/2.), // at (0,0,0)
                                 logicCeiling,     // its logical volume
                                 "World",         // its name
                                 fLogicWorld,               // its mother  volume
                                 false,           // no boolean operations
                                 0,true);         // no field specific to volume
  //logicCeiling->SetVisAttributes(GreyVisAtt);
  logicCeiling->SetVisAttributes(G4Color(1,1,1,0));
  G4cout << "Ceiling bottom Z = " << (fWorldZ/2.-ceilingThickness) / mm << G4endl;

  fSolidOuterShielding = new G4Box("outerShielding",fOuterShieldingX/2,fOuterShieldingY/2,fOuterShieldingZ/2);
  if ( fDetId == '2' || fDetId == '3' ) {
    fLogicOuterShielding = new G4LogicalVolume(fSolidOuterShielding,fOuterShieldingMater,"OuterShielding",0,0,0);
    fPhysiOuterShielding = new G4PVPlacement(0,              // no rotation
					     G4ThreeVector(0,0,0),    // at (0,0,0)
					     fLogicOuterShielding,    // its logical volume
					     "OuterShielding",        // its name
					     fLogicWorld,     // its mother  volume
					     false,           // no boolean operations
					     0,true);         // no particular field 
    G4cout << "Mass of lead = " << fLogicOuterShielding->GetMass(true) / kg << " kg" << G4endl;
    G4cout << "Mass of lead = " << fLogicOuterShielding->GetMass(true,false) / kg << " kg" << G4endl;
  }
  else if ( fDetId == '4' ) {
    fSolidNeckShieldHole = new G4Tubs("neckshieldhole",0,fNeckRadius,fOuterShieldingThickness/2,0,twopi);
    fSolidOuterShieldingreplica = new G4SubtractionSolid("outerShieldingreplica",fSolidOuterShielding,fSolidNeckShieldHole,0,G4ThreeVector(-fNeckLocation,0,-(fOuterShieldingZ-fOuterShieldingThickness)/2));
    fLogicOuterShielding = new G4LogicalVolume(fSolidOuterShieldingreplica,fOuterShieldingMater,"OuterShielding",0,0,0);
    fPhysiOuterShielding = new G4PVPlacement(0,              // no rotation
					     G4ThreeVector(0,0,0),    // at (0,0,0)
					     fLogicOuterShielding,    // its logical volume
					     "OuterShielding",        // its name
					     fLogicWorld,     // its mother  volume
					     false,           // no boolean operations
					     0,true);         // no particular field 
    G4cout << "Mass of lead = " << fLogicOuterShielding->GetMass() / kg << " kg" << G4endl;
  }
  /*
  fSolidOuterCavity = new G4Box("outerCavity",fOuterCavityX/2.,fOuterCavityY/2.,fOuterCavityZ/2.);
  fLogicOuterCavity = new G4LogicalVolume(fSolidOuterCavity,fOuterShieldingMater,"OuterCavity",0,0,0);
  fPhysiOuterCavity = new G4PVPlacement(0,              // no rotation
                                  G4ThreeVector(0,0,0),    // at (0,0,0)
                                  fLogicOuterCavity,    // its logical volume                                  
                                  "OuterCavity",        // its name
                                  fLogicOuterShielding,     // its mother  volume
                                  false,           // no boolean operations
                                  0,true);         // no particular field 
  */

  fSolidInnerShielding = new G4Box("innerShielding",fInnerShieldingX/2,fInnerShieldingY/2,fInnerShieldingZ/2);
  if(fDetId == "2" || fDetId == "3"){
    fLogicInnerShielding = new G4LogicalVolume(fSolidInnerShielding,fInnerShieldingMater,"InnerShielding",0,0,0);
    fPhysiInnerShielding = new G4PVPlacement(0,              // no rotation
					     G4ThreeVector(0,0,0),    // at (0,0,0)
					     fLogicInnerShielding,    // its logical volume
					     "InnerShielding",        // its name
					     //fLogicOuterCavity,     // its mother  volume
					     fLogicOuterShielding,     // its mother  volume
					     false,           // no boolean operations
					     0,true);         // no particular field 
    //cout << "InnerShielding Mass   = " << fLogicInnerShielding->GetMass(true,false) / g << " g\n";
    //cout << "InnerShielding Volume = " << fLogicInnerShielding->GetSolid()->GetCubicVolume() / cm3 << " cm^3\n";
  }
  else if ( fDetId == '4' ) {
    fSolidNeckInnerShieldHole = new G4Tubs("neckinnershieldhole",0,fNeckRadius,fInnerShieldingThickness/2.,0.,twopi);
    fSolidInnerShieldingreplica = new G4SubtractionSolid("innerShieldingreplica",fSolidInnerShielding,fSolidNeckInnerShieldHole,0,G4ThreeVector(-fNeckLocation,0,-(fInnerShieldingZ-fInnerShieldingThickness)/2));
    fLogicInnerShielding = new G4LogicalVolume(fSolidInnerShieldingreplica,fInnerShieldingMater,"InnerShielding",0,0,0);
    fPhysiInnerShielding = new G4PVPlacement(0,              // no rotation
					     G4ThreeVector(0,0,0),    // at (0,0,0)
					     fLogicInnerShielding,    // its logical volume
					     "InnerShielding",        // its name
					     //fLogicOuterCavity,     // its mother  volume
					     fLogicOuterShielding,     // its mother  volume
					     false,           // no boolean operations
					     0,true);         // no particular field 
  }

  // The following removes the inner copper door from this source position for Ge4.
  if ( fSourcePosition == "3_Up" && fDetId == '4' )
    fSolidInnerCavity = new G4Box( "innerCavity",
				   fInnerCavityX/2 + fInnerShieldingThickness/2,
				   fInnerCavityY/2,
				   fInnerCavityZ/2 );
  else fSolidInnerCavity = new G4Box( "innerCavity",
				      fInnerCavityX/2,
				      fInnerCavityY/2,
				      fInnerCavityZ/2 );
  fLogicInnerCavity = new G4LogicalVolume(fSolidInnerCavity,fDefaultMater,"InnerCavity",0,0,0);
  fPhysiInnerCavity = new G4PVPlacement(0,              // no rotation
                                  G4ThreeVector(0,0,0),    // at (0,0,0)
                                  fLogicInnerCavity,    // its logical volume                                  
                                  "InnerCavity",        // its name
                                  fLogicInnerShielding,     // its mother  volume
                                  false,           // no boolean operations
                                  0,true);         // no particular field

  //cout << "InnerCavity Mass = " << fLogicInnerCavity->GetMass(true,false) / g << " g\n";
  //cout << "InnerCavity Volume = " << fLogicInnerCavity->GetSolid()->GetCubicVolume() / cm3 << " cm3\n";
  //------------------------------ 
  // Detector
  //------------------------------
  
  G4RotationMatrix * detectorEnclosureRotation = new G4RotationMatrix();
  detectorEnclosureRotation->rotateY(-90.*deg);
  // simulate misaligned detector
  //double misalignment = 0;//13./32.*inch; //5.*mm;  // y axis  // measured on Mar 10, 2014
  double misalignmentAngle = asin(fMisalignment/(fDetectorEnclosureLength - fNeckOffset));
  double misNeckY = (fDetectorEnclosureLength/2. - fNeckOffset)*sin(misalignmentAngle);
  double misNeckX = -(fDetectorEnclosureLength/2. - fNeckOffset)*(1-cos(misalignmentAngle));
  G4ThreeVector misNeck(misNeckX,misNeckY,0);
  detectorEnclosureRotation->rotateX(misalignmentAngle);
  G4RotationMatrix * neckRotation = new G4RotationMatrix();
  neckRotation->rotateZ(misalignmentAngle);
  G4cout << "Misalignment = " << fMisalignment/mm << " mm in +y direction." << G4endl;
  G4cout << "Misalignment angle = " << misalignmentAngle/deg << " deg." << G4endl;
  

  G4ThreeVector detectorEnclosureLocation(fDetectorEnclosureLength/2.-fNeckOffset-fNeckLocation,0,0);
  detectorEnclosureLocation += misNeck;


  // Encloses the endcap (and everything inside of it) and the mount.
  if(fDetId == "3" || fDetId == "4"){
  fSolidDetectorEnclosure = new G4Tubs("detectorEnclosure",0.,fDetectorEnclosureRadius,fDetectorEnclosureLength/2.,0.,twopi);
  fLogicDetectorEnclosure = new G4LogicalVolume(fSolidDetectorEnclosure ,fDetectorEnclosureMater,"DetectorEnclosure",0,0,0);
  fPhysiDetectorEnclosure = new G4PVPlacement(detectorEnclosureRotation,         
                                  //G4ThreeVector(0,0,0),  // DEBUG
                                  detectorEnclosureLocation,
                                  fLogicDetectorEnclosure,   // its logical volume                                  
                                  "DetectorEnclosure",       // its name
                                  fLogicInnerCavity,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 

  // Ge3 has no rim, hence air pocket is not needed.
  }else if(fDetId == "2"){
    fSolidDetectorEnclosure = new G4Tubs("detectorEnclosure",0.,fDetectorEnclosureRadius,fDetectorEnclosureLength/2.,0.,twopi);
    fSolidAirPocket = new G4Tubs("airPocket",fEndCapRadius,fDetectorEnclosureRadius+100.*epsilon,(fEndCapLength-fEndCapRimThickness)/2.,0.,twopi);
    fSolidDetectorEnclosureGe2 = new G4SubtractionSolid("detectorEnclosure",fSolidDetectorEnclosure, fSolidAirPocket,0, 
                                                                            G4ThreeVector(0,0,(fDetectorEnclosureLength-fEndCapLength+fEndCapRimThickness)/2.));
    fLogicDetectorEnclosure = new G4LogicalVolume(fSolidDetectorEnclosureGe2 ,fDetectorEnclosureMater,"DetectorEnclosure",0,0,0);
    fPhysiDetectorEnclosure = new G4PVPlacement(detectorEnclosureRotation,         
                                  //G4ThreeVector(0,0,0),  // DEBUG
                                  detectorEnclosureLocation,
                                  fLogicDetectorEnclosure,   // its logical volume                                  
                                  "DetectorEnclosure",       // its name
                                  fLogicInnerCavity,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 

  /*
    fLogicAirPocket = new G4LogicalVolume(fSolidAirPocket,fDefaultMater,"AirPocket",0,0,0); 
    fPhysiAirPocket = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,(fDetectorEnclosureLength-fEndCapLength+fEndCapRimThickness)/2.),
                                    fLogicAirPocket,   // its logical volume                                  
                                    "AirPocket",       // its name
                                    fLogicDetectorEnclosure,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
  */
  }
 

  fSolidHeadBody = new G4Tubs("headBody",0.,fHeadRadius,fHeadLength/2.,0.,twopi);
  fSolidHeadCavity = new G4Tubs("headCavity",0.,fHeadRadius-fHeadWallThickness,(fHeadLength - fHeadWallThickness - fHeadOpeningLength)/2.,0.,twopi);
  fSolidHeadOpening = new G4Tubs("headOpening",0.,fHeadOpeningRadius, (fHeadOpeningLength)/2.+epsilon,0.,twopi);
  fSolidThroat = new G4Tubs("throat",0.,fNeckRadius - fNeckThickness,fHeadRadius,0.,twopi); // Extra length to account for curved surface at head-neck junction
  G4RotationMatrix * throatRotation = new G4RotationMatrix();
  throatRotation->rotateY(-90.*deg);
  fSolidHead1 = new G4SubtractionSolid("head1",fSolidHeadBody,fSolidHeadCavity,0,G4ThreeVector(0,0,(fHeadWallThickness - fHeadOpeningLength)/2.));
  fSolidHead2 = new G4SubtractionSolid("head2",fSolidHead1,fSolidHeadOpening,0,G4ThreeVector(0,0,(fHeadLength - fHeadOpeningLength)/2.));
  fSolidHead = new G4SubtractionSolid("head",fSolidHead2,fSolidThroat,throatRotation,G4ThreeVector(fHeadRadius-fHeadWallThickness/2.,0,fNeckOffset-fHeadLength/2.));
  fLogicHead = new G4LogicalVolume(fSolidHead ,fHeadMater,"Head",0,0,0);
  G4ThreeVector headTranslation(0,0,-fSolidDetectorEnclosure->GetZHalfLength() + fHeadLength/2.);
  fPhysiHead = new G4PVPlacement(0,             // no rotation
                                  headTranslation, 
                                  fLogicHead,   // its logical volume                                  
                                  "Head",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 

  // Ge3 and Ge4 each have a snout (part of head that goes into end cap), Ge2 does not.
  if(fDetId == "3" || fDetId == "4"){
    fSolidSnout1 = new G4Tubs("snout1",0.,fSnout1Radius,fSnout1Length/2.,0.,twopi);
    fSolidSnout2 = new G4Tubs("snout2",0.,fSnout2Radius,fSnout2Length/2.+epsilon,0.,twopi);
    fSolidSnout3 = new G4Tubs("snout3",0.,fSnout3Radius,fSnout3Length/2.+epsilon,0.,twopi);
    fSolidSnout4 = new G4Cons("snout4",0.,fSnout4RadiusBottom,0,fSnout4RadiusTop,fSnout4Length/2.,0.,twopi);
    fSolidSnoutBody12 = new G4UnionSolid("snoutBody12",fSolidSnout1,fSolidSnout2,0,G4ThreeVector(0,0,(fSnout1Length+fSnout2Length)/2.));
    fSolidSnoutBody123 = new G4UnionSolid("snoutBody123",fSolidSnoutBody12,fSolidSnout3,0,G4ThreeVector(0,0,(fSnout1Length+2.*fSnout2Length+fSnout3Length)/2.));
    fSolidSnoutBody = new G4UnionSolid("snoutBody",fSolidSnoutBody123,fSolidSnout4,0,G4ThreeVector(0,0,(fSnout1Length+2.*fSnout2Length+2.*fSnout3Length+fSnout4Length)/2.));
    fSolidSnoutCavity = new G4Tubs("snoutCavity",0.,fSnoutCavityRadius,fSnoutCavityLength/2.+epsilon,0.,twopi);
    fSolidSnout = new G4SubtractionSolid("snout",fSolidSnoutBody,fSolidSnoutCavity,0,G4ThreeVector(0,0,-fSnout1Length/2.+fSnoutCavityLength/2.));
    fLogicSnout = new G4LogicalVolume(fSolidSnout ,fHeadMater,"Snout",0,0,0);
    G4ThreeVector snoutTranslation(0,0,-fSolidDetectorEnclosure->GetZHalfLength() + fHeadLength + fSnout1Length/2.); // Assume through hole
    fPhysiSnout = new G4PVPlacement(0,             // no rotation
                                    snoutTranslation,
                                    fLogicSnout,   // its logical volume                                  
                                    "Snout",       // its name
                                    fLogicDetectorEnclosure,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
  }
  //cout << "Head Mass = " << fLogicHead->GetMass(false,false) / g << " g\n";
  //cout << "Snout Mass = " << fLogicSnout->GetMass(false,false) / g << " g\n";

  if ( fDetId == '2' || fDetId == '3' ) {
    G4ThreeVector neckLocation = G4ThreeVector(-fNeckLocation,0,-fInnerCavityZ/4);
    G4ThreeVector detEncFromNeck = detectorEnclosureLocation;
    detEncFromNeck -= neckLocation;
    fSolidNeckBody = new G4Tubs("neckBody",0,fNeckRadius,fInnerCavityZ/4,0,twopi);
    //fSolidNeck = new G4SubtractionSolid("neck",fSolidNeckBody,fSolidDetectorEnclosure,detectorEnclosureRotation,G4ThreeVector(0,0,fInnerCavityZ/4));
    fSolidNeck = new G4SubtractionSolid("neck",fSolidNeckBody,fSolidDetectorEnclosure,detectorEnclosureRotation,detEncFromNeck);
    fLogicNeck = new G4LogicalVolume(fSolidNeck ,fNeckMater,"Neck",0,0,0);
    fPhysiNeck = new G4PVPlacement(0,//neckRotation,             // no rotation
				   neckLocation,
				   fLogicNeck,   // its logical volume                                  
				   "Neck",       // its name
				   fLogicInnerCavity,      // its mother  volume
				   false,            // no boolean operations
				   0,true);          // no particular field 
    //cout << "Neck Mass = " << fLogicNeck->GetMass(false,false) / g << " g\n";

    fSolidNeckCavityBody = new G4Tubs("neckCavityBody",0,fNeckRadius-fNeckThickness,fInnerCavityZ/4.,0.,twopi);
    //fSolidNeckCavity = new G4SubtractionSolid("neckCavity",fSolidNeckCavityBody,fSolidDetectorEnclosure,detectorEnclosureRotation,G4ThreeVector(0,0,fInnerCavityZ/4.));
    fSolidNeckCavity = new G4SubtractionSolid("neckCavity",fSolidNeckCavityBody,fSolidDetectorEnclosure,detectorEnclosureRotation,detEncFromNeck);
    fLogicNeckCavity = new G4LogicalVolume(fSolidNeckCavity ,fNeckCavityMater,"NeckCavity",0,0,0);
    fPhysiNeckCavity = new G4PVPlacement(0,             // no rotation
					 //G4ThreeVector(-fNeckLocation,0,-fInnerCavityZ/4),
					 G4ThreeVector(0,0,0),
					 fLogicNeckCavity,   // its logical volume
					 "NeckCavity",       // its name
					 fLogicNeck,      // its mother  volume
					 false,            // no boolean operations
					 0,true);          // no particular field
  }
  else if ( fDetId == '4' ) {
    G4ThreeVector neckLocation = G4ThreeVector(-fNeckLocation,0,-fInnerCavityZ/4);
    G4ThreeVector neckLocationinCopper = G4ThreeVector(-fNeckLocation,0,-(fInnerShieldingZ-fInnerShieldingThickness)/2);
    G4ThreeVector neckLocationinLead = G4ThreeVector(-fNeckLocation,0,-(fOuterShieldingZ-fOuterShieldingThickness)/2);	
    G4ThreeVector detEncFromNeck = detectorEnclosureLocation;

    detEncFromNeck -= neckLocation;
    fSolidNeckBody = new G4Tubs("neckBody",0,fNeckRadius,fInnerCavityZ/4,0,twopi);

    fSolidNeck = new G4SubtractionSolid("neck",fSolidNeckBody,fSolidDetectorEnclosure,detectorEnclosureRotation,detEncFromNeck);
    fLogicNeck = new G4LogicalVolume(fSolidNeck ,fNeckMater,"Neck",0,0,0);
    fPhysiNeck = new G4PVPlacement(0,//neckRotation,             // no rotation
				   neckLocation,
				   fLogicNeck,   // its logical volume                                  
				   "Neck",       // its name
				   fLogicInnerCavity,      // its mother  volume
				   false,            // no boolean operations
				   0,true);          // no particular field 
    fSolidNeckCavityBody = new G4Tubs("neckCavityBody",0,fNeckRadius-fNeckThickness,fInnerCavityZ/4,0,twopi);
    //fSolidNeckCavity = new G4SubtractionSolid("neckCavity",fSolidNeckCavityBody,fSolidDetectorEnclosure,detectorEnclosureRotation,G4ThreeVector(0,0,fInnerCavityZ/4));
    fSolidNeckCavity = new G4SubtractionSolid("neckCavity",fSolidNeckCavityBody,fSolidDetectorEnclosure,detectorEnclosureRotation,detEncFromNeck);
    fLogicNeckCavity = new G4LogicalVolume(fSolidNeckCavity ,fNeckCavityMater,"NeckCavity",0,0,0);
    fPhysiNeckCavity = new G4PVPlacement(0,             // no rotation
					 //G4ThreeVector(-fNeckLocation,0,-fInnerCavityZ/4),
					 G4ThreeVector(0,0,0),
					 fLogicNeckCavity,   // its logical volume
					 "NeckCavity",       // its name
					 fLogicNeck,      // its mother  volume
					 false,            // no boolean operations
					 0,true);          // no particular field 

    fSolidNeckinCopperShielding = new G4Tubs("neckincopperShielding",0,fNeckRadius,fInnerShieldingThickness/2,0,twopi);
    fSolidNeckCavityBodyinCopper = new G4Tubs("neckCavityBodyinCopper",0,fNeckRadius-fNeckThickness,fInnerShieldingThickness/2,0,twopi);
    fSolidNeckinCopper = new G4SubtractionSolid("neckCavityinCopper",fSolidNeckinCopperShielding,fSolidNeckCavityBodyinCopper,0,neckLocationinCopper);
    fLogicNeckinCopper = new G4LogicalVolume(fSolidNeckinCopper,fNeckMater,"NeckinCopper",0,0,0);
    fPhysiNeckinCopper = new G4PVPlacement(0,//neckRotation,             // no rotation
					   neckLocationinCopper,
					   fLogicNeckinCopper,   // its logical volume
					   "NeckinCopper",       // its name
					   fLogicInnerShielding,      // its mother  volume
					   false,            // no boolean operations
					   0,true);          // no particular field 

    fLogicNeckCavityinCopper = new G4LogicalVolume(fSolidNeckCavityBodyinCopper,fNeckCavityMater,"NeckCavityinCopper",0,0,0);
    fPhysiNeckCavityinCopper = new G4PVPlacement(0,             // no rotation
						 G4ThreeVector(0,0,0),
						 fLogicNeckCavityinCopper,   // its logical volume
						 "NeckCavityinCopper",       // its name
						 fLogicNeckinCopper,      // its mother  volume
						 false,            // no boolean operations
						 0,true);          // no particular field 

    fSolidNeckinLeadShielding = new G4Tubs("neckinLeadShielding",0,fNeckRadius,fOuterShieldingThickness/2,0,twopi);
    fSolidNeckCavityBodyinLead = new G4Tubs("neckCavityBodyinLead",0,fNeckRadius-fNeckThickness,fOuterShieldingThickness/2,0,twopi);
    fSolidNeckinLead = new G4SubtractionSolid("neckCavityinLead",fSolidNeckinLeadShielding,fSolidNeckCavityBodyinLead,0,neckLocationinLead);
    fLogicNeckinLead = new G4LogicalVolume(fSolidNeckinLead,fNeckMater,"NeckinLead",0,0,0);
    fPhysiNeckinLead = new G4PVPlacement(0,//neckRotation,             // no rotation
					 neckLocationinLead,
					 fLogicNeckinLead,   // its logical volume
					 "NeckinLead",       // its name
					 fLogicOuterShielding,      // its mother  volume
					 false,            // no boolean operations
					 0,true);          // no particular field 

    fLogicNeckCavityinLead = new G4LogicalVolume(fSolidNeckCavityBodyinLead,fNeckCavityMater,"NeckCavityinLead",0,0,0);
    fPhysiNeckCavityinLead = new G4PVPlacement(0,             // no rotation
					       G4ThreeVector(0,0,0),
					       fLogicNeckCavityinLead,   // its logical volume
					       "NeckCavityinLead",       // its name
					       fLogicNeckinLead,      // its mother  volume
					       false,            // no boolean operations
					       0,true);          // no particular field 
    // Copper cold finger:
    fSolidColdFingerRod = new G4Tubs("coldFingerRod",0,fColdFingerRadius,fInnerCavityZ/4,0,twopi);
    fSolidColdFingerCavity = new G4Tubs("coldFingerRodCavity",0,fColdFingerCavityRadius,fInnerCavityZ/4,0,twopi);
    fSolidColdFingerHollowRod = new G4SubtractionSolid("coldFingerHollowRod",fSolidColdFingerRod,fSolidColdFingerCavity,0,G4ThreeVector(0,0,0));
    fSolidGroove = new G4Box("coldFingerGroove",fColdFingerSideGrooveDepth/2,fColdFingerSideGrooveDepth/2,fInnerCavityZ/4);
    fSolidColdFingerHollowRodSideGrooved = new G4SubtractionSolid("coldFingerHollowRodSideGrooved",fSolidColdFingerHollowRod,fSolidGroove,0,G4ThreeVector(fColdFingerRadius-(fColdFingerSideGrooveDepth/2),0,0));
    fSolidColdFingerHollowRod2SidesGrooved = new G4SubtractionSolid("coldFingerHollowRod2SidesGrooved",fSolidColdFingerHollowRodSideGrooved,fSolidGroove,0,G4ThreeVector(-fColdFingerRadius+(fColdFingerSideGrooveDepth/2),0,0));
    fSolidColdFinger = new G4SubtractionSolid("coldFinger",fSolidColdFingerHollowRod2SidesGrooved,fSolidDetectorEnclosure,detectorEnclosureRotation,detEncFromNeck);

    fLogicColdFinger = new G4LogicalVolume(fSolidColdFinger,fColdFingerMater,"ColdFinger",0,0,0);
    fPhysiColdFinger = new G4PVPlacement(0,             // no rotation
					 G4ThreeVector(0,0,0),
					 fLogicColdFinger,   // its logical volume
					 "ColdFinger",       // its name
					 fLogicNeckCavity,      // its mother  volume
					 false,            // no boolean operations
					 0,true);          // no particular field 

    fSolidColdFingerCavityShort = new G4SubtractionSolid("coldFingerShort",fSolidColdFingerCavity,fSolidDetectorEnclosure,detectorEnclosureRotation,detEncFromNeck);

    fLogicColdFingerCavity = new G4LogicalVolume(fSolidColdFingerCavityShort,fColdFingerCavityMater,"ColdFingerCavity",0,0,0);
    fPhysiColdFingerCavity = new G4PVPlacement(0,             // no rotation
					       G4ThreeVector(0,0,0),
					       fLogicColdFingerCavity,   // its logical volume
					       "ColdFingerCavity",       // its name
					       fLogicNeckCavity,      // its mother  volume
					       false,            // no boolean operations
					       0,true);          // no particular field 

    fSolidColdFingerRodinCopper = new G4Tubs("coldFingerRodinCopper",0,fColdFingerRadius,fInnerShieldingThickness/2,0,twopi);
    fSolidColdFingerCavityinCopper = new G4Tubs("coldFingerRodCavityinCopper",0,fColdFingerCavityRadius,fInnerShieldingThickness/2,0,twopi);
    fSolidColdFingerHollowRodinCopper = new G4SubtractionSolid("coldFingerHollowRodinCopper",fSolidColdFingerRodinCopper,fSolidColdFingerCavityinCopper,0,G4ThreeVector(0,0,0));
    fSolidGrooveinCopper = new G4Box("coldFingerGrooveinCopper",fColdFingerSideGrooveDepth/2,fColdFingerSideGrooveDepth/2,fInnerShieldingThickness/2);
    fSolidColdFingerHollowRodSideGroovedinCopper = new G4SubtractionSolid("coldFingerHollowRodSideGroovedinCopper",fSolidColdFingerHollowRodinCopper,fSolidGrooveinCopper,0,G4ThreeVector(fColdFingerRadius-(fColdFingerSideGrooveDepth/2),0,0));

    fSolidColdFingerinCopper =  new G4SubtractionSolid("coldFingerHollowRod2SidesGroovedinCopper",fSolidColdFingerHollowRodSideGroovedinCopper,fSolidGrooveinCopper,0,G4ThreeVector(-fColdFingerRadius+(fColdFingerSideGrooveDepth/2),0,0));
    fLogicColdFingerinCopper = new G4LogicalVolume(fSolidColdFingerinCopper,fColdFingerMater,"ColdFingerinCopper",0,0,0);
    fPhysiColdFingerinCopper = new G4PVPlacement(0,             // no rotation
						 G4ThreeVector(0,0,0),
						 fLogicColdFingerinCopper,   // its logical volume
						 "ColdFingerinCopper",       // its name
						 fLogicNeckCavityinCopper,      // its mother  volume
						 false,            // no boolean operations
						 0,true);          // no particular field 

    fLogicColdFingerCavityinCopper = new G4LogicalVolume(fSolidColdFingerCavityinCopper,fColdFingerCavityMater,"ColdFingerCavityinCopper",0,0,0);
    fPhysiColdFingerCavityinCopper = new G4PVPlacement(0,             // no rotation
						       G4ThreeVector(0,0,0),
						       fLogicColdFingerCavityinCopper,// logical volume
						       "ColdFingerCavityinCopper",    // name
						       fLogicNeckCavityinCopper,      // mother  volume
						       false,            // no boolean operations
						       0,true);          // no particular field 

    fSolidColdFingerRodinLead = new G4Tubs("coldFingerRodinLead",0,fColdFingerRadius,fOuterShieldingThickness/2,0,twopi);
    fSolidColdFingerCavityinLead = new G4Tubs("coldFingerRodCavityinLead",0,fColdFingerCavityRadius,fOuterShieldingThickness/2,0,twopi);
    fSolidColdFingerHollowRodinLead = new G4SubtractionSolid("coldFingerHollowRodinLead",fSolidColdFingerRodinLead,fSolidColdFingerCavityinLead,0,G4ThreeVector(0,0,0));
    fSolidGrooveinLead = new G4Box("coldFingerGrooveinLead",fColdFingerSideGrooveDepth/2,fColdFingerSideGrooveDepth/2,fOuterShieldingThickness/2);
    fSolidColdFingerHollowRodSideGroovedinLead = new G4SubtractionSolid("coldFingerHollowRodSideGroovedinLead",fSolidColdFingerHollowRodinLead,fSolidGrooveinLead,0,G4ThreeVector(fColdFingerRadius-(fColdFingerSideGrooveDepth/2),0,0));

    fSolidColdFingerinLead =  new G4SubtractionSolid("coldFingerHollowRod2SidesGroovedinLead",fSolidColdFingerHollowRodSideGroovedinLead,fSolidGrooveinLead,0,G4ThreeVector(-fColdFingerRadius+(fColdFingerSideGrooveDepth/2),0,0));
    fLogicColdFingerinLead = new G4LogicalVolume(fSolidColdFingerinLead,fColdFingerMater,"ColdFingerinLead",0,0,0);
    fPhysiColdFingerinLead = new G4PVPlacement(0,             // no rotation
					       G4ThreeVector(0,0,0),
					       fLogicColdFingerinLead,   // its logical volume
					       "ColdFingerinLead",       // its name
					       fLogicNeckCavityinLead,      // its mother  volume
					       false,            // no boolean operations
					       0,true);          // no particular field 

    fLogicColdFingerCavityinLead = new G4LogicalVolume(fSolidColdFingerCavityinLead,fColdFingerCavityMater,"ColdFingerCavityinLead",0,0,0);
    fPhysiColdFingerCavityinLead = new G4PVPlacement(0,             // no rotation
						     G4ThreeVector(0,0,0),
						     fLogicColdFingerCavityinLead, // logical volume
						     "ColdFingerCavityinLead",     // name
						     fLogicNeckCavityinLead,       // mother  volume
						     false,            // no boolean operations
						     0,true);          // no particular field 
  }
    
  // Ge2 has rim, but Ge3 doesn't. Default: no rim.
  if(fDetId == "2"){
    fSolidEndCapBody = new G4Tubs("endCapBody",0.,fEndCapRadius,fEndCapLength/2.,0.,twopi);
    fSolidEndCapRim = new G4Tubs("endCapRim",0.,fEndCapRadius+fEndCapRimWidth,fEndCapRimThickness/2.,0.,twopi);
    fSolidEndCapMain = new G4UnionSolid("endCapMain",fSolidEndCapBody,fSolidEndCapRim,0,G4ThreeVector(0,0,(fEndCapRimThickness-fEndCapLength)/2.));
    fSolidEndCapCavity = new G4Tubs("endCapCavity",0.,fEndCapCavityRadius,fEndCapCavityLength/2.+10.*epsilon,0.,twopi);
    fSolidEndCap = new G4SubtractionSolid("endCap",fSolidEndCapMain,fSolidEndCapCavity,0,G4ThreeVector(0,0,-fEndCapEntranceWindowThickness/2.-10.*epsilon));
    fLogicEndCap = new G4LogicalVolume(fSolidEndCap ,fGe2EndCapMater,"EndCap",0,0,0);
  }else{
    fSolidEndCapBody = new G4Tubs("endCapBody",0.,fEndCapRadius,fEndCapLength/2.,0.,twopi);
    fSolidEndCapCavity = new G4Tubs("endCapCavity",0.,fEndCapCavityRadius,fEndCapCavityLength/2.+10.*epsilon,0.,twopi);
    fSolidEndCap = new G4SubtractionSolid("endCap",fSolidEndCapBody,fSolidEndCapCavity,0,G4ThreeVector(0,0,-fEndCapEntranceWindowThickness/2.-10.*epsilon));
    fLogicEndCap = new G4LogicalVolume(fSolidEndCap ,fGe3EndCapMater,"EndCap",0,0,0);
  }
  G4ThreeVector endCapTranslation(0,0,fSolidDetectorEnclosure->GetZHalfLength() - fEndCapLength/2.);
  fPhysiEndCap = new G4PVPlacement(0,             // no rotation
                                  endCapTranslation, 
                                  fLogicEndCap,   // its logical volume                                  
                                  "EndCap",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 
  //cout << "End Cap mass = " << fLogicEndCap->GetMass()/g << " g" << endl;

  cout << "Detector Enclosure Center: " << fPhysiDetectorEnclosure->GetTranslation().getX() / mm << " " 
                                        << fPhysiDetectorEnclosure->GetTranslation().getY() / mm << " " 
                                        << fPhysiDetectorEnclosure->GetTranslation().getZ() / mm << " mm " << endl;
  cout << "Detector Enclosure HalfZ : " << fSolidDetectorEnclosure->GetZHalfLength() / mm << " mm " << endl;
  cout << "Detector Enclosure Radius: " << fSolidDetectorEnclosure->GetOuterRadius() / mm << " mm " << endl;
  

  // Holder: 2 cyls - 3 cyls
  // Union of two cylinders
  fSolidHolderTopBody = new G4Tubs("holderTopBody",0.,fHolderTopRadius,fHolderTopLength/2.,0.,twopi);
  fSolidHolderRim = new G4Tubs("holderRim",0.,fHolderTopRadius+fHolderRimThickness,fHolderRimWidth/2.,0.,twopi);
  fSolidHolderRidge1 = new G4Tubs("holderRidge1",0.,fHolderTopRadius+fHolderRidge1Thickness,fHolderRidge1Width/2.,0.,twopi);
  fSolidHolderRidge2 = new G4Tubs("holderRidge2",0.,fHolderTopRadius+fHolderRidge2Thickness,fHolderRidge2Width/2.,0.,twopi);
  fSolidHolderTopMain1 = new G4UnionSolid("holderTopMain1",fSolidHolderTopBody,fSolidHolderRim,0,G4ThreeVector(0,0,(fHolderTopLength-fHolderRimWidth)/2.));
  fSolidHolderTopMain2 = new G4UnionSolid("holderTopMain2",fSolidHolderTopMain1,fSolidHolderRidge1,0,
                             G4ThreeVector(0,0,(fHolderTopLength-fHolderRidge1Width)/2.-fHolderRidge1Position));
  //fSolidHolderTopMain2 = new G4UnionSolid("holderTopMain2",fSolidHolderTopBody,fSolidHolderRidge1,0,G4ThreeVector(0,0,(fHolderTopLength-fHolderRimWidth)/2.-fHolderRidge1Position));
  fSolidHolderTopMain = new G4UnionSolid("holderTopMain",fSolidHolderTopMain2,fSolidHolderRidge2,0,
                            G4ThreeVector(0,0,(fHolderTopLength-fHolderRidge2Width)/2.-fHolderRidge2Position));

  fSolidHolderBottomMain = new G4Tubs("holderBottomMain",0.,fHolderBottomRadius,fHolderBottomLength/2.,0.,twopi);
  fSolidHolderMain = new G4UnionSolid("holderMain",fSolidHolderTopMain,fSolidHolderBottomMain,0,G4ThreeVector(0,0,-(fHolderTopLength+fHolderBottomLength)/2.));
  // Union of three cylinders (cavity)
  if(fDetId == "2"){
    fSolidHolderCavity1 = new G4Tubs("holderCavity1",0.,fHolderCavity1Radius,fHolderCavity1Length/2.,0.,twopi);
    fSolidHolderCavity2 = new G4Tubs("holderCavity2",0.,fHolderCavity2Radius,fHolderCavity2Length/2.,0.,twopi);
    fSolidHolderCavity3 = new G4Tubs("holderCavity3",0.,fHolderCavity3Radius,fHolderCavity3Length/2.,0.,twopi);
    fSolidHolderCavity12 = new G4UnionSolid("holderCavity12",fSolidHolderCavity1,fSolidHolderCavity2,0,G4ThreeVector(0,0,-(fHolderCavity1Length+fHolderCavity2Length)/2.));
    fSolidHolderCavity = new G4UnionSolid("holderCavity",fSolidHolderCavity12,fSolidHolderCavity3,0,G4ThreeVector(0,0,-(fHolderCavity1Length+2*fHolderCavity2Length+fHolderCavity3Length)/2.));
    // Subtract three cylinders from two cylinders.
    fSolidHolder = new G4SubtractionSolid("holder",fSolidHolderMain,fSolidHolderCavity,0,G4ThreeVector(0,0,fHolderCavityOffset));
  }else if(fDetId == "3"){
    // For Ge3, cavity3 is disjoint from cavity12. so two substractions are needed: 2 cyls - 2 cyls - 1 cyl
    fSolidHolderCavity1 = new G4Tubs("holderCavity1",0.,fHolderCavity1Radius,fHolderCavity1Length/2.,0.,twopi);
    fSolidHolderCavity2 = new G4Tubs("holderCavity2",0.,fHolderCavity2Radius,fHolderCavity2Length/2.,0.,twopi);
    fSolidHolderCavity3 = new G4Tubs("holderCavity3",0.,fHolderCavity3Radius,fHolderCavity3Length/2+epsilon,0.,twopi);
    fSolidHolderCavity12 = new G4UnionSolid("holderCavity12",fSolidHolderCavity1,fSolidHolderCavity2,0,G4ThreeVector(0,0,-(fHolderCavity1Length+fHolderCavity2Length)/2.));
    fSolidHolderIntermediate = new G4SubtractionSolid("holderIntermediate",fSolidHolderMain,fSolidHolderCavity12,0,G4ThreeVector(0,0,fHolderCavityOffset));
    fSolidHolder = new G4SubtractionSolid("holder",fSolidHolderIntermediate,fSolidHolderCavity3,0,G4ThreeVector(0,0,(-fHolderTopLength-2.*fHolderBottomLength+fHolderCavity3Length)/2.-epsilon));
  }
  else if ( fDetId == '4' ) {
    // Ge4 has two cavities only.
    fSolidHolderCavity1 = new G4Tubs("holderCavity1",0,fHolderCavity1Radius,fHolderCavity1Length/2,0,twopi);
    fSolidHolderCavity2 = new G4Tubs("holderCavity2",0,fHolderCavity2Radius,fHolderCavity2Length/2,0,twopi);
    fSolidHolderCavity3 = new G4Tubs("holderCavity3",0,fHolderCavity1Radius,20.0*mm,0,twopi);
    fSolidHolderCavity12 = new G4UnionSolid("holderCavity12",fSolidHolderCavity1,fSolidHolderCavity2,0,G4ThreeVector(0,0,-(fHolderCavity1Length+fHolderCavity2Length)/2));
    fSolidHolderIntermediate = new G4SubtractionSolid("holderIntermediate",fSolidHolderMain,fSolidHolderCavity12,0,G4ThreeVector(0,0,fHolderCavityOffset));
    fSolidHolder = new G4SubtractionSolid("holder",fSolidHolderIntermediate,fSolidHolderCavity3,0,G4ThreeVector(0,0,(fHolderTopLength+fHolderBottomLength)/2));
  }else{
    G4cout << "Unknown detector!\n";
    return 0;
  }

  fLogicHolder = new G4LogicalVolume(fSolidHolder ,fHolderMater,"Holder",0,0,0);
  G4ThreeVector holderTranslation(0,0,fHolderOffset+fSolidDetectorEnclosure->GetZHalfLength() - fEndCapEntranceWindowThickness + fSheathFrontThickness - fFrontFaceGap - fHolderTopLength/2); 
  //G4ThreeVector holderTranslation(0,0,fHolderBottomLength + fHolderTopLength/2 - fSolidDetectorEnclosure->GetZHalfLength()); 
  fPhysiHolder = new G4PVPlacement(0,             // no rotation
                                  holderTranslation,
                                  fLogicHolder,   // its logical volume                                  
                                  "Holder",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 
  //cout << "Holder Mass = " << fLogicHolder->GetMass(false,false) / g << " g\n";
  // */

  fSolidSheathMain = new G4Tubs("sheathMain",0.,fSheathRadius,fSheathLength/2.,0.,twopi);
  fSolidSheathCavity = new G4Tubs("sheathCavity",0.,fSheathRadius-fSheathSideThickness,(fSheathLength-fSheathFrontThickness)/2.+epsilon,0.,twopi);
  fSolidSheath = new G4SubtractionSolid("sheath",fSolidSheathMain,fSolidSheathCavity,0,G4ThreeVector(0,0,-fSheathFrontThickness/2.-epsilon));
  fLogicSheath = new G4LogicalVolume(fSolidSheath ,fSheathMater,"Sheath",0,0,0);
  G4ThreeVector sheathTranslation(0,0,fSolidDetectorEnclosure->GetZHalfLength() - fEndCapEntranceWindowThickness + fSheathFrontThickness - fFrontFaceGap - fSheathLength/2. + fSheathGap); 
  //G4ThreeVector sheathTranslation(0,0,fSheathBaseHeight - fSolidDetectorEnclosure->GetZHalfLength() + fSolidSheathMain->GetZHalfLength());
  fPhysiSheath = new G4PVPlacement(0,             // no rotation
                                  sheathTranslation,
                                  fLogicSheath,   // its logical volume                                  
                                  "Sheath",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 
  //cout << "Sheath Mass = " << fLogicSheath->GetMass(false,false) / g << " g\n";

  fSolidDetectorMain = new G4Tubs("detectorMain",0.,fDetectorRadius,fDetectorLength/2.,0.,twopi);
  fSolidDetectorGroove1 = new G4Tubs("detectorGroove1",0,fDetectorGrooveOuterRadius,fDetectorGrooveDepth/2.+epsilon,0.,twopi);
  fSolidDetectorGroove2 = new G4Tubs("detectorGroove2",0,fDetectorGrooveInnerRadius,fDetectorGrooveDepth/2.,0.,twopi);
  fSolidDetectorWell = new G4Tubs("detectorWell",0.,fDetectorWellRadius,fDetectorWellLength/2.+2.*epsilon,0.,twopi);
  fSolidDetector1 = new G4SubtractionSolid("detector1",fSolidDetectorMain,fSolidDetectorGroove1,0,G4ThreeVector(0,0,(fDetectorGrooveDepth-fDetectorLength)/2.-epsilon));
  fSolidDetector2 = new G4UnionSolid("detector2",fSolidDetector1,fSolidDetectorGroove2,0,G4ThreeVector(0,0,(fDetectorGrooveDepth-fDetectorLength)/2.));
  fSolidDetector = new G4SubtractionSolid("detector",fSolidDetector2,fSolidDetectorWell,0,G4ThreeVector(0,0,(fDetectorWellLength-fDetectorLength)/2.-2.*epsilon));

  fLogicDetector = new G4LogicalVolume(fSolidDetector ,fDetectorMater,"Detector",0,0,0);
  G4ThreeVector detectorTranslation(0,0,fSolidDetectorEnclosure->GetZHalfLength() - fEndCapEntranceWindowThickness - fFrontFaceGap - fDetectorLength/2.); 
  //G4ThreeVector detectorTranslation(0,0,fDetectorBaseHeight - fSolidDetectorEnclosure->GetZHalfLength() + fSolidDetectorMain->GetZHalfLength());
  fPhysiDetector = new G4PVPlacement(0,             // no rotation
                                  detectorTranslation,
                                  fLogicDetector,   // its logical volume                                  
                                  "Detector",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 

  fSolidActiveDetectorMain = new G4Tubs("activeDetectorMain",0.,fDetectorRadius-fDeadLayerSideThickness,(fDetectorLength-fDeadLayerTopThickness)/2.,0.,twopi);
  fSolidActiveDetector = new G4IntersectionSolid("activeDetector",fSolidDetector,fSolidActiveDetectorMain,0,G4ThreeVector(0,0,-fDeadLayerTopThickness/2.));
  fLogicActiveDetector = new G4LogicalVolume(fSolidActiveDetector ,fDetectorMater,"ActiveDetector",0,0,0);
  fPhysiActiveDetector = new G4PVPlacement(0,             // no rotation
                                  G4ThreeVector(0,0,0),//-fDeadLayerTopThickness/2.),
                                  fLogicActiveDetector,   // its logical volume                                  
                                  "ActiveDetector",       // its name
                                  fLogicDetector,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);               // no particular field 
  
  fSolidSignalPinMain = new G4Tubs("signalPinMain",0.,fSignalPinRadius,fSignalPinLength/2.,0.,twopi);
  fSolidSignalPinHead = new G4Cons("signalPinHead",0.,fSignalPinHeadLowerRadius,0.,fSignalPinHeadUpperRadius,fSignalPinHeadLength/2.,0.,twopi);
  fSolidSignalPinNeck = new G4Tubs("signalPinNeck",0.,fSignalPinNeckRadius,fSignalPinNeckLength/2.,0.,twopi);
  fSolidSignalPinWell = new G4Tubs("signalPinWell",0.,fSignalPinWellRadius,fSignalPinWellDepth/2.+epsilon,0.,twopi);
  fSolidSignalPin1 = new G4UnionSolid("signalPin1",fSolidSignalPinMain,fSolidSignalPinHead,0,G4ThreeVector(0,0,(fSignalPinLength-fSignalPinHeadLength)/2.));
  fSolidSignalPin2 = new G4UnionSolid("signalPin2",fSolidSignalPin1,fSolidSignalPinNeck,0,G4ThreeVector(0,0,(fSignalPinLength-fSignalPinNeckLength)/2.));
  fSolidSignalPin  = new G4SubtractionSolid("signalPin",fSolidSignalPin2,fSolidSignalPinWell,0,G4ThreeVector(0,0,(fSignalPinWellDepth-fSignalPinLength)/2.-epsilon));
  fLogicSignalPin = new G4LogicalVolume(fSolidSignalPin ,fSignalPinMater,"SignalPin",0,0,0);
  G4double signalPinIntrusion = fSignalPinHeadLength * (fDetectorWellRadius - fSignalPinHeadUpperRadius) / (fSignalPinHeadLowerRadius - fSignalPinHeadUpperRadius);
  G4ThreeVector signalPinTranslation = detectorTranslation + G4ThreeVector(0,0,signalPinIntrusion-(fDetectorLength+fSignalPinLength)/2.);
  fPhysiSignalPin = new G4PVPlacement(0,             // no rotation
                                  signalPinTranslation,
                                  fLogicSignalPin,   // its logical volume                                  
                                  "SignalPin",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);               // no particular field 
  //cout << "SignalPin Mass = " << fLogicSignalPin->GetMass(false,false) / g << " g\n";

  fSolidSignalPinSupportMain1 = new G4Tubs("signalPinSupportMain1",0.,fSignalPinSupportRadius1,fSignalPinSupportLength1/2.,0.,twopi);
  fSolidSignalPinSupportMain2 = new G4Tubs("signalPinSupportMain2",0.,fSignalPinSupportRadius2,(fSignalPinSupportLength1+fSignalPinSupportLength2)/2.,0.,twopi);
  fSolidSignalPinSupportMain  = new G4UnionSolid("signalPinSupportMain",fSolidSignalPinSupportMain2,fSolidSignalPinSupportMain1,0,G4ThreeVector(0,0,(fSignalPinSupportLength2)/2.));
  fSolidSignalPinSupportHole1 = new G4Tubs("signalPinSupportHole1",0.,fSignalPinSupportHoleRadius1,fSignalPinSupportHoleLength1/2.,0.,twopi); 
  fSolidSignalPinSupportHole2 = new G4Tubs("signalPinSupportHole2",0.,fSignalPinSupportHoleRadius2,(fSignalPinSupportHoleLength1+fSignalPinSupportHoleLength2)/2.,0.,twopi);
  fSolidSignalPinSupportHole  = new G4UnionSolid("signalPinSupportHole",fSolidSignalPinSupportHole2,fSolidSignalPinSupportHole1,0,G4ThreeVector(0,0,(fSignalPinSupportHoleLength2)/2.));
  fSolidSignalPinSupport  = new G4SubtractionSolid("signalPinSupport",fSolidSignalPinSupportMain,fSolidSignalPinSupportHole,0,G4ThreeVector(0,0,0));
  fLogicSignalPinSupport = new G4LogicalVolume(fSolidSignalPinSupport ,fSignalPinSupportMater,"SignalPinSupport",0,0,0);
  // Touching signal pin
  G4ThreeVector signalPinSupportTranslation = signalPinTranslation + G4ThreeVector(0,0,(fSignalPinLength-fSignalPinSupportLength1-fSignalPinSupportLength2)/2.-fSignalPinHeadLength);
  // Pushed into holder (Expect 0.1 mm overlap)
  //G4ThreeVector signalPinSupportTranslation = holderTranslation + G4ThreeVector(0,0,fHolderCavity2Length+fHolderCavity3Length-fHolderTopLength/2.-fHolderBottomLength+(fSignalPinSupportLength1-fSignalPinSupportLength2)/2.);
  // Touching holder
  //G4ThreeVector signalPinSupportTranslation = holderTranslation + G4ThreeVector(0,0,fHolderCavity2Length+fHolderCavity3Length-fHolderTopLength/2.-fHolderBottomLength+(fSignalPinSupportLength1+fSignalPinSupportLength2)/2.);
  fPhysiSignalPinSupport = new G4PVPlacement(0,             // no rotation
                                  signalPinSupportTranslation,
                                  fLogicSignalPinSupport,   // its logical volume                                  
                                  "SignalPinSupport",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);               // no particular field 
  //cout << "SignalPinSupport Mass = " << fLogicSignalPinSupport->GetMass(false,false) / g << " g\n";

  if ( fDetId == '4' ) {
    fSolidPlasticMountBody= new G4Tubs("SolidPlasticMountBody",0,fPlasticMountRadius,fPlasticMountLength/2,0,twopi);
    fSolidPlasticMountCavity= new G4Tubs("SolidPlasticMountCavity",0,fPlasticMountCavityRadius,fPlasticMountCavityLength/2,0,twopi);
    G4SubtractionSolid *fSolidPlasticMount= new G4SubtractionSolid("SolidPlasticMount",fSolidPlasticMountBody,fSolidPlasticMountCavity,0,G4ThreeVector(0,0,0));
    fLogicPlasticMount= new G4LogicalVolume( fSolidPlasticMount,fPlasticMountMater,"PlasticMount",0,0,0);
    G4ThreeVector PlasticMountTranslation= headTranslation + G4ThreeVector(0,0,(fHeadLength+2*fSnoutCavityLength+fPlasticMountLength)/2);
    fPhysiPlasticMount= new G4PVPlacement(0,             // no rotation
					  PlasticMountTranslation,
					  fLogicPlasticMount,   // its logical volume
					  "PlasticMount",       // its name
					  fLogicDetectorEnclosure,      // its mother  volume
					  false,            // no boolean operations
					  0,true);               // no particular field 
  }
     
  /*
  G4LogicalVolume * lvs[] = { fLogicOuterShielding, fLogicInnerShielding, fLogicInnerCavity, fLogicHead, fLogicSnout, fLogicNeck, fLogicEndCap, fLogicHolder, fLogicSheath, fLogicSignalPin, fLogicSignalPinSupport};
  for(int i = 0; i < 11; i++){
    PrintVolumeInfo(lvs[i]);
  }*/

  if(fDetId == "4a"){
    // Electronics for Ge4
    // used for resistors, capacitors, FETs, solder and wire.
    /*
    G4double elecRadius = 1.*mm;
    G4Orb * solidElectronics = new G4Orb("electronics",elecRadius);
    G4LogicalVolume * logicElectronics = new G4LogicalVolume(solidElectronics,fExo200SourceMater,"Electronics",0,0,0);
    G4VPhysicalVolume * physiElectronics = new G4PVPlacement(0,
                                  G4ThreeVector(-120.875*mm,0,-30.*mm),
                                  logicElectronics,   // its logical volume                                  
                                  "Electronics",       // its name
                                  fLogicInnerCavity,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 
    // */

  
    // Miscellaneous Components for Ge4
    // Ref: http://130.160.100.75/GeIV/18 

    // IR Shield (Mylar and Kapton): 0.1 g, Mylar and Kapton
    //   Mylar density = 1.38, Kapton density = 1.42,
    //   mass = 0.1 g -> total thickness ~ 0.1/1.4 = 0.00142102627 cm  = 14 um
    //   thickness = 7 um each

    G4double IrShieldMylarThickness = 0.007*mm;
    G4double IrShieldKaptonThickness = 0.007*mm;

    G4ThreeVector irShieldMylarTranslation = detectorTranslation + G4ThreeVector(0,0,fDetectorLength/2.+IrShieldMylarThickness/2.);
    G4ThreeVector irShieldKaptonTranslation = detectorTranslation + G4ThreeVector(0,0,fDetectorLength/2.+IrShieldMylarThickness+IrShieldKaptonThickness/2.);

    G4Tubs * solidIrShieldMylar = new G4Tubs("IrShieldMylar",0,fDetectorRadius, IrShieldMylarThickness/2.,0.,twopi);
    G4LogicalVolume * logicIrShieldMylar = new G4LogicalVolume(solidIrShieldMylar,nistMan->FindOrBuildMaterial("G4_MYLAR"),"IrShieldMylar",0,0,0);
    G4VPhysicalVolume * physiIrShieldMylar = new G4PVPlacement(0,
                                  irShieldMylarTranslation, 
                                  logicIrShieldMylar,   // its logical volume                                  
                                  "IrShieldMylar",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 
    G4Tubs * solidIrShieldKapton = new G4Tubs("IrShieldKapton",0,fDetectorRadius, IrShieldKaptonThickness/2.,0.,twopi);
    G4LogicalVolume * logicIrShieldKapton = new G4LogicalVolume(solidIrShieldKapton,nistMan->FindOrBuildMaterial("G4_KAPTON"),"IrShieldKapton",0,0,0);
    G4VPhysicalVolume * physiIrShieldKapton = new G4PVPlacement(0,
                                  irShieldKaptonTranslation, 
                                  logicIrShieldKapton,   // its logical volume                                  
                                  "IrShieldKapton",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 
    G4cout << "Mylar: "
             << irShieldMylarTranslation.getX() / mm << " "
             << irShieldMylarTranslation.getY() / mm << " "
             << irShieldMylarTranslation.getZ() / mm << " mm\n";
    G4cout << "Kapton: "
             << irShieldKaptonTranslation.getX() / mm << " "
             << irShieldKaptonTranslation.getY() / mm << " "
             << irShieldKaptonTranslation.getZ() / mm << " mm\n";


    // Teflon tape aroud holder: 1 g, 4 layers.
    //   Teflon density = 2.2
    //   Width = 12.7 mm (c.f. fHolderRidge1Position - fHolderRimWidth = 14-1.3 = 12.7 mm = 1/2 inch)
    //   fHolderTopRadius = 83.9/2.0*mm;
    //   if 1g, Thickness = (1/2.2)/(8.39*pi*1.27) = 0.136 mm 

    G4double teflonTapeWidth = 0.5*inch;
    G4double teflonTapeThickness = 0.136*mm;
    G4ThreeVector holderTeflonTapeTranslation = holderTranslation + G4ThreeVector(0,0,fHolderTopLength/2.-fHolderRidge1Position+teflonTapeWidth/2.);
    G4Tubs * solidHolderTeflonTape = new G4Tubs("HolderTeflonTape", fHolderTopRadius, fHolderTopRadius + teflonTapeThickness, teflonTapeWidth/2., 0.,twopi);
    G4LogicalVolume * logicHolderTeflonTape = new G4LogicalVolume(solidHolderTeflonTape,nistMan->FindOrBuildMaterial("G4_TEFLON"),"HolderTeflonTape",0,0,0);
    G4VPhysicalVolume * physiHolderTeflonTape = new G4PVPlacement(0,
                                  holderTeflonTapeTranslation, 
                                  logicHolderTeflonTape,   // its logical volume                                  
                                  "HolderTeflonTape",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 

    // Viton O-ring: 3 g, 1 piece.
    //   density = 1.8
    //   Assume parker size #2-154: ID = 3 1/2", OD = 3 11/16", width = 3/32" ~ 2.38 mm
    //   squeezed to rectangular torus with xsection of 1.8 x 2.5 mm 
    //   Mass = 2.3g  (which is close to 3g)

    G4double vitonORingWidth = 2.5*mm;
    G4double vitonORingThickness = 1.8*mm;
    G4Tubs * solidVitonORing = new G4Tubs("VitonORing", fSnout2Radius, fSnout2Radius+vitonORingThickness, vitonORingWidth/2., 0.,twopi);

    //G4double vitonORingRadius = 3./32./2.*inch;
    //G4Torus * solidVitonORing = new G4Torus("VitonORing", 0., vitonORingRadius, fSnout2Radius+vitonORingRadius, 0.,twopi);

    G4ThreeVector snoutTranslation(0,0,-fSolidDetectorEnclosure->GetZHalfLength() + fHeadLength + fSnout1Length/2.); // Appearing a second time.
    G4ThreeVector vitonORingTranslation = snoutTranslation + G4ThreeVector(0,0,fSnout1Length/2.+fSnout2Length/2.);

    G4LogicalVolume * logicVitonORing = new G4LogicalVolume(solidVitonORing,nistMan->FindOrBuildMaterial("G4_VITON"),"VitonORing",0,0,0);
    G4VPhysicalVolume * physiVitonORing = new G4PVPlacement(0,
                                  vitonORingTranslation, 
                                  logicVitonORing,   // its logical volume                                  
                                  "VitonORing",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 

    // Spider: 28 g, 1 piece
    //   Assume: 
    //      material = G4_POLYETHYLENE (density = 0.94)
    //      radius = 30 mm 
    //      inner radius = 12.7 mm
    //   thickness = 12.8 mm
    // (S1/W3) M4 Screws and washers for spider: 3 pcs

    G4double spiderRadius = 30.*mm;
    G4double spiderThickness = 12.8*mm;

    G4ThreeVector spiderTranslation = snoutTranslation + G4ThreeVector(0,0,fSnout1Length/2.+fSnout2Length+fSnout3Length+fSnout4Length+ spiderThickness/2.);
    G4Tubs * solidSpider = new G4Tubs("Spider", fHolderBottomRadius, spiderRadius, spiderThickness/2., 0.,twopi);

    G4LogicalVolume * logicSpider = new G4LogicalVolume(solidSpider,nistMan->FindOrBuildMaterial("G4_POLYETHYLENE"),"Spider",0,0,0);
    G4VPhysicalVolume * physiSpider = new G4PVPlacement(0,
                                  spiderTranslation, 
                                  logicSpider,   // its logical volume                                  
                                  "Spider",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  0,true);          // no particular field 
    

    G4double spiderScrewRadius = 4./2.*mm;
    G4double spiderScrewEdge = 1.*mm;  // distance between spider edge and screw center.

    G4Tubs * solidSpiderScrew = new G4Tubs("SpiderScrew", 0., spiderScrewRadius, spiderThickness/2.-epsilon, 0.,twopi);
    G4LogicalVolume * logicSpiderScrew = new G4LogicalVolume(solidSpiderScrew,fMaterialsManager->GetMaterial("SS304L"),"SpiderScrew",0,0,0);

    G4ThreeVector spiderScrewTranslation[3];

    for(int i = 0; i < 3; i++){
      spiderScrewTranslation[i].setX(spiderRadius-spiderScrewRadius-spiderScrewEdge);
      spiderScrewTranslation[i].setTheta(pi/2.);
      spiderScrewTranslation[i].setPhi(i*2.*pi/3.);
   
      G4VPhysicalVolume * physiSpiderScrew = new G4PVPlacement(0,
                                  spiderScrewTranslation[i], 
                                  logicSpiderScrew,   // its logical volume                                  
                                  "SpiderScrew",       // its name
                                  logicSpider,      // its mother  volume
                                  false,            // no boolean operations
                                  i,true);          // no particular field 
    }

    // (S2) M3 Screws for endcap in "snout": 2 pcs
    G4double endCapScrewRadius = 3./2.*mm;
    G4double endCapScrewLength = 15.*mm;
    G4double endCapScrewEdge = 1.*mm; //distance between snout edge and screw "top".

    G4Tubs * solidEndCapScrew = new G4Tubs("EndCapScrew", 0., endCapScrewRadius, endCapScrewLength/2., 0.,twopi);
    G4LogicalVolume * logicEndCapScrew = new G4LogicalVolume(solidEndCapScrew,fMaterialsManager->GetMaterial("SS304L"),"EndCapScrew",0,0,0);

    G4ThreeVector endCapScrewTranslation[2];
    G4RotationMatrix * endCapScrewRotation = new G4RotationMatrix();
    endCapScrewRotation->rotateY(90.*deg);

    for(int i = 0; i < 2; i++){
      endCapScrewTranslation[i].setX(fSnout1Radius-endCapScrewEdge-endCapScrewLength/2.);
      endCapScrewTranslation[i].setTheta(pi/2.);
      endCapScrewTranslation[i].setPhi(i*pi);

      G4VPhysicalVolume * physiEndCapScrew = new G4PVPlacement(endCapScrewRotation,
                                  endCapScrewTranslation[i], 
                                  logicEndCapScrew,   // its logical volume                                  
                                  "EndCapScrew",       // its name
                                  fLogicSnout,      // its mother  volume
                                  false,            // no boolean operations
                                  i,true);          // no particular field 
    }

    // (S1/W2/W3) M6 Screws and washers for braid: 2 pcs
    G4double braidScrewRadius = 6./2.*mm;
    G4double braidScrewLength = 25.*mm;
   
    G4Tubs * solidBraidScrew = new G4Tubs("BraidScrew", 0., braidScrewRadius, braidScrewLength/2., 0.,twopi);
    G4LogicalVolume * logicBraidScrew = new G4LogicalVolume(solidBraidScrew,fMaterialsManager->GetMaterial("SS304L"),"BraidScrew",0,0,0);

    // -- first screw on holder
    G4ThreeVector holderBraidScrewTranslation = spiderTranslation + G4ThreeVector(0,0,-spiderThickness/2.+fHolderCavity3Length-braidScrewLength/2.);
    G4VPhysicalVolume * physiHolderBraidScrew = new G4PVPlacement(0,
                                  holderBraidScrewTranslation, 
                                  logicBraidScrew,   // its logical volume                                  
                                  "BraidScrew",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  1,true);          // no particular field 

    // -- second screw on copper rod
    G4RotationMatrix * rodBraidScrewRotation = new G4RotationMatrix();
    rodBraidScrewRotation->rotateY(90.*deg);
    G4ThreeVector rodBraidScrewTranslation = headTranslation + G4ThreeVector(fHeadRadius - fHeadWallThickness - braidScrewLength/2.,0,-fHeadLength/2. + fNeckOffset);
    G4VPhysicalVolume * physiRodBraidScrew = new G4PVPlacement(rodBraidScrewRotation,
                                  rodBraidScrewTranslation, 
                                  logicBraidScrew,   // its logical volume                                  
                                  "BraidScrew",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  2,true);          // no particular field 

    // Cu Braid: 45 g, 1 pcs.
    // cylindrial volume with a mass of 45 g
    // (20*20*pi)*4/1000 * 8.96 = 45.04 g
    // diameter = 40 mm, thickness = 4 mm
    G4double braidRadius = 40./2.*mm;
    G4double braidLength = 4.*mm;
   
    G4Tubs * solidBraid = new G4Tubs("Braid", 0., braidRadius, braidLength/2., 0.,twopi);
    G4LogicalVolume * logicBraid = new G4LogicalVolume(solidBraid,nistMan->FindOrBuildMaterial("G4_Cu"),"Braid",0,0,0);
    G4ThreeVector braidTranslation = holderBraidScrewTranslation + G4ThreeVector(0.,0.,-braidScrewLength/2.-braidLength/2.);
    G4VPhysicalVolume * physiBraid = new G4PVPlacement(0,
                                  braidTranslation, 
                                  logicBraid,   // its logical volume                                  
                                  "Braid",       // its name
                                  fLogicDetectorEnclosure,      // its mother  volume
                                  false,            // no boolean operations
                                  2,true);          // no particular field 

    
  }

  
  //------------------------------
  // Source 
  //------------------------------
  ///
  G4cout << "Source Position: " << fSourcePosition << G4endl;

  G4RotationMatrix * buttonRotation = new G4RotationMatrix();
  G4ThreeVector buttonTranslation; 

  if(fSourcePosition == "1_Down"){
    buttonTranslation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation);
    buttonTranslation.setY(-fInnerCavityY/2.+fButtonRadius);
    buttonTranslation.setZ(-fInnerCavityZ/2.+fButtonLength/2.);
  }else if(fSourcePosition == "1_Up"){
    buttonRotation->rotateY(180.*deg);
    buttonTranslation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation);
    buttonTranslation.setY(-fInnerCavityY/2.+fButtonRadius);
    buttonTranslation.setZ(-fInnerCavityZ/2.+fButtonLength/2.);
  }else if(fSourcePosition == "2_Down"){
    buttonRotation->rotateY(180.*deg);
    buttonTranslation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation);
    buttonTranslation.setY(0);
    buttonTranslation.setZ(fInnerCavityZ/2.-fButtonLength/2.);
  }else if(fSourcePosition == "2_Up"){
    buttonTranslation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation);
    buttonTranslation.setY(0);
    buttonTranslation.setZ(fInnerCavityZ/2.-fButtonLength/2.);
  }else if(fSourcePosition == "3_Down"){
    buttonRotation->rotateY(90.*deg);
    buttonTranslation.setX(fInnerCavityX/2.-fButtonLength/2.);
    buttonTranslation.setY(0);
    buttonTranslation.setZ(0);
  }else if(fSourcePosition == "3_Up"){
    buttonRotation->rotateY(-90.*deg);
    buttonTranslation.setX(fInnerCavityX/2.-fButtonLength/2.);
    if ( fDetId == '4' )
      buttonTranslation.setX(fInnerCavityX/2 - fButtonLength/2 + fInnerShieldingThickness/2);
    buttonTranslation.setY(0);
    buttonTranslation.setZ(0);
  }else if(fSourcePosition == "4_Down"){
    buttonTranslation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation+remoteOffset);
    buttonTranslation.setY(-fInnerCavityY/2.+fButtonRadius);
    buttonTranslation.setZ(-fInnerCavityZ/2.+fButtonLength/2.);
  }else if(fSourcePosition == "4_Up"){
    buttonRotation->rotateY(180.*deg);
    buttonTranslation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation+remoteOffset);
    buttonTranslation.setY(-fInnerCavityY/2.+fButtonRadius);
    buttonTranslation.setZ(-fInnerCavityZ/2.+fButtonLength/2.);
  }else if(fSourcePosition == "5_Up"){
    buttonRotation->rotateY(180.*deg);
    buttonTranslation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation-fButtonRadius);
    buttonTranslation.setY(0);
    buttonTranslation.setZ(fDetectorEnclosureRadius+fButtonLength/2.);
  }else if(fSourcePosition == "0"){
    buttonRotation->rotateY(90.*deg);
    buttonTranslation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation+fButtonLength/2.);
    buttonTranslation.setY(0);
    buttonTranslation.setZ(0);
  }else if(fSourcePosition == "0_1inch"){
    buttonRotation->rotateY(90.*deg);
    buttonTranslation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation+ 1.*inch + fButtonLength/2.);
    buttonTranslation.setY(0);
    buttonTranslation.setZ(0);
  }else if(fSourcePosition == "M17.3"){
    // Det = 3
    // Carbon Fiber Roll 3
    // Sample masses
    double density = fMaterialsManager->GetMaterial("CarbonFiber")->GetDensity();
    G4cout << "Density = " << density/ (g/cm3) << "g/cm3\n";
    double grossMass[6] = {0.0, 312.9*g, 570.0*g, 464.2*g, 653.5*g, 455.7*g};  // [0] is for convenience.
    double bagMass[6] = {0.0, 9.7*g, 10.1*g, 9.7*g, 10.1*g, 9.5*g};

    // Common dimensions
    double depth = 260.*mm - 1.* inch - 1.*cm; // distance of sample 3 from door; 1 inch is door thickness. 
    double bagWidth = 240.*mm;
    double bagHeight = 240.*mm;
    double sampleMass[6];
    double sampleThickness[6];
    double sag[6] = {0.0, 5.0*cm, 6.0*cm, 5.0*cm, 6.0*cm, 5.0*cm};
    for(int i = 1; i < 6; i++){
      sampleMass[i] = grossMass[i] - bagMass[i];
      sampleThickness[i] = sampleMass[i] / density / bagWidth / (bagHeight - sag[i]);
      G4cout << "Thickness " << i << " = " << sampleThickness[i]/mm << " mm.\n";
    }

    // Sample dimensions
    double sampleX[6], sampleY[6], sampleZ[6];
    sampleX[1] = bagWidth;
    sampleY[1] = sampleThickness[1];
    sampleZ[1] = bagHeight - sag[1];

    sampleX[2] = bagHeight - sag[2];
    sampleY[2] = bagWidth;
    sampleZ[2] = sampleThickness[2];

    sampleX[3] = sampleThickness[3];
    sampleY[3] = bagWidth;
    sampleZ[3] = bagHeight - sag[3];

    sampleX[4] = bagHeight - sag[4];
    sampleY[4] = bagWidth;
    sampleZ[4] = sampleThickness[4];

    sampleX[5] = bagWidth;
    sampleY[5] = sampleThickness[5];
    sampleZ[5] = bagHeight - sag[5];

    // Sample positions
    double samplePositionX[6], samplePositionY[6], samplePositionZ[6];
    samplePositionX[3] = fInnerCavityX/2. - depth - sampleX[3]/2.;
    samplePositionY[3] = 0.;
    samplePositionZ[3] = -fInnerCavityZ/2. + sampleZ[3]/2.;

    samplePositionX[1] = samplePositionX[3] - sampleX[3]/2. - sampleX[1]/2.;
    samplePositionY[1] = -fInnerCavityY/2. + sampleY[1]/2.;
    samplePositionZ[1] = -fInnerCavityZ/2. + sampleZ[1]/2.;
   
    samplePositionX[5] = samplePositionX[3] - sampleX[3]/2. - sampleX[1]/2.;
    samplePositionY[5] = fInnerCavityY/2. - sampleY[5]/2.;
    samplePositionZ[5] = -fInnerCavityZ/2. + sampleZ[5]/2.;

    samplePositionX[4] = samplePositionX[3] - sampleX[3]/2. - sampleX[4]/2.;
    samplePositionY[4] = 0.;
    samplePositionZ[4] = -fInnerCavityZ/2. + sampleZ[4]/2.;

    samplePositionX[2] = samplePositionX[3] - sampleX[3]/2. - sampleX[4]/2.;
    samplePositionY[2] = 0.;
    samplePositionZ[2] = -fInnerCavityZ/2. + sampleZ[4] + sampleZ[2]/2.;

    // Solids for samples
    G4Box * solidCarbonFiberSamples[6];
    G4LogicalVolume * logicCarbonFiberSamples[6];
    G4VPhysicalVolume * physiCarbonFiberSamples[6];
    for(int i = 1; i < 6; i++){
      std::ostringstream ss;
      //ss << "carbonFiber_" << i;
      ss << "M17.3_" << i;
      solidCarbonFiberSamples[i] = new G4Box(ss.str().c_str(),sampleX[i]/2.,sampleY[i]/2.,sampleZ[i]/2.);
      logicCarbonFiberSamples[i] = new G4LogicalVolume(solidCarbonFiberSamples[i],fCarbonFiberMater,ss.str().c_str(),0,0,0);
      physiCarbonFiberSamples[i] = new G4PVPlacement(0,
                                                     G4ThreeVector(samplePositionX[i],samplePositionY[i],samplePositionZ[i]),
                                                     logicCarbonFiberSamples[i],
                                                     ss.str().c_str(),
                                                     fLogicInnerCavity,      // its mother  volume
                                                     false,            // no boolean operations
                                                     0,true);          // no particular field 
      G4cout << "Sample " << i << " Mass  = " << logicCarbonFiberSamples[i]->GetMass()/g << "g\n";
      G4cout << "Center: " 
             << physiCarbonFiberSamples[i]->GetTranslation().getX() / mm << " "
             << physiCarbonFiberSamples[i]->GetTranslation().getY() / mm << " "
             << physiCarbonFiberSamples[i]->GetTranslation().getZ() / mm << " mm\n";
      G4cout << "Size: (halflengths)"
             << solidCarbonFiberSamples[i]->GetXHalfLength() / mm << " "
             << solidCarbonFiberSamples[i]->GetYHalfLength() / mm << " " 
             << solidCarbonFiberSamples[i]->GetZHalfLength() / mm << " mm\n";
    }
    //G4cout << "Source specification: {{{ \n";
    //G4cout << "}}}\n"ampleX[i]*
  }else if(fSourcePosition == "M17.1"){
    // Det = 2
    // Carbon Fiber Roll 1
    // Sample masses
    double density = fMaterialsManager->GetMaterial("CarbonFiber")->GetDensity();
    G4cout << "Density = " << density/ (g/cm3) << "g/cm3\n";
    double grossMass[6] = {0.0, 501.3*g, 450.9*g, 534.5*g};  // [0] is for convenience.
    double bagMass[6] = {0.0, 0.0*g, 0.0*g, 0.0*g};
    double sampleMass[6];
    for(int i = 1; i < 6; i++){
      sampleMass[i] = grossMass[i] - bagMass[i];
    }

    // Common dimensions
    double bagLength = fNeckLocation - fNeckRadius + 1.*inch;//240.*mm;
    double locX = -fNeckLocation + fNeckRadius + bagLength/2.;
    double ringInnerRadius = fDetectorEnclosureRadius + epsilon;
    double ringVolume = (sampleMass[1] + sampleMass[2])/density;
    double ringOuterRadius = sqrt(ringVolume/pi/bagLength + ringInnerRadius*ringInnerRadius);

    double blockWidth = 6.*inch; // XXX: Made-up number 
    double blockHeight = sampleMass[3] / density / bagLength / blockWidth; 

    // Solids for samples
    // -- Ring: Samples 1 and 2
    G4Tubs * solidCarbonFiberSample_1x2 = new G4Tubs("M17.1_1x2",ringInnerRadius,ringOuterRadius,bagLength/2.,0.,twopi);
    G4LogicalVolume * logicCarbonFiberSample_1x2 = new G4LogicalVolume(solidCarbonFiberSample_1x2,fCarbonFiberMater,"M17.1_1x2",0,0,0);
    G4RotationMatrix * sampleRotation = new G4RotationMatrix();
    sampleRotation->rotateY(90.*deg);
    G4VPhysicalVolume * physiCarbonFiberSample_1x2 = new G4PVPlacement(sampleRotation,
                                                     G4ThreeVector(locX,0.,0.),
                                                     logicCarbonFiberSample_1x2,
                                                     "M17.1_1x2",
                                                     fLogicInnerCavity,      // its mother  volume
                                                     false,            // no boolean operations
                                                     0,true);          // no particular field 
    // -- Block: Sample 3
    G4Box * solidCarbonFiberSample_3 = new G4Box("M17.1_3",bagLength/2.,blockWidth/2.,blockHeight/2.);
    G4LogicalVolume * logicCarbonFiberSample_3 = new G4LogicalVolume(solidCarbonFiberSample_3,fCarbonFiberMater,"M17.1_3",0,0,0);
    G4VPhysicalVolume * physiCarbonFiberSample_3 = new G4PVPlacement(0,
                                                     G4ThreeVector(locX,0.,-fInnerCavityZ/2.+blockHeight/2.),
                                                     logicCarbonFiberSample_3,
                                                     "M17.1_3",
                                                     fLogicInnerCavity,      // its mother  volume
                                                     false,            // no boolean operations
                                                     0,true);          // no particular field 
    
    G4cout << "Sample 1x2 Mass  = " << logicCarbonFiberSample_1x2->GetMass()/g << "g\n";
    G4cout << "Center: " 
             << physiCarbonFiberSample_1x2->GetTranslation().getX() / mm << " "
             << physiCarbonFiberSample_1x2->GetTranslation().getY() / mm << " "
             << physiCarbonFiberSample_1x2->GetTranslation().getZ() / mm << " mm\n";
    G4cout << "Size: (IR,OR,halfz)"
             << solidCarbonFiberSample_1x2->GetInnerRadius() / mm << " "
             << solidCarbonFiberSample_1x2->GetOuterRadius() / mm << " " 
             << solidCarbonFiberSample_1x2->GetZHalfLength() / mm << " mm\n";
    G4cout << "Sample 3 Mass  = " << logicCarbonFiberSample_3->GetMass()/g << "g\n";
    G4cout << "Center: " 
             << physiCarbonFiberSample_3->GetTranslation().getX() / mm << " "
             << physiCarbonFiberSample_3->GetTranslation().getY() / mm << " "
             << physiCarbonFiberSample_3->GetTranslation().getZ() / mm << " mm\n";
    G4cout << "Size: (halflengths)"
             << solidCarbonFiberSample_3->GetXHalfLength() / mm << " "
             << solidCarbonFiberSample_3->GetYHalfLength() / mm << " " 
             << solidCarbonFiberSample_3->GetZHalfLength() / mm << " mm\n";
  }else if(fSourcePosition == "M17.2"){
    string sampleName[4] = {"1x2","3","4","5"};
    double grossMass[4] = {(606.7+968.2)*g, 994.5*g, 1152.6*g, 839.7*g};  //g
    double bagMass[4] = {(9.3+9.6)*g, 9.6*g, 9.3*g, 9.4*g};
    double sampleMass[4];
    double sampleDensity[4];
    double faceX = fDetectorEnclosureLength-fNeckOffset-fNeckLocation;
    double sample0extrusion = 5.5*cm;
  
    double sampleX[4] = {19.5*cm, 19.*cm, 21.*cm, 5.*cm};
    double sampleY[4] = {22.*cm, 8.*cm, 6.*cm, 21.*cm};
    double sampleZ[4] = {10.*cm, 14.*cm, 16.*cm, 15.*cm};
    double samplePositionX[4];
    double samplePositionY[4];
    double samplePositionZ[4];
 
    for(int i = 0; i < 4; i++){
      sampleMass[i] = grossMass[i] - bagMass[i];
      sampleDensity[i] = sampleMass[i] / (sampleX[i]*sampleY[i]*sampleZ[i]);
      cout << "Sample " << sampleName[i] << " density : " << sampleDensity[i] /(g/cm3) << "g/cm3" << endl;
    }

    // 1x2: underneath
    samplePositionX[0] = faceX + sample0extrusion - sampleX[0]/2.;
    samplePositionY[0] = 0;
    samplePositionZ[0] = -fInnerCavityZ/2. + sampleZ[0]/2.;
    // 3: right (pos y)
    samplePositionX[1] = faceX - sampleX[1]/2.;
    samplePositionY[1] = fDetectorEnclosureRadius + sampleY[1]/2.;
    samplePositionZ[1] = -fInnerCavityZ/2. + sampleZ[0] + sampleZ[1]/2.;
    // 4: left (neg y)
    samplePositionX[2] = faceX - sampleX[2]/2.;
    samplePositionY[2] = -(fDetectorEnclosureRadius + sampleY[2]/2.);
    samplePositionZ[2] = -fInnerCavityZ/2. + sampleZ[0] + sampleZ[2]/2.;
    // 5: front (pos x)
    samplePositionX[3] = faceX + sample0extrusion + sampleX[3]/2.;
    samplePositionY[3] = 0;
    samplePositionZ[3] = -fInnerCavityZ/2. + sampleZ[3]/2.;
    
    

    // Solids for samples
    G4Box * solidCarbonFiberSamples[4];
    G4LogicalVolume * logicCarbonFiberSamples[4];
    G4VPhysicalVolume * physiCarbonFiberSamples[4];
    for(int i = 0; i < 4; i++){
      std::ostringstream ss;
      //ss << "carbonFiber_" << i;
      ss << "M17.2_" << sampleName[i];
      solidCarbonFiberSamples[i] = new G4Box(ss.str().c_str(),sampleX[i]/2.,sampleY[i]/2.,sampleZ[i]/2.);
      logicCarbonFiberSamples[i] = new G4LogicalVolume(solidCarbonFiberSamples[i],fCarbonFiberMater_M17_2[i],ss.str().c_str(),0,0,0);
      physiCarbonFiberSamples[i] = new G4PVPlacement(0,
                                                     G4ThreeVector(samplePositionX[i],samplePositionY[i],samplePositionZ[i]),
                                                     logicCarbonFiberSamples[i],
                                                     ss.str().c_str(),
                                                     fLogicInnerCavity,      // its mother  volume
                                                     false,            // no boolean operations
                                                     0,true);          // no particular field 
      G4cout << "Sample " << sampleName[i] << " Density  = " << fCarbonFiberMater_M17_2[i]->GetDensity()/(g/cm3) << "g/cm3\n";
      G4cout << "Sample " << sampleName[i] << " Mass  = " << logicCarbonFiberSamples[i]->GetMass()/g << "g\n";
      G4cout << "Center: " 
             << physiCarbonFiberSamples[i]->GetTranslation().getX() / mm << " "
             << physiCarbonFiberSamples[i]->GetTranslation().getY() / mm << " "
             << physiCarbonFiberSamples[i]->GetTranslation().getZ() / mm << " mm\n";
      G4cout << "Size (halflengths): "
             << solidCarbonFiberSamples[i]->GetXHalfLength() / mm << " "
             << solidCarbonFiberSamples[i]->GetYHalfLength() / mm << " "  
             << solidCarbonFiberSamples[i]->GetZHalfLength() / mm << " mm\n";
      G4cout << "srccenter=\"" 
             << physiCarbonFiberSamples[i]->GetTranslation().getX() / mm << " "
             << physiCarbonFiberSamples[i]->GetTranslation().getY() / mm << " "
             << physiCarbonFiberSamples[i]->GetTranslation().getZ() / mm << "\" \n";
      G4cout << "srchalflength=\""
             << solidCarbonFiberSamples[i]->GetXHalfLength() / mm << " "
             << solidCarbonFiberSamples[i]->GetYHalfLength() / mm << " "  
             << solidCarbonFiberSamples[i]->GetZHalfLength() / mm << "\" \n";
    }    
    //G4cout << "Source specification: {{{ \n";
    //G4cout << "}}}\n";

  //}else if(fSourcePosition == "PeBottle"){
  }else if(split(fSourcePosition,'_')[0] == "PeBottle"){
    vector<string> sourceConfig = split(fSourcePosition,'_');
    if(sourceConfig[1] == "Water"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("Water");
    }else if (sourceConfig[1] == "LS"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("LiquidScintillator");
    }else if (sourceConfig[1] == "NitricAcid"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("NitricAcid");
    }else if (sourceConfig[1] == "Teflon"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("Teflon");
    }else if (sourceConfig[1] == "AluminaTeflon"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("AluminaTeflon");
    }else if (sourceConfig[1] == "Dust"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("Dust");
    }else if (sourceConfig[1] == "HCl"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("HCl");
    }else if (sourceConfig[1] == "KOH"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("KOH");
    }else if (sourceConfig[1] == "LL134SiWafer"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("LL134_SiWafer");
    }else if (sourceConfig[1] == "Shrink"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("Air");
    }else if (sourceConfig[1] == "MgF2"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("MgF2");
    }else if (sourceConfig[1] == "AlPellets"){
      fBottleLiquidMater = fMaterialsManager->GetMaterial("AlPellets");
    }
    
    istringstream iss(sourceConfig[2]);
    double liquidLevel; // Measured from bottom of bottle exterior (NOT interior)
    iss >> liquidLevel;
    liquidLevel *= mm;

    //G4cout << "Building PE bottle." << G4endl;

    // Fixed dimensions
    double totalHeight      = 99*mm;
    double bodyHeight       = 77*mm;
    double bodyRadius       = 50.2/2*mm;
    double bodyThickness    =  2*mm;
    double capRadius        = 42/2*mm;
    double capHeight        = 15*mm;
    double capThickness     =  2*mm;
    double capSideThickness =  5*mm;
    double neckRadius       = 32.4/2*mm;
    //double neckHeight     = totalHeight - bodyHeight - capHeight;
    double neckThickness    =  2*mm;
    double flapRadius       = 42.2/2 *mm;
    double flapHeight       =  2*mm;
    double flapFromBody     =  2*mm; //distance from top of body to bottom of flap
    double standHeight      =  5*inch;

    G4RotationMatrix * bottleRotation = 0;
    //G4ThreeVector bottlePosition(fDetectorEnclosureLength-fNeckOffset-fNeckLocation - totalHeight/2.,0,bodyRadius + fEndCapRadius);
    G4ThreeVector bottlePosition(0,0,0);//(endCapFace + fEndCapRadius, 0,-fInnerCavityZ/2. + totalHeight/2. + standHeight);

    // Phony geometry for NAA vial at local
    if(sourceConfig[1] == "Shrink"){
      double shrinkFactor = 0.5;
      totalHeight = shrinkFactor * 99.*mm;
      bodyHeight = shrinkFactor * 77.*mm;
      bodyRadius = shrinkFactor * 50.2/2.*mm;
      bodyThickness = shrinkFactor * 2.*mm;
      capRadius = shrinkFactor * 42./2.*mm;
      capHeight = shrinkFactor * 15.*mm;
      capThickness = shrinkFactor * 2.*mm;
      capSideThickness = shrinkFactor * 5.*mm;
      neckRadius = shrinkFactor * 32.4/2.*mm;
      neckThickness = shrinkFactor * 2.*mm;
      flapRadius = shrinkFactor * 42.2/2. *mm;
      flapHeight = shrinkFactor * 2. *mm;
      flapFromBody = shrinkFactor * 2. *mm; 

      // Location
      bottleRotation = new G4RotationMatrix();
      bottleRotation->rotateY(90.*deg);
      bottlePosition.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation - totalHeight/2.);
      bottlePosition.setY(0);
      bottlePosition.setZ(bodyRadius + fEndCapRadius);
    }else{
      bottlePosition.setX(endCapFace + bodyRadius);
      bottlePosition.setY(0);
      bottlePosition.setZ(0);//standHeight - fInnerCavityZ/2. + totalHeight/2.);
      //bottlePosition.setZ(standHeight - fInnerCavityZ/2. + totalHeight/2.);
    }
 
///
    // Bottle exterior (neck + body + cap + flap)
    solidBottleNeck = new G4Tubs("bottleNeck",0.,neckRadius,totalHeight/2.,0.,twopi);
    solidBottleBody = new G4Tubs("bottleBody",0.,bodyRadius,bodyHeight/2.,0.,twopi);
    solidBottleNeckBody = new G4UnionSolid("bottleNeckBody",solidBottleNeck,solidBottleBody,0,G4ThreeVector(0,0,-totalHeight/2.+bodyHeight/2.));
    solidBottleCap = new G4Tubs("bottleCap",0.,capRadius,capHeight/2.,0.,twopi);
    if ( sourceConfig[1] == "AlPellets" ) {
      // cap not installed (to release trapped radon)
      solidBottleNeckBodyCap = solidBottleNeckBody;
    }
    else{
      solidBottleNeckBodyCap = new G4UnionSolid( "bottleNeckBodyCap",
						 solidBottleNeckBody,
						 solidBottleCap, 0,
						 G4ThreeVector(0, 0, totalHeight/2 - capHeight/2) );
    }
    solidBottleFlap = new G4Tubs("bottleFlap",0.,flapRadius,flapHeight/2.,0.,twopi);
    solidBottleExterior = new G4UnionSolid("bottleExterior",solidBottleNeckBodyCap,solidBottleFlap,0,G4ThreeVector(0,0,-totalHeight/2.+bodyHeight+flapFromBody+flapHeight/2.));
    logicBottleExterior = new G4LogicalVolume(solidBottleExterior,fBottleMater,"BottleExterior",0,0,0);
    //logicBottleExterior = new G4LogicalVolume(solidBottleNeckBody,fBottleMater,"BottleExterior",0,0,0);

    // Bottle interior 
    double neckCavityHeight = totalHeight-bodyThickness-capThickness;
    double bodyCavityHeight = bodyHeight-2.*bodyThickness;
    double capCavityHeight = capHeight-2.*capThickness;
    solidBottleNeckCavity = new G4Tubs("bottleNeckCavity",0.,neckRadius-neckThickness,neckCavityHeight/2.,0.,twopi);
    solidBottleBodyCavity = new G4Tubs("bottleBodyCavity",0.,bodyRadius-bodyThickness,bodyCavityHeight/2.,0.,twopi);
    solidBottleNeckBodyCavity = new G4UnionSolid("bottleNeckBodyCavity",solidBottleNeckCavity,solidBottleBodyCavity,0,G4ThreeVector(0,0,-neckCavityHeight/2.+bodyCavityHeight/2.));
    solidBottleCapCavity = new G4Tubs("bottleCapCavity",0.,capRadius-capSideThickness,capCavityHeight/2.,0.,twopi);
    solidBottleInterior = new G4UnionSolid( "bottleInterior",
					    solidBottleNeckBodyCavity,
					    solidBottleCapCavity, 0,
					    G4ThreeVector(0, 0, neckCavityHeight/2 - capCavityHeight/2) );
    solidBottleInterior = new G4UnionSolid("bottleInterior",solidBottleNeckBodyCavity,solidBottleCapCavity,0,G4ThreeVector(0,0,neckCavityHeight/2.-capCavityHeight/2.));
    logicBottleInterior = new G4LogicalVolume(solidBottleInterior,fDefaultMater,"BottleInterior",0,0,0);
    physiBottleInterior = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,(bodyThickness - capThickness)/2.),
                                    logicBottleInterior,   // its logical volume                                  
                                    "BottleInterior",       // its name
                                    logicBottleExterior,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    auto visAttBottleExterior = new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
    logicBottleExterior->SetVisAttributes(visAttBottleExterior);
    auto visAttBottleInterior = new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.0));
    logicBottleInterior->SetVisAttributes(visAttBottleInterior);

    // Place Bottle(s)
    if (    sourceConfig[1] == "LL134SiWafer"
	 || sourceConfig[1] == "LL134SiWaferAlt"
	 || sourceConfig[1] == "MgF2"
	 || sourceConfig[1] == "AlPellets"       ){

      vector<double> standHeights;
      vector<double> xOffsets;
      vector<double> yOffsets;
      standHeights.push_back(0);
      xOffsets.push_back(0);
      yOffsets.push_back(0);

      if (sourceConfig[1] == "LL134SiWafer"){
        standHeights.push_back(12.*cm);
        standHeights.push_back(12.*cm);
        standHeights.push_back(7.8*cm);
        standHeights.push_back(7.8*cm);
        xOffsets.push_back(bodyRadius); 
        xOffsets.push_back(bodyRadius); 
        xOffsets.push_back(-bodyRadius); 
        xOffsets.push_back(-bodyRadius); 
        yOffsets.push_back(-bodyRadius); 
        yOffsets.push_back(bodyRadius); 
        yOffsets.push_back(fEndCapRadius+bodyRadius);
        yOffsets.push_back(-(fEndCapRadius+bodyRadius));
      }
      else if(sourceConfig[1] == "LL134SiWaferAlt"){
        standHeights.push_back(11.2*cm);
        standHeights.push_back(11.2*cm);
        standHeights.push_back(11.2*cm);
        standHeights.push_back(11.2*cm);
        xOffsets.push_back(bodyRadius); 
        xOffsets.push_back(bodyRadius); 
        xOffsets.push_back(3.*bodyRadius); 
        xOffsets.push_back(3.*bodyRadius); 
        yOffsets.push_back(-bodyRadius); 
        yOffsets.push_back(bodyRadius); 
        yOffsets.push_back(bodyRadius);
        yOffsets.push_back(-bodyRadius);
      }
      else if(sourceConfig[1] == "MgF2"){
        standHeights.push_back(116.5*mm);
        standHeights.push_back(116.5*mm);
        xOffsets.push_back(bodyRadius); 
        xOffsets.push_back(bodyRadius); 
        yOffsets.push_back(bodyRadius); 
        yOffsets.push_back(-bodyRadius); 
      }
      else if(sourceConfig[1] == "AlPellets")
      {
        // 3" + 0.5" + 1" of blocks - 2.0mm tweak + 0.5" block
        standHeights.push_back( (114.3 - 2.0 + 25.4/2)*mm );
        standHeights.push_back( (114.3 - 2.0 + 25.4/2)*mm );
        xOffsets.push_back( bodyRadius); 
        xOffsets.push_back( bodyRadius); 
        yOffsets.push_back( bodyRadius); 
        yOffsets.push_back(-bodyRadius);
      }
      int nBottles = standHeights.size()-1;
 
      G4ThreeVector bottlePositions[nBottles+1];
      for(int i = 0; i < nBottles+1; i++){
        bottlePositions[i].setX(endCapFace+xOffsets[i]);
        bottlePositions[i].setY(yOffsets[i]);
        bottlePositions[i].setZ(-fInnerCavityZ/2. + standHeights[i] + totalHeight/2.);
      }
      G4VPhysicalVolume *physiBottleExteriors[nBottles+1];
      for (int i = 1; i < nBottles+1; i++) {

        std::ostringstream ss;
        ss << "BottleExterior_" << i;
        physiBottleExteriors[i] = new G4PVPlacement(bottleRotation,
                                          bottlePositions[i],
                                          logicBottleExterior, // logical volume                                  
                                          ss.str().c_str(),    // name
                                          fLogicInnerCavity,   // mother  volume
                                          false,               // no boolean operations
                                          0, true);            // no particular field
        // sourceLocation needed to print liquid position
        G4ThreeVector sourceLocation = G4ThreeVector(0, 0, -totalHeight/2 + liquidLevel/2 + bodyThickness/2);
        sourceLocation = sourceLocation + bottlePositions[i];
        G4cout << "Endcap face: " << endCapFace / mm << " mm\n";
        G4cout << "Liquid position: " 
               << sourceLocation.getX() / mm << " "
               << sourceLocation.getY() / mm << " "
               << sourceLocation.getZ() / mm << " mm " << G4endl;
        G4cout << "Source liquid height: " << liquidLevel - bodyThickness / mm << " mm" << G4endl;
        G4cout << "Source liquid radius: " << solidBottleBodyCavity->GetOuterRadius() / mm << " mm" << G4endl;
      }
      
      if ( liquidLevel > bodyThickness ) {
        // then there is some liquid in the bottle
        G4cout << "Bottle is filled with " << fBottleLiquidMater->GetName();
        G4cout << " to " << liquidLevel/mm << " mm (measured from bottom of bottle exterior).";
        G4cout << " The remainder is filled with air." << G4endl;
        solidBottleLiquidPre = new G4Tubs("bottleLiquidPre", 0, bodyRadius, liquidLevel/2, 0, twopi);
        solidBottleLiquid    = new G4IntersectionSolid( "bottleLiquid",
                                          solidBottleInterior,
                                          solidBottleLiquidPre,0,
                                          G4ThreeVector(0, 0, -totalHeight/2 + liquidLevel/2) );
        logicBottleLiquid    = new G4LogicalVolume(solidBottleLiquid, fBottleLiquidMater, "BottleLiquid", 0,0,0);
        physiBottleLiquid    = new G4PVPlacement(0,
                                     G4ThreeVector(0,0,0),
                                     logicBottleLiquid,   // logical volume                                  
                                     "BottleLiquid",      // name
                                     logicBottleInterior, // mother  volume
                                     false,               // no boolean operations
                                     0, true);            // no particular field 
        if (sourceConfig[1] == "MgF2") {
          logicBottleLiquid->SetVisAttributes(AzureTransVisAtt);
        }else{
          logicBottleLiquid->SetVisAttributes(G4Colour(0.9,0.9,0.9));//GreyVisAtt);
        }
        G4cout << "Liquid mass   = " << logicBottleLiquid->GetMass()*nBottles / g << " g\n";

        /*
        G4ThreeVector sourceLocation = G4ThreeVector(0,0,-totalHeight/2.+liquidLevel/2.+bodyThickness/2.);
        G4VPhysicalVolume * pvs[] = {physiBottleExterior};
        //G4VPhysicalVolume * pvs[] = {physiBottleLiquid, physiBottleInterior, physiBottleExterior};
        for(int i = 0; i < 1; i++){
          G4RotationMatrix * rm = pvs[i]->GetRotation();
          if(rm != 0){
            sourceLocation = sourceLocation.transform(rm->inverse());
          }
          G4ThreeVector tv = pvs[i]->GetTranslation();
          sourceLocation = sourceLocation + tv;
        }
    
        G4cout << "Liquid position: " 
               << sourceLocation.getX() / mm << " "
               << sourceLocation.getY() / mm << " "
               << sourceLocation.getZ() / mm << " mm " << G4endl;
        G4cout << "Source liquid height: " << liquidLevel-bodyThickness / mm << " mm" << G4endl;
        G4cout << "Source liquid radius: " << solidBottleBodyCavity->GetOuterRadius() / mm << " mm" << G4endl;
        */

        // Add copper stand for MgF2:
        if(sourceConfig[1] == "MgF2"){
          double archHeight = 116.5 *mm;
          double archWidth = 79.0 *mm;
          double archDepth = 49.23 *mm;
          double doorHeight = 90.0 *mm;
          double doorWidth = 26.0 *mm;
          auto solidArch = new G4Box("Arch",archDepth/2.,archWidth/2., archHeight/2.);
          auto solidDoor = new G4Box("Door",archDepth/2.+1.*mm,doorWidth/2., doorHeight/2. + 1.*mm);
          auto solidCuStand = new G4SubtractionSolid("CuStand",solidArch,solidDoor,0,G4ThreeVector(0,0,(doorHeight-archHeight)/2. - 1.*mm));
          G4ThreeVector CuStandPosition;
          CuStandPosition.setX(endCapFace+archDepth/2.);
          CuStandPosition.setY(0);
          CuStandPosition.setZ((archHeight-fInnerCavityZ)/2.);

          auto logicCuStand = new G4LogicalVolume(solidCuStand,fMaterialsManager->GetMaterial("Copper"),"CuStand",0,0,0);
          auto physiCuStand = new G4PVPlacement(0,
                                    CuStandPosition,
                                    logicCuStand,   // its logical volume                                  
                                    "CuStand",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
          logicCuStand->SetVisAttributes(CopperVisAtt);

        }else if(sourceConfig[1] == "SAESSt707"){
          // http://130.160.10.13/Laboratory_log/315
          // Both bottles were placed inside the GeIII detector. Bottles are standing on three copper bricks and touching aluminum endcap (see attachment 5 and 6). Dimensions of the bottles and copper bricks are following:
          // 1. Bottle #1: Height 98.0 mm, Outer diameter 50.0 mm, Filling height 71.0 mm
          // 2. Bottle #2: Height 98.0 mm, Outer diameter 50.0 mm, Filling height 28.0 mm
          // 3. Copper brick Top (LxWxH in mm): 78.9 x 26.1 x 51.1
          // 4. Copper brick Middle (LxWxH in mm): 81.5 x 26.2 x 51.0
          // 5. Copper brick Bottom (LxWxH in mm): 90.2 x 51.3 x 26.3

          vector<G4ThreeVector> CuStandPositions;
          vector<double> CuStandLengthsX;
          vector<double> CuStandLengthsY;
          vector<double> CuStandLengthsZ;
          
          // Bottom
          CuStandPositions.push_back(G4ThreeVector(endCapFace + 51.3/2.*mm, 0, 26.3/2.*mm - fInnerCavityZ/2.));
          CuStandLengthsX.push_back(51.3*mm);
          CuStandLengthsY.push_back(90.2*mm);
          CuStandLengthsZ.push_back(26.3*mm);

          // Middle
          CuStandPositions.push_back(G4ThreeVector(endCapFace + 26.2/2.*mm, 0, (26.3 + 51.0)/2.*mm - fInnerCavityZ/2.));
          CuStandLengthsX.push_back(26.2*mm);
          CuStandLengthsY.push_back(81.5*mm);
          CuStandLengthsZ.push_back(51.0*mm);

          // Top
          CuStandPositions.push_back(G4ThreeVector(endCapFace + 26.1/2.*mm, 0, (26.3 + 51.0 + 51.1)/2.*mm - fInnerCavityZ/2.));
          CuStandLengthsX.push_back(26.1*mm);
          CuStandLengthsY.push_back(78.9*mm);
          CuStandLengthsZ.push_back(51.1*mm);
          

          for(int i = 0; i < CuStandPositions.size(); i++){
            auto solidCuStand = new G4Box("CuStand", CuStandLengthsX[i]/2.,  CuStandLengthsY[i]/2.,  CuStandLengthsZ[i]/2.);
            auto logicCuStand = new G4LogicalVolume(solidCuStand,fMaterialsManager->GetMaterial("Copper"),"CuStand",0,0,0);
            auto physiCuStand = new G4PVPlacement(0,
                                      CuStandPositions[i],
                                      logicCuStand,   // its logical volume                                  
                                      "CuStand",       // its name
                                      fLogicInnerCavity,      // its mother  volume
                                      false,            // no boolean operations
                                      0,true);          // no particular field 
            logicCuStand->SetVisAttributes(CopperVisAtt);
          }

        }else{
          G4cout << "Bottle is empty. (just air.)" << endl;
        }
      }

    }else{
      physiBottleExterior = new G4PVPlacement(bottleRotation,
                                    bottlePosition,
                                    logicBottleExterior,   // its logical volume                                  
                                    "BottleExterior",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

      if(liquidLevel > bodyThickness){
        // has liquid
        G4cout << "Bottle is filled with " << fBottleLiquidMater->GetName() << " to " << liquidLevel/mm << " mm (measured from bottom of bottle exterior). (Rest is air.)\n";
        solidBottleLiquidPre = new G4Tubs("bottleLiquidPre",0.,bodyRadius,liquidLevel/2.,0.,twopi);
        solidBottleLiquid = new G4IntersectionSolid("bottleLiquid",solidBottleInterior,solidBottleLiquidPre,0,G4ThreeVector(0,0,-totalHeight/2.+liquidLevel/2.));
        logicBottleLiquid = new G4LogicalVolume(solidBottleLiquid,fBottleLiquidMater,"BottleLiquid",0,0,0);
        physiBottleLiquid = new G4PVPlacement(0,
                                      G4ThreeVector(0,0,0),
                                      logicBottleLiquid,   // its logical volume                                  
                                      "BottleLiquid",       // its name
                                      logicBottleInterior,      // its mother  volume
                                      false,            // no boolean operations
                                      0,true);          // no particular field 
        if(sourceConfig[1] == "Water"){
          logicBottleLiquid->SetVisAttributes(AzureVisAtt);
        }else if (sourceConfig[1] == "LS"){
          logicBottleLiquid->SetVisAttributes(OrangeVisAtt);
        }else{
          logicBottleLiquid->SetVisAttributes(RoseVisAtt);
        }
      }

    }

  }else if(split(fSourcePosition,'_')[0] == "PeBottle2"){
    // Allow differing liquid levels

    vector<string> sourceConfig = split(fSourcePosition,'_');

    vector<double> liquidLevels;

    vector<double> standHeights;
    vector<double> xOffsets;
    vector<double> yOffsets;

    vector<G4ThreeVector> CuStandPositions;
    vector<double> CuStandLengthsX;
    vector<double> CuStandLengthsY;
    vector<double> CuStandLengthsZ;

    // Fixed dimensions of the PE bottle
    double totalHeight      = 99*mm;
    double bodyHeight       = 77*mm;
    double bodyRadius       = 50.2/2*mm;
    double bodyThickness    =  2*mm;
    double capRadius        = 42/2*mm;
    double capHeight        = 15*mm;
    double capThickness     =  2*mm;
    double capSideThickness =  5*mm;
    double neckRadius       = 32.4/2*mm;
    double neckThickness    =  2*mm;
    double flapRadius       = 42.2/2 *mm;
    double flapHeight       =  2*mm;
    double flapFromBody     =  2*mm; //distance from top of body to bottom of flap
    double standHeight      =  5*inch;

    G4RotationMatrix * bottleRotation = 0;

    // Define geometry here
    if (sourceConfig[1] == "SAESSt707" || sourceConfig[1] == "HPZrPellets"){

      //Material
      fBottleLiquidMater = fMaterialsManager->GetMaterial("SAESSt707");

      // Bottles
      standHeights.push_back(128.4*mm);
      xOffsets.push_back(bodyRadius); 
      yOffsets.push_back(bodyRadius); 

      standHeights.push_back(128.4*mm);
      xOffsets.push_back(bodyRadius); 
      yOffsets.push_back(-bodyRadius); 

      // Liquid levels
      if (sourceConfig[1] == "SAESSt707"){
        liquidLevels.push_back(71.*mm); 
        liquidLevels.push_back(28.*mm); 
      }else if(sourceConfig[1] == "HPZrPellets"){
        liquidLevels.push_back(73.*mm); 
        liquidLevels.push_back(62.*mm); 
      }
          
      // Cu stands
      // Bottom
      CuStandPositions.push_back(G4ThreeVector(endCapFace + 51.3/2.*mm, 0, 26.3/2.*mm - fInnerCavityZ/2.));
      CuStandLengthsX.push_back(51.3*mm);
      CuStandLengthsY.push_back(90.2*mm);
      CuStandLengthsZ.push_back(26.3*mm);

      // Middle
      CuStandPositions.push_back(G4ThreeVector(endCapFace + 26.2/2.*mm, 0, 26.3 + 51.0/2.*mm - fInnerCavityZ/2.));
      CuStandLengthsX.push_back(26.2*mm);
      CuStandLengthsY.push_back(81.5*mm);
      CuStandLengthsZ.push_back(51.0*mm);

      // Top
      CuStandPositions.push_back(G4ThreeVector(endCapFace + 26.1/2.*mm, 0, 26.3 + 51.0 + 51.1/2.*mm - fInnerCavityZ/2.));
      CuStandLengthsX.push_back(26.1*mm);
      CuStandLengthsY.push_back(78.9*mm);
      CuStandLengthsZ.push_back(51.1*mm);
    //} else if (sourceConfig[1] == "XXX"){
    }
 
    // Bottle exterior (neck + body + cap + flap)
    solidBottleNeck = new G4Tubs("bottleNeck",0.,neckRadius,totalHeight/2.,0.,twopi);
    solidBottleBody = new G4Tubs("bottleBody",0.,bodyRadius,bodyHeight/2.,0.,twopi);
    solidBottleNeckBody = new G4UnionSolid("bottleNeckBody",solidBottleNeck,solidBottleBody,0,G4ThreeVector(0,0,-totalHeight/2.+bodyHeight/2.));
    solidBottleCap = new G4Tubs("bottleCap",0.,capRadius,capHeight/2.,0.,twopi);
    solidBottleNeckBodyCap = new G4UnionSolid("bottleNeckBodyCap",
                                                 solidBottleNeckBody,
                                                 solidBottleCap, 0,
                                                 G4ThreeVector(0, 0, totalHeight/2 - capHeight/2) );
    solidBottleFlap = new G4Tubs("bottleFlap",0.,flapRadius,flapHeight/2.,0.,twopi);
    solidBottleExterior = new G4UnionSolid("bottleExterior",solidBottleNeckBodyCap,solidBottleFlap,0,G4ThreeVector(0,0,-totalHeight/2.+bodyHeight+flapFromBody+flapHeight/2.));

    // Bottle interior 
    double neckCavityHeight = totalHeight-bodyThickness-capThickness;
    double bodyCavityHeight = bodyHeight-2.*bodyThickness;
    double capCavityHeight = capHeight-2.*capThickness;
    solidBottleNeckCavity = new G4Tubs("bottleNeckCavity",0.,neckRadius-neckThickness,neckCavityHeight/2.,0.,twopi);
    solidBottleBodyCavity = new G4Tubs("bottleBodyCavity",0.,bodyRadius-bodyThickness,bodyCavityHeight/2.,0.,twopi);
    solidBottleNeckBodyCavity = new G4UnionSolid("bottleNeckBodyCavity",solidBottleNeckCavity,solidBottleBodyCavity,0,G4ThreeVector(0,0,-neckCavityHeight/2.+bodyCavityHeight/2.));
    solidBottleCapCavity = new G4Tubs("bottleCapCavity",0.,capRadius-capSideThickness,capCavityHeight/2.,0.,twopi);
    solidBottleInterior = new G4UnionSolid("bottleInterior",
                                            solidBottleNeckBodyCavity,
                                            solidBottleCapCavity, 0,
                                            G4ThreeVector(0, 0, neckCavityHeight/2 - capCavityHeight/2) );
    solidBottleInterior = new G4UnionSolid("bottleInterior",solidBottleNeckBodyCavity,solidBottleCapCavity,0,G4ThreeVector(0,0,neckCavityHeight/2.-capCavityHeight/2.));

    int nBottles = standHeights.size();
 
    G4ThreeVector bottlePositions[nBottles];
    for(int i = 0; i < nBottles; i++){
      bottlePositions[i].setX(endCapFace+xOffsets[i]);
      bottlePositions[i].setY(yOffsets[i]);
      bottlePositions[i].setZ(-fInnerCavityZ/2. + standHeights[i] + totalHeight/2.);

      auto logicBottleExterior = new G4LogicalVolume(solidBottleExterior,fBottleMater,"BottleExterior",0,0,0);
      auto logicBottleInterior = new G4LogicalVolume(solidBottleInterior,fDefaultMater,"BottleInterior",0,0,0);
      auto physiBottleInterior = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,(bodyThickness - capThickness)/2.),
                                    logicBottleInterior,   // its logical volume                                  
                                    "BottleInterior",       // its name
                                    logicBottleExterior,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

      auto physiBottleExterior = new G4PVPlacement(bottleRotation,
                                        bottlePositions[i],
                                        logicBottleExterior, // logical volume                                  
                                        "BottleExterior",
                                        fLogicInnerCavity,   // mother  volume
                                        false,               // no boolean operations
                                        0, true);            // no particular field


      auto solidBottleLiquidPre = new G4Tubs("bottleLiquidPre",0.,bodyRadius,liquidLevels[i]/2.,0.,twopi);
      auto solidBottleLiquid = new G4IntersectionSolid("bottleLiquid",solidBottleInterior,solidBottleLiquidPre,0,G4ThreeVector(0,0,-totalHeight/2.+liquidLevels[i]/2.));
      auto logicBottleLiquid = new G4LogicalVolume(solidBottleLiquid,fBottleLiquidMater,"BottleLiquid",0,0,0);
      auto physiBottleLiquid = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,0),
                                    logicBottleLiquid,   // its logical volume                                  
                                    "BottleLiquid",       // its name
                                    logicBottleInterior,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

      auto visAttBottleExterior = new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
      logicBottleExterior->SetVisAttributes(visAttBottleExterior);
      auto visAttBottleInterior = new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.0));
      logicBottleInterior->SetVisAttributes(visAttBottleInterior);
      logicBottleLiquid->SetVisAttributes(RoseVisAtt);

      // sourceLocation needed to print liquid position
      G4ThreeVector sourceLocation = G4ThreeVector(0, 0, -totalHeight/2 + liquidLevels[i]/2 + bodyThickness/2);
      sourceLocation = sourceLocation + bottlePositions[i];
      G4cout << "Endcap face: " << endCapFace / mm << " mm\n";
      G4cout << "Liquid position: " 
             << sourceLocation.getX() / mm << " "
             << sourceLocation.getY() / mm << " "
             << sourceLocation.getZ() / mm << " mm " << G4endl;
      G4cout << "Source liquid height: " << liquidLevels[i] - bodyThickness / mm << " mm" << G4endl;
      G4cout << "Source liquid radius: " << solidBottleBodyCavity->GetOuterRadius() / mm << " mm" << G4endl;
    }

    // Place Cu stands
    for(int i = 0; i < CuStandPositions.size(); i++){
      auto solidCuStand = new G4Box("CuStand", CuStandLengthsX[i]/2.,  CuStandLengthsY[i]/2.,  CuStandLengthsZ[i]/2.);
      auto logicCuStand = new G4LogicalVolume(solidCuStand,fMaterialsManager->GetMaterial("Copper"),"CuStand",0,0,0);
      auto physiCuStand = new G4PVPlacement(0,
                                CuStandPositions[i],
                                logicCuStand,   // its logical volume                                  
                                "CuStand",       // its name
                                fLogicInnerCavity,      // its mother  volume
                                false,            // no boolean operations
                                0,true);          // no particular field 
      logicCuStand->SetVisAttributes(CopperVisAtt);
    }


  }else if(split(fSourcePosition,'_')[0] == "Exo200"){

    vector<string> sourceConfig = split(fSourcePosition,'_');
    // Exo200_Remote[12]_[XY][PN]
    G4cout << "Source: Exo200" << G4endl;

    // Det = 2
   
    double totalLength = 570.*mil;
    double totalRadius = 74.8/2.*mil;
    /*//
    double drillBitAngle = 118.*deg;
    //note: depth refers to conical cavity created by drill bit.

    // Capsule
    double mainCavityLength = 344.*mil;
    double mainCavityRadius = 55./2. * mil;
    double mainCavityDepth = mainCavityRadius*tan((180.*deg-drillBitAngle.)/2.);
    double wireCavityLength = 188.*mil;
    double wireCavityRadius = 39./2. * mil;
    double wireCavityDepth = wireCavityRadius*tan((180.*deg-drillBitAngle.)/2.);

    // Capsule Plug
    double capsulePlugTipRadius = 53./2.*mil;
    double capsulePlugTipBaseLength = 30.*mil; //approx. all tapering ignored.
    double capsulePlugBaseRadius = 55./2.*mil; //actually 57/2, but it's squeezed into the main cavity.
    double capsulePlugHoleRadius = 35./2. *mil;
    double capsulePlugHoleLength = 30. *mil;
    double capsulePlugHoleDepth = capsulePlugHoleRadius*tan((180.*deg-drillBitAngle.)/2.);
    
    // Cup
    double cupRadius = 51./2.*mil;
    double cupLength = 110.*mil;
    double cupHoleRadius = 36./2.*mil;
    double cupHoleLength = 86.*mil;
    double cupHoleDepth = cupHoleRadius*tan((180.*deg-drillBitAngle.)/2.);
  
    // Cup Lid
    double cupLidLength = 65.*mil
    double cupLidTipRadius = 37./2.*mil;
    double cupLidTipLength = 45.*mil;
    double cupLidBaseRadius = 51./2.*mil;
    double cupLidBaseLength = cupLidLength-cupLidTipLength;
    
    // Cork
    // -- main taper considered, tip taper ignored
    double corkLength = 78*mil;
    double corkBaseRadius = 57./2.*mil;
    double corkMainTaperAngle = 2.5*deg;
    double corkTipRadius = 57./2.-corkLength*mil;  // */


    // Simplified geometry
    double cavityLength = (27. + 208. + 9.)*mil;
    double cavityRadius = 55./2. * mil;
    double wireCavityLength = 188.*mil;
    double wireCavityRadius = 39./2. * mil;

    double interCavityLength = 38. * mil;

    double innerCapsuleGap = 27.*mil;
    double innerCapsuleLength = 208. * mil;
    double innerCapsuleRadius = cavityRadius;

    double innerCavityLength = 41.*mil;
    double innerCavityRadius = 36./2.*mil;
    double innerCavityOffset = 65.*mil;
 
    double sourceBallRadius = 0.7/2.*mm;

    // Source Holder
    double sourceHolderLength = 1.*inch;
    double sourceHolderThickness = 3.*mm;

    double sourceHolderSlitLength = 18.*mm;
    double sourceHolderSlitWidth = 2.5*mm;
    double sourceHolderSlitDepth = 75.*mil;

    // Source Holder
    G4ThreeVector exo200Translation; 
    if(sourceConfig[1] == "Remote1"){
      G4cout << "Position: Remote1 (Remote)" << G4endl;
      exo200Translation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation);
    }else if (sourceConfig[1] == "Remote2"){
      G4cout << "Position: Remote2 (Remoteremote)" << G4endl;
      exo200Translation.setX(fDetectorEnclosureLength-fNeckOffset-fNeckLocation + 23.2*cm);
    }
    exo200Translation.setY(-fInnerCavityY/2.+sourceHolderLength/2.);
    exo200Translation.setZ(-fInnerCavityZ/2.+sourceHolderThickness/2.);
    G4RotationMatrix * exo200Rotation = new G4RotationMatrix();
    if(sourceConfig[2] == "XP"){
      exo200Rotation->rotateZ(0);
    }else if(sourceConfig[2] == "XN"){
      exo200Rotation->rotateZ(180.*deg);
    }else if(sourceConfig[2] == "YP"){
      exo200Rotation->rotateZ(-90.*deg);
    }else if(sourceConfig[2] == "YN"){
      exo200Rotation->rotateZ(90.*deg);
    }

    // -- main
    G4Box * solidExo200SourceHolder = new G4Box("exo200SourceHolder",sourceHolderLength/2.,sourceHolderLength/2.,sourceHolderThickness/2.);
    G4LogicalVolume * logicExo200SourceHolder = new G4LogicalVolume(solidExo200SourceHolder,fExo200SourceHolderMater,"Exo200SourceHolder",0,0,0);
    G4VPhysicalVolume * physiExo200SourceHolder = new G4PVPlacement(exo200Rotation,
                                    exo200Translation,
                                    //G4ThreeVector(0,0,0),
                                    logicExo200SourceHolder,   // its logical volume                                  
                                    "Exo200SourceHolder",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    // -- slit
    G4Box * solidExo200SourceHolderSlit = new G4Box("exo200SourceHolderSlit",sourceHolderSlitLength/2.,sourceHolderSlitWidth/2.,sourceHolderSlitDepth/2.);
    G4LogicalVolume * logicExo200SourceHolderSlit = new G4LogicalVolume(solidExo200SourceHolderSlit,fDefaultMater,"Exo200SourceHolderSlit",0,0,0);
    G4VPhysicalVolume * physiExo200SourceHolderSlit = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,(sourceHolderThickness-sourceHolderSlitDepth)/2.),
                                    logicExo200SourceHolderSlit,   // its logical volume                                  
                                    "Exo200SourceHolderSlit",       // its name
                                    logicExo200SourceHolder,    // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
     
    // Source Capsule
    G4RotationMatrix * capsuleRotation = new G4RotationMatrix(); 
    capsuleRotation->rotateY(-90.*deg);
    G4Tubs * solidExo200CapsuleMain = new G4Tubs("exo200CapsuleMain",0.,totalRadius,totalLength/2.,0.,twopi);
    G4Tubs * solidExo200WireCavity = new G4Tubs("exo200WireCavity",0.,wireCavityRadius,wireCavityLength,0.,twopi);  // make it twice as long on purpose
    G4SubtractionSolid * solidExo200Capsule = new G4SubtractionSolid("exo200Capsule",solidExo200CapsuleMain,solidExo200WireCavity,0,G4ThreeVector(0,0,-totalLength/2.));
    G4LogicalVolume * logicExo200Capsule = new G4LogicalVolume(solidExo200Capsule,fExo200CapsuleMater,"Exo200Capsule",0,0,0);
    G4VPhysicalVolume * physiExo200Capsule = new G4PVPlacement(capsuleRotation,
                                    G4ThreeVector(0,0,0),
                                    logicExo200Capsule,   // its logical volume                                  
                                    "Exo200Capsule",       // its name
                                    logicExo200SourceHolderSlit,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    // Main Cavity
    G4Tubs * solidExo200Cavity = new G4Tubs("exo200Cavity",0.,cavityRadius,cavityLength/2.,0.,twopi);
    G4LogicalVolume * logicExo200Cavity = new G4LogicalVolume(solidExo200Cavity,fDefaultMater,"Exo200Cavity",0,0,0);
    G4VPhysicalVolume * physiExo200Cavity = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,-totalLength/2.+wireCavityLength+interCavityLength+cavityLength/2.),
                                    logicExo200Cavity,   // its logical volume                                  
                                    "Exo200Cavity",       // its name
                                    logicExo200Capsule,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    // Inner Capsule
    G4Tubs * solidExo200InnerCapsule = new G4Tubs("exo200InnerCapsule",0.,innerCapsuleRadius-epsilon,innerCapsuleLength/2.,0.,twopi);
    G4LogicalVolume * logicExo200InnerCapsule = new G4LogicalVolume(solidExo200InnerCapsule,fExo200PlugMater,"Exo200InnerCapsule",0,0,0);
    G4VPhysicalVolume * physiExo200InnerCapsule = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,-cavityLength/2.+innerCapsuleGap+ innerCapsuleLength/2.),
                                    logicExo200InnerCapsule,   // its logical volume                                  
                                    "Exo200InnerCapsule",       // its name
                                    logicExo200Cavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    
    // Inner Cavity
    G4Tubs * solidExo200InnerCavity = new G4Tubs("exo200InnerCavity",0.,innerCavityRadius,innerCavityLength/2.,0.,twopi);
    G4LogicalVolume * logicExo200InnerCavity = new G4LogicalVolume(solidExo200InnerCavity,fDefaultMater,"Exo200InnerCavity",0,0,0);
    G4VPhysicalVolume * physiExo200InnerCavity = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,-innerCapsuleLength/2.+ innerCavityOffset + innerCavityLength/2.),
                                    logicExo200InnerCavity,   // its logical volume                                  
                                    "Exo200InnerCavity",       // its name
                                    logicExo200InnerCapsule,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    // Source ball
    G4Orb * solidExo200SourceBall = new G4Orb("exo200SourceBall",sourceBallRadius);
    G4LogicalVolume * logicExo200SourceBall = new G4LogicalVolume(solidExo200SourceBall,fExo200SourceMater,"Exo200SourceBall",0,0,0);
    G4VPhysicalVolume * physiExo200SourceBall = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,0),
                                    logicExo200SourceBall,   // its logical volume                                  
                                    "Exo200SourceBall",       // its name
                                    logicExo200InnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4ThreeVector sourceBallLocation = G4ThreeVector(0,0,0);
    G4VPhysicalVolume * pvs[] = {physiExo200SourceBall, physiExo200InnerCavity, physiExo200InnerCapsule, physiExo200Cavity, physiExo200Capsule, physiExo200SourceHolderSlit, physiExo200SourceHolder};
    for(int i = 0; i < 7; i++){
      G4RotationMatrix * rm = pvs[i]->GetRotation();
      if(rm != 0){
        sourceBallLocation = sourceBallLocation.transform(rm->inverse());
      }
      G4ThreeVector tv = pvs[i]->GetTranslation();
      sourceBallLocation = sourceBallLocation + tv;
    }

    G4cout << "Source ball position: " 
           << sourceBallLocation.getX() / mm << " "
           << sourceBallLocation.getY() / mm << " "
           << sourceBallLocation.getZ() / mm << " mm " << G4endl;
    G4cout << "Source ball radius: " << solidExo200SourceBall->GetRadius() / mm << " mm" << G4endl;

    logicExo200SourceHolder->SetVisAttributes(YellowVisAtt);
    //logicExo200SourceHolderSlit->SetVisAttributes(VisAtt);
    logicExo200Capsule->SetVisAttributes(GreenVisAtt);
    //logicExo200Cavity->SetVisAttributes(VisAtt);
    logicExo200InnerCapsule->SetVisAttributes(VioletVisAtt);
    //logicExo200InnerCavity->SetVisAttributes(VisAtt);
    logicExo200SourceBall->SetVisAttributes(RedVisAtt);
    // */
  }else if(fSourcePosition == "GerdaD6"){
    // Dimensions from: http://130.160.100.75/GeII/49
    // Note: slight tilt towards face is ignored.
    //       assume to be SS304L 
    //       physVol name "GerdaD6Block"

    // Block
    double blockX = 2.*cm; //approx
    double blockY = 20.*cm;
    double blockZ = 20.*cm;
    double gapWithFace = 0.*mm;
    double gapOnRight = 5.5*cm;
    double gapOnLeft = 5.2*cm;
    // gaps on left and right plus blockY = 5.5 + 5.2 + 20 = 30.7 > 30.48 = 12 inch = innerCavityY. 
    // interpret gaps on left right as offset of block center.
    double offsetY = (gapOnRight - gapOnLeft) / 2.;
    // Notch (hard-coded to be at lower-left corner)
    double notchX = blockX; 
    double notchY = 2.*cm;
    double notchZ = 3.*cm;

    // Spacers
    double spacerX = 2.*cm; //approx
    double spacerY = 8*cm; //approx
    double spacerZ = 5.*cm + 2.4*mm; // 2.4 mm to account for sim/real discrep in inner cavity dims.
    double spacerPosX = endCapFace + spacerX/2.;
    double spacerPosZ = -fInnerCavityZ/2. + spacerZ/2.;
    double leftSpacerPosX = spacerPosX;
    double leftSpacerPosY = -fInnerCavityY/2. + spacerY/2.;
    double leftSpacerPosZ = spacerPosZ;
    double rightSpacerPosX = spacerPosX;
    double rightSpacerPosY = fInnerCavityY/2. - spacerY/2.;
    double rightSpacerPosZ = spacerPosZ;
    
    // Block position
    double blockPosX = endCapFace + gapWithFace + blockX/2.;
    double blockPosY = offsetY;
    double blockPosZ = -fInnerCavityZ/2. + spacerZ + blockZ/2.;
    
    // Block
    G4Box * solidGerdaD6BlockIntact = new G4Box("gerdaD6BlockIntact",blockX/2.,blockY/2.,blockZ/2.);
    G4Box * solidGerdaD6Notch = new G4Box("gerdaD6Notch",notchX/2.,notchY/2.,notchZ/2.);
    G4SubtractionSolid * solidGerdaD6Block = new G4SubtractionSolid ("gerdaD6Block",solidGerdaD6BlockIntact,solidGerdaD6Notch,0,G4ThreeVector(0,(-blockY+notchY)/2.,(-blockZ+notchZ)/2.));
    G4LogicalVolume * logicGerdaD6Block = new G4LogicalVolume(solidGerdaD6Block,fGerdaD6Mater,"GerdaD6Block",0,0,0);
    G4VPhysicalVolume * physiGerdaD6Block = new G4PVPlacement(0,
                                    G4ThreeVector(blockPosX,blockPosY,blockPosZ),
                                    logicGerdaD6Block,   // its logical volume                                  
                                    "GerdaD6Block",       // its name
                                    fLogicInnerCavity,    // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    // Spacers
    G4Box * solidGerdaD6Spacer = new G4Box("gerdaD6Spacer",spacerX/2.,spacerY/2.,spacerZ/2.);
    G4LogicalVolume * logicGerdaD6Spacer = new G4LogicalVolume(solidGerdaD6Spacer,fCopperBlockMater,"GerdaD6Spacer",0,0,0);
    G4VPhysicalVolume * physiGerdaD6LeftSpacer = new G4PVPlacement(0,
                                    G4ThreeVector(leftSpacerPosX,leftSpacerPosY,leftSpacerPosZ),
                                    logicGerdaD6Spacer,   // its logical volume                                  
                                    "GerdaD6LeftSpacer",       // its name
                                    fLogicInnerCavity,    // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    G4VPhysicalVolume * physiGerdaD6RightSpacer = new G4PVPlacement(0,
                                    G4ThreeVector(rightSpacerPosX,rightSpacerPosY,rightSpacerPosZ),
                                    logicGerdaD6Spacer,   // its logical volume                                  
                                    "GerdaD6RightSpacer",       // its name
                                    fLogicInnerCavity,    // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    cout << "Gerda D6 Physical vol name: " << physiGerdaD6Block->GetName() << endl; 
    cout << "Gerda D6 Block Position   : " << blockPosX << " " << blockPosY << " " << blockPosZ << endl;
    cout << "Gerda D6 Block Halflengths : " << blockX/2. << " " << blockY/2. << " " << blockZ/2. << endl;
 
  }else if(fSourcePosition == "XLPE"){
    // Det: Ge3
    // Foam board dimemsions: 12 x 12 x 1 inch
    // Assumptions: Top bent foam board 1/8 circle arc, which implies radius of curvature = 12*8/2/pi ~ 15.29 inch.
    double foamX = 12.*inch;
    double foamY = 12.*inch;
    double foamZ = 1.*inch;

    //Top foam boards
    double arcAngle1 = 24.*deg;
    double arcAngle2 = 24.*deg;
    double arcAngle3 = 28.*deg;
    double roc1 = 12./arcAngle1*inch;
    double roc2 = 12./arcAngle2*inch;
    double roc3 = 12./arcAngle3*inch;
    double topFoamShiftX = -fInnerCavityX/2.+foamX/2.;
    double topFoamShiftY1 = -fInnerCavityY/2.;//-(roc-fEndCapRadius-foamZ/2.-1.*mm)*sin(pi/8.);
    double topFoamShiftY2 = -fInnerCavityY/2.;//-(roc-fEndCapRadius-foamZ/2.-1.*mm)*sin(pi/8.);
    double topFoamShiftY3 = -fInnerCavityY/2.+0*inch;//-(roc-fEndCapRadius-foamZ/2.-1.*mm)*sin(pi/8.);
    double topFoamShiftZ1 = -roc1+fInnerCavityZ/2.-foamZ/2.;
    double topFoamShiftZ2 = -roc2+fInnerCavityZ/2.-foamZ/2.;
    double topFoamShiftZ3 = -roc3+fInnerCavityZ/2.-foamZ/2.;
    double topFoamOffsetZ = foamZ/cos(arcAngle1);
    // Bottom foam board
    double arcAngle4 = 58.*deg;  //63deg--10inch
    double roc4 = 9.5/arcAngle4*inch;
    double bottomFoamShiftX = -fInnerCavityX/2.+foamX/2.;//+2.*inch;
    double bottomFoamShiftY = roc4-fInnerCavityY/2.+foamZ/2.;
    double bottomFoamShiftZ = fInnerCavityZ/2.-3.*foamZ/cos(arcAngle1);

    // Front foam boards
    double frontFoamPositionX = -fInnerCavityX/2. + foamX + 3.*foamZ;

    G4RotationMatrix * topFoamRotation = new G4RotationMatrix(); 
    topFoamRotation->rotateY(90.*deg);
   
    G4RotationMatrix * bottomFoamRotation = new G4RotationMatrix(); 
    bottomFoamRotation->rotateY(90.*deg);
    bottomFoamRotation->rotateZ(90.*deg+arcAngle4);
    //bottomFoamRotation->rotateZ(-90.*deg);

    G4Tubs * solidTopFoam1 = new G4Tubs("topFoam1",roc1-foamZ/2.,roc1+foamZ/2.,foamX/2.,0,arcAngle1);
    G4LogicalVolume * logicTopFoam1 = new G4LogicalVolume(solidTopFoam1 ,fXLPEMater,"TopFoam1",0,0,0);
    G4VPhysicalVolume * physiTopFoam1 = new G4PVPlacement(topFoamRotation,         
                                    G4ThreeVector(topFoamShiftX,topFoamShiftY1,topFoamShiftZ1),
                                    logicTopFoam1,   // its logical volume                                  
                                    "XLPE_1",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    G4Tubs * solidTopFoam2 = new G4Tubs("topFoam2",roc2-foamZ/2.,roc2+foamZ/2.,foamX/2.,0,arcAngle2);
    G4LogicalVolume * logicTopFoam2 = new G4LogicalVolume(solidTopFoam2 ,fXLPEMater,"TopFoam2",0,0,0);
    G4VPhysicalVolume * physiTopFoam2 = new G4PVPlacement(topFoamRotation,         
                                    G4ThreeVector(topFoamShiftX,topFoamShiftY2,topFoamShiftZ2-topFoamOffsetZ),
                                    logicTopFoam2,   // its logical volume                                  
                                    "XLPE_2",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    G4Tubs * solidTopFoam3 = new G4Tubs("topFoam3",roc3-foamZ/2.,roc3+foamZ/2.,foamX/2.,0,arcAngle3);
    G4LogicalVolume * logicTopFoam3 = new G4LogicalVolume(solidTopFoam3 ,fXLPEMater,"TopFoam3",0,0,0);
    G4VPhysicalVolume * physiTopFoam3 = new G4PVPlacement(topFoamRotation,         
                                    G4ThreeVector(topFoamShiftX,topFoamShiftY3,topFoamShiftZ3-2.*topFoamOffsetZ),
                                    logicTopFoam3,   // its logical volume                                  
                                    "XLPE_3",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4Tubs * solidBottomFoam1 = new G4Tubs("bottomFoam1",roc4-foamZ/2.,roc4+foamZ/2.,foamX/2.,0,arcAngle4);
    G4LogicalVolume * logicBottomFoam1 = new G4LogicalVolume(solidBottomFoam1 ,fXLPEMater,"BottomFoam1",0,0,0);
    G4VPhysicalVolume * physiBottomFoam1 = new G4PVPlacement(bottomFoamRotation,         
                                    G4ThreeVector(bottomFoamShiftX,bottomFoamShiftY,bottomFoamShiftZ),
                                    logicBottomFoam1,   // its logical volume                                  
                                    "XLPE_4",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4Box * solidFrontFoam1Raw = new G4Box("frontFoam1Raw",3.*foamZ,foamX/2.,foamY/2.);
    G4IntersectionSolid * solidFrontFoam1 = new G4IntersectionSolid("frontFoam1",solidFrontFoam1Raw,fSolidInnerCavity);
    G4LogicalVolume * logicFrontFoam1 = new G4LogicalVolume(solidFrontFoam1 ,fXLPEMater,"FrontFoam1",0,0,0);
    G4VPhysicalVolume * physiFrontFoam1 = new G4PVPlacement(0,
                                    G4ThreeVector(frontFoamPositionX,0,0),
                                    logicFrontFoam1,   // its logical volume                                  
                                    "XLPE_5",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    cout << "Foam masses" << endl;
    cout << logicTopFoam1->GetMass() / g << " g" << endl;
    cout << logicTopFoam2->GetMass() / g << " g" << endl;
    cout << logicTopFoam3->GetMass() / g << " g" << endl;
    cout << logicBottomFoam1->GetMass() / g << " g" << endl;
    cout << logicFrontFoam1->GetMass() / g << " g" << endl;
 
  }else if( split( fSourcePosition, '.' )[0] == "M16"){
    // Resin blocks from Spencer composites. (R-16 in new material database)
    // 5 blocks each, all 6" x 6" x Z", where Z is nominally 1, but there is a large variation (+/- ~5 mm) among blocks, so I make it variable.
    // Block numbering: 1. Bottom, 2. Left, 3. Right, 4. Front, 5. Top.
    // M16.4:
    // Copper blocks from Southern Copper and Supply. (R-045 in new material database)
    // 5 blocks each, all 6" x 6" x 1"
    // Block numbering: 1. Bottom, 2. Left, 3. Right, 4. Front, 5. Top.

    int resinNumber;// = atoi(split(fSourcePosition,'.')[1]);
    if(split(fSourcePosition,'.')[1] == "1"){
      resinNumber = 1;
    }else if(split(fSourcePosition,'.')[1] == "2"){
      resinNumber = 2;
    }else if(split(fSourcePosition,'.')[1] == "3"){
      resinNumber = 3;
    }else if(split(fSourcePosition,'.')[1] == "4"){
      resinNumber = 4;
    }

    const int numBlocks = 5;
    char orientation[numBlocks+1] = {' ', 'Z', 'Y', 'Y', 'X', 'Z'};

    // Dimensions
    // indices: resin number, block number
    // Measured
    double thickness[5][numBlocks+1] = {
      {0,0,0,0,0,0},  // for convenience
      {0, 2.7*cm, 3.0*cm, 2.7*cm, 2.7*cm, 2.9*cm},  // M16.1
      {0, 2.5*cm, 2.5*cm, 2.5*cm, 2.5*cm, 2.15*cm},  // M16.2
      {0, 2.8*cm, 3.1*cm, 3.0*cm, 3.0*cm, 3.3*cm},   // M16.3
      {0, 1.*inch, 1.*inch, 1.*inch, 1.*inch, 1.*inch} // M16.4: Phony. for SCSCu
    };
    // Adjusted
    /*double thickness[4][numBlocks+1] = {
      {0,0,0,0,0},  // for convenience
      {0, 2.75*cm, 2.93*cm, 2.68*cm, 2.82*cm, 2.82*cm},  // M16.1
      {0, 2.43*cm, 2.60*cm, 2.38*cm, 2.37*cm, 2.33*cm},  // M16.2
      {0, 2.88*cm, 3.03*cm, 2.97*cm, 2.97*cm, 3.35*cm}   // M16.3
    }; */

    // Position number.
    int posNum[5][numBlocks+1] = {
      {0,0,0,0,0,0},  // for convenience
      {0,1,4,3,5,2},  // M16.1. Actual.
      {0,1,4,3,5,2},  // M16.2  Actual
      {0,1,3,2,5,4},   // M16.3. Actual.
      {0,1,2,3,4,5}   // M16.4. Phony
    };
    int atPos[numBlocks+1];
    for(int i = 0; i < numBlocks+1; i++){
      atPos[i] = posNum[resinNumber][i];
    } 

    double sideLength = 6.*inch;  //both X and Y
    double spacerHeights[5] = {0, 2.*inch, 2.*inch, 1.*inch, 1.*inch}; // Copper spacer(s). Not implemented here. Just for its dimension.
    double spacerHeight = spacerHeights[resinNumber]; 

    // Positions
    double posX[numBlocks+1] = {0, endCapFace + thickness[resinNumber][atPos[4]] - sideLength / 2., 
                                   endCapFace - sideLength / 2., 
                                   endCapFace - sideLength / 2., 
                                   endCapFace + thickness[resinNumber][atPos[4]]/2., 
                                   endCapFace + thickness[resinNumber][atPos[4]] - sideLength / 2.};
    double posY[numBlocks+1] = {0, 0,
                                   -(sideLength/2. - thickness[resinNumber][atPos[2]]/2.),
                                   sideLength/2. - thickness[resinNumber][atPos[3]]/2.,
                                   0, 0};
    double posZ[numBlocks+1] = {0, -fInnerCavityZ/2. + spacerHeight + thickness[resinNumber][atPos[1]]/2.,
                                   -fInnerCavityZ/2. + spacerHeight + thickness[resinNumber][atPos[1]] + sideLength/2.,
                                   -fInnerCavityZ/2. + spacerHeight + thickness[resinNumber][atPos[1]] + sideLength/2.,
                                   -fInnerCavityZ/2. + spacerHeight + thickness[resinNumber][atPos[1]] + sideLength/2.,
                                   -fInnerCavityZ/2. + spacerHeight + thickness[resinNumber][atPos[1]] + sideLength + thickness[resinNumber][atPos[5]]/2.};

    // If interferes with end cap, move blocks at pos 2 and 3 outward.
    if(abs(posY[2]) - thickness[resinNumber][atPos[2]]/2. < fDetectorEnclosureRadius){
      cout << "WARNING: Resin block 2 would interfere with the end cap if placed at current position: " << posX[2]/mm << " " << posY[2]/mm << " " << posZ[2]/mm << " mm\n";
      posY[2] = -(fDetectorEnclosureRadius + thickness[resinNumber][atPos[2]]/2.);
      cout << "WARNING: Resin block 2 is hence moved to: " << posX[2]/mm << " " << posY[2]/mm << " " << posZ[2]/mm << " mm\n";
    }
    if(abs(posY[3]) - thickness[resinNumber][atPos[3]]/2. < fDetectorEnclosureRadius){
      cout << "WARNING: Resin block 3 would interfere with the end cap if placed at current position: " << posX[3]/mm << " " << posY[3]/mm << " " << posZ[3]/mm << " mm\n";
      posY[3] = fDetectorEnclosureRadius + thickness[resinNumber][atPos[3]]/2.;
      cout << "WARNING: Resin block 3 is hence moved to: " << posX[3]/mm << " " << posY[3]/mm << " " << posZ[3]/mm << " mm\n";
    }

    // Create solids, logis and physis
    G4Box * solidResinBlocks[numBlocks+1];
    G4LogicalVolume * logicResinBlocks[numBlocks+1];
    G4VPhysicalVolume * physiResinBlocks[numBlocks+1];
    for(int i = 1; i < numBlocks+1; i++){
      std::ostringstream ss;
      ss << fSourcePosition << "_" << i;
      switch(orientation[i]){
        case 'X': solidResinBlocks[i] = new G4Box(ss.str().c_str(), thickness[resinNumber][atPos[i]]/2., sideLength/2., sideLength/2.); break;
        case 'Y': solidResinBlocks[i] = new G4Box(ss.str().c_str(), sideLength/2., thickness[resinNumber][atPos[i]]/2., sideLength/2.); break;
        case 'Z': solidResinBlocks[i] = new G4Box(ss.str().c_str(), sideLength/2., sideLength/2., thickness[resinNumber][atPos[i]]/2.); break;
        default: cout << fSourcePosition << ": Resin block orientation undefined!\n";
      }
      logicResinBlocks[i] = new G4LogicalVolume(solidResinBlocks[i], fResinMater_M16[resinNumber], ss.str().c_str(),0,0,0);
      physiResinBlocks[i] = new G4PVPlacement(0,
                                    G4ThreeVector(posX[i],posY[i],posZ[i]),
                                    logicResinBlocks[i],   // its logical volume                                  
                                    fSourcePosition, //ss.str().c_str(),       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    i,true);          // no particular field 
      //if(i != 4) logicResinBlocks[i]->SetVisAttributes(G4VisAttributes::Invisible);
    }
   
    // Spacer for M16.4 only
    G4Box * solidSpacer = new G4Box("M16.4_spacer", 16./2.*inch,3.5/2.*inch,1./2.*inch);
    G4LogicalVolume * logicSpacer = new G4LogicalVolume(solidSpacer, fResinMater_M16[resinNumber], "M16.4_spacer",0,0,0);
    G4VPhysicalVolume * physiSpcaer = new G4PVPlacement(0,
                                    G4ThreeVector(fInnerCavityX/2.-8.5*inch,0,-fInnerCavityZ/2.+1./2.*inch),
                                    logicSpacer,   // its logical volume                                  
                                    "M16.4_spacer", //ss.str().c_str(),       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    cout << "M16 resin block center:\n";
    cout << "M16 resin block dimensions:\n";
    for(int i = 1; i < numBlocks+1; i++){
      G4ThreeVector tv = physiResinBlocks[i]->GetTranslation();
      cout << "\"" << i << "\")\n" << "  srccenter='" << tv.getX()/mm << " " << tv.getY()/mm << " " << tv.getZ()/mm << "'\n";
      cout << "  srchalflength='" << solidResinBlocks[i]->GetXHalfLength()/mm << " " 
                       << solidResinBlocks[i]->GetYHalfLength()/mm << " " 
                       << solidResinBlocks[i]->GetZHalfLength()/mm << "'\n  ;;\n"; 
    }

  }else if(split(fSourcePosition, '_' )[0] == "WippSalt"){
    // WIPP salt samples and wipe samples
    // Salt: 1-5, Wipe: 11-16 (corr. to Wipe #1 to #6)

    int sampleNumber;// = atoi(split(fSourcePosition,'.')[1]);
    istringstream iss(split(fSourcePosition,'_')[1]);
    iss >> sampleNumber;

    double sampleX[17] = {0, 1.0*cm, 1.0*cm, 1.5*cm, 1.18*cm, 0.4*cm, 0,0,0,0,0,
                             0.17*cm, 0.17*cm, 0.17*cm, 0.17*cm, 0.17*cm, 0.17*cm,
                             }; // 11-15 are phony 
    double sampleY[17] = {0, 7.5*cm, 7.2*cm, 9.0*cm, 7.09*cm, 11.*cm, 0,0,0,0,0,
                             6.963*cm, 6.963*cm, 6.963*cm, 6.963*cm, 6.963*cm, 6.963*cm, 
                             }; // 11-15 are phony 
    double sampleZ[17] = {0, 7.0*cm, 11.*cm, 7.0*cm, 4.18*cm, 7.4*cm, 0,0,0,0,0,
                             6.847*cm, 6.847*cm, 6.847*cm, 6.847*cm, 6.847*cm, 6.847*cm, 
                             }; // 11-15 are phony 

    G4ThreeVector wippSaltTranslation(endCapFace + sampleX[sampleNumber]/2.,0,0);

    G4Box * solidWippSalt = new G4Box("wippSalt_"+split(fSourcePosition,'_')[1],sampleX[sampleNumber]/2.,sampleY[sampleNumber]/2.,sampleZ[sampleNumber]/2.);
    G4LogicalVolume * logicWippSalt = new G4LogicalVolume(solidWippSalt,fWippSaltMater[sampleNumber],"WippSalt_"+split(fSourcePosition,'_')[1],0,0,0);
    G4VPhysicalVolume * physiWippSalt = new G4PVPlacement(0,//buttonRotation,         
                                    wippSaltTranslation,
                                    logicWippSalt,   // its logical volume                                  
                                    "WippSalt_"+split(fSourcePosition,'_')[1],       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4cout << "srccenter='" 
           << wippSaltTranslation.getX() / mm << " "
           << wippSaltTranslation.getY() / mm << " "
           << wippSaltTranslation.getZ() / mm << "' #mm " << G4endl;
    G4cout << "srchalflength='" 
             << solidWippSalt->GetXHalfLength() / mm << " "
             << solidWippSalt->GetYHalfLength() / mm << " " 
             << solidWippSalt->GetZHalfLength() / mm << "' #mm\n";
  }else if(split(fSourcePosition, '_' )[0] == "WippSalt5Detailed"){
    // Slightly more detailed representation of Wipp Salt sample #5.
    // Accounts for attenuation in ziploc bags. 
    // Two versions:
    //   A: All but two ziploc layers are in between salt/soot mixture and the detector.
    //   B: All but two ziploc layers are behind the salt/soot mixture.
    // Both have 2 outermost layers
    // Graphically: 
    //  A: (Det)  [[ |||||||||| XXXXX ]]
    //  B: (Det)  [[ XXXXX ||||||||||]]  
    //  where X is salt/soot,   '[', ']', '|' are ziploc layers.

    string version = split(fSourcePosition,'_')[1];

    double ziploc1Thickness = 3.*mil; //innermost
    double ziploc2Thickness = 3.*mil;
    double ziploc3Thickness = 1.5*mil; //outermost
    
    //double ziplocX = 0.4*cm.;
    double ziplocX = 1.706*mm;
    double ziplocY = 11*cm;
    double ziplocZ = 7.4*cm;
  
    double saltX = ziplocX - 2.*ziploc1Thickness - 12.*ziploc2Thickness - 2.*ziploc3Thickness;
    double saltY = ziplocY                       - 2.*ziploc2Thickness - 2.*ziploc3Thickness;
    double saltZ = ziplocZ                       - 2.*ziploc2Thickness - 2.*ziploc3Thickness;

    G4cout << "Volume of ziploc = " << ziplocX * ziplocY *ziplocZ  /cm3 << " cm3\n";
    G4cout << "Volume of salt = " << saltX * saltY *saltZ  /cm3 << " cm3\n";

    G4ThreeVector ziplocTranslation(endCapFace + ziplocX/2.,0.,0.);
    G4ThreeVector saltTranslation((version == "A" ? 1 : -1)*(5.*ziploc2Thickness + ziploc1Thickness), 0., 0.);
    
    G4Box * solidZiploc = new G4Box("ziploc",ziplocX/2.,ziplocY/2.,ziplocZ/2.);
    G4LogicalVolume * logicZiploc = new G4LogicalVolume(solidZiploc,fZiplocMater,"Ziploc",0,0,0);
    G4VPhysicalVolume * physiZiploc = new G4PVPlacement(0,//buttonRotation,         
                                    ziplocTranslation,
                                    logicZiploc,   // its logical volume                                  
                                    "Ziploc",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    G4cout << "Mass of Ziploc = " << logicZiploc->GetMass() / g << " g" << G4endl;

    G4Box * solidSalt = new G4Box("salt",saltX/2.,saltY/2.,saltZ/2.);
    G4LogicalVolume * logicSalt = new G4LogicalVolume(solidSalt,fWippSalt5DetailedMater,"Salt",0,0,0);
    G4VPhysicalVolume * physiSalt = new G4PVPlacement(0,//buttonRotation,         
                                    saltTranslation,
                                    logicSalt,   // its logical volume                                  
                                    version == "A" ? "Salt_A" : "Salt_B",       // its name
                                    logicZiploc,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4cout << "srccenter='" 
           << (ziplocTranslation.getX() + saltTranslation.getX()) / mm << " "
           << (ziplocTranslation.getY() + saltTranslation.getY()) / mm << " "
           << (ziplocTranslation.getZ() + saltTranslation.getZ()) / mm << "' #mm " << G4endl;
    G4cout << "srchalflength='" 
             << solidSalt->GetXHalfLength() / mm << " "
             << solidSalt->GetYHalfLength() / mm << " " 
             << solidSalt->GetZHalfLength() / mm << "' #mm\n";

  }else if(fSourcePosition == "R-028"){
    // R-028: Silver Cable (Ge3)
    double diameter = 150.23*mm;
    double height = 17.54*mm;
    G4Tubs * solidR028 = new G4Tubs("R-028",0.,diameter/2.,height/2.,0.,twopi);
    G4LogicalVolume * logicR028 = new G4LogicalVolume(solidR028,fR028Mater,"R-028",0,0,0);

    G4RotationMatrix * r028Rotation = new G4RotationMatrix(); 
    r028Rotation->rotateY(90.*deg);

    G4ThreeVector r028Translation(endCapFace + height/2.,0.,0.);

    G4VPhysicalVolume * physiR028 = new G4PVPlacement(r028Rotation,//buttonRotation,         
                                    r028Translation,
                                    logicR028,   // its logical volume                                  
                                    "R-028",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4cout << "srccenter='"
           << (r028Translation.getX() ) / mm << " "
           << (r028Translation.getY() ) / mm << " "
           << (r028Translation.getZ() ) / mm << "' #mm " << G4endl;
    G4cout << "srchalflength='" 
             << solidR028->GetZHalfLength() / mm << " "
             << solidR028->GetOuterRadius() / mm << " " 
             << solidR028->GetOuterRadius() / mm << "' #mm\n";
    G4cout << " Mass = " << logicR028->GetMass() / g << " g\n";
    
  }else if(fSourcePosition == "R-029"){
    // R-029: Twisted ethernet cable (Ge2)
    double x = 45.5*mm;
    double y = 133.4*mm;
    double z = 85.4*mm;
    G4Box * solidR029 = new G4Box("R-029",x/2.,y/2.,z/2.);
    G4LogicalVolume * logicR029 = new G4LogicalVolume(solidR029,fR029Mater,"R-029",0,0,0);

    G4ThreeVector r029Translation(endCapFace + x/2.,0.,0.);

    G4VPhysicalVolume * physiR029 = new G4PVPlacement(0,
                                    r029Translation,
                                    logicR029,   // its logical volume                                  
                                    "R-029",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4cout << "srccenter='"
           << (r029Translation.getX() ) / mm << " "
           << (r029Translation.getY() ) / mm << " "
           << (r029Translation.getZ() ) / mm << "' #mm " << G4endl;
    G4cout << "srchalflength='" 
             << solidR029->GetXHalfLength() / mm << " "
             << solidR029->GetYHalfLength() / mm << " " 
             << solidR029->GetZHalfLength() / mm << "' #mm\n";

  }else if(split(fSourcePosition, '_' )[0] == "R-030"){
    // IHEP acrylic samples
    // Sample A: 1, Ge2: 4 (with hole) around detector, 2 in front, 2 above, 1 below.
    // Sample B: 2, Ge3: 4 (with hole) around detector, 2 in front, 1 above, 2 below.
    // Numbering: 
    //   0-3: with hole, increasing X
    //   4-5: in front, increasing X
    //   6-8: bottom-up (increasing Z)

    int sampleNumber;// = atoi(split(fSourcePosition,'.')[1]);
    istringstream iss(split(fSourcePosition,'_')[1]);
    iss >> sampleNumber;

    G4double length = 6*inch;
    G4double thickness = 1*inch;
    G4double hole = 4*inch; //diameter

    G4Box * solidBlock = new G4Box("Block",length/2.,length/2.,thickness/2.);
    G4Tubs * solidHole = new G4Tubs("Hole",0.,hole/2.,thickness/2.,0.,twopi);
    G4SubtractionSolid * solidBlockWithHole = new G4SubtractionSolid("BlockWithHole",solidBlock,solidHole);

    G4LogicalVolume * logicBlock = new G4LogicalVolume(solidBlock,fR030Mater,"R-030_Block",0,0,0);
    G4LogicalVolume * logicBlockWithHole = new G4LogicalVolume(solidBlockWithHole,fR030Mater,"R-030_BlockWithHole",0,0,0);

    G4RotationMatrix * frontRotation = new G4RotationMatrix(); 
    frontRotation->rotateY(90.*deg);
    
    int count = 0;
    // 4 blocks with holes
    for(int i = 0; i < 4; i++){
      G4ThreeVector blockTranslation(endCapFace-thickness*(0.5+3-i),0.,0.);
      G4VPhysicalVolume * physiR030_through = new G4PVPlacement(frontRotation,
                                    blockTranslation,
                                    logicBlockWithHole,   // its logical volume                                  
                                    "R-030",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    count,true);          // no particular field 
      count++;
    }
    // Two blocks in front
    for(int i = 0; i < 2; i++){
      G4ThreeVector frontBlockTranslation(endCapFace+thickness*(0.5+i),0.,0.);
      G4VPhysicalVolume * physiR030_front = new G4PVPlacement(frontRotation,
                                    frontBlockTranslation,
                                    logicBlock,   // its logical volume                                  
                                    "R-030",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    count,true);          // no particular field 
      count++;
    }

    // One/Two block(s) below
    for(int i = 0; i < (sampleNumber == 1 ? 1 : 2); i++){
      G4ThreeVector underBlockTranslation(endCapFace+thickness*2-length/2.,0.,-length/2.-thickness*(0.5+i));
      G4VPhysicalVolume * physiR030_under = new G4PVPlacement(0,
                                    underBlockTranslation,
                                    logicBlock,   // its logical volume                                  
                                    "R-030",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    count,true);          // no particular field 
      count++;
    }
    
    // Two/One block(s) above
    for(int i = 0; i < (sampleNumber == 1 ? 2 : 1); i++){
      G4ThreeVector aboveBlockTranslation(endCapFace+thickness*2-length/2.,0.,length/2.+thickness*(0.5+i));
      G4VPhysicalVolume * physiR030_above = new G4PVPlacement(0,
                                    aboveBlockTranslation,
                                    logicBlock,   // its logical volume                                  
                                    "R-030",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    count,true);          // no particular field 
      count++;
    }


  }else if(split(fSourcePosition, '_' )[0] == "WmCu"){

    double x = 2.*inch;
    double y = 4.*inch;
    double z = 4.*inch;

    G4Box * solidBlock = new G4Box("Block",x/2.,y/2.,z/2.);
    G4LogicalVolume * logicBlock = new G4LogicalVolume(solidBlock,fInnerShieldingMater,"WmCu",0,0,0);
    
    G4ThreeVector blockTranslation(endCapFace+x/2.,0.,0.);
    G4VPhysicalVolume * physiBlock = new G4PVPlacement(0,
                                    blockTranslation,
                                    logicBlock,   // its logical volume                                  
                                    "WmCu",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    PrintVolumeInfo(logicBlock);
  }else if(split(fSourcePosition, '_' )[0] == "WmCu2"){
    // Sample B: 2, Ge3: 4 (with hole) around detector, 2 in front, 1 above, 2 below.
    //   0-3: with hole, increasing X
    //   4-5: in front, increasing X
    //   6-8: bottom-up (increasing Z)

    int sampleNumber = 2;// = atoi(split(fSourcePosition,'.')[1]);
    //istringstream iss(split(fSourcePosition,'_')[1]);
    //iss >> sampleNumber;

    G4double length = 6*inch;
    G4double thickness = 1*inch;
    G4double hole = 4*inch; //diameter

    G4Box * solidBlock = new G4Box("Block",length/2.,length/2.,thickness/2.);
    G4Tubs * solidHole = new G4Tubs("Hole",0.,hole/2.,thickness/2.,0.,twopi);
    G4SubtractionSolid * solidBlockWithHole = new G4SubtractionSolid("BlockWithHole",solidBlock,solidHole);

    G4LogicalVolume * logicBlock = new G4LogicalVolume(solidBlock,fInnerShieldingMater,"WmCu2_Block",0,0,0);
    G4LogicalVolume * logicBlockWithHole = new G4LogicalVolume(solidBlockWithHole,fInnerShieldingMater,"WmCu2_BlockWithHole",0,0,0);

    PrintVolumeInfo(logicBlock);  // 5 pcs
    PrintVolumeInfo(logicBlockWithHole); // 4 pcs

    G4RotationMatrix * frontRotation = new G4RotationMatrix(); 
    frontRotation->rotateY(90.*deg);
    
    int count = 0;
    // 4 blocks with holes
    for(int i = 0; i < 4; i++){
      G4ThreeVector blockTranslation(endCapFace-thickness*(0.5+3-i),0.,0.);
      G4VPhysicalVolume * physiR030_through = new G4PVPlacement(frontRotation,
                                    blockTranslation,
                                    logicBlockWithHole,   // its logical volume                                  
                                    "WmCu2",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    count,true);          // no particular field 
      count++;
    }
    // Two blocks in front
    for(int i = 0; i < 2; i++){
      G4ThreeVector frontBlockTranslation(endCapFace+thickness*(0.5+i),0.,0.);
      G4VPhysicalVolume * physiR030_front = new G4PVPlacement(frontRotation,
                                    frontBlockTranslation,
                                    logicBlock,   // its logical volume                                  
                                    "WmCu2",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    count,true);          // no particular field 
      count++;
    }

    // One/Two block(s) below
    for(int i = 0; i < (sampleNumber == 1 ? 1 : 2); i++){
      G4ThreeVector underBlockTranslation(endCapFace+thickness*2-length/2.,0.,-length/2.-thickness*(0.5+i));
      G4VPhysicalVolume * physiR030_under = new G4PVPlacement(0,
                                    underBlockTranslation,
                                    logicBlock,   // its logical volume                                  
                                    "WmCu2",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    count,true);          // no particular field 
      count++;
    }
    
    // Two/One block(s) above
    for(int i = 0; i < (sampleNumber == 1 ? 2 : 1); i++){
      G4ThreeVector aboveBlockTranslation(endCapFace+thickness*2-length/2.,0.,length/2.+thickness*(0.5+i));
      G4VPhysicalVolume * physiR030_above = new G4PVPlacement(0,
                                    aboveBlockTranslation,
                                    logicBlock,   // its logical volume                                  
                                    "WmCu2",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    count,true);          // no particular field 
      count++;
    }
  }else if(split(fSourcePosition, '_' )[0] == "CalibSilica"){
    G4double silicaDiameter = 16.*mm;
    G4double silicaHeight = 15.2*mm;
    G4double bottleThickness = 2.*mm;
    G4double bottleDiameter = silicaDiameter + 2*bottleThickness;
    G4double bottleHeight = silicaHeight + 2*bottleThickness;
    G4double bottleZPosition = 132.*mm; // bottom of bottle from chamber ground

    G4ThreeVector calibBottleTranslation(endCapFace + bottleDiameter/2., 0., -fInnerCavityZ/2. + bottleZPosition + bottleHeight/2.);
    // Standard teflon bottles for cross-institute comparison
    G4Tubs * fSolidCalibBottle = new G4Tubs("calibBottle",0.,bottleDiameter/2.,bottleHeight/2.,0.,twopi);
    G4LogicalVolume * fLogicCalibBottle = new G4LogicalVolume(fSolidCalibBottle ,fCalibBottleMater,"CalibBottle",0,0,0);
    G4VPhysicalVolume * fPhysiCalibBottle = new G4PVPlacement(0,//calibBottleRotation,         
                                    calibBottleTranslation,
                                    fLogicCalibBottle,   // its logical volume                                  
                                    "CalibBottle",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4Tubs * fSolidCalibSilica = new G4Tubs("calibSilica",0.,silicaDiameter/2.,silicaHeight/2.,0.,twopi);
    G4LogicalVolume * fLogicCalibSilica = new G4LogicalVolume(fSolidCalibSilica ,fCalibSilicaMater,"CalibSilica",0,0,0);
    G4VPhysicalVolume * fPhysiCalibSilica = new G4PVPlacement(0,//calibSilicaRotation,         
                                    G4ThreeVector(0,0,0),
                                    fLogicCalibSilica,   // its logical volume                                  
                                    "CalibSilica",       // its name
                                    fLogicCalibBottle,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    fLogicCalibBottle->SetVisAttributes(GreyVisAtt);
    fLogicCalibSilica->SetVisAttributes(MagentaVisAtt);
    
  }else if(split(fSourcePosition, '_' )[0] == "Front2"){
    // KamLAND source capsule (bottom surface) at 123 mm away from Ge2 front face
    // Capsule aligns with Ge crystal axis of symmetry.
    double distance = 123.*mm;
    double capsuleHeight = 22.*mm;
    double capsuleDiameter = 6.*mm;
    double cavityDiameter = 4.*mm;
    double cavityHeight = 16.*mm;
    double cavityOffset = -1.5*mm;

    G4RotationMatrix * capsuleRotation = new G4RotationMatrix();
    capsuleRotation->rotateY(-90.*deg);

    G4Tubs * solidCapsule = new G4Tubs("capsule",0.,capsuleDiameter/2.,capsuleHeight/2.,0.,twopi);
    G4LogicalVolume * logicCapsule = new G4LogicalVolume(solidCapsule , fMaterialsManager->GetMaterial("SS304L"),"Capsule",0,0,0);
    G4VPhysicalVolume * physiCapsule = new G4PVPlacement(capsuleRotation,
                                    G4ThreeVector(distance+capsuleHeight/2.,0,0),
                                    logicCapsule,   // its logical volume                                  
                                    "Capsule",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4Tubs * solidCavity = new G4Tubs("cavity",0.,cavityDiameter/2.,cavityHeight/2.,0.,twopi);
    G4LogicalVolume * logicCavity = new G4LogicalVolume(solidCavity ,  fMaterialsManager->GetMaterial("Air"),"Cavity",0,0,0);
    G4VPhysicalVolume * physiCavity = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,cavityOffset),
                                    logicCavity,   // its logical volume                                  
                                    "Cavity",       // its name
                                    logicCapsule,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

  }else if(split(fSourcePosition,'_')[0] == "Interlab2020"){
    // Interlab comparison 2020
    // SNOLAB sources: 19-004-[UTK][12]

    // Dimension measured at UA (Not used in sim. Instead, used SNO specs.)
    // #	D	H	L
    // U1	17	62	14
    // T1	17	62	23
    // K1	17	62	5
    // U2	17	62	14
    // T2	17	62	23
    // K2	17	62	5

    // Materials
    auto powderMaterial = fMaterialsManager->GetMaterial("LooseSilica");
    auto bottleMaterial = fMaterialsManager->GetMaterial("Polypropylene");
    auto cavityMaterial = fMaterialsManager->GetMaterial("Air");

    // Powder heights
    // SNO Specs: U: 13, Th: 21, K: 4
    vector<string> sourceConfig = split(fSourcePosition,'_');
    double powderHeight;
    if(sourceConfig[1] == "U"){
      powderHeight = 13.*mm;
    }else if(sourceConfig[1] == "Th"){
      powderHeight = 21.*mm;
    }else if(sourceConfig[1] == "K"){
      powderHeight = 4.*mm;
    } 

    // Dimensions (Specs from SNO)
    double bottleHeight = 60 *mm;  
    double bottleDiameter = 15 *mm;
    double bottleThickness = 0.5*mm;
    double cavityHeight = bottleHeight - 2*bottleThickness;
    double cavityDiameter = bottleDiameter - 2*bottleThickness;
    double powderDiameter = cavityDiameter;
    
    G4cout << "Endcap Face: " << endCapFace / mm << " mm\n";

    auto solidBottle = new G4Tubs("bottle",0.,bottleDiameter/2.,bottleHeight/2.,0.,twopi);
    auto logicBottle = new G4LogicalVolume(solidBottle, bottleMaterial,"Bottle",0,0,0);
    auto physiBottle = new G4PVPlacement(0,
                                    G4ThreeVector(endCapFace + bottleDiameter/2.,0,fEndCapRadius - bottleHeight/2.),
                                    logicBottle,   // its logical volume                                  
                                    "Bottle",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    auto visAttBottle = new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
    logicBottle->SetVisAttributes(visAttBottle);

    auto solidCavity = new G4Tubs("cavity",0.,cavityDiameter/2.,cavityHeight/2.,0.,twopi);
    auto logicCavity = new G4LogicalVolume(solidCavity, cavityMaterial,"Cavity",0,0,0);
    auto physiCavity = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,0),
                                    logicCavity,   // its logical volume                                  
                                    "Cavity",       // its name
                                    //"Powder",       // TEMPORARY XXX
                                    logicBottle,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    auto visAttCavity = new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.0));
    logicCavity->SetVisAttributes(visAttCavity);

    auto solidPowder = new G4Tubs("powder",0.,powderDiameter/2.,powderHeight/2.,0.,twopi);
    auto logicPowder = new G4LogicalVolume(solidPowder, powderMaterial,"Powder",0,0,0);
    auto physiPowder = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,(powderHeight-cavityHeight)/2.),
                                    logicPowder,   // its logical volume                                  
                                    "Powder",       // its name
                                    logicCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    auto visAttPowder = new G4VisAttributes(G4Colour(0.6,0.6,0.7));
    logicPowder->SetVisAttributes(visAttPowder);
    
  }else if(split(fSourcePosition,'_')[0] == "NaaVial"){

    // Materials
    vector<string> sourceConfig = split(fSourcePosition,'_');
    string powderMaterialName = sourceConfig[1]; 
    //auto powderMaterial = nistMan->FindOrBuildMaterial("G4_KAPTON"); // XXX make it changeable
    G4Material* powderMaterial;

    if(powderMaterialName == "Kapton"){
      powderMaterial = nistMan->FindOrBuildMaterial("G4_KAPTON");
    }else if (powderMaterialName == "Si"){
      powderMaterial = nistMan->FindOrBuildMaterial("G4_Si"); 
    }else if (powderMaterialName == "NitricAcid"){
      powderMaterial = fMaterialsManager->GetMaterial("NitricAcid");
    }else{
      powderMaterial = fMaterialsManager->GetMaterial("Air");
    }

    auto bottleMaterial = fMaterialsManager->GetMaterial("Polypropylene");
    auto cavityMaterial = fMaterialsManager->GetMaterial("Air");

    // Dimensions
    double bottleHeight    = 60 * mm;  
    double bottleDiameter  = 15 * mm;
    double bottleThickness =  0.5*mm;
    double powderHeight    = bottleHeight;
    double powderLevel     = 10 * mm;
    double powderDiameter  = bottleDiameter - 2*bottleThickness;
    double cavityHeight    = bottleHeight;
    double cavityDiameter  = powderDiameter;

    // Location
    string vialLocation = sourceConfig[2]; 
    auto bottleRotation = new G4RotationMatrix();
    
    G4ThreeVector bottlePosition(0,0,0);//(endCapFace + fEndCapRadius, 0,-fInnerCavityZ/2. + totalHeight/2. + standHeight);
    G4ThreeVector bottle2Position(0,0,0);
    G4ThreeVector bottle3Position(0,0,0);
    if(vialLocation == "remote"){
      bottleRotation->rotateY(-90.*deg);
      bottlePosition.setX(endCapFace - bottleHeight/2.);
      bottlePosition.setY(-fInnerCavityY/2. + bottleDiameter/2. );
      bottlePosition.setZ(-fInnerCavityZ/2. + bottleDiameter/2.);
    }else if (vialLocation == "endcapCenterCombo"){
      bottleRotation->rotateZ(0.*deg);
      bottlePosition.setX(endCapFace + bottleDiameter/2.);
      bottlePosition.setY(0);
      bottlePosition.setZ(0);

      bottle2Position.setX(endCapFace + bottleDiameter/2.);
      bottle2Position.setY(bottleDiameter);
      bottle2Position.setZ(0);

      bottle3Position.setX(endCapFace + bottleDiameter/2.);
      bottle3Position.setY(-bottleDiameter);
      bottle3Position.setZ(0);
    }else{
      bottleRotation->rotateY(-90.*deg);
      bottlePosition.setX(endCapFace - bottleHeight/2.);
      bottlePosition.setY(0);
      bottlePosition.setZ(fEndCapRadius + bottleDiameter/2.);
    }
    G4cout << "Bottle center: " << bottlePosition << G4endl;

    auto solidBottle = new G4Tubs("bottle",0.,bottleDiameter/2.,bottleHeight/2.,0.,twopi);
    auto logicBottle = new G4LogicalVolume(solidBottle, bottleMaterial,"Bottle",0,0,0);
    auto physiBottle = new G4PVPlacement(bottleRotation,
                                    //G4ThreeVector(endCapFace + bottleDiameter/2.,0,fEndCapRadius - bottleHeight/2.),
                                    bottlePosition,
                                    logicBottle,   // its logical volume                                  
                                    "Bottle",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    if (vialLocation == "endcapCenterCombo"){
      auto physiBottle2 = new G4PVPlacement(bottleRotation,
                                    //G4ThreeVector(endCapFace + bottleDiameter/2.,0,fEndCapRadius - bottleHeight/2.),
                                    bottle2Position,
                                    logicBottle,   // its logical volume                                  
                                    "Bottle2",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);
      auto physiBottle3 = new G4PVPlacement(bottleRotation,
                                    //G4ThreeVector(endCapFace + bottleDiameter/2.,0,fEndCapRadius - bottleHeight/2.),
                                    bottle3Position,
                                    logicBottle,   // its logical volume                                  
                                    "Bottle3",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);  
    }
    auto visAttBottle = new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.5));
    logicBottle->SetVisAttributes(visAttBottle);

    auto solidCavity = new G4Tubs("cavity",0.,cavityDiameter/2.,cavityHeight/2.,0.,twopi);
    auto logicCavity = new G4LogicalVolume(solidCavity, cavityMaterial,"Cavity",0,0,0);
    auto physiCavity = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,0),
                                    logicCavity,   // its logical volume                                  
                                    "Cavity",       // its name
                                    //"Powder",       // TEMPORARY XXX
                                    logicBottle,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    auto visAttCavity = new G4VisAttributes(G4Colour(1.0,0.0,1.0,0.75));
    logicCavity->SetVisAttributes(visAttCavity);

    auto solidFilling = new G4Box("filling",cavityDiameter/2.,cavityDiameter/2.,cavityHeight/2.);
    auto solidPowder = new G4IntersectionSolid("powder", solidCavity, solidFilling,0,G4ThreeVector(0,0,0)); //G4ThreeVector(cavityDiameter-(powderLevel-bottleThickness),0,0
    //auto solidPowder = new G4Tubs("powder",0.,powderDiameter/2.,powderHeight/2.,0.,twopi);
    auto logicPowder = new G4LogicalVolume(solidPowder, powderMaterial,"Powder",0,0,0);
    auto physiPowder = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,(powderHeight-cavityHeight)/2.),
                                    logicPowder,   // its logical volume                                  
                                    "Powder",       // its name
                                    logicCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    auto visAttPowder = new G4VisAttributes(G4Colour(1.0,1.0,0.0,1.0));
    logicPowder->SetVisAttributes(visAttPowder);

  }else if(split(fSourcePosition,'_')[0] == "StanfordGasBottle"){
    // Gas bottle for Xe activation by Brian Lenardo at Stanford.
    // http://130.160.10.13/Laboratory_log/296
    // Extra information from Brian Lenardo:
    //   One side is plugged, on side has the 1/4" tubing coming out. I'll get the length of that in just a second
    //   About 2.9" of tubing (the NPT-to-VCR adapter plus half of the valve length)
    //   ID of that volume is 0.18"
    //
    // Assumed to be a cylinder tube capped by two hemipsheres. 
    
    G4double BottleTh = 0.090 * inch;
    G4double BodyID = 1.820 * inch;
    G4double BodyL = 2.617 * inch;
    G4double CapID = 2* 0.910 * inch;  
    G4double EndOD = 0.800 * inch;  
    G4double EndL = 0.400 * inch;  
    G4double TubeOD = 0.25 * inch;
    G4double TubeID = 0.18 * inch;
    G4double TubeL = 2.9 * inch;

    G4double EndID = TubeID; // my assumption because end thickness is not known.
    G4double BodyOD = BodyID + 2.*BottleTh;    
    G4double CapOD = CapID + 2.*BottleTh;  
    G4double BottleL = BodyL + CapOD + 2* EndL;
    G4double BottleWOEndsL = BodyL + CapID; // For GXe

    auto bottleMaterial = fMaterialsManager->GetMaterial("SS316L");
    auto gasMaterial = fMaterialsManager->GetMaterial("GXe");
    auto blockMaterial = fMaterialsManager->GetMaterial("Copper");

    // Bottle
    auto solidBody = new G4Tubs("Body", 0., BodyOD/2. + 0.01*mm, BodyL/2., 0., twopi); // Added a tiny length for visualization.
    auto solidTopCap = new G4Sphere("TopCap", 0, CapOD/2., 0, twopi, 0, pi/2.);
    auto solidBottomCap = new G4Sphere("BottomCap", 0, CapOD/2., 0, twopi, pi/2., pi/2.);
    auto solidEnds = new G4Tubs("Ends", 0, EndOD/2., BottleL/2., 0., twopi);
    auto solidTube = new G4Tubs("Tube", 0, TubeOD/2., (BottleL + TubeL)/2., 0., twopi);

    auto bottleRotation = new G4RotationMatrix();
    bottleRotation->rotateY(-90.*deg);

    auto solidBottle_step1 = new G4UnionSolid("Bottle_step1",solidBody,solidBottomCap,0,G4ThreeVector(0,0,-BodyL/2.));
    auto solidBottle_step2 = new G4UnionSolid("Bottle_step2",solidBottle_step1,solidTopCap,0,G4ThreeVector(0,0,BodyL/2.));
    auto solidBottle_step3 = new G4UnionSolid("Bottle_step3",solidBottle_step2,solidEnds,0,G4ThreeVector(0,0,0));
    auto solidBottle = new G4UnionSolid("Bottle",solidBottle_step3,solidTube,0,G4ThreeVector(0,0,TubeL/2.));
    auto logicBottle = new G4LogicalVolume(solidBottle ,bottleMaterial,"Bottle",0,0,0);
    auto physiBottle = new G4PVPlacement(bottleRotation,
                                    G4ThreeVector(endCapFace - CapOD/2. - BodyL/2.,0, -fEndCapRadius - BodyOD/2. - 0.01*mm), // Added a tiny length to avoid overlap
                                    logicBottle,   // its logical volume                                  
                                    "Bottle",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    auto visAttBottle = new G4VisAttributes(G4Colour(0.7,0.7,0.7,0.5));
    logicBottle->SetVisAttributes(visAttBottle);
    G4cout << "Gas bottle center = " << G4ThreeVector(endCapFace - CapOD/2. - BodyL/2.,0, -fEndCapRadius - BodyOD/2.) << G4endl;
    G4cout << "Gas bottle length = " << BottleL + TubeL << G4endl;
    
    // GXe
    auto solidBodyGXe = new G4Tubs("BodyGXe", 0., BodyID/2. + 0.01*mm, BodyL/2., 0., twopi); // Added a tiny length for visualization.
    auto solidTopCapGXe = new G4Sphere("TopCapGXe", 0, CapID/2., 0, twopi, 0, pi/2.);
    auto solidBottomCapGXe = new G4Sphere("BottomCapGXe", 0, CapID/2., 0, twopi, pi/2., pi/2.);
    //auto solidEndsGXe = new G4Tubs("EndsGXe", 0, EndOD/2., BottleL/2., 0., twopi);
    auto solidTubeGXe = new G4Tubs("TubeGXe", 0, TubeID/2., (BottleWOEndsL + BottleTh + EndL + TubeL)/2., 0., twopi);
    
    auto solidGXe_step1 = new G4UnionSolid("GXe_step1",solidBodyGXe,solidBottomCapGXe,0,G4ThreeVector(0,0,-BodyL/2.));
    auto solidGXe_step2 = new G4UnionSolid("GXe_step2",solidGXe_step1,solidTopCapGXe,0,G4ThreeVector(0,0,BodyL/2.));
    auto solidGXe = new G4UnionSolid("GXe",solidGXe_step2,solidTubeGXe,0,G4ThreeVector(0,0,(BottleTh + EndL + TubeL)/2.));
    auto logicGXe = new G4LogicalVolume(solidGXe, gasMaterial,"GXe",0,0,0);
    auto physiGXe = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,0),
                                    logicGXe,   // its logical volume                                  
                                    "GXe",       // its name
                                    logicBottle,
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    auto visAttGXe = new G4VisAttributes(G4Colour(0.5,0.5,0.8,0.8));
    logicGXe->SetVisAttributes(visAttGXe);
    
    // Cu Block 1: bottom
    G4double CuBlockBottomX = 3.*inch;
    G4double CuBlockBottomY = BodyOD;
    G4double CuBlockBottomZ = 2.*inch;
    // Cu Block 2: sides
    G4double CuBlockSidesX = 2.*inch;
    G4double CuBlockSidesY = 1.*inch;
    G4double CuBlockSidesZ = 3.*inch;

    auto solidCuBlockBottom = new G4Box("CuBlockBottom",CuBlockBottomX/2., CuBlockBottomY/2., CuBlockBottomZ/2.);
    auto logicCuBlockBottom = new G4LogicalVolume(solidCuBlockBottom ,blockMaterial,"CuBlockBottom",0,0,0);
    auto physiCuBlockBottom = new G4PVPlacement(0,
                                    G4ThreeVector(endCapFace - CuBlockBottomX/2., 0, -fInnerCavityZ/2. + CuBlockBottomZ/2.),
                                    logicCuBlockBottom,   // its logical volume                                  
                                    "CuBlockBottom",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    auto visAttCuBlockBottom = new G4VisAttributes(G4Colour(0.7,0.7,0.7,0.5));
    logicCuBlockBottom->SetVisAttributes(G4Color(0.75,0.5,0));

    auto solidCuBlockSides = new G4Box("CuBlockSides",CuBlockSidesX/2., CuBlockSidesY/2., CuBlockSidesZ/2.);
    auto logicCuBlockSides = new G4LogicalVolume(solidCuBlockSides ,blockMaterial,"CuBlockSides",0,0,0);

    auto physiCuBlockSides1 = new G4PVPlacement(0,
                                    G4ThreeVector(endCapFace - CuBlockSidesX/2., -CuBlockBottomY/2. -CuBlockSidesY/2., -fInnerCavityZ/2. + CuBlockSidesZ/2.),
                                    logicCuBlockSides,   // its logical volume                                  
                                    "CuBlockSides",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    auto physiCuBlockSides2 = new G4PVPlacement(0,
                                    G4ThreeVector(endCapFace - CuBlockSidesX/2., CuBlockBottomY/2. + CuBlockSidesY/2., -fInnerCavityZ/2. + CuBlockSidesZ/2.),
                                    logicCuBlockSides,   // its logical volume                                  
                                    "CuBlockSides",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    auto visAttCuBlockSides = new G4VisAttributes(G4Colour(0.7,0.7,0.7,0.5));
    logicCuBlockSides->SetVisAttributes(G4Color(0.75,0.5,0));

  }
  else if ( split(fSourcePosition,'_')[0] == "AlDisk" ) {
    // Pb-210 solution on an Al disk
    // http://130.160.10.13/Pb-210/19
    // http://130.160.10.13/GeIII/234

    double diskDiameter    =  3    * inch;
    double diskThickness   =  0.085* inch;
    double spotDiameter    = 13    * mm; 
    double spotThickness   =  0.01 * mm;   // 10 microns
    double ziplocThickness =  8e-3 * inch; // four layers of ziploc bags of 2 mil each
    double ziplocSize      = 10    * cm;   // arbitrary
    auto diskMaterial      = fMaterialsManager->GetMaterial("Aluminum");

    // ========

    // Ziploc
    auto solidZiploc = new G4Box("Ziploc",ziplocThickness/2.,ziplocSize/2.,ziplocSize/2.);
    auto logicZiploc = new G4LogicalVolume(solidZiploc, fZiplocMater,"Ziploc",0,0,0);
    auto physiZiploc = new G4PVPlacement(0,
                                         G4ThreeVector(endCapFace + ziplocThickness/2, 0,0),
                                         logicZiploc,      // logical volume                                  
                                         "Ziploc",         // name
                                         fLogicInnerCavity,// mother  volume
                                         false,            // no boolean operations
                                         0,true);          // no particular field 
    auto visAttZiploc = new G4VisAttributes( G4Colour(0.7, 0.7, 0.9, 0.5) );
    logicZiploc->SetVisAttributes(visAttZiploc);

    // Air disk for Pb-210 generation
    auto spotRotation = new G4RotationMatrix();
    spotRotation->rotateY(90*deg); // top faces end cap
    G4ThreeVector spotLocation(endCapFace + ziplocThickness + spotThickness/2, 0,0);
    auto solidSpot = new G4Tubs("Spot", 0, spotDiameter/2, spotThickness/0, 0, twopi);
    auto logicSpot = new G4LogicalVolume(solidSpot, fDefaultMater, "Spot", 0,0,0);
    auto physiSpot = new G4PVPlacement(spotRotation,
                                       spotLocation,
                                       logicSpot,        // logical volume                                  
                                       "Spot",           // name
                                       fLogicInnerCavity,// mother  volume
                                       false,            // no boolean operations
                                       0,true);          // no particular field 
    auto visAttSpot = new G4VisAttributes( G4Colour(0.5, 0.5, 0.5, 0.2) );
    logicSpot->SetVisAttributes(visAttSpot);
    cout << "Spot location: " << spotLocation << endl;

    // Al disk
    auto diskRotation = new G4RotationMatrix();
    diskRotation->rotateY(90*deg); // top faces end cap
    auto solidDisk = new G4Tubs("Disk", 0, diskDiameter/2, diskThickness/2, 0, twopi);
    auto logicDisk = new G4LogicalVolume(solidDisk, diskMaterial, "Disk", 0,0,0);
    auto physiDisk = new G4PVPlacement(diskRotation,
                                       G4ThreeVector(endCapFace + ziplocThickness + spotThickness + diskThickness/2, 0,0),
                                       logicDisk,        // logical volume                                  
                                       "Disk",           // name
                                       fLogicInnerCavity,// mother  volume
                                       false,            // no boolean operations
                                       0,true);          // no particular field 
    auto visAttDisk = new G4VisAttributes( G4Colour(0.5, 0.5, 0.6, 0.5) );
    logicDisk->SetVisAttributes(visAttDisk);
  }    

  else if (fSourcePosition == "Ge4LZMarinelliBeaker" && fDetId == '4') {
    //cylinder
    double Diameter     = 150 * mm;
    double DiameterWell =  97 * mm;
    double Height       = 165 * mm;
    double HeightWell   = 100 * mm;

    G4Tubs * MarenilliBody   = new G4Tubs("MarenilliBody",   0, Diameter/2,     Height/2,     0, twopi);
    G4Tubs * MarenilliCavity = new G4Tubs("MarenilliCavity", 0, DiameterWell/2, HeightWell/2, 0, twopi);
    G4SubtractionSolid *fSolidMarenilli = new G4SubtractionSolid( "SolidMarenilli",
								  MarenilliBody,
								  MarenilliCavity,
								  0, G4ThreeVector(0,0,32.5) );
    logicLZMarenilli = new G4LogicalVolume(fSolidMarenilli, fSolderMater, "LZMarenilli", 0,0,0);
    G4RotationMatrix * Rotation = new G4RotationMatrix();
    Rotation->rotateY(90*deg);
    physiLZMarenilli = new G4PVPlacement( Rotation,
					  G4ThreeVector(Height/2 - HeightWell + endCapFace, 0,0),
					  logicLZMarenilli,  // logical volume              
					  "LZMarenilli",     // name
					  fLogicInnerCavity, // mother  volume
					  false,             // no boolean operations
					  0, true);          // no particular field 
    G4cout << "LZ Marenilli volume name...: " << physiLZMarenilli->GetName() << G4endl; 
    G4cout << "Sample material............: " << fSolderMater->GetName()     << G4endl;

    logicLZMarenilli->SetVisAttributes(SpringGreenVisAtt);
  }
  else if ( split(fSourcePosition,'_')[0] == "AirCuboid" ) {  // Wipes from swipe test added for Rafi, Mar 31, 2021
    double lengthX = 1.5 * inch;
    double lengthY = 3 * inch;
    double lengthZ = 3 * inch;
    G4ThreeVector position(endCapFace + lengthX/2., 0, 0);

    auto solidAirCuboid = new G4Box("AirCuboid",lengthX/2.,lengthY/2.,lengthZ/2.);
    auto logicAirCuboid = new G4LogicalVolume(solidAirCuboid, fMaterialsManager->GetMaterial("Air"), "AirCuboid",0,0,0);
    auto physiAirCuboid = new G4PVPlacement(0,
                                    position,
                                    logicAirCuboid,   // its logical volume                                  
                                    "AirCuboid",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    logicAirCuboid->SetVisAttributes(AzureVisAtt);
    
  }
  else if ( split(fSourcePosition,'_')[0] == "R-157" ) {  // Assembled PCBs 

    double Board1X = 146.5 *mm;
    double Board1Y = 2.0 *mm;
    double Board1Z = 181.5 *mm;
    double Board2X = 2.2 *mm;
    double Board2Y = 200.0 *mm;
    double Board2Z = 245.0 *mm;
    double Board1Xextent = 16.*mm;
    auto BoardMaterial = fMaterialsManager->GetMaterial("R-157");

    G4ThreeVector positionBoard1(endCapFace + 16.*mm - Board1X/2., -fEndCapRadius - Board1Y/2., -fInnerCavityZ/2. + Board1Z/2.);
    G4ThreeVector positionBoard2(endCapFace + 16.*mm + Board2X/2., 0, -fInnerCavityZ/2. + Board2Z/2.);
   
    auto solidBoard1 = new G4Box("Board1",Board1X/2.,Board1Y/2.,Board1Z/2.);
    auto logicBoard1 = new G4LogicalVolume(solidBoard1, BoardMaterial, "Board1",0,0,0);
    auto physiBoard1 = new G4PVPlacement(0,
                                    positionBoard1,
                                    logicBoard1,   // its logical volume                                  
                                    "Board1",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    auto solidBoard2 = new G4Box("Board2",Board2X/2.,Board2Y/2.,Board2Z/2.);
    auto logicBoard2 = new G4LogicalVolume(solidBoard2, BoardMaterial, "Board2",0,0,0);
    auto physiBoard2 = new G4PVPlacement(0,
                                    positionBoard2,
                                    logicBoard2,   // its logical volume                                  
                                    "Board2",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    logicBoard1->SetVisAttributes(GreenVisAtt);
    logicBoard2->SetVisAttributes(GreenVisAtt);
    
  }
  else if ( split(fSourcePosition,'-')[0] == "LZAmLiCapsule" ) {
    
    // these codes below are adopted from Jerry Busenitz's AmLi capsule geant4 simulation
    // here the outercapsule/middlecapsule will be the mother volume for the source which is
    // to be placed inside the inner cavity of the Ge detector
   

    G4double dsource=5.0*CLHEP::mm;
    G4double hsource=10.0*CLHEP::mm;
    G4double dinnercapsule=9.0*CLHEP::mm;
    G4double hinnercapsule=18.0*CLHEP::mm;
    G4double hinnerplug=6.0*CLHEP::mm;
    G4double tinnercapsule=2.0*CLHEP::mm;
    G4double dmiddlecapsule=13.0*CLHEP::mm;
    G4double hmiddlecapsule=29.0*CLHEP::mm;
    G4double tmiddlecapsule=2.0*CLHEP::mm;  //actual 1.9875 for sides, 1.975 for bottom
    G4double doutercapsule=16.0*CLHEP::mm; // actual 16.010
    G4double houtercapsule=42.5*CLHEP::mm; // actual 42.525
    G4double toutercapsule=1.5*CLHEP::mm; // actual 1.4925 for sides, 1.5 for bottom
    


  //The inner capsule is inserted plug first into middle capsule
  //Define zoffsets for capsule components
  //
    G4double zoffsetoutercapsule,zoffsetmiddlecapsule,zoffsetinnercapsule,zoffsetsource;
    G4double zoffsetoutercapsulew,zoffsetmiddlecapsulew,zoffsetinnercapsulew,zoffsetsourcew;
  //Offsets of capsule component relative to a reference point (in this case center of source volume)
    zoffsetoutercapsulew=-0.5*hsource-hinnerplug-tmiddlecapsule-toutercapsule+0.5*houtercapsule;
    zoffsetmiddlecapsulew=-0.5*hsource-hinnerplug-tmiddlecapsule+0.5*hmiddlecapsule;
    zoffsetinnercapsulew=-0.5*hsource-hinnerplug+0.5*hinnercapsule;
    zoffsetsourcew=endCapFace-0.5*hsource-hinnerplug-tmiddlecapsule-toutercapsule; //set this to position the source along the x-axis. 
  //Offset of outer capsule relative to world
    zoffsetoutercapsule=-zoffsetoutercapsulew+zoffsetsourcew;
  //Offset of middle capsule relative to outer capsule
    zoffsetmiddlecapsule=zoffsetmiddlecapsulew-zoffsetoutercapsulew;
  //Offset of inner capsule relative to middle capsule
    zoffsetinnercapsule=zoffsetinnercapsulew-zoffsetmiddlecapsulew;
    //zoffsetinnercapsule=-0.5*hmiddlecapsule + 0.5*hinnercapsule;
  //Offset of source relative to inner capsule
    //zoffsetsource=zoffsetsourcew-zoffsetinnercapsulew;
    zoffsetsource= 0.5*hsource + hinnerplug - 0.5*hinnercapsule ;
  
    G4cout<< "endcapface : " << endCapFace << G4endl; 
    auto rotation = new G4RotationMatrix();
    rotation->rotateY(90.*deg);
    G4ThreeVector position(zoffsetoutercapsule, -fInnerCavityY/2 + 0.5*doutercapsule ,-fInnerCavityZ/2 + 0.5*doutercapsule );
  
   //outer capsule
   
   G4Tubs *outercapsule = new G4Tubs("outercapsule",0.,0.5*doutercapsule,0.5*houtercapsule,0.,2.0*CLHEP::pi);
   auto outercapsule_log = new G4LogicalVolume(outercapsule,fMaterialsManager->GetMaterial("SS304L"),"outercapsule_log");
   auto outercapsule_phys=new G4PVPlacement(rotation,position,outercapsule_log,"outercapsule__phys",fLogicInnerCavity,false,0);

   auto visAttOuter = new G4VisAttributes(G4Colour(0.8,0.8,0.7,0.25));
   outercapsule_log->SetVisAttributes(visAttOuter); 
   
   //middle capsule
   G4Tubs *middlecapsule = new G4Tubs("middlecapsule",0.,0.5*dmiddlecapsule,0.5*hmiddlecapsule,0.,2.0*CLHEP::pi);
   G4LogicalVolume *middlecapsule_log = new G4LogicalVolume(middlecapsule,fMaterialsManager->GetMaterial("TungstenAlloy"),"middlecapsule_log");
   middlecapsule_phys=new G4PVPlacement(0,G4ThreeVector(0,0,zoffsetmiddlecapsule),middlecapsule_log,"middlecapsule__phys",outercapsule_log,false,0);
   
   auto visAttMiddle = new G4VisAttributes(G4Colour(0.7,0.7,0.9,0.5));
   middlecapsule_log->SetVisAttributes(visAttMiddle); 
   //inner capsule
   G4Tubs *innercapsule = new G4Tubs("innercapsule",0.,0.5*dinnercapsule,0.5*hinnercapsule,0.,2.0*CLHEP::pi);
   G4LogicalVolume *innercapsule_log = new G4LogicalVolume(innercapsule,fMaterialsManager->GetMaterial("TungstenAlloy"),"innercapsule_log");
   auto innercapsule_phys=new G4PVPlacement(0,G4ThreeVector(0,0,zoffsetinnercapsule),innercapsule_log,"innercapsule__phys",middlecapsule_log,false,0);
   
   auto visAttInner = new G4VisAttributes( G4Colour(0.5,0.5,0.5,0.75) );
   innercapsule_log->SetVisAttributes(visAttInner);
   
   //source
   G4Tubs *source = new G4Tubs("source",0.,0.5*dsource,0.5*hsource,0.,2.0*CLHEP::pi);
   G4LogicalVolume *source_log = new G4LogicalVolume(source,fMaterialsManager->GetMaterial("Gold-Lithium-Sandwich"),"source_log");
   auto source_phys=new G4PVPlacement(0,G4ThreeVector(0,0,zoffsetsource),source_log,"source__phys",innercapsule_log,false,0);
   

   auto visAttsrc = new G4VisAttributes( G4Colour(0.5,0.5,0.6,1.0) );
   source_log->SetVisAttributes( visAttsrc);

  }

  else if ( split(fSourcePosition,'-')[0] == "LZAmLiCapsule2" ) {
    
    // these codes below are adapted from Jerry Busenitz's AmLi capsule geant4 simulation
    // here the outercapsule/middlecapsule will be the mother volume for the source which is
    // to be placed inside the innercavity of the Ge detector
   

    G4double dsource=5.0*CLHEP::mm;
    G4double hsource=10.0*CLHEP::mm;
    G4double dinnercapsule=9.0*CLHEP::mm;
    G4double hinnercapsule=18.0*CLHEP::mm;
    G4double hinnerplug=6.0*CLHEP::mm;
    G4double tinnercapsule=2.0*CLHEP::mm;
    G4double dmiddlecapsule=13.0*CLHEP::mm;
    G4double hmiddlecapsule=29.0*CLHEP::mm;
    G4double tmiddlecapsule=2.0*CLHEP::mm;  //actual 1.9875 for sides, 1.975 for bottom
    G4double doutercapsule=16.0*CLHEP::mm; // actual 16.010
    G4double houtercapsule=42.5*CLHEP::mm; // actual 42.525
    G4double toutercapsule=1.5*CLHEP::mm; // actual 1.4925 for sides, 1.5 for bottom
    

 
    G4cout<< "endcapface : " << endCapFace << G4endl; 
    auto rotation = new G4RotationMatrix();
    rotation->rotateY(90.*deg);
    G4ThreeVector position(endCapFace, -fInnerCavityY/2 + 0.5*doutercapsule ,-fInnerCavityZ/2 + 0.5*doutercapsule );
    G4cout<< "Y coordinate : " << -fInnerCavityY/2 + 0.5*doutercapsule << G4endl;
    G4cout<< "Z coordinate : " << -fInnerCavityZ/2 + 0.5*doutercapsule << G4endl;
    
  
   //outer capsule
   G4int numZplanes1 = 6;
   G4double zPlanes1[6] = {0,toutercapsule,toutercapsule,toutercapsule+hmiddlecapsule,toutercapsule+hmiddlecapsule,houtercapsule};
   G4double rInner1[6] = {0,0,dmiddlecapsule/2.,dmiddlecapsule/2.,0,0};
   G4double rOuter1[6] = {doutercapsule/2,doutercapsule/2,doutercapsule/2,doutercapsule/2,doutercapsule/2,doutercapsule/2};
    
   auto *outercapsule = new G4Polycone("outercapsule",0.*deg, 360*deg,numZplanes1,zPlanes1,rInner1,rOuter1); 
   auto outercapsule_log = new G4LogicalVolume(outercapsule,fMaterialsManager->GetMaterial("SS304L"),"outercapsuleLV");
   auto outercapsule_phys=new G4PVPlacement(rotation,position,outercapsule_log,"outercapsulePV",fLogicInnerCavity,false,0,true);

   auto visAttOuter = new G4VisAttributes(G4Colour(0.8,0.8,0.7,0.5));
   outercapsule_log->SetVisAttributes(visAttOuter); 
   
   //middle and inner Tungsten capsule
   G4int numZplanes2 = 6;
   G4double zPlanes2[6] = {0,tmiddlecapsule+hinnerplug,tmiddlecapsule+hinnerplug,tmiddlecapsule+hinnerplug+hsource,tmiddlecapsule+hinnerplug+hsource,hmiddlecapsule};
   G4double rInner2[6] = {0,0,dsource/2.,dsource/2.,0,0};
   G4double rOuter2[6] = {dmiddlecapsule/2,dmiddlecapsule/2,dmiddlecapsule/2,dmiddlecapsule/2,dmiddlecapsule/2,dmiddlecapsule/2};
   auto *middlecapsule = new G4Polycone("middlecapsule",0.*deg, 360*deg,numZplanes2,zPlanes2,rInner2,rOuter2);
   G4LogicalVolume *middlecapsule_log = new G4LogicalVolume(middlecapsule,fMaterialsManager->GetMaterial("TungstenAlloy"),"middlecapsuleLV");
   middlecapsule_phys=new G4PVPlacement(0,G4ThreeVector(0,0,toutercapsule),middlecapsule_log,"middlecapsulePV",outercapsule_log,false,0,true);
   
   auto visAttMiddle = new G4VisAttributes(G4Colour(0.7,0.7,0.9,0.75));
   middlecapsule_log->SetVisAttributes(visAttMiddle); 
   
   
   //source
   G4Tubs *source = new G4Tubs("source",0.,0.5*dsource,0.5*hsource,0.,2.0*CLHEP::pi);
   G4LogicalVolume *source_log = new G4LogicalVolume(source,fMaterialsManager->GetMaterial("Gold-Lithium-Sandwich"),"sourceLV");
   source_phys=new G4PVPlacement(0,G4ThreeVector(0,0,tmiddlecapsule+hinnerplug+0.5*hsource),source_log,"sourcePV",middlecapsule_log,false,0,true);
   

   auto visAttsrc = new G4VisAttributes( G4Colour(0.5,0.5,0.6,1.0) );
   source_log->SetVisAttributes( visAttsrc);

   G4double srcCentre = endCapFace - toutercapsule - tmiddlecapsule -hinnerplug - 0.5*hsource;

   G4cout << "source center position: " << srcCentre << G4endl;

  }



  else if ( split(fSourcePosition,'-')[0] == "LZAmLiCapsule3" ) {
    
    // these codes below are adapted from Jerry Busenitz's AmLi capsule geant4 simulation
    // here the outercapsule/middlecapsule will be the mother volume for the source which is
    // to be placed inside the innercavity of the Ge detector
   

    G4double dsource=5.0*CLHEP::mm;
    G4double hsource=10.0*CLHEP::mm;

    G4double dinnercapsule=9.0*CLHEP::mm;
    G4double hinnercapsule=18.0*CLHEP::mm;
    G4double tinnercapsule=2.0*CLHEP::mm;
    G4double hinnerplug=6.0*CLHEP::mm;

    G4double dmiddlecapsule=13.0*CLHEP::mm;
    G4double hmiddlecapsule=29.0*CLHEP::mm;
    G4double hmiddleplug = 9.0*CLHEP::mm;
    G4double tmiddlecapsule=2.0*CLHEP::mm;  //actual 1.9875 for sides, 1.975 for bottom

    G4double doutercapsule=16.0*CLHEP::mm; // actual 16.010
    G4double houtercapsule=42.5*CLHEP::mm; // actual 42.525
    G4double toutercapsule=1.5*CLHEP::mm; // actual 1.4925 for sides, 1.5 for bottom
    G4double touterplug = 12.0*CLHEP::mm;
    
    G4double outerBase = toutercapsule;
    G4double outerRoof = touterplug;
    G4double middleInnerBase = tmiddlecapsule + hinnerplug;
    G4double middleInnerRoof = tinnercapsule + hmiddleplug;
    
    
    // we will implemented the capsule design in nested cylinders.
    //first the SS outer cylinder >> then tungsten middle-inner assembly together one single cylinder >> then the core of the source as a cylinder


    G4double middleInnerCenterOffset = (outerBase - outerRoof)/2.;
    G4double sourceCenterOffset = (middleInnerBase - middleInnerRoof)/2.;

    G4cout<< "endcapface : " << endCapFace << G4endl; 
    auto rotation = new G4RotationMatrix();
    rotation->rotateY(90.*deg);
    G4ThreeVector position(endCapFace-0.5*houtercapsule, -fInnerCavityY/2 + 0.5*doutercapsule ,-fInnerCavityZ/2 + 0.5*doutercapsule );
    G4cout << "Outer capsule X coordinate: " << endCapFace-0.5*houtercapsule << G4endl;
    G4cout<< "Source core/outer capsule center Y coordinate : " << -fInnerCavityY/2 + 0.5*doutercapsule << G4endl;
    G4cout<< "Source core/outer capsule center Z coordinate : " << -fInnerCavityZ/2 + 0.5*doutercapsule << G4endl;
    
  
   //outer capsule
    
   auto outercapsule = new G4Tubs("outercapsule",0.,0.5*doutercapsule,0.5*houtercapsule,0.,2.0*CLHEP::pi);
   auto outercapsule_log = new G4LogicalVolume(outercapsule,fMaterialsManager->GetMaterial("SS304L"),"outercapsuleLV");
   outercapsule_phys=new G4PVPlacement(rotation,position,outercapsule_log,"outercapsulePV",fLogicInnerCavity,false,0,true);

   auto visAttOuter = new G4VisAttributes(G4Colour(0.8,0.8,0.7,0.5));
   outercapsule_log->SetVisAttributes(visAttOuter); 
   
   //middle and inner Tungsten capsule implemented as one sigle cylinder
   
   auto middlecapsule = new G4Tubs("middlecapsule",0.,0.5*dmiddlecapsule,0.5*hmiddlecapsule,0.,2.0*CLHEP::pi);
   auto middlecapsule_log = new G4LogicalVolume(middlecapsule,fMaterialsManager->GetMaterial("TungstenAlloy"),"middlecapsuleLV");
   middlecapsule_phys=new G4PVPlacement(0,G4ThreeVector(0,0,middleInnerCenterOffset),middlecapsule_log,"middlecapsulePV",outercapsule_log,false,0,true);
   
   auto visAttMiddle = new G4VisAttributes(G4Colour(0.7,0.7,0.9,0.75));
   middlecapsule_log->SetVisAttributes(visAttMiddle); 
   
   
   //source
   auto source = new G4Tubs("source",0.,0.5*dsource,0.5*hsource,0.,2.0*CLHEP::pi);
   //auto source_log = new G4LogicalVolume(source,fMaterialsManager->GetMaterial("Gold-Lithium-Nitrate-Sandwich"),"sourceLV");
   auto source_log = new G4LogicalVolume(source,fMaterialsManager->GetMaterial("Gold-Lithium-Sandwich"),"sourceLV");
   source_phys=new G4PVPlacement(0,G4ThreeVector(0,0,sourceCenterOffset),source_log,"sourcePV",middlecapsule_log,false,0,true);
   
   float layerDia = 0.5*dsource;
   float layerThickness = 1.e-5*CLHEP::cm;   
   auto nitrateLayer = new G4Tubs("nitrateLayer",0.,0.5*layerDia,0.5*layerThickness,0.,2.0*CLHEP::pi);
   auto nitrateLayer_log = new G4LogicalVolume(nitrateLayer,fMaterialsManager->GetMaterial("Americium_nitrate"),"nitrateLayerLV");
   nitrateLayer_phys=new G4PVPlacement(0,G4ThreeVector(0,0,0),nitrateLayer_log,"nitrateLayerPV",source_log,false,0,true);
   
   auto visAttsrc = new G4VisAttributes( G4Colour(0.5,0.5,0.6,1.0) );
   source_log->SetVisAttributes( visAttsrc);

   G4double srcCentre = endCapFace - toutercapsule - tmiddlecapsule -hinnerplug - 0.5*hsource;

   G4cout << "source core center position X coordinate: " << srcCentre << G4endl;

  }



 //AmBe exact geometry implementation (no outer W capsule, remote position, oriented along x axis) // by Mariia Fedkevych
  else if ( split(fSourcePosition,'-')[0] == "LZAmBe2Remote" ) {
  
   float unit=1*CLHEP::mm;
   float ssShieldOuterDia = 2.01*unit;
   float ssShieldInnerDia = 1.22*unit;
   float ssShieldTopVoidHeight = 0.89*unit;
   float ssShieldTopVoidDia = 1.605*unit;
   float ssShieldBottomVoidHeight = 0.25267*unit; // ssShieldInnerDia/2*tg(22.5deg) if the bore was made with 135deg point angle drill bit
   float ssShieldCentralVoidHeight = 5.11*unit-ssShieldBottomVoidHeight;
   float ssShieldHeight = 6.50*unit;

   float ssShieldLidBottomDia = 1.6*unit;
   float ssShieldLidBottomHeight = 1.02*unit;
   float ssShieldLidTopHeight = 5*0.2*unit;
   float ssShieldLidTopDia = 1.4*unit;

   float wShieldOuterDia = 1.13*unit;
   float wShieldHeight = 4.19*unit;
   float wShieldBoreHeight = 4*unit;

   float wWireDia = 0.5*unit;
   float wWireHeight = 1.8*unit;
   float wWireTop = 0.3*unit;
   float wWireOffset = wShieldBoreHeight/2-wWireHeight/2+wWireTop;

   float srcCoreDia = 0.57*unit;
   float srcCoreHeight = wShieldBoreHeight-(wWireHeight-wWireTop);
   float srcCoreOffset = 0.5*wShieldHeight - (wWireHeight-wWireTop)-0.5*srcCoreHeight;//(wShieldBoreHeight-srcCoreHeight)/2;//(wShieldBoreHeight+wWireHeight-wWireTop-wShieldHeight)/2;

   auto rotationSS = new G4RotationMatrix();
   rotationSS->rotateX(90.*deg);

   float y_shift = 0*unit;
   auto positionSS = G4ThreeVector(-23.375*CLHEP::mm,-142.812*CLHEP::mm+y_shift,-142.812*CLHEP::mm);



   //outer SS capsule


   auto ssShieldCylinder = new G4Tubs("ssShieldCylinder",0.,0.5*ssShieldOuterDia,0.5*ssShieldHeight,0.,2.0*CLHEP::pi);

   auto ssShieldVoidTop = new G4Tubs("ssShieldVoidTop",0.,0.5*ssShieldTopVoidDia,0.5*ssShieldTopVoidHeight,0.,2.0*CLHEP::pi);
   auto ssShieldVoidCenter = new G4Tubs("ssShieldVoidCenter",0.,0.5*ssShieldInnerDia,0.5*(ssShieldCentralVoidHeight+0.002*unit),0.,2.0*CLHEP::pi);
   auto ssShieldVoidBottom = new G4Cons("ssShieldVoidBottom", 0., 0, 0, 0.5*ssShieldInnerDia, 0.5*ssShieldBottomVoidHeight,0.,2.0*CLHEP::pi);


   auto SubtractssShieldVoidTop = new G4SubtractionSolid("SubtractssShieldVoidTop", ssShieldCylinder, ssShieldVoidTop, 0, G4ThreeVector(0,0.,0.5*ssShieldHeight-0.5*ssShieldTopVoidHeight));
   auto SubtractssShieldVoidCenter = new G4SubtractionSolid("SubtractssShieldVoidCenter", SubtractssShieldVoidTop, ssShieldVoidCenter, 0, G4ThreeVector(0,0.,0.5*ssShieldHeight-ssShieldTopVoidHeight-0.5*ssShieldCentralVoidHeight));

   auto ssShieldS = new G4SubtractionSolid("ssShieldS", SubtractssShieldVoidCenter, ssShieldVoidBottom, 0, G4ThreeVector(0,0.,0.5*ssShieldHeight-ssShieldTopVoidHeight-ssShieldCentralVoidHeight-0.5*ssShieldBottomVoidHeight));

   auto ssShield_log = new G4LogicalVolume(ssShieldS, fMaterialsManager->GetMaterial("SS304L"),"ssShieldLV");
   ssShield_phys = new G4PVPlacement(rotationSS,positionSS,ssShield_log,"ssShieldPV",fLogicInnerCavity,false,0,true);

   auto visAttSS = new G4VisAttributes(G4Colour(0.8,0.8,0.7,0.5));
   ssShield_log->SetVisAttributes(visAttSS);


   //Lid of the outer SS capsule




   auto ssShieldLidSTop = new G4Tubs("ssShieldCylinder",0.,0.5*ssShieldLidTopDia,0.5*ssShieldLidTopHeight,0.,2.0*CLHEP::pi);
   auto ssShieldLidSBottom = new G4Tubs("ssShieldCylinder",0.,0.5*ssShieldLidBottomDia,0.5*ssShieldLidBottomHeight,0.,2.0*CLHEP::pi);
   auto ssShieldLidS =  new G4UnionSolid("ssShieldLidS", ssShieldLidSBottom, ssShieldLidSTop,0,G4ThreeVector(0,0.,0.5*ssShieldLidBottomHeight+0.5*ssShieldLidTopHeight)); 


   auto ssShieldLid_log = new G4LogicalVolume(ssShieldLidS, fMaterialsManager->GetMaterial("SS304L"),"ssShieldLidLV");
   ssShieldLid_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0.5*ssShieldHeight-0.5*ssShieldTopVoidHeight),ssShieldLid_log,"ssShieldLidPV",ssShield_log,false,0,true);


   ssShieldLid_log->SetVisAttributes(visAttSS);

   //inner W capsule


   auto wShieldCylinder = new G4Tubs("wShieldCylinder",0.,0.5*wShieldOuterDia,0.5*wShieldHeight,0.,2.0*CLHEP::pi);

   auto wShieldVoid = new G4Tubs("wShieldVoid",0.,0.5*srcCoreDia,0.5*wShieldBoreHeight,0.,2.0*CLHEP::pi);


   auto wShieldS = new G4SubtractionSolid("wShieldS", wShieldCylinder, wShieldVoid, 0, G4ThreeVector(0,0.,0.5*wShieldHeight-0.5*wShieldBoreHeight));






   auto wShield_log = new G4LogicalVolume(wShieldS, fMaterialsManager->GetMaterial("TungstenAlloy"),"wShieldLV");
   wShield_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0.5*ssShieldHeight-ssShieldTopVoidHeight-0.5*ssShieldCentralVoidHeight),wShield_log,"wShieldPV",ssShield_log,false,0,true);

   auto visAttw = new G4VisAttributes(G4Colour(0.7,0.7,0.9,0.75));
   wShield_log->SetVisAttributes(visAttw);


   //SS lid of the inner W capsule


   auto wShieldLidS = new G4Tubs("wShieldS",0.,0.5*wWireDia,0.5*wWireHeight,0.,2.0*CLHEP::pi);
   auto wShieldLid_log = new G4LogicalVolume(wShieldLidS, fMaterialsManager->GetMaterial("SS304L"),"wShieldLidLV");
   wShieldLid_phys = new G4PVPlacement(0,G4ThreeVector(0,0,wWireOffset),wShieldLid_log,"wShieldLidPV",ssShield_log,false,0,true);


   wShieldLid_log->SetVisAttributes(visAttw);


   //source core
   auto srcCoreS = new G4Tubs("srcCoreS",0.,0.5*srcCoreDia,0.5*srcCoreHeight,0.,2.0*CLHEP::pi);
   auto srcCore_log = new G4LogicalVolume(srcCoreS, fMaterialsManager->GetMaterial("Americium_Di-Oxide_Beryllium_mixture"),"srcCoreLV");
   srcCore_phys = new G4PVPlacement(0,G4ThreeVector(0,0,srcCoreOffset),srcCore_log,"srcCorePV",wShield_log,false,0,true);

   auto visAttsrcCore = new G4VisAttributes( G4Colour(0.5,0.5,0.6,1.0) );
   srcCore_log->SetVisAttributes( visAttsrcCore);

  } 

//AmBe exact geometry implementation (with outer W capsule, remote position, oriented along y axis, with axial rotation) // by Mariia Fedkevych
  else if ( split(fSourcePosition,'-')[0] == "LZAmBe2inOuterWCapsuleRemote" ) {
  
   float unit=1*CLHEP::mm;
   float ssShieldOuterDia = 2.01*unit;
   float ssShieldInnerDia = 1.22*unit;
   float ssShieldTopVoidHeight = 0.89*unit;
   float ssShieldTopVoidDia = 1.605*unit;
   float ssShieldBottomVoidHeight = 0.25267*unit; // ssShieldInnerDia/2*tg(22.5deg) if the bore was made with 135deg point angle drill bit
   float ssShieldCentralVoidHeight = 5.11*unit-ssShieldBottomVoidHeight;
   float ssShieldHeight = 6.50*unit;

   float ssShieldLidBottomDia = 1.6*unit;
   float ssShieldLidBottomHeight = 1.02*unit;
   float ssShieldLidTopHeight = 5*0.2*unit;
   float ssShieldLidTopDia = 1.4*unit;

   float wShieldOuterDia = 1.13*unit;
   float wShieldHeight = 4.19*unit;
   float wShieldBoreHeight = 4*unit;

   float wWireDia = 0.5*unit;
   float wWireHeight = 1.8*unit;
   float wWireTop = 0.3*unit;
   float wWireOffset = wShieldBoreHeight/2-wWireHeight/2+wWireTop;

   float srcCoreDia = 0.57*unit;
   float srcCoreHeight = wShieldBoreHeight-(wWireHeight-wWireTop);
   float srcCoreOffset = 0.5*wShieldHeight - (wWireHeight-wWireTop)-0.5*srcCoreHeight;




   float wCapsBaseOuterDia = 18*unit;
   float wCapsBaseInnerDia = 16*unit;//????????????
   float wCapsBaseHeight = 16*unit;
   float wCapsBaseBottomHeight = 9*unit;
   float wCapsBaseBoreHeight = 7.62*unit;
   float wCapsBaseBoreDia = 2.06*unit;
   float wCapsBaseBoreOffset = wCapsBaseOuterDia/2 - 5*unit;



   float wCapsCapOuterDia = 18*unit;
   float wCapsCapInnerDia = 16*unit;//????????????
   float wCapsCapHeight = 21*unit;
   float wCapsCapBottomVoidHeight = 7*unit;
   float wCapsCapBoreHeight = 10*unit;
   float wCapsCapBoreDia = 4*unit;
   float wCapsCapOffset = 0.5*wCapsBaseBottomHeight+0.5*wCapsCapHeight; //will be added/subtracted to the corresponding dimension depending on rotation




   auto rotationSS = new G4RotationMatrix();
   rotationSS->rotateY(90.*deg);


   G4double theta = 45*deg;//angle of rotation along own z axis

   G4ThreeVector AxisOfRotation = G4ThreeVector(0,0,1).unit();
   G4RotationMatrix* rotationW  = new G4RotationMatrix();
   rotationW->rotateY(90.*deg);
   rotationW -> rotate(theta, AxisOfRotation);



   auto positionSS = G4ThreeVector(-23.375*CLHEP::mm,-142.812*CLHEP::mm,-142.812*CLHEP::mm);


   //outer W capsule base
   auto wCapsuleBaseBottom = new G4Tubs("wCapsuleBaseBottom",0.,0.5*wCapsBaseOuterDia,0.5*wCapsBaseBottomHeight,0.,2.0*CLHEP::pi);
   auto wCapsuleBaseTop = new G4Tubs("wCapsuleBaseTop",0.,0.5*wCapsBaseInnerDia,0.5*(wCapsBaseHeight-wCapsBaseBottomHeight),0.,2.0*CLHEP::pi);

   auto wCapsuleBaseBody =  new G4UnionSolid("wCapsuleBaseBody", wCapsuleBaseBottom, wCapsuleBaseTop,0,G4ThreeVector(0,0.,0.5*wCapsBaseBottomHeight+0.5*(wCapsBaseHeight-wCapsBaseBottomHeight))); 



   auto wCapsuleBaseVoid = new G4Tubs("wCapsuleBaseVoid",0.,0.5*wCapsBaseBoreDia,0.5*wCapsBaseBoreHeight,0.,2.0*CLHEP::pi);



   auto wCapsuleBaseS = new G4SubtractionSolid("wCapsuleBaseS", wCapsuleBaseBody, wCapsuleBaseVoid, 0, G4ThreeVector(0.,wCapsBaseBoreOffset,wCapsBaseHeight-0.5*wCapsBaseBottomHeight-0.5*wCapsBaseBoreHeight));


 

   auto wCapsuleBase_log = new G4LogicalVolume(wCapsuleBaseS, fMaterialsManager->GetMaterial("TungstenAlloy"),"wCapsuleBaseLV");
   wCapsuleBase_phys = new G4PVPlacement(rotationW,positionSS,wCapsuleBase_log,"wCapsuleBasePV",fLogicInnerCavity,false,0,true);

   auto visAttw = new G4VisAttributes(G4Colour(0.7,0.7,0.9,0.75));
   wCapsuleBase_log->SetVisAttributes(visAttw);


   //outer W capsule cap
   auto wCapsuleCapCylinder = new G4Tubs("wCapsuleCapCylinder",0.,0.5*wCapsCapOuterDia,0.5*wCapsCapHeight,0.,2.0*CLHEP::pi);
   auto wCapsuleCapBottomVoid = new G4Tubs("wCapsuleCapBottomVoid",0.,0.5*wCapsBaseInnerDia,0.5*wCapsCapBottomVoidHeight,0.,2.0*CLHEP::pi);
   auto wCapsuleCapBore = new G4Tubs("wCapsuleCapBore",0.,0.5*wCapsCapBoreDia,0.5*wCapsCapBoreHeight,0.,2.0*CLHEP::pi);

   auto SubtractwCapsuleCapBottomVoid = new G4SubtractionSolid("SubtractwCapsuleCapBottomVoid", wCapsuleCapCylinder, wCapsuleCapBottomVoid, 0, G4ThreeVector(0.,0.,-0.5*wCapsCapHeight+0.5*wCapsCapBottomVoidHeight));

   auto wCapsuleCapS = new G4SubtractionSolid("wCapsuleCapS", SubtractwCapsuleCapBottomVoid, wCapsuleCapBore, 0, G4ThreeVector(0.,0.,0.5*wCapsCapHeight-0.5*wCapsCapBoreHeight));

   auto wCapsuleCap_log = new G4LogicalVolume(wCapsuleCapS, fMaterialsManager->GetMaterial("TungstenAlloy"),"wCapsuleCapLV");
   wCapsuleCap_phys = new G4PVPlacement(rotationSS,G4ThreeVector(positionSS.getX()- wCapsCapOffset, positionSS.getY(), positionSS.getZ()),wCapsuleCap_log,"wCapsuleBasePV",fLogicInnerCavity,false,0,true);


   wCapsuleCap_log->SetVisAttributes(visAttw);

   //outer SS capsule


   auto ssShieldCylinder = new G4Tubs("ssShieldCylinder",0.,0.5*ssShieldOuterDia,0.5*ssShieldHeight,0.,2.0*CLHEP::pi);

   auto ssShieldVoidTop = new G4Tubs("ssShieldVoidTop",0.,0.5*ssShieldTopVoidDia,0.5*ssShieldTopVoidHeight,0.,2.0*CLHEP::pi);
   auto ssShieldVoidCenter = new G4Tubs("ssShieldVoidCenter",0.,0.5*ssShieldInnerDia,0.5*(ssShieldCentralVoidHeight+0.002*unit),0.,2.0*CLHEP::pi);
   auto ssShieldVoidBottom = new G4Cons("ssShieldVoidBottom", 0., 0, 0, 0.5*ssShieldInnerDia, 0.5*ssShieldBottomVoidHeight,0.,2.0*CLHEP::pi);


   auto SubtractssShieldVoidTop = new G4SubtractionSolid("SubtractssShieldVoidTop", ssShieldCylinder, ssShieldVoidTop, 0, G4ThreeVector(0,0.,0.5*ssShieldHeight-0.5*ssShieldTopVoidHeight));
   auto SubtractssShieldVoidCenter = new G4SubtractionSolid("SubtractssShieldVoidCenter", SubtractssShieldVoidTop, ssShieldVoidCenter, 0, G4ThreeVector(0,0.,0.5*ssShieldHeight-ssShieldTopVoidHeight-0.5*ssShieldCentralVoidHeight));

   auto ssShieldS = new G4SubtractionSolid("ssShieldS", SubtractssShieldVoidCenter, ssShieldVoidBottom, 0, G4ThreeVector(0,0.,0.5*ssShieldHeight-ssShieldTopVoidHeight-ssShieldCentralVoidHeight-0.5*ssShieldBottomVoidHeight));

   auto ssShield_log = new G4LogicalVolume(ssShieldS, fMaterialsManager->GetMaterial("SS304L"),"ssShieldLV");
   ssShield_phys = new G4PVPlacement(0,G4ThreeVector(0.,wCapsBaseBoreOffset,wCapsBaseHeight-0.5*wCapsBaseBottomHeight-wCapsBaseBoreHeight+0.5*ssShieldHeight),ssShield_log,"ssShieldPV",wCapsuleBase_log,false,0,true);

   auto visAttSS = new G4VisAttributes(G4Colour(0.8,0.8,0.7,0.5));
   ssShield_log->SetVisAttributes(visAttSS);


   //Lid of the outer SS capsule




   auto ssShieldLidSTop = new G4Tubs("ssShieldCylinder",0.,0.5*ssShieldLidTopDia,0.5*ssShieldLidTopHeight,0.,2.0*CLHEP::pi);
   auto ssShieldLidSBottom = new G4Tubs("ssShieldCylinder",0.,0.5*ssShieldLidBottomDia,0.5*ssShieldLidBottomHeight,0.,2.0*CLHEP::pi);
   auto ssShieldLidS =  new G4UnionSolid("ssShieldLidS", ssShieldLidSBottom, ssShieldLidSTop,0,G4ThreeVector(0,0.,0.5*ssShieldLidBottomHeight+0.5*ssShieldLidTopHeight)); 


   auto ssShieldLid_log = new G4LogicalVolume(ssShieldLidS, fMaterialsManager->GetMaterial("SS304L"),"ssShieldLidLV");
   ssShieldLid_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0.5*ssShieldHeight-0.5*ssShieldTopVoidHeight),ssShieldLid_log,"ssShieldLidPV",ssShield_log,false,0,true);


   ssShieldLid_log->SetVisAttributes(visAttSS);

   //inner W capsule


   auto wShieldCylinder = new G4Tubs("wShieldCylinder",0.,0.5*wShieldOuterDia,0.5*wShieldHeight,0.,2.0*CLHEP::pi);

   auto wShieldVoid = new G4Tubs("wShieldVoid",0.,0.5*srcCoreDia,0.5*wShieldBoreHeight,0.,2.0*CLHEP::pi);


   auto wShieldS = new G4SubtractionSolid("wShieldS", wShieldCylinder, wShieldVoid, 0, G4ThreeVector(0,0.,0.5*wShieldHeight-0.5*wShieldBoreHeight));






   auto wShield_log = new G4LogicalVolume(wShieldS, fMaterialsManager->GetMaterial("TungstenAlloy"),"wShieldLV");
   wShield_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0.5*ssShieldHeight-ssShieldTopVoidHeight-0.5*ssShieldCentralVoidHeight),wShield_log,"wShieldPV",ssShield_log,false,0,true);

   //auto visAttw = new G4VisAttributes(G4Colour(0.7,0.7,0.9,0.75));
   wShield_log->SetVisAttributes(visAttw);


   //SS lid of the inner W capsule


   auto wShieldLidS = new G4Tubs("wShieldS",0.,0.5*wWireDia,0.5*wWireHeight,0.,2.0*CLHEP::pi);
   auto wShieldLid_log = new G4LogicalVolume(wShieldLidS, fMaterialsManager->GetMaterial("SS304L"),"wShieldLidLV");
   wShieldLid_phys = new G4PVPlacement(0,G4ThreeVector(0,0,wWireOffset),wShieldLid_log,"wShieldLidPV",ssShield_log,false,0,true);


   wShieldLid_log->SetVisAttributes(visAttw);


   //source core
   auto srcCoreS = new G4Tubs("srcCoreS",0.,0.5*srcCoreDia,0.5*srcCoreHeight,0.,2.0*CLHEP::pi);
   auto srcCore_log = new G4LogicalVolume(srcCoreS, fMaterialsManager->GetMaterial("Americium_Di-Oxide_Beryllium_mixture"),"srcCoreLV");
   srcCore_phys = new G4PVPlacement(0,G4ThreeVector(0,0,srcCoreOffset),srcCore_log,"srcCorePV",wShield_log,false,0,true);

   auto visAttsrcCore = new G4VisAttributes( G4Colour(0.5,0.5,0.6,1.0) );
   srcCore_log->SetVisAttributes( visAttsrcCore);

  }


//AmBe exact geometry implementation (with outer W capsule, on Endcap, oriented along z axis, with axial rotation) // by Mariia Fedkevych
  else if ( split(fSourcePosition,'-')[0] == "LZAmBe2inOuterWCapsuleEndcap" ) {
  
   float unit=1*CLHEP::mm;
   float ssShieldOuterDia = 2.01*unit;
   float ssShieldInnerDia = 1.22*unit;
   float ssShieldTopVoidHeight = 0.89*unit;
   float ssShieldTopVoidDia = 1.605*unit;
   float ssShieldBottomVoidHeight = 0.25267*unit; // ssShieldInnerDia/2*tg(22.5deg) if the bore was made with 135deg point angle drill bit
   float ssShieldCentralVoidHeight = 5.11*unit-ssShieldBottomVoidHeight;
   float ssShieldHeight = 6.50*unit;

   float ssShieldLidBottomDia = 1.6*unit;
   float ssShieldLidBottomHeight = 1.02*unit;
   float ssShieldLidTopHeight = 5*0.2*unit;
   float ssShieldLidTopDia = 1.4*unit;

   float wShieldOuterDia = 1.13*unit;
   float wShieldHeight = 4.19*unit;
   float wShieldBoreHeight = 4*unit;

   float wWireDia = 0.5*unit;
   float wWireHeight = 1.8*unit;
   float wWireTop = 0.3*unit;
   float wWireOffset = wShieldBoreHeight/2-wWireHeight/2+wWireTop;

   float srcCoreDia = 0.57*unit;
   float srcCoreHeight = wShieldBoreHeight-(wWireHeight-wWireTop);
   float srcCoreOffset = 0.5*wShieldHeight - (wWireHeight-wWireTop)-0.5*srcCoreHeight;




   float wCapsBaseOuterDia = 18*unit;
   float wCapsBaseInnerDia = 16*unit;//????????????
   float wCapsBaseHeight = 16*unit;
   float wCapsBaseBottomHeight = 9*unit;
   float wCapsBaseBoreHeight = 7.62*unit;
   float wCapsBaseBoreDia = 2.06*unit;
   float wCapsBaseBoreOffset = wCapsBaseOuterDia/2 - 5*unit;



   float wCapsCapOuterDia = 18*unit;
   float wCapsCapInnerDia = 16*unit;//????????????
   float wCapsCapHeight = 21*unit;
   float wCapsCapBottomVoidHeight = 7*unit;
   float wCapsCapBoreHeight = 10*unit;
   float wCapsCapBoreDia = 4*unit;
   float wCapsCapOffset = 0.5*wCapsBaseBottomHeight+0.5*wCapsCapHeight; //will be added/subtracted to the corresponding dimension depending on rotation




   auto rotationSS = new G4RotationMatrix();



   G4double theta = 270*deg;//angle of rotation along own z axis (270 deg for "near", 90 deg for "far" orientation)

   G4ThreeVector AxisOfRotation = G4ThreeVector(0,0,1).unit();
   G4RotationMatrix* rotationW  = new G4RotationMatrix();
   rotationW -> rotate(theta, AxisOfRotation);


   //G4cout << "endCapFace DetectorConstruction :" << endCapFace << G4endl;

   auto positionSS = G4ThreeVector(endCapFace+0.5*wCapsBaseOuterDia,0*CLHEP::mm,-0.5*wCapsCapHeight);


   //outer W capsule base
   auto wCapsuleBaseBottom = new G4Tubs("wCapsuleBaseBottom",0.,0.5*wCapsBaseOuterDia,0.5*wCapsBaseBottomHeight,0.,2.0*CLHEP::pi);
   auto wCapsuleBaseTop = new G4Tubs("wCapsuleBaseTop",0.,0.5*wCapsBaseInnerDia,0.5*(wCapsBaseHeight-wCapsBaseBottomHeight),0.,2.0*CLHEP::pi);

   auto wCapsuleBaseBody =  new G4UnionSolid("wCapsuleBaseBody", wCapsuleBaseBottom, wCapsuleBaseTop,0,G4ThreeVector(0,0.,0.5*wCapsBaseBottomHeight+0.5*(wCapsBaseHeight-wCapsBaseBottomHeight))); 



   auto wCapsuleBaseVoid = new G4Tubs("wCapsuleBaseVoid",0.,0.5*wCapsBaseBoreDia,0.5*wCapsBaseBoreHeight,0.,2.0*CLHEP::pi);



   auto wCapsuleBaseS = new G4SubtractionSolid("wCapsuleBaseS", wCapsuleBaseBody, wCapsuleBaseVoid, 0, G4ThreeVector(0.,wCapsBaseBoreOffset,wCapsBaseHeight-0.5*wCapsBaseBottomHeight-0.5*wCapsBaseBoreHeight));


 

   auto wCapsuleBase_log = new G4LogicalVolume(wCapsuleBaseS, fMaterialsManager->GetMaterial("TungstenAlloy"),"wCapsuleBaseLV");
   wCapsuleBase_phys = new G4PVPlacement(rotationW,positionSS,wCapsuleBase_log,"wCapsuleBasePV",fLogicInnerCavity,false,0,true);

   auto visAttw = new G4VisAttributes(G4Colour(0.7,0.7,0.9,0.75));
   wCapsuleBase_log->SetVisAttributes(visAttw);


   //outer W capsule cap
   auto wCapsuleCapCylinder = new G4Tubs("wCapsuleCapCylinder",0.,0.5*wCapsCapOuterDia,0.5*wCapsCapHeight,0.,2.0*CLHEP::pi);
   auto wCapsuleCapBottomVoid = new G4Tubs("wCapsuleCapBottomVoid",0.,0.5*wCapsBaseInnerDia,0.5*wCapsCapBottomVoidHeight,0.,2.0*CLHEP::pi);
   auto wCapsuleCapBore = new G4Tubs("wCapsuleCapBore",0.,0.5*wCapsCapBoreDia,0.5*wCapsCapBoreHeight,0.,2.0*CLHEP::pi);

   auto SubtractwCapsuleCapBottomVoid = new G4SubtractionSolid("SubtractwCapsuleCapBottomVoid", wCapsuleCapCylinder, wCapsuleCapBottomVoid, 0, G4ThreeVector(0.,0.,-0.5*wCapsCapHeight+0.5*wCapsCapBottomVoidHeight));

   auto wCapsuleCapS = new G4SubtractionSolid("wCapsuleCapS", SubtractwCapsuleCapBottomVoid, wCapsuleCapBore, 0, G4ThreeVector(0.,0.,0.5*wCapsCapHeight-0.5*wCapsCapBoreHeight));

   auto wCapsuleCap_log = new G4LogicalVolume(wCapsuleCapS, fMaterialsManager->GetMaterial("TungstenAlloy"),"wCapsuleCapLV");
   wCapsuleCap_phys = new G4PVPlacement(rotationSS,G4ThreeVector(positionSS.getX(), positionSS.getY(), positionSS.getZ()+ wCapsCapOffset),wCapsuleCap_log,"wCapsuleBasePV",fLogicInnerCavity,false,0,true);


   wCapsuleCap_log->SetVisAttributes(visAttw);

   //outer SS capsule


   auto ssShieldCylinder = new G4Tubs("ssShieldCylinder",0.,0.5*ssShieldOuterDia,0.5*ssShieldHeight,0.,2.0*CLHEP::pi);

   auto ssShieldVoidTop = new G4Tubs("ssShieldVoidTop",0.,0.5*ssShieldTopVoidDia,0.5*ssShieldTopVoidHeight,0.,2.0*CLHEP::pi);
   auto ssShieldVoidCenter = new G4Tubs("ssShieldVoidCenter",0.,0.5*ssShieldInnerDia,0.5*(ssShieldCentralVoidHeight+0.002*unit),0.,2.0*CLHEP::pi);
   auto ssShieldVoidBottom = new G4Cons("ssShieldVoidBottom", 0., 0, 0, 0.5*ssShieldInnerDia, 0.5*ssShieldBottomVoidHeight,0.,2.0*CLHEP::pi);


   auto SubtractssShieldVoidTop = new G4SubtractionSolid("SubtractssShieldVoidTop", ssShieldCylinder, ssShieldVoidTop, 0, G4ThreeVector(0,0.,0.5*ssShieldHeight-0.5*ssShieldTopVoidHeight));
   auto SubtractssShieldVoidCenter = new G4SubtractionSolid("SubtractssShieldVoidCenter", SubtractssShieldVoidTop, ssShieldVoidCenter, 0, G4ThreeVector(0,0.,0.5*ssShieldHeight-ssShieldTopVoidHeight-0.5*ssShieldCentralVoidHeight));

   auto ssShieldS = new G4SubtractionSolid("ssShieldS", SubtractssShieldVoidCenter, ssShieldVoidBottom, 0, G4ThreeVector(0,0.,0.5*ssShieldHeight-ssShieldTopVoidHeight-ssShieldCentralVoidHeight-0.5*ssShieldBottomVoidHeight));

   auto ssShield_log = new G4LogicalVolume(ssShieldS, fMaterialsManager->GetMaterial("SS304L"),"ssShieldLV");
   ssShield_phys = new G4PVPlacement(0,G4ThreeVector(0.,wCapsBaseBoreOffset,wCapsBaseHeight-0.5*wCapsBaseBottomHeight-wCapsBaseBoreHeight+0.5*ssShieldHeight),ssShield_log,"ssShieldPV",wCapsuleBase_log,false,0,true);

   auto visAttSS = new G4VisAttributes(G4Colour(0.8,0.8,0.7,0.5));
   ssShield_log->SetVisAttributes(visAttSS);


   //Lid of the outer SS capsule




   auto ssShieldLidSTop = new G4Tubs("ssShieldCylinder",0.,0.5*ssShieldLidTopDia,0.5*ssShieldLidTopHeight,0.,2.0*CLHEP::pi);
   auto ssShieldLidSBottom = new G4Tubs("ssShieldCylinder",0.,0.5*ssShieldLidBottomDia,0.5*ssShieldLidBottomHeight,0.,2.0*CLHEP::pi);
   auto ssShieldLidS =  new G4UnionSolid("ssShieldLidS", ssShieldLidSBottom, ssShieldLidSTop,0,G4ThreeVector(0,0.,0.5*ssShieldLidBottomHeight+0.5*ssShieldLidTopHeight)); 


   auto ssShieldLid_log = new G4LogicalVolume(ssShieldLidS, fMaterialsManager->GetMaterial("SS304L"),"ssShieldLidLV");
   ssShieldLid_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0.5*ssShieldHeight-0.5*ssShieldTopVoidHeight),ssShieldLid_log,"ssShieldLidPV",ssShield_log,false,0,true);


   ssShieldLid_log->SetVisAttributes(visAttSS);

   //inner W capsule


   auto wShieldCylinder = new G4Tubs("wShieldCylinder",0.,0.5*wShieldOuterDia,0.5*wShieldHeight,0.,2.0*CLHEP::pi);

   auto wShieldVoid = new G4Tubs("wShieldVoid",0.,0.5*srcCoreDia,0.5*wShieldBoreHeight,0.,2.0*CLHEP::pi);


   auto wShieldS = new G4SubtractionSolid("wShieldS", wShieldCylinder, wShieldVoid, 0, G4ThreeVector(0,0.,0.5*wShieldHeight-0.5*wShieldBoreHeight));






   auto wShield_log = new G4LogicalVolume(wShieldS, fMaterialsManager->GetMaterial("TungstenAlloy"),"wShieldLV");
   wShield_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0.5*ssShieldHeight-ssShieldTopVoidHeight-0.5*ssShieldCentralVoidHeight),wShield_log,"wShieldPV",ssShield_log,false,0,true);

   //auto visAttw = new G4VisAttributes(G4Colour(0.7,0.7,0.9,0.75));
   wShield_log->SetVisAttributes(visAttw);


   //SS lid of the inner W capsule


   auto wShieldLidS = new G4Tubs("wShieldS",0.,0.5*wWireDia,0.5*wWireHeight,0.,2.0*CLHEP::pi);
   auto wShieldLid_log = new G4LogicalVolume(wShieldLidS, fMaterialsManager->GetMaterial("SS304L"),"wShieldLidLV");
   wShieldLid_phys = new G4PVPlacement(0,G4ThreeVector(0,0,wWireOffset),wShieldLid_log,"wShieldLidPV",ssShield_log,false,0,true);


   wShieldLid_log->SetVisAttributes(visAttw);


   //source core
   auto srcCoreS = new G4Tubs("srcCoreS",0.,0.5*srcCoreDia,0.5*srcCoreHeight,0.,2.0*CLHEP::pi);
   auto srcCore_log = new G4LogicalVolume(srcCoreS, fMaterialsManager->GetMaterial("Americium_Di-Oxide_Beryllium_mixture"),"srcCoreLV");
   srcCore_phys = new G4PVPlacement(0,G4ThreeVector(0,0,srcCoreOffset),srcCore_log,"srcCorePV",wShield_log,false,0,true);

   auto visAttsrcCore = new G4VisAttributes( G4Colour(0.5,0.5,0.6,1.0) );
   srcCore_log->SetVisAttributes( visAttsrcCore);

  }
  // Add new geometry here

  // Button sources are used
  if(fSourcePosition == "1_Down" || fSourcePosition == "2_Down" || fSourcePosition == "3_Down" || fSourcePosition == "4_Down" || 
     fSourcePosition == "1_Up"   || fSourcePosition == "2_Up"   || fSourcePosition == "3_Up"   || fSourcePosition == "4_Up"   || 
     fSourcePosition == "5_Up"   || 
     fSourcePosition == "0" || fSourcePosition == "0_1inch"){
    //G4cout << "Source Center: " << buttonTranslation.getX()/mm << " " << buttonTranslation.getY()/mm << " " << buttonTranslation.getZ()/mm << " mm\n";

    double resinThickness = 3.18 * mm;
    double resinRadius = 5./2.*mm;
    double windowThickness = 2.77 * mm;
  
    fSolidButton = new G4Tubs("button",0.,fButtonRadius,fButtonLength/2.,0.,twopi);
    fLogicButton = new G4LogicalVolume(fSolidButton ,fButtonMater,"Button",0,0,0);
    fPhysiButton = new G4PVPlacement(buttonRotation,         
                                    buttonTranslation,
                                    fLogicButton,   // its logical volume                                  
                                    "Button",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
    
    G4ThreeVector fillerTranslation(0,0,fButtonLength/2.-windowThickness-resinThickness/2.);

    G4Tubs * solidButtonFiller = new G4Tubs("button",0.,resinRadius,resinThickness/2.,0.,twopi);
    G4LogicalVolume * logicButtonFiller = new G4LogicalVolume(solidButtonFiller ,fButtonFillerMater,"ButtonFiller",0,0,0);
    G4VPhysicalVolume * physiButtonFiller = new G4PVPlacement(0,//buttonRotation,         
                                    fillerTranslation,
                                    logicButtonFiller,   // its logical volume                                  
                                    "ButtonFiller",       // its name
                                    fLogicButton,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 

    G4ThreeVector sourceBallLocation = G4ThreeVector(0,0,resinThickness/2.);
    G4VPhysicalVolume * pvs[] = {physiButtonFiller, fPhysiButton};
    for(int i = 0; i < 2; i++){
      G4RotationMatrix * rm = pvs[i]->GetRotation();
      if(rm != 0){
        sourceBallLocation = sourceBallLocation.transform(rm->inverse());
      }
      G4ThreeVector tv = pvs[i]->GetTranslation();
      sourceBallLocation = sourceBallLocation + tv;
    }

    G4cout << "Source position: " 
           << sourceBallLocation.getX() / mm << " "
           << sourceBallLocation.getY() / mm << " "
           << sourceBallLocation.getZ() / mm << " mm " << G4endl;
    G4cout << "Source radius: " << solidButtonFiller->GetOuterRadius() / mm << " mm" << G4endl;
  }
    

  /*// Not used anymore.
  if(fSourcePosition == "0"){

    G4ThreeVector copperBlockTranslation(100.*mm,0,fCopperBlockZ/2.-152.4*mm);
    fSolidCopperBlock= new G4Box("copperBlock",fCopperBlockX/2.,fCopperBlockY/2.,fCopperBlockZ/2.);
    fLogicCopperBlock = new G4LogicalVolume(fSolidCopperBlock,fCopperBlockMater,"CopperBlock",0,0,0);
    fPhysiCopperBlock = new G4PVPlacement(0,              // no rotation
                                    copperBlockTranslation,
                                    fLogicCopperBlock,    // its logical volume                                  
                                    "CopperBlock",        // its name
                                    fLogicInnerCavity,     // its mother  volume
                                    false,           // no boolean operations
                                    0,true);         // no particular field 

    G4ThreeVector copperTubeTranslation(100.*mm,0,fCopperBlockZ+fCopperTubeLength/2.-152.4*mm);
    fSolidCopperTube = new G4Tubs("copperTube",fCopperTubeRadius-fCopperTubeThickness,fCopperTubeRadius,fCopperTubeLength/2.,0.,twopi);
    fLogicCopperTube = new G4LogicalVolume(fSolidCopperTube ,fCopperTubeMater,"CopperTube",0,0,0);
    fPhysiCopperTube = new G4PVPlacement(0,
                                    //G4ThreeVector(0,0,0),  // DEBUG
                                    copperTubeTranslation,
                                    fLogicCopperTube,   // its logical volume                                  
                                    "CopperTube",       // its name
                                    fLogicInnerCavity,      // its mother  volume
                                    false,            // no boolean operations
                                    0,true);          // no particular field 
  } 
  // */

  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  //  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  // G4String detectortargetSDname = "gesim/DetectorTargetSD";
  // gesimDetectorSD* aDetectorSD = new gesimDetectorSD( detectorTargetSDname );
  // SDman->AddNewDetector( aDetectorSD );
  //fLogicTarget->SetSensitiveDetector( aDetectorSD );
  // fLogicDetector->SetSensitiveDetector( aDetectorSD );
  //
  //-------------------------------------------------
  // regions
  //
  //  if(fTargetRegion) delete fTargetRegion;
  // if(fDetectorRegion) delete fDetectorRegion;
  //  fTargetRegion = new G4Region("Target");
  //  fDetectorRegion   = new G4Region("Detector");
  //  fTargetRegion->AddRootLogicalVolume(fLogicTarget);
  //  fDetectorRegion->AddRootLogicalVolume(fLogicDetector);

  //--------- Visualization attributes -------------------------------
  //fLogicDetectorEnclosure->SetVisAttributes(G4VisAttributes::Invisible);
  //G4VisAttributes* TargetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  //fLogicTarget ->SetVisAttributes(TargetVisAtt);


  // Colors
  fLogicOuterShielding->SetVisAttributes(GreyVisAtt);
  G4VisAttributes* innerShieldingVisAtt  = CopperVisAtt;
  //innerShieldingVisAtt->SetVisibility(false);
  innerShieldingVisAtt->SetForceWireframe(true);
  fLogicInnerShielding->SetVisAttributes(innerShieldingVisAtt);
  //fLogicOuterCavity->SetVisAttributes(GreyVisAtt);
  fLogicInnerCavity->SetVisAttributes(CopperVisAtt);
  fLogicDetector->SetVisAttributes(YellowVisAtt);
  fLogicActiveDetector->SetVisAttributes(RedVisAtt);
  fLogicDetectorEnclosure->SetVisAttributes( G4VisAttributes::Invisible );
  fLogicEndCap->SetVisAttributes(GreyVisAtt);
  fLogicHead->SetVisAttributes(VioletVisAtt);
  if(fDetId == "3" || fDetId == "4") fLogicSnout->SetVisAttributes(VioletVisAtt);
  fLogicNeck->SetVisAttributes(OrangeVisAtt);
  if (fDetId == '4') {
    fLogicNeckinCopper->SetVisAttributes(OrangeVisAtt);
    fLogicNeckinLead->SetVisAttributes(OrangeVisAtt);
  }
  fLogicHolder->SetVisAttributes(OrangeVisAtt);
  fLogicSignalPin->SetVisAttributes(SpringGreenVisAtt);
  fLogicSignalPinSupport->SetVisAttributes(OrangeVisAtt);
  fLogicSheath->SetVisAttributes(MagentaVisAtt);

  fLogicHead->SetVisAttributes( OrangeVisAtt );
  G4cout << "Mass of detector = " << fLogicDetector->GetMass(true,true) / g << G4endl;

  // Invisibility
  fLogicWorld->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicDetector->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicActiveDetector->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicOuterShielding->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicInnerShielding->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicOuterCavity->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicInnerCavity->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicDetectorEnclosure->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicEndCap->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicHolder->SetVisAttributes(G4VisAttributes::Invisible);
//  fLogicSheath->SetVisAttributes(G4VisAttributes::Invisible);

  //------------ set the incident position ------

 // get the pointer to the User Interface manager 
    
  G4UImanager* UI = G4UImanager::GetUIpointer();  
  //      UI->ApplyCommand("/run/verbose 1");
  //      UI->ApplyCommand("/event/verbose 2");
  //      UI->ApplyCommand("/tracking/verbose 1");  

  G4double zpos = -fWorldZ/2.;
  G4String command = "/gps/pos/centre ";
  std::ostringstream os;
  os << zpos ; 
  G4String xs = os.str();
  UI->ApplyCommand(command+"0. 0. "+xs+" mm");
  UI->ApplyCommand("/gps/pos/type Point");
  command = "/gps/position ";
  //  UI->ApplyCommand(command+"0. 0. "+xs+" mm");
  UI->ApplyCommand("/gps/particle proton");
  UI->ApplyCommand("/gps/direction 0 0 1");
  UI->ApplyCommand("/gps/energy 100 MeV");
  //       
  
  return fPhysiWorld;
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 
//void gesimDetectorConstruction::SetTargetMaterial(G4String materialName)
//{
//  // search the material by its name 
//  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
//  if (pttoMaterial)
//     {fTargetMater = pttoMaterial;
//      if (fLogicTarget) fLogicTarget->SetMaterial(pttoMaterial); 
//      G4cout << "\n----> The target has been changed to " << fTargetLength/cm
//                     << " cm of "
//             << materialName << G4endl;
//     }             
//}
// 
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void gesimDetectorConstruction::SetDetectorMaterial(G4String materialName)
//{
//  // search the material by its name 
//  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
//  if (pttoMaterial)
//     {fDetectorMater = pttoMaterial;
//      if (fLogicDetector) fLogicDetector->SetMaterial(pttoMaterial); 
//      G4cout << "\n----> The Detector has been changed to" << fDetectorLength/cm
//                     << " cm of "
//             << materialName << G4endl;
//     }             
//}
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
