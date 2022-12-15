#include "run_test.hpp"
#include <gtest/gtest.h>
#include <hlp/hlp.hpp>
#include <json/json.hpp>
#include <string>
#include <vector>

TEST(IPParser, parser)
{
    auto fn = [](std::string in) -> json::Json
    {
        json::Json doc {in.c_str()};
        return doc;
    };

    std::vector<TestCase> testCases {
        TestCase {"0.0.0.0", true, {""}, Options {}, fn(R"("0.0.0.0")"), strlen("0.0.0.0")},
        TestCase {"0.0.0.1", true, {""}, Options {}, fn(R"("0.0.0.1")"), strlen("0.0.0.1")},
        TestCase {
            "0.0.0.10", true, {""}, Options {}, fn(R"("0.0.0.10")"), strlen("0.0.0.10")},
        TestCase {
            "0.0.0.100", true, {""}, Options {}, fn(R"("0.0.0.100")"), strlen("0.0.0.100")},
        TestCase {"0.0.1.0", true, {""}, Options {}, fn(R"("0.0.1.0")"), strlen("0.0.1.0")},
        TestCase {
            "0.0.10.0", true, {""}, Options {}, fn(R"("0.0.10.0")"), strlen("0.0.10.0")},
        TestCase {
            "0.0.100.0", true, {""}, Options {}, fn(R"("0.0.100.0")"), strlen("0.0.100.0")},
        TestCase {"0.1.0.0", true, {""}, Options {}, fn(R"("0.1.0.0")"), strlen("0.1.0.0")},
        TestCase {
            "0.10.0.0", true, {""}, Options {}, fn(R"("0.10.0.0")"), strlen("0.10.0.0")},
        TestCase {
            "0.100.0.0", true, {""}, Options {}, fn(R"("0.100.0.0")"), strlen("0.100.0.0")},
        TestCase {"1.0.0.0", true, {""}, Options {}, fn(R"("1.0.0.0")"), strlen("1.0.0.0")},
        TestCase {
            "10.0.0.0", true, {""}, Options {}, fn(R"("10.0.0.0")"), strlen("10.0.0.0")},
        TestCase {
            "100.0.0.0", true, {""}, Options {}, fn(R"("100.0.0.0")"), strlen("100.0.0.0")},
        TestCase {"100.20.0.55",
                  true,
                  {""},
                  Options {},
                  fn(R"("100.20.0.55")"),
                  strlen("100.20.0.55")},
        TestCase {"192.168.0.1",
                  true,
                  {""},
                  Options {},
                  fn(R"("192.168.0.1")"),
                  strlen("192.168.0.1")},
        TestCase {"255.255.255.255",
                  true,
                  {""},
                  Options {},
                  fn(R"("255.255.255.255")"),
                  strlen("255.255.255.255")},
        TestCase {"256.168.0.1", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"100.500.0.1", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"20.200.1000.1", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"20.200.0.950", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"20.200.0.a", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.20.30.40.50", false, {""}, Options {}, fn("{}"), 0},
        TestCase {".20.30.40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10..30.40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.20..40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.20.30.", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10 .20.30.40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.20.30. 40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.-20.30.40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.20.-30.40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.20.30.-40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10x.20.30.40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.20x.30.40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.20.30x.40", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"10.20.30.40x", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"2345:425:2CA1:0000:0000:567:5673:23b5",
                  true,
                  {""},
                  Options {},
                  fn(R"("2345:425:2CA1:0000:0000:567:5673:23b5")"),
                  strlen("2345:425:2CA1:0000:0000:567:5673:23b5")},
        TestCase {"2345:0425:2CA1:0:0:0567:5673:23b5",
                  true,
                  {""},
                  Options {},
                  fn(R"("2345:0425:2CA1:0:0:0567:5673:23b5")"),
                  strlen("2345:0425:2CA1:0:0:0567:5673:23b5")},
        TestCase {"2345:0425:2CA1::0567:5673:23b5",
                  true,
                  {""},
                  Options {},
                  fn(R"("2345:0425:2CA1::0567:5673:23b5")"),
                  strlen("2345:0425:2CA1::0567:5673:23b5")},
        TestCase {"::1", true, {""}, Options {}, fn(R"("::1")"), strlen("::1")},
        TestCase {"0:0:0:0:0:0:0:1",
                  true,
                  {""},
                  Options {},
                  fn(R"("0:0:0:0:0:0:0:1")"),
                  strlen("0:0:0:0:0:0:0:1")},
        TestCase {"::", true, {""}, Options {}, fn(R"("::")"), strlen("::")},
        TestCase {"0:0:0:0:0:0:0:0",
                  true,
                  {""},
                  Options {},
                  fn(R"("0:0:0:0:0:0:0:0")"),
                  strlen("0:0:0:0:0:0:0:0")},
        TestCase {"2001:db8::1",
                  true,
                  {""},
                  Options {},
                  fn(R"("2001:db8::1")"),
                  strlen("2001:db8::1")},
        TestCase {"2001:DB8::1",
                  true,
                  {""},
                  Options {},
                  fn(R"("2001:DB8::1")"),
                  strlen("2001:DB8::1")},
        TestCase {"2001:db8:0:0:0:0:2:1",
                  true,
                  {""},
                  Options {},
                  fn(R"("2001:db8:0:0:0:0:2:1")"),
                  strlen("2001:db8:0:0:0:0:2:1")},
        TestCase {"2001:db8::2:1",
                  true,
                  {""},
                  Options {},
                  fn(R"("2001:db8::2:1")"),
                  strlen("2001:db8::2:1")},
        TestCase {"ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff",
                  true,
                  {""},
                  Options {},
                  fn(R"("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff")"),
                  strlen("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff")},
        TestCase {"ff00::", true, {""}, Options {}, fn(R"("ff00::")"), strlen("ff00::")},
        TestCase {"::ffff:0:0.0.0.0",
                  true,
                  {""},
                  Options {},
                  fn(R"("::ffff:0:0.0.0.0")"),
                  strlen("::ffff:0:0.0.0.0")},
        TestCase {"2345:425:2CA1:0000:0000:567:5673:23b5###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("2345:425:2CA1:0000:0000:567:5673:23b5")"),
                  strlen("2345:425:2CA1:0000:0000:567:5673:23b5")},
        TestCase {"2345:0425:2CA1:0:0:0567:5673:23b5###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("2345:0425:2CA1:0:0:0567:5673:23b5")"),
                  strlen("2345:0425:2CA1:0:0:0567:5673:23b5")},
        TestCase {"2345:0425:2CA1::0567:5673:23b5###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("2345:0425:2CA1::0567:5673:23b5")"),
                  strlen("2345:0425:2CA1::0567:5673:23b5")},
        TestCase {"::1###", true, {"###"}, Options {}, fn(R"("::1")"), strlen("::1")},
        TestCase {"0:0:0:0:0:0:0:1###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("0:0:0:0:0:0:0:1")"),
                  strlen("0:0:0:0:0:0:0:1")},
        TestCase {"::###", true, {"###"}, Options {}, fn(R"("::")"), strlen("::")},
        TestCase {"0:0:0:0:0:0:0:0###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("0:0:0:0:0:0:0:0")"),
                  strlen("0:0:0:0:0:0:0:0")},
        TestCase {"2001:db8::1###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("2001:db8::1")"),
                  strlen("2001:db8::1")},
        TestCase {"2001:DB8::1###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("2001:DB8::1")"),
                  strlen("2001:DB8::1")},
        TestCase {"2001:db8:0:0:0:0:2:1###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("2001:db8:0:0:0:0:2:1")"),
                  strlen("2001:db8:0:0:0:0:2:1")},
        TestCase {"2001:db8::2:1###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("2001:db8::2:1")"),
                  strlen("2001:db8::2:1")},
        TestCase {"ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff")"),
                  strlen("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff")},
        TestCase {
            "ff00::###", true, {"###"}, Options {}, fn(R"("ff00::")"), strlen("ff00::")},
        TestCase {"::ffff:0:0.0.0.0###",
                  true,
                  {"###"},
                  Options {},
                  fn(R"("::ffff:0:0.0.0.0")"),
                  strlen("::ffff:0:0.0.0.0")},
        TestCase {"0:0:0:0:0:0:0:0:0", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"2001:db8:x:0:0:0:2:1", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"2001:db8:0.0:0:0:2:1", false, {""}, Options {}, fn("{}"), 0},
        TestCase {"fffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff",
                  false,
                  {""},
                  Options {},
                  fn("{}"),
                  0},
        TestCase {
            "192.168.0.1:8080", true, {":"}, Options {}, fn(R"("192.168.0.1")"), 11},
        TestCase {"192.168.1.1", true, {""}, Options {}, fn(R"("192.168.1.1")"), 11},
        TestCase {"::", true, {""}, Options {}, fn(R"("::")"), 2},
        TestCase {":: ", true, {" "}, Options {}, fn(R"("::")"), 2},
        TestCase {"192.168.1.1 ", true, {" "}, Options {}, fn(R"("192.168.1.1")"), 11},
        TestCase {"555.168.1.1 ", false, {" "}, Options {}, fn(R"("192.168.1.1")"), 0},
        TestCase {
            "192.168.1.1192", false, {"192"}, Options {}, fn(R"("192.168.1.1")"), 0},
        TestCase {"127.1", false, {""}, Options {}, fn(R"("127.1")"), 0},
        TestCase {"1:2:3:4:5:6:77.77.88.88",
                  true,
                  {""},
                  Options {},
                  fn(R"("1:2:3:4:5:6:77.77.88.88")"),
                  23},
        TestCase {"0xc0.0xa8.0x8c.0xff",
                  false,
                  {""},
                  Options {},
                  fn(R"("0xc0.0xa8.0x8c.0xff")"),
                  0},
        TestCase {
            "001.002.003.004", false, {""}, Options {}, fn(R"("001.002.003.004")"), 0},
        TestCase {"1::1.2.3.4", true, {""}, Options {}, fn(R"("1::1.2.3.4")"), 10},
    };

    for (auto t : testCases)
    {
        runTest(t, hlp::getIPParser);
    }
}

TEST(IPParser, build)
{
    // ok
    ASSERT_NO_THROW(hlp::getIPParser({}, {""}, {}));
    ASSERT_NO_THROW(hlp::getIPParser({}, {" "}, {}));
    // Stop token is necessary
    ASSERT_THROW(hlp::getIPParser({}, {}, {}), std::runtime_error);
    // No allowed options
    ASSERT_THROW(hlp::getIPParser({}, {}, {""}), std::runtime_error);
    ASSERT_THROW(hlp::getIPParser({}, {}, {" "}), std::runtime_error);
}
