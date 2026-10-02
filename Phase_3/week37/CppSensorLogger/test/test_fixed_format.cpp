#include "unity.h"
#include "fixed_format.hpp"
#include <cstdint>
#include <cstring>

void setUp(void) {}
void tearDown(void) {}

// Formats `value` into a buffer pre-filled with 'X' and checks return value + text.
static void check(int32_t value, uint8_t frac, const char* expected)
{
    char buf[32];
    std::memset(buf, 'X', sizeof buf);
    const std::size_t n = format_fixed(buf, sizeof buf, value, frac);
    TEST_ASSERT_EQUAL_UINT(std::strlen(expected), n);
    TEST_ASSERT_EQUAL_STRING(expected, buf);
}

// --- Normal values -----------------------------------------------------------
static void test_zero(void)                 { check(0,    2, "0.00");  }
static void test_positive(void)             { check(150,  2, "1.50");  }
static void test_no_fraction_digits(void)   { check(7,    0, "7");     }

// --- Negatives: the Week 17 "%ld.%02ld" bugs ---------------------------------
static void test_negative(void)                        { check(-150, 2, "-1.50"); } // was "-1.-50"
static void test_negative_with_zero_integer_part(void) { check(-5,   2, "-0.05"); } // was "0.-5"
static void test_negative_whole(void)                  { check(-100, 2, "-1.00"); }

// --- Range limits ------------------------------------------------------------
static void test_int32_min(void)      { check(INT32_MIN, 2, "-21474836.48"); } // -INT32_MIN would be UB
static void test_int32_max(void)      { check(INT32_MAX, 2, "21474836.47");  }
static void test_max_frac_digits(void){ check(INT32_MIN, 9, "-2.147483648"); } // 10^9 still fits uint32_t

// --- Buffer capacity contract ------------------------------------------------
static void test_cap_exact_fit(void)
{
    char buf[16];
    std::memset(buf, 'X', sizeof buf);
    // "-1.50" = 5 chars + '\0' = 6 bytes
    TEST_ASSERT_EQUAL_UINT(5, format_fixed(buf, 6, -150, 2));
    TEST_ASSERT_EQUAL_STRING("-1.50", buf);
    TEST_ASSERT_EQUAL_CHAR('X', buf[6]);          // nothing written past cap
}

static void test_cap_too_small_returns_zero_and_empty_string(void)
{
    char buf[16];
    std::memset(buf, 'X', sizeof buf);
    TEST_ASSERT_EQUAL_UINT(0, format_fixed(buf, 5, -150, 2));
    TEST_ASSERT_EQUAL_STRING("", buf);            // out[0] set to '\0'
    TEST_ASSERT_EQUAL_CHAR('X', buf[1]);          // nothing else touched
}

static void test_cap_zero_writes_nothing(void)
{
    char buf[4];
    std::memset(buf, 'X', sizeof buf);
    TEST_ASSERT_EQUAL_UINT(0, format_fixed(buf, 0, 1, 0));
    TEST_ASSERT_EQUAL_CHAR('X', buf[0]);          // cap 0: not even a '\0'
}

static void test_null_buffer_returns_zero(void)
{
    TEST_ASSERT_EQUAL_UINT(0, format_fixed(nullptr, 16, 1, 0));
}

// --- Invalid arguments -------------------------------------------------------
static void test_frac_digits_too_large(void)
{
    char buf[32];
    std::memset(buf, 'X', sizeof buf);
    TEST_ASSERT_EQUAL_UINT(0, format_fixed(buf, sizeof buf, 1, 10)); // 10^10 overflows uint32_t
    TEST_ASSERT_EQUAL_STRING("", buf);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_zero);
    RUN_TEST(test_positive);
    RUN_TEST(test_no_fraction_digits);

    RUN_TEST(test_negative);
    RUN_TEST(test_negative_with_zero_integer_part);
    RUN_TEST(test_negative_whole);

    RUN_TEST(test_int32_min);
    RUN_TEST(test_int32_max);
    RUN_TEST(test_max_frac_digits);

    RUN_TEST(test_cap_exact_fit);
    RUN_TEST(test_cap_too_small_returns_zero_and_empty_string);
    RUN_TEST(test_cap_zero_writes_nothing);
    RUN_TEST(test_null_buffer_returns_zero);

    RUN_TEST(test_frac_digits_too_large);

    return UNITY_END();
}