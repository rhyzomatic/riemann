#include <iostream>
#include <vector>
#include <algorithm>

#include <armadillo>

using namespace std;
using namespace arma;

class Dimension {
  public:
    vector<int> r;
    double size, value, max=1;
    int d, current=0, step=1, end=0, diag;

    friend bool operator<(Dimension const& left, Dimension const& right) {
        return left.value < right.value;
    }
};

int main() {
    cin.exceptions(ios::failbit | ios::badbit);
    cout.precision(12);

    int N;
    cin >> N;

    mat P(N, N);
    for (auto& v: P) cin >> v;

    // Calculate box containing unit elipsoid
    mat A = chol(P);
    mat Ainv  = A.i();
    mat Ainvt = Ainv.t();
    mat Pinv = Ainv*Ainvt;

    vector<Dimension> dimension(N);
    for (int i=0; i<N;++i) {
        dimension[i].size = Pinv(i,i)/norm(Ainvt.col(i));
        dimension[i].value = dimension[i].max / dimension[i].size;
        dimension[i].d = i;
        dimension[i].diag = P(i, i);
        for (int j=0; j<N; ++j) dimension[i].r.emplace_back(P(i, j));
    }
    sort(dimension.begin(), dimension.end());

    cout << "Box:" << endl;
    for (auto& d: dimension) {
        cout << "    dimension " << d.d << " " << d.size << endl;
    }
    cout << endl;

    double sum = 0.5, change = 0;
    int accu=0;

    vector<int> x(N, 0);
    int l=0;
    while (change > 9e-5/N || l < N) {
        // Grow box along 1 dimension
        ++l;
        change = 0;

        int t = 0;
        for (int i=0; i<N; ++i) t += dimension[0].r[i] * x[i];
        accu -= dimension[0].diag - 2 * dimension[0].step * t;

        ++dimension[0].max;
        dimension[0].end     += dimension[0].step;
        dimension[0].current -= dimension[0].step;
        x[dimension[0].d] = dimension[0].current;
        while (1) {
            change += exp(accu);

            int k=1;
          RETRY:
            if (dimension[k].current == dimension[k].end) {
                dimension[k].step = -dimension[k].step;
                dimension[k].end  = -dimension[k].end;
                ++k;
                if (k < N) goto RETRY;
                break;
            }
            if (k ==1 && dimension[k].current != -dimension[k].end) {
                t += dimension[k].diag;
            } else {
                t = 0;
                for (int i=0; i<N; ++i) t += dimension[k].r[i] * x[i];
                t *= dimension[k].step;
            }
            accu -= 2 * t + dimension[k].diag;
            dimension[k].current += dimension[k].step;
            x[dimension[k].d] = dimension[k].current;
        }

        sum += change;
        cout << "l=" << l << ", dimension=" << dimension[0].d << ", sum=" << sum*2 << ", change=" << change << endl;

        dimension[0].value = dimension[0].max / dimension[0].size;
        sort(dimension.begin(), dimension.end());
    }

    cout << "sum=" << sum*2 << endl;
}