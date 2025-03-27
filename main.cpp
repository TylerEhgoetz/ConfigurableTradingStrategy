#include <iostream>
#include <vector>
#include <string>

class TradingStrategy
{
    public:
        virtual std::string generateSignal(const std::vector<double>& marketData) = 0;
        virtual ~TradingStrategy() = default;
};


int main()
{
    return 0;
}
