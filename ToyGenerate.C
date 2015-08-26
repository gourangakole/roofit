// Run me as: root -l ToyGenerate.C


#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"
#include<iostream>
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"

using namespace RooFit ;
using namespace std;
void ToyGenerate()
{

//  TFile *inputFile=new TFile("showerShapeTemplate.root","READ");
//
//  
//  
//  THStack *hall = new THStack("hs","Prompt + NonPrompt SB");
//  
//
//  TH1F *promptSBHisto = (TH1F*)inputFile->Get("sieie_promptHistoIsoSideband_Gjets400");
//  promptSBHisto->SetNameTitle( "promptHistoSB" , "promptHistoSB" );
//  promptSBHisto->Add( (TH1F*)inputFile->Get("sieie_promptHistoIsoSideband_Gjets600") );
//
//
//
//  TH1F *nonPromptSBHisto = (TH1F*)inputFile->Get("sieie_nonPromptHistoIsoSideband_Gjets400");
//  nonPromptSBHisto->SetNameTitle( "nonPromptHistoSB" , "nonPromptHistoSB" );
//  nonPromptSBHisto->Add( (TH1F*)inputFile->Get("sieie_nonPromptHistoIsoSideband_Gjets600") );
//
//
//  hall->Add(promptSBHisto);
//  hall->Add(nonPromptSBHisto);

  //hall->SetLineColor(kCyan);
  //hall->SetLineStyle(2);



  RooRealVar SigmaIe("SigmaIe","SigmaIeta",0.002,0.022) ;

  RooRealVar mean1_sig("mean1_sig","mean1_sig",0.009167,0.006,0.012);
  mean1_sig.setConstant(true);
  RooRealVar sigma1_sig("sigma1_sig","sigma1_sig",0.000552,0.0000023,0.00055200);
  sigma1_sig.setConstant(true);

  RooGaussian Gauss1_sig("Gauss1_sig","Gauss1_sig",SigmaIe,mean1_sig,sigma1_sig);


  RooRealVar sigma2_sig("sigma2_sig","sigma2_sig",0.000300,0.00005,0.0005);
  sigma2_sig.setConstant(true);

  RooGaussian Gauss2_sig("Gauss2_sig","Gauss2_sig",SigmaIe,mean1_sig,sigma2_sig); 


  RooRealVar fracGauss1_sig("fracGauss1","fracGauss1",0.46,0.1,0.6);
  fracGauss1_sig.setConstant(true);


  RooAddPdf Twogausspdf_sig("Twogauss","Twogauss", RooArgList(Gauss1_sig,Gauss2_sig),RooArgList(fracGauss1_sig));



  RooRealVar slope_cheb_sig("slope","slope",1.0,0.9,1.3);
  slope_cheb_sig.setConstant(true);

  RooChebychev cheb_sig("cheb_sig","cheb_sig",SigmaIe,slope_cheb_sig);

  RooRealVar fracTwogauss_sig("fracTwogauss_sig","fracTwogauss_sig",0.948,0.001,1.00);
  fracTwogauss_sig.setConstant(true);


  RooAddPdf TwogaussChebpdf_sig("pdf","total Pdf", RooArgList(Twogausspdf_sig,cheb_sig),RooArgList(fracTwogauss_sig));

  //bkg
  RooRealVar sigma1_bkg("sigma1_bkg","sigma1_bkg",0.00154,0.00002,0.002);
  sigma1_bkg.setConstant(true);

  RooRealVar mean1_bkg("mean1_bkg","mean1_bkg",0.01100,0.011,0.22);
  mean1_bkg.setConstant(true);
  RooGaussian Gauss1_bkg("gtop2","gtop2",SigmaIe,mean1_bkg,sigma1_bkg);



  RooRealVar mean2_bkg("mean2_bkg","mean2_bkg",0.011441,0.002,0.022);
  mean2_bkg.setConstant(true);

  RooRealVar sigma2_bkg("sigma2_bkg","sigma2_bkg",0.00208,0.0000023,0.555200);
  sigma2_bkg.setConstant(true);

  RooRealVar alpha_bkg("alpha_bkg","alpha_bkg",21,0,100);
  alpha_bkg.setConstant(true);

  RooRealVar n_bkg("n_bkg","n_bkg",2,0,100);
  n_bkg.setConstant(true);

  RooRealVar  frac_CB_bkg("frac_CB_bkg","frac_CB_bkg",0.7,0.0,1.0);
  frac_CB_bkg.setConstant(true);

  RooCBShape  crystalBall_bkg("CBall_bkg","Crystal ball",SigmaIe,mean2_bkg,sigma2_bkg,alpha_bkg,n_bkg);


  RooAddPdf CB_Gausspdf("CBGausspdf","CBGausspdf", RooArgList(crystalBall_bkg,Gauss1_bkg),RooArgList(frac_CB_bkg));



  RooRealVar  fracSigBkg("fracSigBkg","fracSigBkg",0.8056722,0,1);
  fracSigBkg.setConstant(false);
  
  
  // build combined model
  RooAddPdf model("model","model",TwogaussChebpdf_sig,CB_Gausspdf,fracSigBkg);



  TH1F *fitVal =new TH1F("fitVal","fitVal",200,0,1);
  fitVal->SetLineWidth(2);
  TH1F *fitError =new TH1F("fitError","fitError",500,0.01,0.04);
  fitError->SetLineWidth(2);

  RooPlot* frame_Sigbkg = SigmaIe.frame(Title("Model Sig+Bkg")) ;

  for(int i=0;i<10;i++){
    mean1_sig.setVal(0.009167);  
    sigma1_sig.setVal(0.000552);
    sigma2_sig.setVal(0.000300);
    fracGauss1_sig.setVal(0.46);
    slope_cheb_sig.setVal(0.90); 
    fracTwogauss_sig.setVal(0.948);
    mean1_bkg.setVal(0.01100);
    sigma1_bkg.setVal(0.00154);
    mean2_bkg.setVal(0.01441);
    sigma2_bkg.setVal(0.00208);
    alpha_bkg.setVal(21);
    n_bkg.setVal(2);
    frac_CB_bkg.setVal(0.357);
    fracSigBkg.setVal(0.8056722);
   

    //Entries = 32229(signal)+10853(bkg) = 43082
    RooDataSet *data = model.generate(SigmaIe,344) ;


    model.fitTo(*data);
  
    fitVal->Fill( fracSigBkg.getVal() );

    fitError->Fill( fracSigBkg.getError() / fracSigBkg.getVal() );
   

    TLegend* leg2=new TLegend(0.6,0.75,0.8,0.95);         
  
    leg2->SetTextFont(62);
    leg2->SetLineColor(1);
    leg2->SetLineStyle(1);
    leg2->SetLineWidth(3);
    leg2->SetFillColor(0);
    leg2->SetFillStyle(1001);
    leg2->SetShadowColor(0);
    leg2->SetDrawOption(0);
    leg2->SetBorderSize(0);
    leg2->SetTextSize(0.03);





    /*char Legname1[100];
TLegend *leg_1D[24];
for(int k0=0;k0<24;k0++){
sprintf(Legname1,"leg_1D%i",k0);
leg_1D[k0]=new TLegend(0.5,0.7,0.80,0.89);
leg_1D[k0]->SetTextFont(62);
leg_1D[k0]->SetLineColor(1);
leg_1D[k0]->SetLineStyle(1);
leg_1D[k0]->SetLineWidth(3);
leg_1D[k0]->SetFillColor(0);
leg_1D[k0]->SetFillStyle(1001);
leg_1D[k0]->SetShadowColor(0);
leg_1D[k0]->SetDrawOption(0);
leg_1D[k0]->SetBorderSize(0);
leg_1D[k0]->SetTextSize(0.03);
}
    */










    if(i==0){

      data->plotOn(frame_Sigbkg) ;
      model.plotOn(frame_Sigbkg, Name("Fit"));
    

      model.plotOn(frame_Sigbkg, Components(TwogaussChebpdf_sig), Name("Signal"),LineColor(kRed),LineStyle(2), LineWidth(4));
      model.plotOn(frame_Sigbkg, Components(CB_Gausspdf), Name("Bkg"), LineColor(kGreen),LineStyle(2), LineWidth(4));

      frame_Sigbkg->Draw(); 
      leg2->AddEntry("Fit","Fit(1 fb^{-1})","l"); leg2->AddEntry("Signal","Signal","l");leg2->AddEntry("Bkg","Bkg","l");
      leg2->Draw();        
   
      //TwogaussChebpdf_sig.plotOn(frame_Sigbkg);
      //    CB_Gausspdf.plotOn(frame_Sigbkg);


    }


  }


  


  //RooPlot* frame_Sigbkg = SigmaIe.frame(Title("Model Sig+Bkg")) ;

  //data->plotOn(frame_Sigbkg) ;
  //model.plotOn(frame_Sigbkg);


  TCanvas *canModel =new TCanvas("canModel","canModel");
  canModel->cd();
  frame_Sigbkg->Draw();
  //hall->Draw("SAME");


  TCanvas *canFitVal =new TCanvas("canFitVal","canFigVal",500,500);
  canFitVal->cd();
  fitVal->Draw();

  TCanvas *canFitErr =new TCanvas("canFitErr","canFitErr",500,500);
  canFitErr->cd();
  fitError->Draw();


  canModel->SaveAs("canModel.pdf");
  canFitVal->SaveAs("canFitVal.pdf");
  canFitErr->SaveAs("canFitErr.pdf");


  TCanvas *call=new TCanvas("call","call");

  call->cd();
  //hall->Draw();

  //cout<<"hall integral = "<<hall->GetEntriesFast()<<endl;


//  cout<<"Prompt Histo : "<<promptSBHisto->Integral()<<endl;

//  cout<<"Non prompt Histo : "<<nonPromptSBHisto->Integral()<<endl;







}
