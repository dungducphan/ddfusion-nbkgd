#include "NeutronAna.h"
#include "THStack.h"
#include "TGraph.h"
#include "TGraphErrors.h"

void Ana() {
    NeutronAna* n_1d5 = new NeutronAna(1.5);
    NeutronAna* n_2d0 = new NeutronAna(2.0);
    NeutronAna* n_2d5 = new NeutronAna(2.5);
    NeutronAna* n_3d0 = new NeutronAna(3.0);
    NeutronAna* n_3d5 = new NeutronAna(3.5);
    NeutronAna* n_4d0 = new NeutronAna(4.0);

    TH1D* h_1d5 = n_1d5->GetEneHist();
    TH1D* h_2d0 = n_2d0->GetEneHist();
    TH1D* h_2d5 = n_2d5->GetEneHist();
    TH1D* h_3d0 = n_3d0->GetEneHist();
    TH1D* h_3d5 = n_3d5->GetEneHist();
    TH1D* h_4d0 = n_4d0->GetEneHist();

    THStack *hs = new THStack("hs","");
    h_1d5->SetLineColor(kViolet);
    h_2d0->SetLineColor(kBlue);
    h_2d5->SetLineColor(kCyan);
    h_3d0->SetLineColor(kGreen);
    h_3d5->SetLineColor(kOrange);
    h_4d0->SetLineColor(kRed);
    hs->Add(h_1d5);
    hs->Add(h_2d0);
    hs->Add(h_2d5);
    hs->Add(h_3d0);
    hs->Add(h_3d5);
    hs->Add(h_4d0);

    TGraphErrors* gr = new TGraphErrors();
    gr->AddPoint(1.5, h_1d5->GetMean());
    gr->SetPointError(0, 0, h_1d5->GetStdDev());
    gr->AddPoint(2.0, h_2d0->GetMean());
    gr->SetPointError(1, 0, h_2d0->GetStdDev());
    gr->AddPoint(2.5, h_2d5->GetMean());
    gr->SetPointError(2, 0, h_2d5->GetStdDev());
    gr->AddPoint(3.0, h_3d0->GetMean());
    gr->SetPointError(3, 0, h_3d0->GetStdDev());
    gr->AddPoint(3.5, h_3d5->GetMean());
    gr->SetPointError(4, 0, h_3d5->GetStdDev());
    gr->AddPoint(4.0, h_4d0->GetMean());
    gr->SetPointError(5, 0, h_4d0->GetStdDev());

    TCanvas* c = new TCanvas("c", "c", 1800, 900);
    c->Divide(2, 1);

    c->cd(1);
    hs->Draw("nostack");

    c->cd(2);
    gr->Draw("APL");

    c->SaveAs("CompareExE.pdf");
}