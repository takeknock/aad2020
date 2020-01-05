#include "pch.h"

double calculateAmericanOptionPrice(double currentPrice, double riskfreeRate, double volatility, double strike, double maturity, int numOfSteps, int numOfPath, int seed) {
    return 0.0;
}


TEST(AmericanOptionPricerTest, CalculatePriceTest) {
    double currentPrice = 50.0;
    double riskfreeRate = 0.01;
    double volatility = 0.2;
    double strike = 50.0;
    double maturity = 1.0;
    int numOfSteps = 365;
    int numOfPaths = 5000;
    int seed = 100;

    


  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}