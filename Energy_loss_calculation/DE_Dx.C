#include <iostream>
#include <cmath>

// Constants
const double me = 0.511;                // Electron mass in MeV/c^2
const double Z = 13;                    // Atomic number of aluminum
const double A = 26.98;                 // Atomic mass of aluminum in g/mol
const double density = 2.7;             // Density of aluminum in g/cm^3
const double I = 166e-6;                // Mean excitation energy of aluminum in MeV
const double K = 0.307075;              // Bethe constant in MeV cm^2/mol
const double beta2_min = 1e-6;          // To avoid division by zero
const double thickness = 5.0;           // Aluminum thickness in cm

double BetheBloch(double K_E, double M, double C = 1.0) {
    double gamma = 1 + K_E / M;
    double beta2 = 1.0 - 1.0 / (gamma * gamma);
    beta2 = std::max(beta2, beta2_min); // Avoid beta^2 = 0

    double Tmax = 2 * me * pow(beta2 * gamma, 2) / (1 + 2 * gamma * me / M + pow(me / M, 2)); // Max transferable energy

    double stoppingPower = (K * Z / A) * (1 / beta2) * density / (std::log(2 * me * beta2 * gamma * gamma * Tmax / (I * I)) - beta2);

    return stoppingPower;
}

int DE_Dx() {
    double K_E_0 = 1.0; // Initial energy 1 MeV increment by 1 MeV steps
    double M = me;
    double C = 1.0;
    const int n_steps = 1000;
    double E_loss[n_steps] = {};
    double K_Es  [n_steps] = {};
    
    for (int i = 1; i <= n_steps; i++) {
        double K_E = i * K_E_0;
        double stoppingPower = BetheBloch(K_E, M, C);
    
        // Calculate total energy loss
        double energyLoss = stoppingPower * thickness;
        E_loss[i-1] = energyLoss;
        K_Es[i-1] = K_E;
        // Output results
        std::cout << "The kinetic energy: " << K_E / 1000.0 << " GeV"<< std::endl;
        std::cout << "Energy loss per unit of distance (cm) (-dE/dx): " << stoppingPower << " MeV/cm" << std::endl;
        std::cout << "Total Energy Loss: " << energyLoss << " MeV" << std::endl;
    }
    
    TGraph * g = new TGraph(n_steps, K_Es, E_loss);
    g->SetTitle("The energy loss vs initial energy of an electron passing through 5 cm of aluminum.");
    g->GetYaxis()->SetTitle("Energy loss [MeV]");
    g->GetXaxis()->SetTitle("Initial energy [MeV]");
    g->Draw();
    return 0;
}
