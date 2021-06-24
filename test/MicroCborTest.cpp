// SPDX-License-Identifier: MIT
#include <microcbor/MicroCbor.hpp>

#include "gtest/gtest.h"
#ifdef CONFIG_MICROCBOR_STD_VECTOR
#include <vector>
#endif
using namespace entazza;

TEST(microcbor, empty) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf));
  // Should return default value (-1) if empty buffer
  ASSERT_EQ(-1, cbor.get<int32_t>("i32", -1));
  cbor.startMap();
  cbor.endMap();

  ASSERT_EQ(-1, cbor.get<int32_t>("i32", -1));

  MicroCbor cbor2;
  // Should return default value (-1) if no buffer
  ASSERT_EQ(-1, cbor2.get<int32_t>("i32", -1));
}

TEST(microcbor, maps) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf));
  cbor.startMap();
  cbor.add("i32", int32_t(1));

  // Add nested map
  cbor.startMap("map1");
  cbor.add("f32", 3.14f);
  cbor.endMap();

  // Add an item after the map
  cbor.add("i16", int16_t(2));
  cbor.endMap();

  cbor.restart();
  ASSERT_EQ(1, cbor.get("i32", -1));
  ASSERT_EQ(2, cbor.get<int16_t>("i16", -1));

  auto map = cbor.getMap("map1");
  ASSERT_EQ(3.14f, map.get("f32", -1.0f));
  ASSERT_EQ(2, cbor.get<int16_t>("i16", -1));
}
TEST(microcbor, str) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf), false);
  cbor.startMap();
  cbor.add("s", "Hello World");
  cbor.add("null", "");
  cbor.endMap();
  cbor.restart();

  // Test const char * defaults
  const char *s = cbor.get("s", "Error");
  ASSERT_EQ(11, cbor.getLength("s"));
  ASSERT_EQ(0, strncmp("Hello World", s, 11));
  ASSERT_EQ(0, cbor.getLength("null"));

  // Test not found case
  s = cbor.get("xyz", "Not Found");
  ASSERT_EQ(0, strcmp("Not Found", s));

  // Test char * default
  char temp[5] = {'h', 'i', 0};
  s = cbor.get("s", temp);
  ASSERT_EQ(0, strncmp("Hello World", s, 11));

  cbor.restart();
  cbor.startMap();
  cbor.add("s", temp);
  cbor.endMap();

  cbor.restart();
  ASSERT_EQ(0, strncmp("hi", cbor.get("s", "Error"), 2));
}

TEST(microcbor, str_null) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf), true);
  cbor.startMap();
  cbor.add("s", "Hello World");
  cbor.add("null", "");
  cbor.endMap();
  cbor.restart();

  // Test const char * defaults
  const char *s = cbor.get("s", "Error");
  ASSERT_EQ(11, cbor.getLength("s"));
  ASSERT_EQ(0, strcmp("Hello World", s));
  ASSERT_EQ(0, cbor.getLength("null"));
  ASSERT_EQ(0, strcmp("", cbor.get("null", "Error")));

  // Test not found case
  s = cbor.get("xyz", "Not Found");
  ASSERT_EQ(0, strcmp("Not Found", s));

  // Test char * default
  char temp[5] = {'h', 'i', 0};
  s = cbor.get("s", temp);
  ASSERT_EQ(0, strcmp("Hello World", s));

  cbor.restart();
  cbor.startMap();
  cbor.add("s", temp);
  cbor.endMap();

  cbor.restart();
  ASSERT_EQ(0, strcmp("hi", cbor.get("s", "Error")));
}

TEST(microcbor, floats) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf));
  cbor.startMap();
  cbor.add("f32", 3.14159f);
  cbor.endMap();

  cbor.restart();
  ASSERT_EQ(3.14159f, cbor.get("f32", 0.0f));
}
TEST(microcbor, boolean) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf));
  cbor.startMap();
  cbor.add("true", true);
  cbor.endMap();

  cbor.restart();

  ASSERT_EQ(true, cbor.get("true", false));
}
TEST(microcbor, uint8) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf));
  cbor.startMap();
  cbor.add<uint8_t>("ui8", 8);
  cbor.endMap();

  cbor.restart();

  ASSERT_EQ(8, cbor.get<uint8_t>("ui8", 0));
  ASSERT_EQ(8, cbor.get("ui8", uint8_t(0)));
}

TEST(microcbor, basic) {
  MicroCbor cbor((void *)nullptr, 0);
  cbor.startMap();
  cbor.add("i", 12345);
  cbor.endMap();
  ASSERT_EQ(-1, cbor.getResult());
  ASSERT_EQ(8, cbor.bytesNeeded());

  uint8_t buf[100];
  cbor.initBuffer(buf, sizeof(buf));
  cbor.startMap();
  cbor.add("i", 12345);
  cbor.endMap();

  ASSERT_EQ(0, cbor.getResult());
  ASSERT_EQ(8, cbor.bytesSerialized());
  ASSERT_EQ(8, cbor.bytesNeeded());
  cbor.restart();
  ASSERT_EQ(12345, cbor.get("i", 12345));
}

TEST(microcbor, ints) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf));
  cbor.startMap();
  cbor.add("true", true);
  cbor.add("false", false);
  cbor.add<int8_t>("i8", -80);
  cbor.add<int16_t>("i16", -16000);
  cbor.add<int32_t>("i32", -32000000);
  cbor.add<int64_t>("i64", -30000000000L);

  cbor.add<uint8_t>("ui8", 80);
  cbor.add<uint16_t>("ui16", 16000);
  cbor.add<uint32_t>("ui32", 32000000);
  cbor.add<uint64_t>("ui64", 30000000000);
  cbor.endMap();

  cbor.restart();
  ASSERT_EQ(true, cbor.get<bool>("true", false));
  ASSERT_EQ(false, cbor.get<bool>("false", true));

  ASSERT_EQ(-80, cbor.get<int8_t>("i8", 0));
  ASSERT_EQ(-16000, cbor.get<int16_t>("i16", 0));
  ASSERT_EQ(-32000000, cbor.get<int32_t>("i32", 0));
  ASSERT_EQ(-30000000000L, cbor.get<int64_t>("i64", 0));

  ASSERT_EQ(-80, cbor.get("i8", int8_t(0)));
  ASSERT_EQ(-16000, cbor.get("i16", int16_t(0)));
  ASSERT_EQ(-32000000, cbor.get("i32", int32_t(0)));

  ASSERT_EQ(80, cbor.get<uint8_t>("ui8", 0));
  ASSERT_EQ(16000, cbor.get<uint16_t>("ui16", 0));
  ASSERT_EQ(32000000, cbor.get<uint32_t>("ui32", 0));
  ASSERT_EQ(30000000000, cbor.get<uint64_t>("ui64", 0));

  ASSERT_EQ(80, cbor.get("ui8", uint8_t(0)));
  ASSERT_EQ(16000, cbor.get("ui16", uint16_t(0)));
  ASSERT_EQ(32000000, cbor.get("ui32", uint32_t(0)));
}

TEST(microcbor, constbuf) {
  const uint8_t buf[200] = {0};
  MicroCbor cbor(buf, sizeof(buf));
  ASSERT_EQ(0, cbor.getResult());

  // startMap should have an error
  ASSERT_NE(0, cbor.startMap());
  ASSERT_NE(0, cbor.getResult());
}

#ifdef CONFIG_MICROCBOR_STD_VECTOR
TEST(microcbor, array) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf));

  int32_t pts[] = {1, 2, 3, 4};
  cbor.startMap();
  cbor.add("pts", pts, 4, true);
  cbor.endMap();
  cbor.restart();

  auto array = cbor.getPointer<int32_t>("pts", nullptr);
  auto p = array.p;
  ASSERT_NE(nullptr, p);
  ASSERT_EQ(4, array.length);

  ASSERT_EQ(pts[0], p[0]);
  ASSERT_EQ(pts[1], p[1]);
  ASSERT_EQ(pts[2], p[2]);
  ASSERT_EQ(pts[3], p[3]);
  ASSERT_EQ(sizeof(pts), cbor.getLength("pts"));
}

TEST(microcbor, vector) {
  uint8_t buf[200];
  MicroCbor cbor(buf, sizeof(buf));

  std::vector<int32_t> pts = {1, 2, 3, 4};
  cbor.startMap();
  cbor.add("pts", pts);
  cbor.endMap();
  cbor.restart();

  auto array = cbor.getPointer<int32_t>("pts", nullptr);
  auto p = array.p;
  ASSERT_NE(nullptr, p);
  ASSERT_EQ(4, array.length);

  ASSERT_EQ(pts[0], p[0]);
  ASSERT_EQ(pts[1], p[1]);
  ASSERT_EQ(pts[2], p[2]);
  ASSERT_EQ(pts[3], p[3]);
}
#endif

int main(int argc, char **argv) {
  // debug arguments such as pause --gtest_filter=microcbor.basic*
  bool pauseOnExit = argc > 1 && std::string(argv[1]) == "pause";
  ::testing::InitGoogleTest(&argc, argv);

  auto result = RUN_ALL_TESTS();
  if (pauseOnExit) getchar();
  return result;
}
