# include <fstream>
# include <TH2D.h>
# include <TStyle.h>
# include <TGraphErrors.h>
# include <TF1.h>

// x,yの範囲はTH2Dで指定
void tgraph () {
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);
    TH2D* hist = new TH2D("hist", "title", 100, 100, 230, 100, 0, 5);
    hist->Draw("AXIS");
    hist->SetXTitle("x title");
    hist->SetYTitle("y title");
    TGraphErrors* g1 = new TGraphErrors();
    std::ifstream fin1("data.txt");
    double run, con, mean, meanErr, sigma, sigmaErr, gev;
    int index = 0;
    while (fin1 >> run >> con >> mean >> meanErr >> sigma >> sigmaErr) {
        g1->SetPoint(index, run, mean);
        g1->SetPointError(index, 0, meanErr);
        index++;
    }
    g1->SetMarkerStyle(8);
    g1->Draw("P same");
}
