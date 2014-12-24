#include <vector>
#include <QLibrary>
#include <iostream>
#include <memory>
#include "./SingingSynthesizerLoader.hpp"
#include "../../singing/ISingingSynthesizer.hpp"

namespace cadencii {

struct SingingSynthesizerLoader::StaticImpl
{
    StaticImpl()
    {}

    ~StaticImpl()
    {
        std::for_each(std::begin(loaded_libraries_), std::end(loaded_libraries_), [](std::shared_ptr<QLibrary> & lib) {
            lib->unload();
        });
    }

    cadencii::audio::generator::ISingingSynthesizer * load(std::string const& dll_path, int sample_rate)
    {
        cadencii::audio::generator::ISingingSynthesizer * result = nullptr;

        std::shared_ptr<QLibrary> temp;
        temp.reset(new QLibrary(QString::fromStdString(dll_path)));
        temp->setLoadHints(QLibrary::ResolveAllSymbolsHint);
        if (temp->load()) {
            QFunctionPointer ptr = temp->resolve(LOADER_FUNCTION_NAME);
            if (ptr) {
                CreateFunc * creator_func = reinterpret_cast<CreateFunc *>(ptr);
                try {
                    result = creator_func(sample_rate);
                } catch (...) {
                    return nullptr;
                }
                loaded_libraries_.push_back(temp);
            }
        }

        return result;
    }

private:
    typedef cadencii::audio::generator::ISingingSynthesizer * (CreateFunc)(int sample_rate);
    static char const* LOADER_FUNCTION_NAME;

    std::vector<std::shared_ptr<QLibrary>> loaded_libraries_;
};

std::shared_ptr<SingingSynthesizerLoader::StaticImpl> SingingSynthesizerLoader::static_impl_ = std::make_shared<SingingSynthesizerLoader::StaticImpl>();

char const* SingingSynthesizerLoader::StaticImpl::LOADER_FUNCTION_NAME = "cadencii_create_synthesizer";

cadencii::audio::generator::ISingingSynthesizer * SingingSynthesizerLoader::load(std::string const& dll_path, int sample_rate)
{
    return static_impl_->load(dll_path, sample_rate);
}

}
