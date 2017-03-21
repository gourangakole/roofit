#ifndef __CINT__
#endif
#include <iostream>
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"
//#include "RooSigmoid.h"                                                                                                             
//#include "RooCMSShape.h"                                                                                                            
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooGlobalFunc.h"
#include "RooBreitWigner.h"
#include "RooGaussian.h"
#include "RooVoigtian.h"
#include  "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include  "RooFitResult.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPaveLabel.h"
#include "TPave.h"
#include "TStyle.h"
#include "TROOT.h"
//#include "fstream.h"                                                                                                                

using namespace RooFit ;

void fit(){
  RooRealVar x("x","e_RECO/e_GEN",0.9,1.1);
  TFile *inputfile = new TFile("Histos.root","READ");
  inputfile->cd();
  TH1F *h  = (TH1F*)inputfile->Get("minus5_1");
  RooDataHist dh0("dh0","dh0",x,Import(*h)) ;
  RooPlot* frame0 = x.frame(Title("Minus5_1")) ;
  dh0.plotOn(frame0,Name("dh0")) ;


  RooRealVar sigmean1("sigmean1","B",1.0,0.95,1.05) ; 
  //  RooRealVar sigwidth1("sigwidth1","B^{#pm} width",0.0027,0.001,3.) ; 
  RooRealVar sigwidth1("sigwidth1","B^{#pm} width",0.005,0.001,0.09) ; 
  RooGaussian gauss1("gauss1","gaussian PDF",x,sigmean1,sigwidth1) ;
  RooRealVar sigmean2("sigmean2","B",0.95,.9,1.05) ; 
  //RooRealVar sigwidth2("sigwidth2","B^{#pm} width",10.27,0.001,15.) ; 
RooRealVar sigwidth2("sigwidth2","B^{#pm} width",0.27,0.01,15.) ; 
  RooGaussian gauss2("gauss2","gaussian PDF",x,sigmean2,sigwidth2) ;
  RooRealVar sigmean3("sigmean3","B",0.98,0.95,1.05) ; 
  //  RooRealVar sigwidth3("sigwidth3","B^{#pm} width",0.027,0.001,15.) ; 
  RooRealVar sigwidth3("sigwidth3","B^{#pm} width",0.027,0.001,15.) ; 
  RooGaussian gauss3("gauss3","gaussian PDF",x,sigmean3,sigwidth3) ;
  RooRealVar nsig1("nsig1","#signal1 events",20000,0.,100000000) ; 
  RooRealVar nsig2("nsig2","#Signal2 events",800,0.,1000000) ; 
  RooRealVar nsig3("nsig3","#Signal3 events",1800,0.,100000000) ; 
  RooAddPdf sum("sum","g1+g2+g3",RooArgList(gauss1,gauss2,gauss3),RooArgList(nsig1,nsig2,nsig3)) ; 
  //RooAddPdf sum("sum","g1+g2",RooArgList(gauss1,gauss2),RooArgList(nsig1,nsig2)) ; 
  /*
  //  sum.chi2FitTo(dh0,Range(0.9,1.2));
    sum.fitTo(dh0,Extended(),Range(0.9,1.2));
  // Display fit parameters.
  sum.paramOn(frame0, Format("NELU", AutoPrecision(2)), Layout(0.1, 0.4,0.9) );
  // Plot just the background.
  sum.plotOn(frame0, Components(gauss1) , LineStyle(kDashed), LineColor(kGreen) );
  sum.plotOn(frame0, Components(gauss2) , LineStyle(kDashed), LineColor(kBlue) );
  sum.plotOn(frame0, Components(gauss3) , LineStyle(kDashed), LineColor(kYellow) );
  // Plot total PDF.
  sum.plotOn(frame0,LineColor(kRed)); // plot fit pdf
  // The plot is not on the screen yet ­­ we have only created a RooPlot object.
  // Put the plot on the screen.
  */

  RooFitResult* res = sum.fitTo(dh0,Range(0.90,1.1),Minos(kTRUE),PrintLevel(3),Save());
  //sum.fitTo(dh0,Extended(),Range(0.9,1.2));
  res->Print();
  sum.plotOn(frame0, Name("sum"));
  sum.plotOn(frame0,Components(gauss1),LineStyle(kDashed)) ;
  sum.plotOn(frame0,Components(gauss2),LineColor(kGreen)) ;
  sum.plotOn(frame0,Components(gauss3),LineColor(kRed)) ;
  sum.paramOn(frame0,Layout(0.62,0.90),Format("NEU",AutoPrecision(1))) ;
  frame0->getAttText()->SetTextSize(0.026) ;

  //std::cout<< "test:  " << sum.mean() << std::endl;
  
  /*
  RooChi2Var chi2("chi2","chi2",sum,dh0) ;

  // Start Minuit session on Chi2
  RooMinuit m2(chi2) ;
  m2.migrad() ;
  m2.hesse() ;
  RooFitResult* r2 = m2.save() ;

  // Print results 
  cout << "result of chi2 fit" << endl ;
  r2->Print("v") ;
  */
  TCanvas* c0 = new TCanvas("c0","e_RECO/e_GEN",800,800) ;
  c0->cd();
  gPad->SetLeftMargin(0.19) ; frame0->GetYaxis()->SetTitleOffset(1.8) ;
  gPad->SetRightMargin(0.10) ; frame0->GetXaxis()->SetTitleOffset(1.2) ;
  Double_t chi20 = frame0->chiSquare("sum", "dh0", 9);

  std::cout<<"Chi Square2 for 0=:"<<chi20<<std::endl;


  //std::cout << "mean:: " << frame0->mean("sum", "dh0", 9); << std::endl;

  frame0-> Draw();
  c0->SaveAs("threeGauss_nom_1_new.png");
  // TF1 *g1    = new TF1("g1","gaus",0.9,1.2);
  // TF1 *g2    = new TF1("g2","gaus",0.9,1.2);
  // TF1 *g3    = new TF1("g3","gaus",0.9,1.2);
  // TF1 *total = new TF1("total","gaus(0)+gaus(3)+gaus(6)",0.9,1.2);
  // total->SetLineColor(2);

  // h->Fit(g1,"R");
  // h->Fit(g2,"R+");
  // h->Fit(g3,"R+");
  // g1->GetParameters(&par[0]);
  // g2->GetParameters(&par[3]);
  // g3->GetParameters(&par[6]);
  // total->SetParameters(par);
  // h->Fit(total,"R+");
  }
