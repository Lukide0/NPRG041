#include "utils.h"
#include <doctest.h>
#include <koterm/event/EventListener.h>
#include <koterm/event/EventSender.h>
#include <thread>

using namespace koterm::event;

constexpr double TIMEOUT_SINGLE   = 0.5; // 0.5s
constexpr double TIMEOUT_MULTIPLE = 1; // 1s

constexpr std::size_t EVENTS_COUNT_PER_THREAD = 10;

void sender_event(EventListener<int>* listener) {
    constexpr auto events = create_sequence<int, EVENTS_COUNT_PER_THREAD>();

    auto sender = listener->make_sender();

    for (auto&& event : events) {
        sender.send(event);
    }
}

TEST_CASE("Events: 1 thread" * doctest::timeout(TIMEOUT_SINGLE)) {
    EventListener<int> listener;

    auto sender_1 = listener.make_sender();
    auto sender_2 = listener.make_sender();

    int out = 0;

    CHECK_EQ(listener.get_non_blocking(out), false);

    sender_1.send(1);
    sender_2.send(2);

    REQUIRE_EQ(listener.get_non_blocking(out), true);
    CHECK_EQ(out, 1);

    REQUIRE_EQ(listener.get_non_blocking(out), true);
    CHECK_EQ(out, 2);

    CHECK_EQ(listener.get_non_blocking(out), false);
}

TEST_CASE("Events: 2 threads" * doctest::timeout(TIMEOUT_MULTIPLE)) {
    EventListener<int> listener;

    std::thread thread { sender_event, &listener };
    // use instead of atomic_flag
    auto _ = listener.make_sender();

    int event = 0;

    for (std::size_t i = 0; i < EVENTS_COUNT_PER_THREAD; i++) {
        CHECK_EQ(listener.get_blocking(event), true);
        CHECK_EQ(event, i);
    }

    thread.join();
    CHECK_EQ(listener.has_pending(), false);
}
