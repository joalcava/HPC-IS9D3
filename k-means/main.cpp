// g++ -std=c++11 -fopenmp -o kmeans kMeans.cc
// ./kMeans iris.data

#include <fstream>
#include <iostream>
#include <omp.h>
#include <random>
#include <unordered_map>
#include <vector>

using namespace std;

using Point = vector<double>;

ostream& operator<<(ostream& os, const Point& p) {
    os << "<";
    for (double i : p) {
        os << " " << i;
    }
    os << ">";
}

/**
 * Reads the Iris dataset
 */
vector<Point> readIris(const string& fileName)
{
    vector<Point> ds;
    ifstream input(fileName);
    string label;
    double sl, sw, pl, pw;
    while (input >> sl >> sw >> pl >> pw >> label) {
        Point p{sl, sw, pl, pw};
        ds.push_back(move(p));
    }
    cout << "Input has " << ds.size() << " points." << endl;
    return ds;
}

void printClustering(const vector<Point>& dataset,
                     const vector<size_t>& clustering,
                     size_t k)
{
    size_t n = dataset.size();
    vector<size_t> count(k, 0);
    for (size_t i = 0; i < n; i++) {
        size_t ci = clustering[i];
        count[ci]++;
    }

    for(size_t i = 0; i < k; i++) {
        cout << " cluster " << i << ": " << count[i] << endl;
    }
}

vector<Point> randomPoints(size_t k, const vector<Point>& ds)
{
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> unif(0, ds.size()-1);
    size_t dim = ds[0].size();
    vector<Point> centroids(k, Point(dim, 0.0));

    for (size_t i = 0; i < k; i++)
    {
        auto r = unif(rd);
        centroids[i] = ds[r];
    }

    return centroids;
}


double sqdistance(const Point& p, const Point& q)
{
    double d = 0.0;
    for (size_t i = 0; i < p.size(); i++)
    {
        d += pow(p[i] - q[i], 2);
    }
    return d;
}

tuple<size_t, double> closestCentroid(const Point& p,
                                      const vector<Point>& centroids) {
    double d = numeric_limits<double>::max();
    size_t c = 0;

    for (size_t i = 0; i < centroids.size(); i++) {
        double dt = sqdistance(p, centroids[i]);
        if (dt < d) {
            d = dt;
            c = i;
        }
    }

    return {c, d};
}

pair<vector<size_t>, double> cluster(const vector<Point>& dataset,
                                     const vector<Point>& centroids) {
    size_t n = dataset.size();
    vector<size_t> clustering(n, 0);
    double ssd = 0.0;
    for (size_t i = 0; i < n; i++) {
        size_t c;
        double d;
        tie(c, d) = closestCentroid(dataset[i], centroids);
        clustering[i] = c;
        ssd += d;
    }
    return {clustering, ssd};
}


vector<Point> newCentroids(const vector<size_t>& clustering,
                           const vector<Point>& dataset,
                           vector<Point>& centroids) {
    size_t k = centroids.size();
    size_t dim = centroids[0].size();
    vector<Point> newCentroids(k, Point(dim, 0.0));
    vector<double> count(k, 0.0);
    for (size_t i = 0; i < dataset.size(); i++) {
        size_t ci = clustering[i];
        for (size_t j = 0; j < dim; j++) {
            newCentroids[ci][j] += dataset[i][j];
        }
        count[ci]++;
    }

    for (size_t i = 0; i < k; i++) {
        for (size_t j = 0; j < dim; j++) {
            newCentroids[i][j] /= count[i];
        }
    }
    return newCentroids;
}


vector<size_t> kMeans(const vector<Point> &data_set, // ds
                      size_t k,                      // 3
                      double epsilon,                // 0.001
                      size_t maxIter)                // 40
{
    size_t iter = 0;
    size_t n = data_set.size();
    vector<Point> centroids = randomPoints(k, data_set);
    vector<size_t> clustering(n, 0);
    double ssd = 0.0;
    double difference, ssdPrev;

    do {
        ssdPrev = ssd;
        cout << "Iteration " << iter << endl;
        tie(clustering, ssd) = cluster(data_set, centroids);
        cout << "SSD: " << ssd << endl;
        //printClustering(data_set, clustering);
        centroids = newCentroids(clustering, data_set, centroids);
        iter++;
        difference = abs(ssdPrev - ssd);
        cout << "----> " << difference << endl;
    } while(difference > epsilon);

    return clustering;
}

int main(int argc, char** argv)
{
    if (argc != 2) return -1;

    string file_name(argv[1]);
    vector<Point> ds = readIris(file_name);
    vector<size_t> clustering(4500, 0);

    clustering = kMeans(ds, 3, 0.001, 40);
    printClustering(ds, clustering, 3);

    return 0;
}
