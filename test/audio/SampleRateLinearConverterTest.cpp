#include <string>
#include <iostream>
#include "../Util.hpp"
#include "../../audio/SampleRateLinearConverter.hpp"
#include "AudioGeneratorStub.hpp"
#include "MemoryAudioOutput.hpp"
#include <gtest/gtest.h>

using namespace std;

class SampleRateLinearConverterStub : public cadencii::audio::SampleRateLinearConverter{
public:
    explicit SampleRateLinearConverterStub( int convertFrom, int convertTo, int bufferLength ) :
        SampleRateLinearConverter( convertFrom, convertTo )
    {
        delete [] bufferLeft;
        delete [] bufferRight;
        unitBufferLength = bufferLength;
        bufferLeft = new double[unitBufferLength];
        bufferRight = new double[unitBufferLength];
    }
};

TEST(SampleRateLinearConverterTest, testIncreaseSampleRate)
{
    const int convertFrom = 4;
    const int convertTo = 13;

    const int length = 6;
    double fixture[length];
    fixture[0] = -0.2; // 0.0 sec
    fixture[1] = 0.0; // 0.25 sec
    fixture[2] = 0.4; // 0.5 sec
    fixture[3] = 0.6; // 0.75 sec
    fixture[4] = 0.7; // 1.0 sec
    fixture[5] = 0.1; // 1.25 sec
    AudioGeneratorStub input( convertFrom, fixture, length );

    auto out = std::make_shared<MemoryAudioOutput>(convertTo);

    auto filter = std::make_shared<SampleRateLinearConverterStub>(convertFrom, convertTo, 10);

    input.setReceiver(filter);
    filter->setReceiver(out);
    input.start( length );

    int expectedLength = 17; // = convertTo * (length - 1) / convertFrom + 1;
    EXPECT_EQ( (uint64_t)expectedLength, out->getBufferLength() );

    double *expected = new double[expectedLength];
    expected[0] = /* 0 sec */ fixture[0];

    expected[1] = /* 0.0769230769230769 sec */ fixture[0] + convertFrom * (fixture[1] - fixture[0]) * (1.0 / convertTo - 0.0 / convertFrom);
    expected[2] = /* 0.153846153846154 sec */ fixture[0] + convertFrom * (fixture[1] - fixture[0]) * (2.0 / convertTo - 0.0 / convertFrom);
    expected[3] = /* 0.230769230769231 sec */ fixture[0] + convertFrom * (fixture[1] - fixture[0]) * (3.0 / convertTo - 0.0 / convertFrom);

    expected[4] = /* 0.307692307692308 sec */ fixture[1] + convertFrom * (fixture[2] - fixture[1]) * (4.0 / convertTo - 1.0 / convertFrom);
    expected[5] = /* 0.384615384615385 sec */ fixture[1] + convertFrom * (fixture[2] - fixture[1]) * (5.0 / convertTo - 1.0 / convertFrom);
    expected[6] = /* 0.461538461538462 sec */ fixture[1] + convertFrom * (fixture[2] - fixture[1]) * (6.0 / convertTo - 1.0 / convertFrom);

    expected[7] = /* 0.538461538461538 sec */ fixture[2] + convertFrom * (fixture[3] - fixture[2]) * (7.0 / convertTo - 2.0 / convertFrom);
    expected[8] = /* 0.615384615384615 sec */ fixture[2] + convertFrom * (fixture[3] - fixture[2]) * (8.0 / convertTo - 2.0 / convertFrom);
    expected[9] = /* 0.692307692307692 sec */ fixture[2] + convertFrom * (fixture[3] - fixture[2]) * (9.0 / convertTo - 2.0 / convertFrom);

    expected[10] = /* 0.769230769230769 sec */ fixture[3] + convertFrom * (fixture[4] - fixture[3]) * (10.0 / convertTo - 3.0 / convertFrom);
    expected[11] = /* 0.846153846153846 sec */ fixture[3] + convertFrom * (fixture[4] - fixture[3]) * (11.0 / convertTo - 3.0 / convertFrom);
    expected[12] = /* 0.923076923076923 sec */ fixture[3] + convertFrom * (fixture[4] - fixture[3]) * (12.0 / convertTo - 3.0 / convertFrom);

    expected[13] = /* 1 sec */ fixture[4];

    expected[14] = /* 1.07692307692308 sec */ fixture[4] + convertFrom * (fixture[5] - fixture[4]) * (14.0 / convertTo - 4.0 / convertFrom);
    expected[15] = /* 1.15384615384615 sec */ fixture[4] + convertFrom * (fixture[5] - fixture[4]) * (15.0 / convertTo - 4.0 / convertFrom);
    expected[16] = /* 1.23076923076923 sec */ fixture[4] + convertFrom * (fixture[5] - fixture[4]) * (16.0 / convertTo - 4.0 / convertFrom);

    double *actualLeft = new double[expectedLength];
    double *actualRight = new double[expectedLength];
    out->getResult( actualLeft, actualRight, expectedLength );

    for( int i = 0; i < expectedLength; i++ ){
        EXPECT_FLOAT_EQ(expected[i], actualLeft[i]);
        EXPECT_FLOAT_EQ(expected[i], actualRight[i]);
    }

    delete [] expected;
    delete [] actualLeft;
    delete [] actualRight;
}

TEST(SampleRateLinearConverterTest, testDecreaseSampleRate)
{
    const int convertFrom = 17;
    const int convertTo = 4;

    const int length = 6;
    double fixture[length];
    fixture[0] = 0.0;/* 0 sec */
    fixture[1] = 0.5;/* 0.058823529 sec */
    fixture[2] = 0.7;/* 0.117647059 sec */
    fixture[3] = 0.6;/* 0.176470588 sec */
    fixture[4] = -0.1;/* 0.235294118 sec */
    fixture[5] = -0.3;/* 0.294117647 sec */
    AudioGeneratorStub input( convertFrom, fixture, length );

    auto out = std::make_shared<MemoryAudioOutput>(convertTo);

    auto filter = std::make_shared<SampleRateLinearConverterStub>(convertFrom, convertTo, 13);

    input.setReceiver(filter);
    filter->setReceiver(out);
    input.start( length );

    int expectedLength = 2; // = convertTo * (length - 1) / convertFrom + 1;
    EXPECT_EQ( (uint64_t)expectedLength, out->getBufferLength() );

    double *expected = new double[expectedLength];
    expected[0] = /* 0 sec */ fixture[0];
    expected[1] = /* 0.25 sec */ fixture[4] + convertFrom * (fixture[5] - fixture[4]) * (1.0 / convertTo - 4.0 / convertFrom);

    double *actualLeft = new double[expectedLength];
    double *actualRight = new double[expectedLength];
    out->getResult( actualLeft, actualRight, expectedLength );

    for( int i = 0; i < expectedLength; i++ ){
        EXPECT_FLOAT_EQ(expected[i], actualLeft[i]);
        EXPECT_FLOAT_EQ(expected[i], actualRight[i]);
    }

    delete [] expected;
    delete [] actualLeft;
    delete [] actualRight;
}

TEST(SampleRateLinearConverterTest, testIncreaseSampleRateByMinimumPushLength)
{
    const int convertFrom = 4;
    const int convertTo = 13;

    const int length = 6;
    double fixture[length];
    fixture[0] = -0.2; // 0.0 sec
    fixture[1] = 0.0; // 0.25 sec
    fixture[2] = 0.4; // 0.5 sec
    fixture[3] = 0.6; // 0.75 sec
    fixture[4] = 0.7; // 1.0 sec
    fixture[5] = 0.1; // 1.25 sec

    auto out = std::make_shared<MemoryAudioOutput>(convertTo);
    SampleRateLinearConverterStub filter( convertFrom, convertTo, 10 );

    double value;
    filter.setReceiver(out);
    value = fixture[0];
    filter.push( &value, &value, 1, 0 );
    value = fixture[1];
    filter.push( &value, &value, 1, 0 );
    value = fixture[2];
    filter.push( &value, &value, 1, 0 );
    value = fixture[3];
    filter.push( &value, &value, 1, 0 );
    value = fixture[4];
    filter.push( &value, &value, 1, 0 );
    value = fixture[5];
    filter.push( &value, &value, 1, 0 );
    filter.flush();

    int expectedLength = 17; // = convertTo * (length - 1) / convertFrom + 1;
    EXPECT_EQ( (uint64_t)expectedLength, out->getBufferLength() );

    double *expected = new double[expectedLength];
    expected[0] = /* 0 sec */ fixture[0];

    expected[1] = /* 0.0769230769230769 sec */ fixture[0] + convertFrom * (fixture[1] - fixture[0]) * (1.0 / convertTo - 0.0 / convertFrom);
    expected[2] = /* 0.153846153846154 sec */ fixture[0] + convertFrom * (fixture[1] - fixture[0]) * (2.0 / convertTo - 0.0 / convertFrom);
    expected[3] = /* 0.230769230769231 sec */ fixture[0] + convertFrom * (fixture[1] - fixture[0]) * (3.0 / convertTo - 0.0 / convertFrom);

    expected[4] = /* 0.307692307692308 sec */ fixture[1] + convertFrom * (fixture[2] - fixture[1]) * (4.0 / convertTo - 1.0 / convertFrom);
    expected[5] = /* 0.384615384615385 sec */ fixture[1] + convertFrom * (fixture[2] - fixture[1]) * (5.0 / convertTo - 1.0 / convertFrom);
    expected[6] = /* 0.461538461538462 sec */ fixture[1] + convertFrom * (fixture[2] - fixture[1]) * (6.0 / convertTo - 1.0 / convertFrom);

    expected[7] = /* 0.538461538461538 sec */ fixture[2] + convertFrom * (fixture[3] - fixture[2]) * (7.0 / convertTo - 2.0 / convertFrom);
    expected[8] = /* 0.615384615384615 sec */ fixture[2] + convertFrom * (fixture[3] - fixture[2]) * (8.0 / convertTo - 2.0 / convertFrom);
    expected[9] = /* 0.692307692307692 sec */ fixture[2] + convertFrom * (fixture[3] - fixture[2]) * (9.0 / convertTo - 2.0 / convertFrom);

    expected[10] = /* 0.769230769230769 sec */ fixture[3] + convertFrom * (fixture[4] - fixture[3]) * (10.0 / convertTo - 3.0 / convertFrom);
    expected[11] = /* 0.846153846153846 sec */ fixture[3] + convertFrom * (fixture[4] - fixture[3]) * (11.0 / convertTo - 3.0 / convertFrom);
    expected[12] = /* 0.923076923076923 sec */ fixture[3] + convertFrom * (fixture[4] - fixture[3]) * (12.0 / convertTo - 3.0 / convertFrom);

    expected[13] = /* 1 sec */ fixture[4];

    expected[14] = /* 1.07692307692308 sec */ fixture[4] + convertFrom * (fixture[5] - fixture[4]) * (14.0 / convertTo - 4.0 / convertFrom);
    expected[15] = /* 1.15384615384615 sec */ fixture[4] + convertFrom * (fixture[5] - fixture[4]) * (15.0 / convertTo - 4.0 / convertFrom);
    expected[16] = /* 1.23076923076923 sec */ fixture[4] + convertFrom * (fixture[5] - fixture[4]) * (16.0 / convertTo - 4.0 / convertFrom);

    double *actualLeft = new double[expectedLength];
    double *actualRight = new double[expectedLength];
    out->getResult( actualLeft, actualRight, expectedLength );

    for( int i = 0; i < expectedLength; i++ ){
        EXPECT_FLOAT_EQ(expected[i], actualLeft[i]);
        EXPECT_FLOAT_EQ(expected[i], actualRight[i]);
    }

    delete [] expected;
    delete [] actualLeft;
    delete [] actualRight;
}

TEST(SampleRateLinearConverterTest, testDecreaseSampleRateByMinimumPushLength)
{
    const int convertFrom = 17;
    const int convertTo = 4;

    const int length = 6;
    double fixture[length];
    fixture[0] = 0.0;/* 0 sec */
    fixture[1] = 0.5;/* 0.058823529 sec */
    fixture[2] = 0.7;/* 0.117647059 sec */
    fixture[3] = 0.6;/* 0.176470588 sec */
    fixture[4] = -0.1;/* 0.235294118 sec */
    fixture[5] = -0.3;/* 0.294117647 sec */

    auto out = std::make_shared<MemoryAudioOutput>(convertTo);
    SampleRateLinearConverterStub filter( convertFrom, convertTo, 13 );
    filter.setReceiver(out);

    double value;
    value = fixture[0];
    filter.push( &value, &value, 1, 0 );
    value = fixture[1];
    filter.push( &value, &value, 1, 0 );
    value = fixture[2];
    filter.push( &value, &value, 1, 0 );
    value = fixture[3];
    filter.push( &value, &value, 1, 0 );
    value = fixture[4];
    filter.push( &value, &value, 1, 0 );
    value = fixture[5];
    filter.push( &value, &value, 1, 0 );

    int expectedLength = 2; // = convertTo * (length - 1) / convertFrom + 1;
    EXPECT_EQ( (uint64_t)expectedLength, out->getBufferLength() );

    double *expected = new double[expectedLength];
    expected[0] = /* 0 sec */ fixture[0];
    expected[1] = /* 0.25 sec */ fixture[4] + convertFrom * (fixture[5] - fixture[4]) * (1.0 / convertTo - 4.0 / convertFrom);

    double *actualLeft = new double[expectedLength];
    double *actualRight = new double[expectedLength];
    out->getResult( actualLeft, actualRight, expectedLength );

    for( int i = 0; i < expectedLength; i++ ){
        EXPECT_FLOAT_EQ(expected[i], actualLeft[i]);
        EXPECT_FLOAT_EQ(expected[i], actualRight[i]);
    }

    delete [] expected;
    delete [] actualLeft;
    delete [] actualRight;
}
