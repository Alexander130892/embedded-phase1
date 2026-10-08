#include "unity.h"
#include "ring_buffer.hpp"

#include <cstdint>

void setUp(void) {}
void tearDown(void) {}

// --- Empty buffer ------------------------------------------------------------
static void test_new_buffer_is_empty(void)
{
    ring_buffer<uint8_t, 8> rb;
    TEST_ASSERT_TRUE(rb.is_empty());
    TEST_ASSERT_EQUAL_UINT32(0, rb.get_count());
}

static void test_pop_on_empty_returns_false_and_leaves_out_untouched(void)
{
    ring_buffer<uint8_t, 8> rb;
    uint8_t out = 0xAA;
    TEST_ASSERT_FALSE(rb.pop(&out));
    TEST_ASSERT_EQUAL_HEX8(0xAA, out);
}

// --- Basic operation ---------------------------------------------------------
static void test_push_then_pop_returns_same_value(void)
{
    ring_buffer<uint8_t, 8> rb;
    uint8_t out = 0;
    TEST_ASSERT_TRUE(rb.push(42));
    TEST_ASSERT_FALSE(rb.is_empty());
    TEST_ASSERT_TRUE(rb.pop(&out));
    TEST_ASSERT_EQUAL_UINT8(42, out);
    TEST_ASSERT_TRUE(rb.is_empty());
}

static void test_fifo_order(void)
{
    ring_buffer<uint8_t, 8> rb;
    for (uint8_t i = 1; i <= 5; ++i) { rb.push(i); }
    for (uint8_t i = 1; i <= 5; ++i) {
        uint8_t out = 0;
        TEST_ASSERT_TRUE(rb.pop(&out));
        TEST_ASSERT_EQUAL_UINT8(i, out);
    }
    TEST_ASSERT_TRUE(rb.is_empty());
}

// --- Capacity: one slot is always kept empty ---------------------------------
static void test_capacity_is_n_minus_one(void)
{
    ring_buffer<uint8_t, 8> rb;
    TEST_ASSERT_EQUAL_UINT32(7, rb.capacity());
    for (uint8_t i = 0; i < 7; ++i) {
        TEST_ASSERT_TRUE(rb.push(i));
    }
    TEST_ASSERT_EQUAL_UINT32(7, rb.get_count());
    TEST_ASSERT_FALSE(rb.push(99));                 // 8th push rejected
    TEST_ASSERT_EQUAL_UINT32(7, rb.get_count());
}

// Week 17 regression: filling the buffer used to wrap head onto tail,
// making a full buffer look empty and losing every byte in it.
static void test_full_buffer_is_not_seen_as_empty(void)
{
    ring_buffer<uint8_t, 8> rb;
    for (uint8_t i = 0; i < 20; ++i) { rb.push(i); } // far more than fits
    TEST_ASSERT_FALSE(rb.is_empty());
    TEST_ASSERT_EQUAL_UINT32(7, rb.get_count());
}

// Full buffer drops the NEW element; the oldest data survives.
static void test_full_buffer_keeps_oldest_and_drops_newest(void)
{
    ring_buffer<uint8_t, 4> rb;                     // capacity 3
    rb.push(1); rb.push(2); rb.push(3);
    TEST_ASSERT_FALSE(rb.push(4));                  // dropped
    uint8_t out = 0;
    rb.pop(&out); TEST_ASSERT_EQUAL_UINT8(1, out);
    rb.pop(&out); TEST_ASSERT_EQUAL_UINT8(2, out);
    rb.pop(&out); TEST_ASSERT_EQUAL_UINT8(3, out);
    TEST_ASSERT_FALSE(rb.pop(&out));                // 4 never appears
}

// After a pop frees a slot, push works again.
static void test_push_succeeds_again_after_pop_from_full(void)
{
    ring_buffer<uint8_t, 4> rb;
    rb.push(1); rb.push(2); rb.push(3);
    uint8_t out = 0;
    rb.pop(&out);
    TEST_ASSERT_TRUE(rb.push(4));
    rb.pop(&out); TEST_ASSERT_EQUAL_UINT8(2, out);
    rb.pop(&out); TEST_ASSERT_EQUAL_UINT8(3, out);
    rb.pop(&out); TEST_ASSERT_EQUAL_UINT8(4, out);
}

// --- Wraparound --------------------------------------------------------------
// Interleave push/pop so head and tail lap the array many times.
static void test_wraparound_preserves_order_and_count(void)
{
    ring_buffer<uint8_t, 5> rb;
    uint8_t next_in = 0, next_out = 0;
    for (int round = 0; round < 50; ++round) {
        TEST_ASSERT_TRUE(rb.push(next_in++));
        TEST_ASSERT_TRUE(rb.push(next_in++));
        TEST_ASSERT_EQUAL_UINT32(2, rb.get_count());
        uint8_t out = 0;
        TEST_ASSERT_TRUE(rb.pop(&out)); TEST_ASSERT_EQUAL_UINT8(next_out++, out);
        TEST_ASSERT_TRUE(rb.pop(&out)); TEST_ASSERT_EQUAL_UINT8(next_out++, out);
        TEST_ASSERT_TRUE(rb.is_empty());
    }
}

// get_count() must stay right when head has wrapped below tail.
static void test_count_when_head_wrapped_below_tail(void)
{
    ring_buffer<uint8_t, 8> rb;
    uint8_t out = 0;
    for (uint8_t i = 0; i < 6; ++i) { rb.push(i); }  // head = 6
    for (uint8_t i = 0; i < 5; ++i) { rb.pop(&out); } // tail = 5
    for (uint8_t i = 0; i < 4; ++i) { rb.push(i); }  // head wraps: 6,7,0,1 -> head = 2
    TEST_ASSERT_EQUAL_UINT32(5, rb.get_count());      // 1 left + 4 new
}

// --- Other element types -----------------------------------------------------
struct Sample { int16_t x, y, z; };

static void test_works_with_struct_elements(void)
{
    ring_buffer<Sample, 4> rb;
    TEST_ASSERT_TRUE(rb.push(Sample{1, -2, 3}));
    Sample s{};
    TEST_ASSERT_TRUE(rb.pop(&s));
    TEST_ASSERT_EQUAL_INT16(1, s.x);
    TEST_ASSERT_EQUAL_INT16(-2, s.y);
    TEST_ASSERT_EQUAL_INT16(3, s.z);
}

// --- Constant initialisation (no startup code needed) ------------------------
constinit ring_buffer<char, 64> g_rx;   // fails to compile if not constant-initialised

static void test_constinit_global_starts_empty(void)
{
    TEST_ASSERT_TRUE(g_rx.is_empty());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_new_buffer_is_empty);
    RUN_TEST(test_pop_on_empty_returns_false_and_leaves_out_untouched);

    RUN_TEST(test_push_then_pop_returns_same_value);
    RUN_TEST(test_fifo_order);

    RUN_TEST(test_capacity_is_n_minus_one);
    RUN_TEST(test_full_buffer_is_not_seen_as_empty);
    RUN_TEST(test_full_buffer_keeps_oldest_and_drops_newest);
    RUN_TEST(test_push_succeeds_again_after_pop_from_full);

    RUN_TEST(test_wraparound_preserves_order_and_count);
    RUN_TEST(test_count_when_head_wrapped_below_tail);

    RUN_TEST(test_works_with_struct_elements);
    RUN_TEST(test_constinit_global_starts_empty);

    return UNITY_END();
}