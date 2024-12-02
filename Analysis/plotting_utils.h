#include <TH1D.h>
#include <TH2D.h>
#include <iostream>

using namespace std;

TH1D Make1DPlot(string name, string title, int Nbins, double min, double max) {
    TH1D * hist = new TH1D(name, title, Nbins, min, max);
    // .
    return hist;
}

TH2D Make2DPlot(string name, string title, int NbinsX, double minX, double maxX, int NbinsY, double minY, double maxY) {
    TH1D * hist = new TH2D(name, title, NbinsX, minX, maxX, NbinsY, minY, maxY);
    // .
    return hist;
}
