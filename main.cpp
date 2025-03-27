#include <iostream>
#include <memory>
#include <string>
#include <vector>

class TradingStrategy
{
public:
    virtual std::string generateSignal(const std::vector<double>& marketData
    )                          = 0;
    virtual ~TradingStrategy() = default;
};

class MeanReversionStrategy : public TradingStrategy
{
public:
    std::string generateSignal(const std::vector<double>& marketData) override
    {
        if (marketData.size() < 5)
            return "HOLD";   // Not enough data
        double sum{ 0.0 };
        for (size_t i{ marketData.size() - 5 }; i < marketData.size(); ++i)
        {
            sum += marketData[i];
        }

        double mean         = sum / 5.0;
        double currentPrice = marketData.back();
        return (currentPrice < mean) ? "BUY" : "SELL";
    }
};

int main()
{
    try
    {
        std::string strategyChoice;
        std::cout << "Enter trading strategy (meanreversion): ";
        std::cin >> strategyChoice;

        std::unique_ptr<TradingStrategy> strategy;
        if (strategyChoice == "meanreversion")
        {
            strategy = std::make_unique<MeanReversionStrategy>();
        }
        else
        {
            throw std::invalid_argument("Unknown strategy: " + strategyChoice);
        }

        std::vector<double> marketData = { 100.0, 101.0, 102.0,
                                           103.0, 104.0, 105.0 };
        std::string         signal     = strategy->generateSignal(marketData);
        std::cout << "Generated signal: " << signal << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
