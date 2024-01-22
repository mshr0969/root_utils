#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TSystemDirectory.h>
#include <TList.h>
#include <TSystemFile.h>
#include <TROOT.h>
#include <fstream>


// data/にあるrootファイル全てをループして9つ並べてガウスフィット、pdf保存
// ch, ファイル名, パラメータは別途変更
void gaus_fit(TString fileName) {
    gStyle->SetOptStat(10);
    gStyle->SetOptFit(11);

    TFile* file = TFile::Open(fileName, "READ");
    TTree* tree = (TTree*)file->Get("tree");

    TH1D *hist = new TH1D("hist", fileName, 512, 0, 16384);
    tree->Draw("hoge[2]>>hist");

    double FitMin = 3800;
    double FitMax = 5600;

    TF1* func = new TF1("func", "[0]*exp(-0.5*((x-[1])/[2])**2)", FitMin, FitMax);
    func->SetParName(0, "Const");
    func->SetParName(1, "Mean");
    func->SetParName(2, "Sigma");
    func->SetParameter(0, 100.);
    func->SetParameter(1, 4000.);
    func->SetParameter(2, 100.);

    hist->Fit("func", "", "", FitMin, FitMax);

    hist->SetXTitle("ADC");
    hist->SetYTitle("Count");
    func->SetLineColor(kRed);
    hist->Draw();
    func->Draw("same");

    double fitConst = func->GetParameter(0);
    double fitMean = func->GetParameter(1);
    double meanErr = func->GetParError(1);
    double fitSigma = func->GetParameter(2);
    double sigmaErr = func->GetParError(2);
    std::ofstream fout("data.txt", std::ios::app);
    Int_t start = fileName.Index("run") + 3;
    TString run = fileName(start, 4);
    fout << run << " " << fitConst << " " << fitMean << " " << meanErr << " " << fitSigma << " " << sigmaErr <<  "\n";
    fout.close();
}

void gaus_compare() {
    gROOT->SetBatch(kTRUE);
    TString dir = "data/";
    TSystemDirectory sysdir(dir, dir);
    TList *files = sysdir.GetListOfFiles();
    files->Sort();

    TCanvas* canvas = nullptr;
    int canvasCount = 0;
    int padCount = 0;
    int fileCount = files->GetEntries() - 1;

    TString pdfFileName = "pdf/hoge.pdf";


    if (files) {
        TSystemFile *file;
        TString fname;
        TIter next(files);
        while ((file = (TSystemFile*)next())) {
            fname = file->GetName();
            if (!file->IsDirectory() && fname.EndsWith(".root")) {
                if (padCount == 0) {
                    canvas = new TCanvas(Form("Canvas%d", ++canvasCount), "", 1200, 800);
                    canvas->Divide(3, 3);
                    canvas->Print(pdfFileName + "["); // PDFファイルの開始
                }
                if (padCount != 0 & padCount % 9 == 0){
                    canvas->Print(pdfFileName); // 既存のキャンバスをPDFに追加
                    canvas = new TCanvas(Form("Canvas%d", ++canvasCount), "", 1200, 800);
                    canvas->Divide(3, 3);
                }
                canvas->cd(padCount % 9 + 1);
                gaus_fit(dir + "/" + fname);
                padCount++;
                if (padCount == fileCount - 2) {
                    canvas->Print(pdfFileName);
                    canvas->Print(pdfFileName + "]"); // PDFファイルの終了
                    std::cout << "end" << std::endl;
                    delete canvas;
                }
            }
        }
    }
}
