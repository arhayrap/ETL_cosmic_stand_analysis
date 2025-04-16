#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <math.h>

using namespace std;

/*
double correct(double scale, double bias, double x, double y, double y_max) {
    double y_pred = scale*x + bias;
    double correction = y_max - y_pred;
    return y + correction;
}

double correct(double value, std::vector<double> params, double x) {
    double y_pred = params->at(0);
    for (int i = 1; i <= n_degrees; i++) {
        y_pred += scale*x;
    }
    double corrected = value - y_pred;
    return corrected;
    // TF1 * line = new TF1("Linear", "[0]+[1]*pow(x,1)");
    // double scale = line->GetParameter(0);
    // double bias = line->GetParameter(1);
    // params.push_back(scale);
    // params.push_back(bias);
    // std::cout<<scale<<"   "<<bias<<std::endl;
    //     formula += "+[" + std::atoi(i) + "]*pow(x," + std::atoi(i) + ")";
    // line->Draw();
}*/

std::vector<double> timewalk_correction(TH2D * histo, int n_degrees) {
    TH1D * x = (TH1D*)histo->ProjectionX();
    TH1D * y = (TH1D*)histo->ProjectionY();
    std::string formula = "[0]";
    for (int i = 1; i <= n_degrees; i++) {
        formula += "+[" + std::to_string(i) + "]*pow(x," + std::to_string(i) + ")";
    }
    std::cout<<formula<<std::endl;
    double minx = x->GetBinCenter(x->GetMinimumBin());
    double maxx = x->GetBinCenter(x->GetMaximumBin());
    double miny = y->GetMean() + 2 * y->GetRMS();
    double maxy = y->GetMean() - 2 * y->GetRMS();
    TF1 * line = new TF1("Linear", formula.c_str(), minx, maxx);
    std::cout<<minx<<"   "<<maxx<<std::endl;
    std::cout<<miny<<"   "<<maxy<<std::endl;
    std::vector<double> params;
    histo->Fit(line);
    for (int i = 0; i <= n_degrees; i++) {
        double param = line->GetParameter(i);
        params.push_back(param);
    }
    TCanvas * c = new TCanvas("tw_corr", "tw_corr", 1200, 1200);
    histo->Draw("COLZ");
    line->SetLineWidth(3);
    line->SetLineColor(kRed);
    line->Draw("SAME");
    c->SaveAs("tw_corr.png");
    c->Close();
    return params;
}

void Analysis(string filename) {
    
    string base = "../MergedData/";
    string full_path = base + filename;
    string output_file_path = "./output_files/" + filename;
    cout<<"Reading the file"<<endl;
    TFile * file  = TFile::Open(full_path.c_str());
    TTree * pulse = (TTree * ) file -> Get("pulse");
    
    string print_variables[] = {
        "toa_code*3.125/cal_code", \
        "(tot_code*2 - floor(tot_code/32))*3.125/cal_code", \
        "cal_code", \
        "nhits", \
        "LP2_20[0]*1e9", \
        "LP2_20[1]*1e9", \
        "LP2_20[2]*1e9", \
        "amp[0]", \
        "amp[1]", \
        "amp[2]", \
        "(toa_code*3.125/cal_code)/(Clock[0])", \
        "toa_code", \
        "tot_code", \
        "((toa_code*3.125/cal_code)-(Clock[0] - LP2_20[2]*1e9))", \
        "(LP2_20[3]-LP2_20[2])*1e9", \
        "(LP2_20[4]-LP2_20[2])*1e9", \
        "(LP2_20[5]-LP2_20[2])*1e9", \
        "(LP2_20[6]-LP2_20[2])*1e9", \
    };
    
    string pixel_mask = "!((row==9&&col==2)||(row==2&&col==4))";      // !((col==0&&row==13)||(col==12&&row==15)||(col==12&&row==4)||(col==15&&row==6)||(col==11&&row==15)||(col==12&&row==11)||(col==8&&row==0))";
    string feducial_region = ""; // "(row==4&&col==8)";
    // string condition = "nhits>0&&cal_code>140&&cal_code<190&&((LP2_20[3]!=0.0)||(LP2_20[4]!=0.0)||(LP2_20[5]!=0.0)||(LP2_20[6]!=0.0))"; // (2*tot_code-floor(tot_code/32))*3.125/cal_code>2";
    // string condition = "amp[2]>400&&amp[2]<1200&&nhits>0&&cal_code>170&&cal_code<190&&(2*tot_code-floor(tot_code/32))*3.125/cal_code>0"; // (LP2_20[2]!=0&&amp[2]>400&&amp[2]<1200)"; // &&((LP2_20[3]!=0.0&&amp[3]>=40&&amp[3]<=120)||(LP2_20[4]!=0.0&&amp[4]>=40&&amp[4]<=120)||(LP2_20[5]!=0.0&&amp[5]>=40&&amp[5]<=120)||(LP2_20[6]!=0.0&&amp[6]>=40&&amp[6]<=120))";
    string condition = "amp[2]>400&&amp[2]<1200&&nhits>0&&cal_code>170&&cal_code<190&&(2*tot_code-floor(tot_code/32))*3.125/cal_code>2&&(2*tot_code-floor(tot_code/32))*3.125/cal_code<10"; // (LP2_20[2]!=0&&amp[2]>400&&amp[2]<1200)"; // &&((LP2_20[3]!=0.0&&amp[3]>=40&&amp[3]<=120)||(LP2_20[4]!=0.0&&amp[4]>=40&&amp[4]<=120)||(LP2_20[5]!=0.0&&amp[5]>=40&&amp[5]<=120)||(LP2_20[6]!=0.0&&amp[6]>=40&&amp[6]<=120))";
    // &&((amp[3]>=40&&amp[3]<=120)||(amp[4]>=40&&amp[4]<=120)||(amp[5]>=40&&amp[5]<=120)||(amp[6]>=40&&amp[6]<=120))&&(amp[2]>=400&&amp[2]<=1200)"; // (2*tot_code-floor(tot_code/32))*3.125/cal_code>2";
    condition = condition; // + "&&" + feducial_region;

    string labels[] = {
        "TOA [ns]", \
        "TOT [ns]", \
        "CAL", \
        "N_{hits}", \
        "SiPm 2 (bottom) [ns]", \
        "SiPm 1 (top) [ns]", \
        "Photek [ns]", \
        "SiPm 2 (bottom) [mV]", \
        "SiPm 1 (top) [mV]", \
        "Photek [mV]", \
        "TOA_Clock", \
        "TOA code", \
        "TOT code", \
        "#Delta T [ns]", \
        "#Delta T [ns] (LGAD pixel 1)", \
        "#Delta T [ns] (LGAD pixel 2)", \
        "#Delta T [ns] (LGAD pixel 3)", \
        "#Delta T [ns] (LGAD pixel 4)", \
    };
    
    string histnames[] = {
        "TOA", \
        "TOT", \
        "CAL", \
        "Nhits", \
        "SiPm2_bottom", \
        "SiPm1_top", \
        "Photek", \
        "SiPm2_bottom_amp", \
        "SiPm1_top_amp", \
        "Photek_amp", \
        "TOA_Clock", \
        "TOA_code", \
        "TOT_code", \
        "DeltaT_to_fit", \
        "DeltaT_LGAD_1", \
        "DeltaT_LGAD_2", \
        "DeltaT_LGAD_3", \
        "DeltaT_LGAD_4", \
    };
    
    int n_vars = sizeof(print_variables) / sizeof(print_variables[0]);
    int c_cols = 4;
    int TIMEWALK_FIT_FUNCTION_DEGREES = 5;
    int c_rows = ceil(n_vars*1.0 / c_cols);
    int plotsize = 400;
    int plotting_index = 0;
    int pixel_combine = 16;
    TFile * output_file = new TFile(output_file_path.c_str(), "RECREATE");
    TCanvas * c = new TCanvas("Histograms", "Histograms", c_cols*plotsize, c_rows*plotsize);
    cout<<c_cols<<" "<<c_rows<<endl;
    c->Divide(c_cols, c_rows);
    for (int i = 0; i < n_vars; i++) { // 1D histograms
        // string condition = ; // cal_code>160&&cal_code<190&&nhits>0&&(2*tot_code-floor(tot_code/32))*3.125/cal_code>2";
        plotting_index++;
        c->cd(plotting_index);
        string new_histname = histnames[i];
        string tmp_condition = condition;
        string prefix = "";
        if (histnames[i] == "Nhits") {
            prefix = print_variables[i] +">>"+new_histname + "(512,0,256)";
        }
        else if (histnames[i] == "DeltaT_to_fit") {
             prefix = print_variables[i] +">>"+new_histname;
        }
        else if (histnames[i] == "DeltaT_LGAD_1") { // || histnames[i] == "DeltaT_LGAD_2" || histnames[i] == "DeltaT_LGAD_3" || histnames[i] == "DeltaT_LGAD_4") {
            prefix = print_variables[i] +">>"+new_histname+"(30,4.7,5.3)";
            tmp_condition += "&&(LP2_20[3]!=0.0&&amp[3]>=40&&amp[3]<=120)";
        }
        else if (histnames[i] == "DeltaT_LGAD_2") { // || histnames[i] == "DeltaT_LGAD_2" || histnames[i] == "DeltaT_LGAD_3" || histnames[i] == "DeltaT_LGAD_4") {
            prefix = print_variables[i] +">>"+new_histname+"(30,4.7,5.3)";
            tmp_condition += "&&(LP2_20[4]!=0.0&&amp[4]>=40&&amp[4]<=120)";
        }
        else if (histnames[i] == "DeltaT_LGAD_3") { // || histnames[i] == "DeltaT_LGAD_2" || histnames[i] == "DeltaT_LGAD_3" || histnames[i] == "DeltaT_LGAD_4") {
            prefix = print_variables[i] +">>"+new_histname+"(30,4.7,5.3)";
            tmp_condition += "&&(LP2_20[5]!=0.0&&amp[5]>=40&&amp[5]<=120)";
        }
        else if (histnames[i] == "DeltaT_LGAD_4") { // || histnames[i] == "DeltaT_LGAD_2" || histnames[i] == "DeltaT_LGAD_3" || histnames[i] == "DeltaT_LGAD_4") {
            prefix = print_variables[i] +">>"+new_histname+"(30,4.7,5.3)";
            tmp_condition += "&&(LP2_20[6]!=0.0&&amp[6]>=40&&amp[6]<=120)";
        }
        else {
            prefix = print_variables[i] +">>"+new_histname;
        }
        pulse->Draw(prefix.c_str(), tmp_condition.c_str(), "");
        TH1D * tmp_hist = (TH1D*)gDirectory->Get(new_histname.c_str());
        tmp_hist->SetTitle("");
        tmp_hist->GetYaxis()->SetTitle("Events");
        tmp_hist->GetXaxis()->SetTitle(labels[i].c_str());
        tmp_hist->Write();
    }

    string print_variables_2D[] = {
        "(tot_code*2 - floor(tot_code/32))*3.125/cal_code:toa_code*3.125/cal_code", \
        "Clock:toa_code*3.125/cal_code", \
        "LP2_20[0]:LP2_20[1]", \
        "amp[0]:amp[1]", \
        "(tot_code*2 - floor(tot_code/32))*3.125/cal_code:(row*16+col)", \
        "row:col", \
        "toa_code*3.125/cal_code:LP2_20[2]", \
        "Clock:(tot_code*2 - floor(tot_code/32))*3.125/cal_code", \
        "((toa_code*3.125/cal_code)-(Clock[0] - LP2_20[2]*1e9)):(row*16+col)", \
        "((toa_code*3.125/cal_code)-(Clock[0] - LP2_20[2]*1e9)):(tot_code*2 - floor(tot_code/32))*3.125/cal_code", \
        "((toa_code*3.125/cal_code)-(Clock[0] - LP2_20[2]*1e9)):(toa_code*3.125/cal_code)", \
        "((toa_code*3.125/cal_code)-(Clock[0] - LP2_20[2]*1e9)):(cal_code)", \
        "LP2_20[2]:amp[2]", \
    };

    string histnames_2D[] = {
        "TOA_vs_TOT", \
        "TOA_vs_Clock", \
        "sipm1_vs_sipm2_amp", \
        "sipm1_vs_sipm2_ns", \
        "TOT_vs_pixelid", \
        "HITMAP", \
        "TOA_vs_MCP", \
        "TOT_vs_Clock", \
        "DeltaT_vs_pixelid", \
        "DeltaT_vs_TOT", \
        "DeltaT_vs_TOA", \
        "DeltaT_vs_CAL", \
        "MCP_T_MCP_amp", \
    };

    string labels_x[] = {
        "TOA", \
        "TOA", \
        "sipm1_amp", \
        "sipm1_ns", \
        "tot", \
        "col", \
        "MCP", \
        "TOT", \
        "pixelid", \
        "TOT", \
        "TOA", \
        "CAL", \
        "A_{MCP} [mV]", \
    };

    string labels_y[] = {
        "TOT", \
        "Clock", \
        "sipm2_amp", \
        "sipm2_ns", \
        "pixelid", \
        "row", \
        "TOA", \
        "Clock", \
        "#Delta T", \
        "#Delta T", \
        "#Delta T", \
        "#Delta T", \
        "T_{MCP} [ns]", \
    };

    n_vars = sizeof(print_variables_2D) / sizeof(print_variables_2D[0]);

    for (int i = 0; i < n_vars; i++) { // 2D histograms
        string new_histname = histnames_2D[i];
        string prefix = "";
        if (histnames_2D[i] == "TOT_vs_pixelid") {
            prefix = print_variables_2D[i] +">>"+new_histname + "(256,0,256,100,0,20)";
        } else {
            prefix = print_variables_2D[i] +">>"+new_histname;
        }
        pulse->Draw(prefix.c_str(), condition.c_str(), "colz");
        TH2D * tmp_hist = (TH2D*)gDirectory->Get(new_histname.c_str());
        tmp_hist->SetTitle("");
        tmp_hist->GetXaxis()->SetTitle(labels_x[i].c_str());
        tmp_hist->GetYaxis()->SetTitle(labels_y[i].c_str());
        tmp_hist->Write();
        if (histnames_2D[i] == "DeltaT_vs_TOT") {
            std::cout<<"timewalk correction"<<std::endl;
            std::vector<double> params = timewalk_correction(tmp_hist, TIMEWALK_FIT_FUNCTION_DEGREES);
            // double scale = params.at(0);
            // double bias = params.at(1);
            // double y_max = tmp_hist->ProjectionY()->GetMean();
            std::string predicted = std::to_string(params.at(0));
            for (int i = 1; i <= TIMEWALK_FIT_FUNCTION_DEGREES; i++) {
                predicted += "+" + std::to_string(params.at(i)) + "*pow((tot_code*2-floor(tot_code/32))*3.125/cal_code," + i + ")";
            }
            // string corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)+abs(" + std::to_string(y_max) + "-((" + std::to_string(scale) + "*(tot_code*2-floor(tot_code/32))*3.125/cal_code+" + std::to_string(bias) + ")))):(tot_code*2-floor(tot_code/32))*3.125/cal_code>>DeltaT_vs_TOT_corr";
            string corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)-(" + predicted + ")):(tot_code*2-floor(tot_code/32))*3.125/cal_code>>DeltaT_vs_TOT_corr";
            std::cout<<corr_prefix<<std::endl;
            // std::cout<<y_max<<std::endl;
            pulse->Draw(corr_prefix.c_str(), condition.c_str(), "colz");
            TH2D * tmp_hist_corr = (TH2D*)gDirectory->Get("DeltaT_vs_TOT_corr");
            tmp_hist_corr->SetTitle("");
            tmp_hist_corr->GetXaxis()->SetTitle(labels_x[i].c_str());
            tmp_hist_corr->GetYaxis()->SetTitle(labels_y[i].c_str());
            tmp_hist_corr->Write();
            // corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)+abs(" + std::to_string(y_max) + "-((" + std::to_string(scale) + "*(tot_code*2-floor(tot_code/32))*3.125/cal_code+" + std::to_string(bias) + "))))>>DeltaT_corr"; // (100,-3,7)";
            std::cout<<predicted<<std::endl;
            corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)-(" + predicted + "))>>DeltaT_corr(100,-2,2)";
            std::cout<<corr_prefix<<std::endl;
            pulse->Draw(corr_prefix.c_str(), condition.c_str(), "HIST");
            TH1D * tmp_hist_corr_dt = (TH1D*)gDirectory->Get("DeltaT_corr");
            double mean = tmp_hist_corr_dt->GetMean();
            double std = tmp_hist_corr_dt->GetRMS();
            TF1 * gaus_corr = new TF1("gaus", "gaus");
            tmp_hist_corr_dt->SetTitle("");
            tmp_hist_corr_dt->GetXaxis()->SetTitle("#Delta T");
            tmp_hist_corr_dt->GetYaxis()->SetTitle("Events");
            tmp_hist_corr_dt->Write();
            // tmp_hist_corr_dt->Rebin(5);
            gStyle->SetOptStat(1110);
            gStyle->SetOptFit(1111);
            TCanvas * c_corr = new TCanvas("c_corr", "c_corr", plotsize*pixel_combine, plotsize*pixel_combine);
            gaus_corr->SetRange(mean-2*std,mean+2*std);
            tmp_hist_corr_dt->Fit(gaus_corr, "0");
            tmp_hist_corr_dt->Draw("HIST E");
            gaus_corr->Draw("same");
            c_corr->SaveAs("DeltaT_corr.png");

            corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9))>>DeltaT(100,12,16)";
            std::cout<<corr_prefix<<std::endl;
            // std::cout<<y_max<<std::endl;
            pulse->Draw(corr_prefix.c_str(), condition.c_str(), "HIST");
            tmp_hist_corr_dt = (TH1D*)gDirectory->Get("DeltaT");
            mean = tmp_hist_corr_dt->GetMean();
            std = tmp_hist_corr_dt->GetRMS();
            TF1 * gaus = new TF1("gaus", "gaus");
            tmp_hist_corr_dt->SetTitle("");
            tmp_hist_corr_dt->GetXaxis()->SetTitle("#Delta T");
            tmp_hist_corr_dt->GetYaxis()->SetTitle("Events");
            tmp_hist_corr_dt->Write();
            // tmp_hist_corr_dt->Rebin(5);
            gStyle->SetOptStat(1110);
            gStyle->SetOptFit(1111);
            TCanvas * c = new TCanvas("c", "c", plotsize*pixel_combine, plotsize*pixel_combine);
            gaus->SetRange(mean-2*std,mean+2*std);
            tmp_hist_corr_dt->Fit(gaus, "0");
            tmp_hist_corr_dt->Draw("HIST E");
            gaus->Draw("same");
            c->SaveAs("DeltaT.png");

        }
    }

    std::cout<<"pixel or combined pixel analysis deltaT"<<std::endl;
    for (int i = 0; i < 16/pixel_combine; i++) {
        for (int j = 0; j < 16/pixel_combine; j++) {
            cout<<"Pixel combination: "<<i<<"   "<<j<<endl;
            string new_condition = condition + "&&row>=" + std::to_string(i*pixel_combine) + "&&row<" + std::to_string((i+1)*pixel_combine) + "&&col>=" + std::to_string(j*pixel_combine) + "&&col<" + std::to_string((j+1)*pixel_combine);
            cout<<new_condition<<endl;
            pulse->Draw(("((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9))>>DeltaT_pixel_"+std::to_string(i)+"_"+std::to_string(j)).c_str(), new_condition.c_str(), "HIST E");
            TH1D * tmp_hist = (TH1D*)gDirectory->Get(("DeltaT_pixel_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
            tmp_hist->SetTitle("");
            tmp_hist->Write();
        }
    }

    std::cout<<"pixel or combined pixel analysis TOT"<<std::endl;
    for (int i = 0; i < 16/pixel_combine; i++) {
        for (int j = 0; j < 16/pixel_combine; j++) {
            cout<<"Pixel combination: "<<i<<"   "<<j<<endl;
            string new_condition = condition + "&&row>=" + std::to_string(i*pixel_combine) + "&&row<" + std::to_string((i+1)*pixel_combine) + "&&col>=" + std::to_string(j*pixel_combine) + "&&col<" + std::to_string((j+1)*pixel_combine);
            cout<<new_condition<<endl;
            pulse->Draw(("((tot_code*2-floor(tot_code/32))*3.125/cal_code)>>TOT_pixel_"+std::to_string(i)+"_"+std::to_string(j)).c_str(), new_condition.c_str(), "HIST E");
            TH1D * tmp_hist = (TH1D*)gDirectory->Get(("TOT_pixel_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
            tmp_hist->SetTitle("");
            tmp_hist->Write();
        }
    }

    std::cout<<"pixel or combined pixel analysis DeltaT vs TOT"<<std::endl;
    for (int i = 0; i < 16/pixel_combine; i++) {
        for (int j = 0; j < 16/pixel_combine; j++) {
            cout<<"Pixel combination: "<<i<<"   "<<j<<endl;
            string new_condition = condition + "&&row>=" + std::to_string(i*pixel_combine) + "&&row<" + std::to_string((i+1)*pixel_combine) + "&&col>=" + std::to_string(j*pixel_combine) + "&&col<" + std::to_string((j+1)*pixel_combine);
            cout<<new_condition<<endl;
            pulse->Draw(("((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)):((tot_code*2-floor(tot_code/32))*3.125/cal_code)>>DeltaT_TOT_pixel_"+std::to_string(i)+"_"+std::to_string(j)).c_str(), new_condition.c_str(), "HIST E");
            TH2D * tmp_hist = (TH2D*)gDirectory->Get(("DeltaT_TOT_pixel_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
            tmp_hist->SetTitle("");
            tmp_hist->Write();
            std::cout<<"timewalk correction"<<std::endl;
            std::vector<double> params = timewalk_correction(tmp_hist, TIMEWALK_FIT_FUNCTION_DEGREES);
            std::string predicted = std::to_string(params.at(0));
            for (int i = 1; i <= TIMEWALK_FIT_FUNCTION_DEGREES; i++) {
                predicted += "+" + std::to_string(params.at(i)) + "*pow((tot_code*2-floor(tot_code/32))*3.125/cal_code," + i + ")";
            }
            // string corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)+abs(" + std::to_string(y_max) + "-((" + std::to_string(scale) + "*(tot_code*2-floor(tot_code/32))*3.125/cal_code+" + std::to_string(bias) + ")))):(tot_code*2-floor(tot_code/32))*3.125/cal_code>>DeltaT_vs_TOT_corr";
            string corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)-" + predicted + "):(tot_code*2-floor(tot_code/32))*3.125/cal_code>>DeltaT_vs_TOT_corr_pixel_"+std::to_string(i)+"_"+std::to_string(j);
            std::cout<<corr_prefix<<std::endl;
            pulse->Draw(corr_prefix.c_str(), new_condition.c_str(), "colz");
            TH2D * tmp_hist_corr = (TH2D*)gDirectory->Get(("DeltaT_vs_TOT_corr_pixel_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
            tmp_hist_corr->SetTitle("");
            tmp_hist_corr->GetXaxis()->SetTitle("TOT");
            tmp_hist_corr->GetYaxis()->SetTitle("DeltaT");
            tmp_hist_corr->Write();
            corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)-" + predicted + ")>>DeltaT_corr_pixel_"+std::to_string(i)+"_"+std::to_string(j);
            // corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)+abs(" + std::to_string(y_max) + "-((" + std::to_string(scale) + "*(tot_code*2-floor(tot_code/32))*3.125/cal_code+" + std::to_string(bias) + "))))>>DeltaT_corr_pixel_"+std::to_string(i)+"_"+std::to_string(j).c_str();
            std::cout<<corr_prefix<<std::endl;
            pulse->Draw(corr_prefix.c_str(), new_condition.c_str(), "hist e");
            TH1D * tmp_hist_corr_dt = (TH1D*)gDirectory->Get(("DeltaT_corr_pixel_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
            tmp_hist_corr_dt->SetTitle("");
            tmp_hist_corr_dt->GetXaxis()->SetTitle("DeltaT");
            tmp_hist_corr_dt->GetYaxis()->SetTitle("Events");
            tmp_hist_corr_dt->Write();
            corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)-" + predicted + "):(16*row+col)>>DeltaT_pixelid_corr_pixel_"+std::to_string(i)+"_"+std::to_string(j).c_str();
            // corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)+abs(" + std::to_string(y_max) + "-((" + std::to_string(scale) + "*(tot_code*2-floor(tot_code/32))*3.125/cal_code+" + std::to_string(bias) + ")))):(16*row+col)>>DeltaT_pixelid_corr_pixel_"+std::to_string(i)+"_"+std::to_string(j).c_str();
            std::cout<<corr_prefix<<std::endl;
            // std::cout<<y_max<<std::endl;
            pulse->Draw(corr_prefix.c_str(), new_condition.c_str(), "hist e");
            TH1D * tmp_hist_corr_pixelid_dt = (TH1D*)gDirectory->Get(("DeltaT_pixelid_corr_pixel_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
            tmp_hist_corr_pixelid_dt->SetTitle("");
            tmp_hist_corr_pixelid_dt->GetXaxis()->SetTitle("PixelID");
            tmp_hist_corr_pixelid_dt->GetYaxis()->SetTitle("DeltaT");
            tmp_hist_corr_pixelid_dt->Write();
        }
    }
    /*
    pixel_combine = 16;
    // row analysis DeltaT vs TOT
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16/pixel_combine; j++) {
            cout<<"Pixel combination: "<<i<<"   "<<j<<endl;
            string new_condition = condition + "&&row==" + std::to_string((i)) + "&&col>=" + std::to_string(j*pixel_combine) + "&&col<" + std::to_string((j+1)*pixel_combine);
            cout<<new_condition<<endl;
            pulse->Draw(("((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)):((tot_code*2-floor(tot_code/32))*3.125/cal_code)>>DeltaT_TOT_row_"+std::to_string(i)+"_col_comb_"+std::to_string(j)).c_str(), new_condition.c_str(), "HIST E");
            TH2D * tmp_hist = (TH2D*)gDirectory->Get(("DeltaT_TOT_row_"+std::to_string(i)+"_col_comb_"+std::to_string(j)).c_str());
            tmp_hist->SetTitle("");
            tmp_hist->Write();
            std::cout<<"timewalk correction"<<std::endl;
            std::vector<double> params = timewalk_correction(tmp_hist, TIMEWALK_FIT_FUNCTION_DEGREES);
            double scale = params.at(0);
            double bias = params.at(1);
            double y_max = tmp_hist->ProjectionY()->GetMean();
            string corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)+abs(" + std::to_string(y_max) + "-((" + std::to_string(scale) + "*(tot_code*2-floor(tot_code/32))*3.125/cal_code+" + std::to_string(bias) + ")))):(tot_code*2-floor(tot_code/32))*3.125/cal_code>>DeltaT_vs_TOT_corr_pixel_row_col_comb_"+std::to_string(i)+"_"+std::to_string(j);
            std::cout<<corr_prefix<<std::endl;
            std::cout<<y_max<<std::endl;
            pulse->Draw(corr_prefix.c_str(), new_condition.c_str(), "colz");
            TH2D * tmp_hist_corr = (TH2D*)gDirectory->Get(("DeltaT_vs_TOT_corr_pixel_row_col_comb_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
            tmp_hist_corr->SetTitle("");
            tmp_hist_corr->GetXaxis()->SetTitle("TOT");
            tmp_hist_corr->GetYaxis()->SetTitle("DeltaT");
            tmp_hist_corr->Write();
            corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)+abs(" + std::to_string(y_max) + "-((" + std::to_string(scale) + "*(tot_code*2-floor(tot_code/32))*3.125/cal_code+" + std::to_string(bias) + "))))>>DeltaT_corr_pixel_row_col_comb_"+std::to_string(i)+"_"+std::to_string(j);
            std::cout<<corr_prefix<<std::endl;
            std::cout<<y_max<<std::endl;
            pulse->Draw(corr_prefix.c_str(), new_condition.c_str(), "hist e");
            TH1D * tmp_hist_corr_dt = (TH1D*)gDirectory->Get(("DeltaT_corr_pixel_row_col_comb_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
            tmp_hist_corr_dt->SetTitle("");
            tmp_hist_corr_dt->GetXaxis()->SetTitle("DeltaT");
            tmp_hist_corr_dt->GetYaxis()->SetTitle("Events");
            tmp_hist_corr_dt->Write();
            corr_prefix = "((toa_code*3.125/cal_code)-(Clock[0]-LP2_20[2]*1e9)+abs(" + std::to_string(y_max) + "-((" + std::to_string(scale) + "*(tot_code*2-floor(tot_code/32))*3.125/cal_code+" + std::to_string(bias) + ")))):(16*row+col)>>DeltaT_pixelid_corr_pixel_row_col_comb_"+std::to_string(i)+"_"+std::to_string(j);
            std::cout<<corr_prefix<<std::endl;
            std::cout<<y_max<<std::endl;
            pulse->Draw(corr_prefix.c_str(), new_condition.c_str(), "hist e");
            TH1D * tmp_hist_corr_pixelid_dt = (TH1D*)gDirectory->Get(("DeltaT_pixelid_corr_pixel_row_col_comb_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
            tmp_hist_corr_pixelid_dt->SetTitle("");
            tmp_hist_corr_pixelid_dt->GetXaxis()->SetTitle("PixelID");
            tmp_hist_corr_pixelid_dt->GetYaxis()->SetTitle("DeltaT");
            tmp_hist_corr_pixelid_dt->Write();
        }
    }*/
    
    output_file->Close();
}