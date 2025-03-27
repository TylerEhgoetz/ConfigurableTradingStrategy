#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
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

class TradingSystem
{
private:
    std::unique_ptr<TradingStrategy> strategy;
    std::vector<double>              marketData;
    const int maxMarketDataSize{ 20 };   // Maximum size of market data

public:
    TradingSystem(std::unique_ptr<TradingStrategy> strat)
        : strategy(std::move(strat))
    {}

    void updateMarketData(double price)
    {
        marketData.push_back(price);
        if (marketData.size() > maxMarketDataSize)
        {
            marketData.erase(marketData.begin());
        }
    }

    std::string executeTrading()
    {
        if (marketData.size() < 5)
            return "HOLD";   // Not enough data
        return strategy->generateSignal(marketData);
    }

    void printMarketData() const
    {
        std::cout << "Market data: ";
        for (const auto& data : marketData)
        {
            std::cout << data << ' ';
        }
        std::cout << std::endl;
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
        TradingSystem tradingSystem(std::move(strategy));

        std::cout << "Simulating market data...\n";
        for (int i{ 0 }; i < 30; ++i)
        {
            double marketPrice{ simulateMarketData() };
            tradingSystem.updateMarketData(marketPrice);
            tradingSystem.printMarketData();
            std::string signal = tradingSystem.executeTrading();
            std::cout << "Trading signal: " << signal << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500)
            );   // Simulate time delay
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
