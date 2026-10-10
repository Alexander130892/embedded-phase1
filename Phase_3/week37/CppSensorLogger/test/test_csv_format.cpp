// test/test_csv_format.cpp — host tests for format_csv
#include <cstring>

#include "csv_format.hpp"
#include "unity.h"

namespace {

LogRecord make_record()
{
    LogRecord r{};
    r.timestamp_ms = 12345;
    r.env = EnvSample{1971, 101140};               // 19.71 °C, 1011.40 hPa
    r.imu = ImuSample{-157, 881, -392, 687, 877, 1534};
    r.env_valid = true;
    r.imu_valid = true;
    return r;
}

char buf[kCsvLineMax];

} // namespace

void setUp(void) { std::memset(buf, 'X', sizeof buf); }
void tearDown(void) {}

// --- content ---------------------------------------------------------------

static void test_full_record(void)
{
    const LogRecord r = make_record();
    const std::size_t n = format_csv(buf, sizeof buf, r);
    TEST_ASSERT_EQUAL_STRING(
        "12345,19.71,1011.40,-0.157,0.881,-0.392,0.687,0.877,1.534\r\n", buf);
    TEST_ASSERT_EQUAL_UINT(std::strlen(buf), n);
}

static void test_negative_temperature_and_small_magnitudes(void)
{
    // The Week 17 bug: -50 printed as "0.-50". And values below 1 need a leading "0".
    LogRecord r = make_record();
    r.env.temp_centi_c = -50;
    r.imu = ImuSample{-5, 0, 1000, -1, 0, 1};
    (void)format_csv(buf, sizeof buf, r);
    TEST_ASSERT_EQUAL_STRING(
        "12345,-0.50,1011.40,-0.005,0.000,1.000,-0.001,0.000,0.001\r\n", buf);
}

static void test_env_invalid_leaves_two_empty_fields(void)
{
    LogRecord r = make_record();
    r.env_valid = false;
    (void)format_csv(buf, sizeof buf, r);
    TEST_ASSERT_EQUAL_STRING(
        "12345,,,-0.157,0.881,-0.392,0.687,0.877,1.534\r\n", buf);
}

static void test_imu_invalid_leaves_six_empty_fields(void)
{
    LogRecord r = make_record();
    r.imu_valid = false;
    (void)format_csv(buf, sizeof buf, r);
    TEST_ASSERT_EQUAL_STRING("12345,19.71,1011.40,,,,,,\r\n", buf);
}

static void test_both_invalid_keeps_column_count(void)
{
    LogRecord r = make_record();
    r.env_valid = false;
    r.imu_valid = false;
    (void)format_csv(buf, sizeof buf, r);
    TEST_ASSERT_EQUAL_STRING("12345,,,,,,,,\r\n", buf);
}

static void test_column_count_matches_header(void)
{
    // Every line must have as many commas as the header, valid or not.
    auto commas = [](const char* s) {
        std::size_t c = 0;
        for (; *s != '\0'; ++s) c += (*s == ',');
        return c;
    };
    const std::size_t header_commas = commas(kCsvHeader);

    LogRecord r = make_record();
    for (int mask = 0; mask < 4; ++mask) {
        r.env_valid = (mask & 1) != 0;
        r.imu_valid = (mask & 2) != 0;
        TEST_ASSERT_NOT_EQUAL(0, format_csv(buf, sizeof buf, r));
        TEST_ASSERT_EQUAL_UINT(header_commas, commas(buf));
    }
}

static void test_timestamp_beyond_int32(void)
{
    // After ~24.8 days the ms counter passes INT32_MAX; it must not print negative.
    LogRecord r = make_record();
    r.timestamp_ms = 4294967295u;
    r.env_valid = false;
    r.imu_valid = false;
    (void)format_csv(buf, sizeof buf, r);
    TEST_ASSERT_EQUAL_STRING("4294967295,,,,,,,,\r\n", buf);
}

static void test_timestamp_zero(void)
{
    LogRecord r = make_record();
    r.timestamp_ms = 0;
    r.env_valid = false;
    r.imu_valid = false;
    (void)format_csv(buf, sizeof buf, r);
    TEST_ASSERT_EQUAL_STRING("0,,,,,,,,\r\n", buf);
}

// --- capacity contract -----------------------------------------------------

static void test_worst_case_fits_in_kCsvLineMax(void)
{
    LogRecord r{};
    r.timestamp_ms = 4294967295u;
    r.env = EnvSample{INT32_MIN, INT32_MIN};
    r.imu = ImuSample{INT32_MIN, INT32_MIN, INT32_MIN, INT32_MIN, INT32_MIN, INT32_MIN};
    r.env_valid = true;
    r.imu_valid = true;
    const std::size_t n = format_csv(buf, sizeof buf, r);
    TEST_ASSERT_NOT_EQUAL(0, n);
    TEST_ASSERT_LESS_THAN_UINT(kCsvLineMax, n + 1);   // including '\0'
}

static void test_exact_fit_succeeds(void)
{
    const LogRecord r = make_record();
    const std::size_t len = format_csv(buf, sizeof buf, r);
    char small[kCsvLineMax];
    TEST_ASSERT_EQUAL_UINT(len, format_csv(small, len + 1, r));   // len chars + '\0'
    TEST_ASSERT_EQUAL_STRING(buf, small);
}

static void test_one_byte_short_writes_nothing(void)
{
    const LogRecord r = make_record();
    const std::size_t len = format_csv(buf, sizeof buf, r);
    char small[kCsvLineMax];
    std::memset(small, 'X', sizeof small);
    TEST_ASSERT_EQUAL_UINT(0, format_csv(small, len, r));         // no room for '\0'
    TEST_ASSERT_EQUAL_CHAR('\0', small[0]);                       // empty, not truncated
}

static void test_too_small_in_the_middle_writes_nothing(void)
{
    // Fails inside a format_fixed call, not in the separators.
    const LogRecord r = make_record();
    char small[12];
    TEST_ASSERT_EQUAL_UINT(0, format_csv(small, sizeof small, r));
    TEST_ASSERT_EQUAL_CHAR('\0', small[0]);
}

static void test_cap_one_writes_empty_string(void)
{
    const LogRecord r = make_record();
    char one[1] = {'X'};
    TEST_ASSERT_EQUAL_UINT(0, format_csv(one, 1, r));
    TEST_ASSERT_EQUAL_CHAR('\0', one[0]);
}

static void test_null_or_zero_cap_is_harmless(void)
{
    const LogRecord r = make_record();
    TEST_ASSERT_EQUAL_UINT(0, format_csv(nullptr, 64, r));
    TEST_ASSERT_EQUAL_UINT(0, format_csv(buf, 0, r));
    TEST_ASSERT_EQUAL_CHAR('X', buf[0]);                          // cap 0: untouched
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_full_record);
    RUN_TEST(test_negative_temperature_and_small_magnitudes);
    RUN_TEST(test_env_invalid_leaves_two_empty_fields);
    RUN_TEST(test_imu_invalid_leaves_six_empty_fields);
    RUN_TEST(test_both_invalid_keeps_column_count);
    RUN_TEST(test_column_count_matches_header);
    RUN_TEST(test_timestamp_beyond_int32);
    RUN_TEST(test_timestamp_zero);
    RUN_TEST(test_worst_case_fits_in_kCsvLineMax);
    RUN_TEST(test_exact_fit_succeeds);
    RUN_TEST(test_one_byte_short_writes_nothing);
    RUN_TEST(test_too_small_in_the_middle_writes_nothing);
    RUN_TEST(test_cap_one_writes_empty_string);
    RUN_TEST(test_null_or_zero_cap_is_harmless);
    return UNITY_END();
}