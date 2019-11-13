#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"

using namespace RooFit ;
void pull_WJet_v2()
{

  //TCanvas* c = new TCanvas("c","test",880,500);
  TFile *f0 = new TFile("./Mu_histograms_2J_1TTest_2_new.root");
  // Get the histogram
  TH1F *h1 = (TH1F*)(f0->Get("Mu_topMass__WJets"))->Clone("h1");
  // Declare observable mtop
  RooRealVar mtop("mtop","M (GeV/c^{2})",80,360) ;
  RooRealVar mtop1("mtop1","M (GeV/c^{2})",120,220) ;
  RooDataHist ds("ds","ds",mtop,Import(*h1)) ;
  // Print number of events in dataset
  ds.Print();
  // Print binned dataset with default frame 
  RooPlot* frame = mtop.frame(Title("WJets")) ;
  
  ds.plotOn(frame) ;
  
  RooRealVar mean("mean","mean",167.0,165,200);
  RooRealVar sigma("sigma","sigma",22.04,18.0,30.0);
  RooRealVar mean2("mean2","mean",155.5,150,160);
  RooGaussian gtop("gtop","gtop",mtop,mean,sigma);

  RooRealVar sigma1("sigma1","sigma1",16.5,15.0,25.0);
  RooRealVar sigma2("sigma2","sigma2",85.5,70.0,100.0);
  RooBifurGauss bgtop("bgtop","bgtop",mtop,mean2,sigma1,sigma2);

  RooRealVar frac0("frac0","frac0",0.96,0.955,0.999);
  RooAddPdf pdfg("pdfg","double gaussian Pdf", RooArgList(bgtop,gtop),RooArgList(frac0));
  RooRealVar slope("slope","slope",1.0,0.9,1.3);
  RooChebychev cheb("cheb","cheb",mtop,slope);
  RooRealVar frac("frac","frac",0.95,0.75,1.0);
  
  //  RooAddPdf pdf("pdf","total Pdf", RooArgList(gtop,cheb),RooArgList(frac));
  RooAddPdf pdf("pdf","total Pdf", RooArgList(pdfg,cheb),RooArgList(frac));

  /*  
  gtop.fitTo(ds);
  gtop.plotOn(frame);
  gtop.paramOn(frame);
  */

  /*
  bgtop.fitTo(ds);
  bgtop.plotOn(frame);
  bgtop.paramOn(frame);
  pdf.plotOn(frame,Components(RooArgList(bgtop)), LineColor(kRed),LineStyle(2), LineWidth(4));
  frame->Draw();
  */
  
      
  pdf.fitTo(ds);
  pdf.plotOn(frame);
  pdf.paramOn(frame);
  std::cout<<"Gouranga kole's chi square=================================== ="<<frame->chiSquare()<<std::endl;  

  // pull 
  RooPlot* z1frame = mtop.frame(Title("WJets"));
  RooHist* hpull1 = frame->pullHist();
  z1frame->addPlotable(hpull1,"P");

  RooPlot* z2frame = mtop.frame(Title("WJets"));
  RooHist* hresid = frame->residHist();
  z2frame->addPlotable(hresid,"P") ;

  TCanvas* c2 = new TCanvas("pull1","pull1",850,500) ;
  c2->Divide(2,1) ; // column, row
  double xmin = 80.0; double xmax = 360.0;
  TLine *line = new TLine(xmin,0.0,xmax,0.0);
 
  c2->cd(2); line->SetLineColor(kRed); line->SetLineWidth(2); gPad->SetLeftMargin(0.15); z1frame->GetYaxis()->SetTitleOffset(1.6); z1frame->GetYaxis()->SetRangeUser(-5.0, 5.0); z1frame->Draw(); line->Draw("SAME");
  //  c2->cd(1); gPad->SetLeftMargin(0.15); z2frame->GetYaxis()->SetTitleOffset(1.6); z2frame->Draw(); line->Draw("SAME");
  c2->cd(1); gPad->SetLeftMargin(0.15); frame->GetYaxis()->SetTitleOffset(1.6); frame->Draw();
  
 pdf.plotOn(frame,Components(RooArgList(gtop)), LineColor(kRed),LineStyle(2), LineWidth(4));
 pdf.plotOn(frame,Components(RooArgList(cheb)), LineColor(kBlue),LineStyle(2), LineWidth(4));
 pdf.plotOn(frame,Components(RooArgList(bgtop)), LineColor(kGreen),LineStyle(2), LineWidth(4));
  
  

 frame->Draw();
  // Double_t signalchi = frame->chiSquare(); 
 
  c2->SaveAs("TTbar_pull.root");


}
