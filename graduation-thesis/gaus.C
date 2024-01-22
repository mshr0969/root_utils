#include <fstream>
#include <TStyle.h>
#include <TH1D.h>
#include <TF1.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TFile.h>

// dir/<run_num>.rootからヒストグラム描画、ガウスフィット
// data.txtにrun_num, fit min, gaus mean, fit maxを用意
// hoge[ch]を指定
void gaus(TString run) {
    TString ch = "2";
    gStyle->SetOptStat(10);
    gStyle->SetOptFit(111);
    TCanvas *canvas = new TCanvas("canvas", "", 800, 600);
    TFile* file = TFile::Open("dir/" + run + ".root", "READ");
    TTree* tree = dynamic_cast<TTree*>(file->Get("tree"));

    // Create histogram
    TH1D *hist = new TH1D("hist", "", 512, 0, 16384);
    tree->Draw("hoge["+ch+"]>>hist");

    std::ifstream fin1("data.txt");
    double FitMin, Mean, FitMax;
    double fmin, m, fmax;
    TString run_num;
    while (fin1 >> run_num >> fmin >> m >> fmax) {
        if (run_num == run) {
            FitMin = fmin;
            Mean = m;
            FitMax = fmax;
        }
    }

    // Prepare fitting functions
    TF1* func = new TF1("func", "[0]*exp(-0.5*((x-[1])/[2])**2)", FitMin, FitMax);

    // Set parameter names
    func->SetParName(0, "Const");
    func->SetParName(1, "Mean" );
    func->SetParName(2, "Sigma");

    // Set initial parameters
    func->SetParameter(0, 100.);
    func->SetParameter(1, Mean);
    func->SetParameter(2, 10.);

    // Fitting
    hist->Fit("func", "", "", FitMin, FitMax);
    hist->SetXTitle("ADC");
    hist->SetYTitle("Count");
    // Get and print the overall mean value
    double overallMean = hist->GetMean();

    // Draw fit function
    func->SetLineColor(kRed);
    func->Draw("same");

    // Show canvas
    canvas->Draw();
}
