// Alexey Tkachenko

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;

pair<double, double> model(double v0, double k0, double a1, double a2, double b1, double b2, double t) {
    double local_v0 = v0 - a2 / b2;
    double local_k0 = k0 - a1 / b1;
    return {
        local_v0 * cos(sqrt(a1 * a2) * t) - (local_k0 * sqrt(a2) * b1 * sin(sqrt(a1 * a2) * t)) / (b2 * sqrt(a1)) + a2 / b2,
        (local_v0 * sqrt(a1) * b2 * sin(sqrt(a1 * a2) * t)) / (b1 * sqrt(a2)) + local_k0 * cos(sqrt(a1 * a2) * t) + a1 / b1
    };
}

int main() {
    double v0, k0, alpha1, beta1, alpha2, beta2;
    int time_limit, number_of_points;
    cin >> v0 >> k0 >> alpha1 >> beta1 >> alpha2 >> beta2 >> time_limit >> number_of_points;

    vector<double> time_fragments(number_of_points + 1);
    vector<double> vs(number_of_points + 1);
    vector<double> ks(number_of_points + 1);

    for (int i = 0; i < number_of_points + 1; i++) {
        double time = (double) time_limit / number_of_points * i;
        time_fragments[i] = time;
        pair<double,double> res = model(v0, k0, alpha1, alpha2, beta1, beta2, time);
        vs[i] = res.first;
        ks[i] = res.second;
    }

    cout << setprecision(2) << fixed;

    cout << "t:" << endl;
    for(const auto &t : time_fragments)
        cout << t << " ";

    cout << endl << "v:" << endl;
    for(const auto &v : vs)
        cout << v << " ";

    cout << endl << "k:" << endl;
    for(const auto &k : ks)
        cout << k << " ";

    // GNUPlot
    FILE *pipe = popen("gnuplot -persist", "w");

    if (pipe == NULL) {
        cout << "Pipe failed" << endl;
        return 0;
    }

    // Setup GNUPlot
    fprintf(pipe, "set xrange [%d:%d]\n", 0, 70);
    fprintf(pipe, "set yrange [%d:%d]\n", 0, 400);
//    fprintf(pipe, "set xtics 1\n");
//    fprintf(pipe, "set ytics 1\n");
    fprintf(pipe, "set grid xtics ytics\n");

    // Print graphs
    fprintf(pipe, "plot '-' with lines title \"Victims\", '-' with lines title \"Killers\"\n");
    for (int i = 0; i < number_of_points + 1; i++)
        fprintf(pipe, "%f %f\n", time_fragments[i], vs[i]);
    fprintf(pipe, "e\n");

    for (int i = 0; i < number_of_points + 1; i++)
        fprintf(pipe, "%f %f\n", time_fragments[i], ks[i]);
    fprintf(pipe, "e\n");

    fflush(pipe);
    pclose(pipe);

    return 0;
}
