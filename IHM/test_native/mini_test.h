#pragma once
// Tiny header-only test framework for host-native unit tests of this
// firmware's hardware-independent logic. Not a general-purpose library:
// just enough (TEST/CHECK_TRUE/CHECK_EQ + auto-registration) to run our
// suites as plain .exe files on a dev machine, without pulling in
// PlatformIO's native platform (no gcc is installed on this machine) or
// any third-party framework.
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

namespace mini_test {

struct Test {
    const char* suite;
    const char* name;
    std::function<void()> fn;
};

inline std::vector<Test>& registry() {
    static std::vector<Test> r;
    return r;
}

inline int& failure_count() {
    static int f = 0;
    return f;
}

inline const char*& current_test_name() {
    static const char* n = "";
    return n;
}

struct Registrar {
    Registrar(const char* suite, const char* name, std::function<void()> fn) {
        registry().push_back({suite, name, std::move(fn)});
    }
};

inline int run_all() {
    int total = 0;
    int failed_tests = 0;
    for (auto& t : registry()) {
        total++;
        int before = failure_count();
        current_test_name() = t.name;
        std::printf("[ RUN  ] %s.%s\n", t.suite, t.name);
        t.fn();
        if (failure_count() != before) {
            failed_tests++;
            std::printf("[ FAIL ] %s.%s\n", t.suite, t.name);
        } else {
            std::printf("[  OK  ] %s.%s\n", t.suite, t.name);
        }
    }
    std::printf("\n%d test case(s) run, %d failed, %d assertion failure(s) total.\n",
                total, failed_tests, failure_count());
    return failed_tests == 0 ? 0 : 1;
}

}  // namespace mini_test

// Defines a test function under `suite` and registers it to run.
// Usage: TEST(SuiteName, TestName) { ... CHECK_*(...) ... }
#define TEST(suite, name)                                                  \
    static void suite##_##name##_fn();                                    \
    static mini_test::Registrar suite##_##name##_registrar(               \
        #suite, #name, suite##_##name##_fn);                              \
    static void suite##_##name##_fn()

#define CHECK_TRUE(cond)                                                   \
    do {                                                                   \
        if (!(cond)) {                                                     \
            std::printf("    %s:%d: CHECK_TRUE(%s) failed\n", __FILE__,    \
                         __LINE__, #cond);                                 \
            mini_test::failure_count()++;                                  \
        }                                                                  \
    } while (0)

#define CHECK_FALSE(cond) CHECK_TRUE(!(cond))

#define CHECK_EQ(actual, expected)                                         \
    do {                                                                   \
        auto actual_ = (actual);                                          \
        auto expected_ = (expected);                                      \
        if (!(actual_ == expected_)) {                                     \
            std::printf(                                                   \
                "    %s:%d: CHECK_EQ(%s, %s) failed: got=%lld expected=%lld\n", \
                __FILE__, __LINE__, #actual, #expected,                    \
                (long long)actual_, (long long)expected_);                 \
            mini_test::failure_count()++;                                  \
        }                                                                  \
    } while (0)
