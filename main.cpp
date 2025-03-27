#include <iostream>
#include <memory>
#include <random>
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

class MomentumStrategy : public TradingStrategy
{
    std::string generateSignal(const std::vector<double>& marketData) override
    {
        if (marketData.size() < 2)
            return "HOLD";   // Not enough data
        return (marketData.back() > marketData[marketData.size() - 2]) ? "BUY"
                                                                       : "SELL";
    }
};

class StrategyFactory
{
public:
    static std::unique_ptr<TradingStrategy>
    createStrategy(const std::string& strategyName)
    {
        if (strategyName == "meanreversion")
        {
            return std::make_unique<MeanReversionStrategy>();
        }
        else if (strategyName == "momentum")
        {
            return std::make_unique<MomentumStrategy>();
        }
        else
        {
            throw std::invalid_argument("Unknown strategy: " + strategyName);
        }
    }
};

double simulateMarketData()
{
    static std::random_device               rd;
    static std::mt19937                     gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 100.0);
    return dis(gen);
}

int main()
{
    try
    {
        std::string strategyChoice;
        std::cout << "Enter trading strategy (meanreversion/momentum): ";
        std::cin >> strategyChoice;

        auto strategy = StrategyFactory::createStrategy(strategyChoice);

        std::vector<double> marketData;
        for (int i = 0; i < 25; ++i)
        {
            marketData.push_back(simulateMarketData());
        }
        std::cout << "Market data: ";
        for (const auto& data : marketData)
        {
            std::cout << data << ' ';
        }
        std::cout << "\n\n";

        std::string signal = strategy->generateSignal(marketData);
        std::cout << "Generated signal: " << signal << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
