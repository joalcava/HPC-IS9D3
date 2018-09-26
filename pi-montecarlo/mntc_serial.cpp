/*
 * Source:
 * https://www.algorithm-archive.org/contents/monte_carlo_integration/monte_carlo_integration.html
 */

#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>

constexpr int radious = 1; //Unit circle
constexpr double PI = 3.14159265358979323846264338; //Actual PI value

bool in_circle(double x_pos, double y_pos)
{
    return pow(x_pos, 2) + pow(y_pos, 2) < pow(radious, 2);
}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Please, enter the number of points to throw." << std::endl;
    }

    // Initialize random device
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> dist(0, 1);

    unsigned samples = atoi(argv[1]);
    unsigned pi_count = 0;

    auto startTime = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < samples; ++i)
    {
        double x = dist(generator);
        double y = dist(generator);

        if (in_circle(x, y))
        {
            pi_count += 1;
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();

    auto elapsedTime = (endTime - startTime).count();

    double pi_estimate = 4.0 * pi_count / samples;
    printf("Pi estimate: %f\n", pi_estimate);
    printf("Percent error: %f\n", std::abs(pi_estimate - PI) / PI);
    printf("Elapsed time: %f\n", elapsedTime);
}