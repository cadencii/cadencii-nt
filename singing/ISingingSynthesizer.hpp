#pragma once

#include <string>
#include <memory>
#include "./IWaveformReceiver.hpp"

namespace cadencii {
namespace singing {

class IScoreProvider;

class ISingingSynthesizer : public IWaveformReceiver
{
public:
    explicit ISingingSynthesizer(int sample_rate)
        : sample_rate_(sample_rate)
    {}

    virtual ~ISingingSynthesizer()
    {}

    void bind(std::shared_ptr<cadencii::singing::IScoreProvider> const& provider)
    {
        provider_ = provider;
        processed_samles_ = 0;
    }

    void unbind()
    {
        provider_.reset();
    }

    virtual bool setConfig(std::string const& key, std::string const& value) = 0;

    void render(double * left, double * right, size_t length)
    {
        this->operator ()(left, right, length);
        processed_samles_ += length;
    }

    size_t processedSamples() const
    {
        return processed_samles_;
    }

    size_t sampleRate() const
    {
        return sample_rate_;
    }

protected:
    std::shared_ptr<cadencii::singing::IScoreProvider> provider_;

private:
    size_t processed_samles_;
    size_t const sample_rate_;
};

}
}
