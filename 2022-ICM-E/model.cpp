#include<bits/stdc++.h>

using namespace std;


double calc_PI_water() {
    static const int num = 4;
    // TP  TN  1/DO  H+
    static const double C_water[]  = {0.166, 7.668, 1.0 / 12.48, pow(10, -7)};
    static const double C0_water[] = {0.4, 2.0, 1.0 / 2, pow(10, -6)};
    double PI_water = 0;
    for (int i = 0; i < num; i++)
        PI_water += C_water[i] / C0_water[i];
    printf(" PI_water = %.4f\n", PI_water);
    return PI_water;
}

double calc_PI_air() {
	static const int num = 5;
	// PM10  PM2.5  CO  NO2  SO2
    static const double C_air[]  = {74, 41, 0.7, 25, 26};
    static const double C0_air[] = {150, 75, 10, 200, 150};
    double PI_air = 0, max_C_air = 0;
    for (int i = 0; i < num; i++)
    	PI_air += C_air[i] / C0_air[i],
    	max_C_air = fmax(max_C_air, C_air[i] / C0_air[i]);
    PI_air = sqrt(1.0 / num * PI_air) * sqrt(max_C_air);
    printf(" PI_air = %.4f\n", PI_air);
    return PI_air;
}

double calc_PI_M() {
	static const double W_water = 0.1, W_air = 0.6;
	double PI_M = (W_water * calc_PI_water() + W_air * calc_PI_air()) / 2;
	printf(" PI_M = %.4f\n", PI_M);
	return PI_M;
}

double calc_Shannon(){
	static const int num = 6;
	static const int N_Shannon[]={7, 5, 8, 9, 13, 25};
	double H = 0, sum = 0;
	for(int i = 0; i < num; i++)
		sum += N_Shannon[i];
	for(int i = 0; i < num; i++)
		H += -(N_Shannon[i] / sum) * log(N_Shannon[i] / sum);
	printf(" H = %.4f\n", H);
	return H;
}

double calc_Economy(){
	static const double W_forest_related = 1, W_forest_unrelated = 0.02;
	static const double E_forest_related = 33.1, E_forest_unrelated = 5183.8;
	double E = E_forest_related * W_forest_related + E_forest_unrelated * W_forest_unrelated;
	return E;
}

double calc_C_SID(double H){
	static const int num = 12; 
	static const double B_A = 0.2438;
	// Pinus koraiensis, Fraxinus mandshuri, Acer triflorum, Acer tegmentosum, Acer ukurunduense, Ulmils macrocarpa
	// Tilia amurensis, Quercus mongolica, Acer mono, Ulmus laciniata, Maackia amurensis, Syringa reticulatasubsp amurens
	static const double I_v[] ={44.24, 14.57, 3.55, 0.46, 0.12, 5.74, 23.28, 0.09, 6.92, 0.06, 0.66, 0.31};
	static const double C_an[]={10, 2.4, 5.2, 5.2, 5.2, 3.8, 4.3, 3, 5.2, 3.8, 1.2, 2.4};
	double C_I = 0;
	for(int i = 0; i < num; i++) 
		C_I += I_v[i] * C_an[i];
	printf(" C_I = %.4f\n", C_I);
	double C_SID = H * log(pow(C_I * B_A, 2));
	printf(" C_SID = %.4f\n",C_SID);
	return C_SID;
}

const double W_max = 105.56;  // tC/hm2
const double miu = 0.8, lambda = 0.005;
double b, theta;

void init(){
	static const double A_1 = 21, W_1 = 38.95;
	static const double A_2 = 10, W_2 = 0.33;
	double y_1 = log(W_max / W_1 - 1);
	double y_2 = log(W_max / W_2 - 1);
	theta = (y_1 - y_2) / (A_2 - A_1);
	b = theta * A_1 + y_1;
}

double calc_W(double A){
	return W_max / (1 + exp(b - theta * A));
}

void Ecology_prioritized(){
	static const double alpha = 0.8; // ? alpha 0.8 
	double A_logging = (b + log(alpha) - log(1-alpha)) / theta;
	double delta_A = 1;
	double delta_W = calc_W(A_logging + delta_A) - calc_W(A_logging);
	double delta_E = miu * (lambda * calc_W(A_logging + delta_A) - calc_W(A_logging)) / delta_A;
	printf("\n Harvest %.4f tC/ha every %.0f year(s), GDP can increase %.0f%%.\n", delta_W, delta_A, lambda * 100);
}

void Economics_prioritized(){
	double E = calc_Economy();
	double delta_A = 5;
	double r = W_max * miu / lambda / E;
	double W_logging = 2 * W_max / (r - sqrt(r * r - 4 * r));
	printf("\n Harvest to %.4f tC/ha every %.0f year(s), GDP can increase %.2f%%.\n", W_logging, delta_A, lambda * 100);
}

signed main() {
	
	init();
	
	double PI_M = calc_PI_M();
	
	if(PI_M > 0.75) {
		printf("\n PI_M is not meet the standard\n Harvesting is Prohibited!\n");
		return 0;
	}
	
	double H = calc_Shannon();
	
	if(H <= 1){
		printf("\n Biology Diversity is not meet the standard\n Harvesting is Prohibited!\n");
		return 0;
	}
	
	printf("\n Harvesting is Permitted!\n\n");
	
	double C_SID = calc_C_SID(H);
	
	if(C_SID <= 9){
		printf("\n Unhealthy Forest!\n Ecology-prioritized Strategy!\n");
		Ecology_prioritized();
	}
	
	double E = calc_Economy();
	
	if(9 < C_SID && C_SID < 15){
		printf("\n Sub-healthy Forest!\n");
		if(E > 15){
			printf(" Ecology-prioritized Strategy!\n");
			Ecology_prioritized();
		}
		if(E <= 15){
			printf(" Economics-prioritized Strategy!\n");
			Economics_prioritized();
		}
	}
	
	if(15 <= C_SID){
		printf("\n Healthy Forest!\n Economics-prioritized Strategy!\n");
		Economics_prioritized();
	}
	
}
