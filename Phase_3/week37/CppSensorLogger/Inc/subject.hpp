#pragma once

#include <array>
#include <cstddef>
#include "etl/delegate.h"

// Observer subject: keeps up to N callbacks and calls each one with every published event.
// No heap: the subscriber list is a fixed array. Wiring is static (subscribe once at startup).
template <class Event, std::size_t N>
class Subject {
    public:
        using Callback = etl::delegate<void(const Event&)>;

        static_assert(N >= 1, "a subject without subscriber slots is useless");

        constexpr Subject() = default;

        Subject(const Subject&)            = delete;   // subscribers point to *this* subject's observers
        Subject& operator=(const Subject&) = delete;

        // Adds a callback. Returns false if all N slots are taken (or cb is empty).
        [[nodiscard]] bool subscribe(Callback cb)
        {
           
            if(!cb.is_valid()){   // Reject an empty delegate
                return false;
            }else if(count_ >= N){         // Reject when full
                return false;
            }else{              // Store cb, bump the count, return true
                subscribers_[count_]=cb;
                count_++;
                return true;
            } 
        }

        // Calls every subscriber, in subscription order.
        void publish(const Event& e) const
        {
            // TODO 4: loop over the *used* slots only and call each callback with e
            for (std::size_t i = 0; i < count_; ++i) {
                subscribers_[i](e);
            }
        }

        std::size_t subscriber_count() const { 
            return count_; 
        }
        static constexpr std::size_t capacity() { 
            return N; 
        }

    private:
        std::array<Callback, N> subscribers_{};
        std::size_t             count_{0};
};