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
pull_TTbar_v2()
{
  TCanvas* c = new TCanvas("c","test",570,400);
  TFile *f0 = new TFile("./Mu_histograms_2J_1TTest_2_new.root");
  // Declare observable mtop
  RooRealVar mtop("mtop","M (GeV/c^{2})",80,360) ;
  RooDataHist ds("ds","ds",mtop,Import(*Mu_topMass__TTBar)) ;
  // Print number of events in dataset
  ds.Print();
  // Print binned dataset with default frame 
  RooPlot* frame = mtop.frame(Title("TTBar")) ;
  
  ds.plotOn(frame) ;
  
  RooRealVar mean("mean","mean",167.0,165,170);
  RooRealVar sigma("sigma","sigma",22.04,18.0,30.0);
  RooGaussian gtop("gtop","gtop",mtop,mean,sigma);

  RooRealVar sigma1("sigma1","sigma1",20.5,20.0,30.0);
  RooRealVar sigma2("sigma2","sigma2",65.5,65.0,75.0);
  RooBifurGauss bgtop("bgtop","bgtop",mtop,mean,sigma1,sigma2);

  RooRealVar frac0("frac0","frac0",0.25,0.1,0.6);
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
  std::cout<<"AMAR CHI^2 ="<<frame->chiSquare()<<std::endl;  

  // pull 
  RooPlot* z1frame = mtop.frame(Title("TTBar"));
  RooHist* hpull1 = frame->pullHist();
  z1frame->addPlotable(hpull1,"P");

  RooPlot* z2frame = mtop.frame(Title("TTBar"));
  RooHist* hresid = frame->residHist();
  z2frame->addPlotable(hresid,"P") ;

  TCanvas* c2 = new TCanvas("pull1","pull1",900,300) ;
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
  // TPaveText *t1 = new TPaveText(0.2,0.6,0.3,0.68);//, signalchi));
  // t1->AddText("signalchi");
  // t1->Draw(); 
  // frame->addObject(t1);
  // frame->Draw();
    
  // RooPlot* z1frame = mtop.frame(Title("Top Mass(m_{top} = 178.5)"));
  // RooHist* hpull1 = frame->pullHist();
  // z1frame->addPlotable(hpull1,"P");
  
  // RooPlot* z2frame = mtop.frame(Title("Top Mass(m_{top} = 178.5)"));
  // RooHist* hresid = frame->residHist();
  // z2frame->addPlotable(hresid,"P") ;
  
  // TCanvas* c2 = new TCanvas("pull1","pull1",900,300) ;
  // c2->Divide(3,1) ; // column, row
  // double xmin = 80.0; double xmax = 360.0;
  // TLine *line = new TLine(xmin,0.0,xmax,0.0);
  // c2->cd(1); gPad->SetLeftMargin(0.15); frame->GetYaxis()->SetTitleOffset(1.6); frame->Draw();
  // c2->cd(2); line->SetLineColor(kRed); line->SetLineWidth(2); gPad->SetLeftMargin(0.15); z1frame->GetYaxis()->SetTitleOffset(1.6); z1frame->GetYaxis()->SetRangeUser(-5.0, 5.0); z1frame->Draw(); line->Draw("SAME");
  // c2->cd(3); gPad->SetLeftMargin(0.15); z2frame->GetYaxis()->SetTitleOffset(1.6); z2frame->Draw(); line->Draw("SAME");
  
  //c->SaveAs("MassDown.png");
  c2->SaveAs("TTbar_pull.root");
}
