// test/test_subject.cpp — host tests for Subject<Event, N>
#include <array>
#include <cstddef>

#include "subject.hpp"
#include "unity.h"

namespace {

struct TestEvent {
    int value;
};

// Observer that records how it was called.
struct Recorder {
    int              calls = 0;
    int              last_value = 0;
    const TestEvent* last_address = nullptr;

    void on_event(const TestEvent& e)
    {
        ++calls;
        last_value = e.value;
        last_address = &e;
    }
};

// Shared call log to check the order in which observers run.
std::array<int, 8> g_order{};
std::size_t        g_order_len = 0;

struct Tagged {
    int tag;
    void on_event(const TestEvent&)
    {
        if (g_order_len < g_order.size()) {
            g_order[g_order_len++] = tag;
        }
    }
};

// Free function observer.
int g_free_calls = 0;
void free_observer(const TestEvent&) { ++g_free_calls; }

using Sub3 = Subject<TestEvent, 3>;
using Cb   = Sub3::Callback;

Cb bind(Recorder& r) { return Cb::create<Recorder, &Recorder::on_event>(r); }
Cb bind(Tagged& t)   { return Cb::create<Tagged, &Tagged::on_event>(t); }

} // namespace

void setUp(void)
{
    g_order.fill(0);
    g_order_len = 0;
    g_free_calls = 0;
}

void tearDown(void) {}

// --- subscribe -------------------------------------------------------------

static void test_starts_empty(void)
{
    Sub3 s;
    TEST_ASSERT_EQUAL_UINT(0, s.subscriber_count());
    TEST_ASSERT_EQUAL_UINT(3, Sub3::capacity());
}

static void test_subscribe_valid_returns_true(void)
{
    Sub3 s;
    Recorder r;
    TEST_ASSERT_TRUE(s.subscribe(bind(r)));
    TEST_ASSERT_EQUAL_UINT(1, s.subscriber_count());
}

static void test_subscribe_empty_delegate_rejected(void)
{
    Sub3 s;
    Cb empty;                                   // default-constructed: stub == nullptr
    TEST_ASSERT_FALSE(s.subscribe(empty));
    TEST_ASSERT_EQUAL_UINT(0, s.subscriber_count());
}

static void test_subscribe_when_full_rejected(void)
{
    Sub3 s;
    Recorder a, b, c, d;
    TEST_ASSERT_TRUE(s.subscribe(bind(a)));
    TEST_ASSERT_TRUE(s.subscribe(bind(b)));
    TEST_ASSERT_TRUE(s.subscribe(bind(c)));
    TEST_ASSERT_FALSE(s.subscribe(bind(d)));    // 4th into 3 slots
    TEST_ASSERT_EQUAL_UINT(3, s.subscriber_count());

    s.publish(TestEvent{1});
    TEST_ASSERT_EQUAL_INT(0, d.calls);          // the rejected one is really not stored
}

// --- publish ---------------------------------------------------------------

static void test_publish_without_subscribers_is_harmless(void)
{
    Sub3 s;
    s.publish(TestEvent{42});                   // must not call an empty slot
    TEST_PASS();
}

static void test_publish_partially_filled_skips_empty_slots(void)
{
    // Regression test for looping over all N slots: 2 of 3 used.
    Sub3 s;
    Recorder a, b;
    TEST_ASSERT_TRUE(s.subscribe(bind(a)));
    TEST_ASSERT_TRUE(s.subscribe(bind(b)));
    s.publish(TestEvent{7});                    // would crash on slot 2 if it were called
    TEST_ASSERT_EQUAL_INT(1, a.calls);
    TEST_ASSERT_EQUAL_INT(1, b.calls);
}

static void test_publish_delivers_event_value(void)
{
    Sub3 s;
    Recorder r;
    TEST_ASSERT_TRUE(s.subscribe(bind(r)));
    s.publish(TestEvent{-123});
    TEST_ASSERT_EQUAL_INT(1, r.calls);
    TEST_ASSERT_EQUAL_INT(-123, r.last_value);
}

static void test_publish_passes_event_by_reference(void)
{
    Sub3 s;
    Recorder r;
    TEST_ASSERT_TRUE(s.subscribe(bind(r)));
    const TestEvent e{5};
    s.publish(e);
    TEST_ASSERT_EQUAL_PTR(&e, r.last_address);  // no copy on the way
}

static void test_publish_calls_in_subscription_order(void)
{
    Sub3 s;
    Tagged t1{1}, t2{2}, t3{3};
    TEST_ASSERT_TRUE(s.subscribe(bind(t2)));
    TEST_ASSERT_TRUE(s.subscribe(bind(t3)));
    TEST_ASSERT_TRUE(s.subscribe(bind(t1)));
    s.publish(TestEvent{0});
    TEST_ASSERT_EQUAL_UINT(3, g_order_len);
    TEST_ASSERT_EQUAL_INT(2, g_order[0]);
    TEST_ASSERT_EQUAL_INT(3, g_order[1]);
    TEST_ASSERT_EQUAL_INT(1, g_order[2]);
}

static void test_every_publish_reaches_every_subscriber(void)
{
    Sub3 s;
    Recorder a, b;
    TEST_ASSERT_TRUE(s.subscribe(bind(a)));
    TEST_ASSERT_TRUE(s.subscribe(bind(b)));
    s.publish(TestEvent{1});
    s.publish(TestEvent{2});
    s.publish(TestEvent{3});
    TEST_ASSERT_EQUAL_INT(3, a.calls);
    TEST_ASSERT_EQUAL_INT(3, b.calls);
    TEST_ASSERT_EQUAL_INT(3, b.last_value);
}

static void test_free_function_observer(void)
{
    Sub3 s;
    TEST_ASSERT_TRUE(s.subscribe(Cb::create<&free_observer>()));
    s.publish(TestEvent{0});
    s.publish(TestEvent{0});
    TEST_ASSERT_EQUAL_INT(2, g_free_calls);
}

static void test_same_observer_twice_is_called_twice(void)
{
    // Documents current behaviour: no duplicate detection.
    Sub3 s;
    Recorder r;
    TEST_ASSERT_TRUE(s.subscribe(bind(r)));
    TEST_ASSERT_TRUE(s.subscribe(bind(r)));
    s.publish(TestEvent{9});
    TEST_ASSERT_EQUAL_INT(2, r.calls);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_starts_empty);
    RUN_TEST(test_subscribe_valid_returns_true);
    RUN_TEST(test_subscribe_empty_delegate_rejected);
    RUN_TEST(test_subscribe_when_full_rejected);
    RUN_TEST(test_publish_without_subscribers_is_harmless);
    RUN_TEST(test_publish_partially_filled_skips_empty_slots);
    RUN_TEST(test_publish_delivers_event_value);
    RUN_TEST(test_publish_passes_event_by_reference);
    RUN_TEST(test_publish_calls_in_subscription_order);
    RUN_TEST(test_every_publish_reaches_every_subscriber);
    RUN_TEST(test_free_function_observer);
    RUN_TEST(test_same_observer_twice_is_called_twice);
    return UNITY_END();
}