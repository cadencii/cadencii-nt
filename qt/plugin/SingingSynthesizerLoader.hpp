#pragma once

#include <memory>
#include <string>

namespace cadencii {
namespace audio {
namespace generator {
class ISingingSynthesizer;
}
}
}

namespace cadencii {

class SingingSynthesizerLoader
{
public:
    static cadencii::audio::generator::ISingingSynthesizer * load(std::string const& dll_path, int sample_rate);

private:
    SingingSynthesizerLoader() = delete;
    SingingSynthesizerLoader(SingingSynthesizerLoader const& other) = delete;
    SingingSynthesizerLoader & operator = (SingingSynthesizerLoader const& other) = delete;

    struct StaticImpl;
    static std::shared_ptr<StaticImpl> static_impl_;
};

}
