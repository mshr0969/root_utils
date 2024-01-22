#include <fstream>
#include <TStyle.h>
#include <TH1D.h>
#include <TF1.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TFile.h>

// hoge[ch]をヒストグラム描画
void hist(TString fileName) {
    TString ch = "2";
    gStyle->SetOptStat(10);
    TCanvas *canvas = new TCanvas("canvas", "", 800, 600);
    TFile* file = TFile::Open(fileName, "READ");
    TTree* tree = dynamic_cast<TTree*>(file->Get("tree"));

    // Create histogram
    TH1D *hist = new TH1D("hist", "", 512, 0, 16384);
    tree->Draw("hoge["+ch+"]>>hist");
    hist->SetXTitle("ADC");
    hist->SetYTitle("Count");

    // Show canvas
    canvas->Draw();
}
