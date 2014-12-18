#pragma once

#include <libvsq/libvsq.h>

class TestUtil{
private:
    static std::string fixtureRootPath;

public:
    static void setFixtureRootPath( const std::string &fixtureRootPath ){
        TestUtil::fixtureRootPath = fixtureRootPath;
    }

    static std::string getFixtureRootPath(){
        return TestUtil::fixtureRootPath;
    }
};

#define CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR(ENUM_NAME) \
    /**																											\
     * @fn std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, ENUM_NAME v)	\
     * @brief ENUM_NAME をストリームに書き込む.																	\
     * @param os 書き込み先のストリーム.																			\
     * @param v 書き込む ENUM_NAME の値.																			\
     * @return 書き込んだ後のストリーム @a os.																		\
     */																											\
    template<class Traits>																						\
    inline std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, ENUM_NAME v)	\
    {																											\
        os << static_cast<std::underlying_type<ENUM_NAME>::type>(v);											\
        return os;																								\
    }

LIBVSQ_BEGIN_NAMESPACE

CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR(DynamicsMode)
CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR(PlayMode)
CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR(HandleType)
CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR(ArticulationType)
CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR(EventListIndexIteratorKind)
CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR(MidiParameterType)
CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR(VoiceLanguage)
CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR(EventType)

LIBVSQ_END_NAMESPACE

#undef CADENCII_DECLARE_ENUM_TO_OSTREAM_OPERATOR
