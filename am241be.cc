//Questions email rwm33@drexel.edu Ryan McKeown

#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void emon(double* rnd,int &ngen,double &mass1,int &idpdg1, double *p1, double &ekemev)
{

/*
 *** this routine generates Am-241Be-9 gammas above 59.5 keV

 *** rnd(5) (real*4): array of 5 random numbers U(0,1)
 *** ngen (integer): number of particles generated
 *** mass1,mass2 (real*4): masses in GeV of particles generated
 *** idpdg1,idpdg2 (integer*4): PDG codes of particles generated
 *** p1(3), p2(3) (real*4): momentum in GeV/c of particles generated
 */

    int j1,j2;
    int dat0,dat1,dat2,dat3;
    float fdat0,fdat1,fdat2,fdat3;
    double esel,nke,gke,costh,sinth,phi,ptot,sum;
    double twopi=6.283185;

    //gamma
    ngen=1;
    mass1=0.;
    idpdg1=22;

    for(int i=0;i<3;i++)
      {
	p1[i]=0;
      }
    costh=(1-2.0*rnd[0]);
    sinth=sqrt(1.-pow(costh,2.0));
    phi=twopi*rnd[1];
    ptot=ekemev/1e3; //converting to MeV
    p1[0]=ptot*sinth*cos(phi);
    p1[1]=ptot*sinth*sin(phi);
    p1[2]=ptot*costh;
}

int main(int argc, char**argv)
{

  int nevent=1000000;
  unsigned int user_sd=0;  
  double ekemev;
  float sum;
  float energy[150],csum[150];
  std::ifstream spectrum;

   if(argc > 1)
    nevent=atoi(argv[1]);

  if(argc > 2)
    user_sd = (unsigned int) atoi(argv[2]);
  else 
    user_sd = (unsigned int) time(0);
  srand(user_sd);


      sum=0.;
      float fdat0,fdat1;
      spectrum.open("am241be.spectrum");
      int counter=0;
      while(!spectrum.eof()){
	spectrum >> fdat0 >> fdat1;
	counter++;
	energy[counter-1]=fdat0;
	sum=sum+fdat1/100.;
	csum[counter-1]=sum;
//	std::cout << "energy BR, sum: " << fdat0 << " " << fdat1 << " " << sum << std::endl;
      }
      //      std::cout << "Sum of branching ratios is " << sum << std::endl;
      for (int k=0;k<counter;k++){
	  csum[k]=csum[k]/sum;
      }
      
  double rnd[5],p1[3],p2[3];
  
  int ngen,idpdg1,idpdg2;
  double mass1,mass2;

  for (int j=0;j<nevent;j++){
//    seedRand(static_cast<unsigned int>(j) );
    for (int i=0;i<5;i++)//need 5 random numbers to pass to ambe
      rnd[i]=rand()/(RAND_MAX+1.0);
    
    for (int j=0;j<counter;j++){
      if(j == 0){
	if (rnd[2] <= csum[j]){
	  ekemev=energy[j]/1e3;//MeV to GeV
	  //	  std::cout << " j is 0, energy is  " << ekemev << std::endl;
	}
      }
      else{
	if (rnd[2] > csum[j-1] && rnd[2] <= csum[j]){
	  ekemev=energy[j]/1e3;//MeV to GeV
	  //	  std::cout << " j, energy is is " << j << " " << ekemev << std::endl;
	}
      }
    }
    emon(rnd,ngen,mass1,idpdg1,p1,ekemev);
    
    std::cout << ngen <<std::endl ; //number of particles gen

    std::cout << "1 " <<idpdg1<<" 0 0 " << p1[0] << " "<< p1[1]
	   << " " << p1[2]<<" " << mass1 << std::endl ;
  }
}
