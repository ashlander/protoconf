#include <gtest/gtest.h>

#include <Protoconf/module.h>
#include <Protoconf/protoconf.h>
#include <Schema/schema.pb.h>

    using namespace bsw;
    //using namespace protoconf; // DO NOT USE using protoconf !!! Really don't

    class ProtoconfTests: public ::testing::Test
    {
        public:
            ProtoconfTests() {}
            virtual ~ProtoconfTests() {}

        public:
            virtual void SetUp() override
            {
            }
    };//class

    TEST_F(ProtoconfTests, CheckVersion)
    {
        std::string version = protoconf::module::version();
        printf("%s\n", version.c_str());
        ASSERT_TRUE(!version.empty());
    }

    TEST_F(ProtoconfTests, SaveLoadConfiguration)
    {
       const std::string path("/tmp/protoconfiguration.json");
       protoconf::configuration conf;

       std::string json;
       bool result = protoconf::toJson(conf, json);
       ASSERT_TRUE(result);

       result = protoconf::save(path, conf);
       ASSERT_TRUE(result);

       result = protoconf::load(path, conf);
       ASSERT_TRUE(result);

       std::string json2;
       result = protoconf::toJson(conf, json2);
       ASSERT_TRUE(result);

       ASSERT_EQ(json, json2);
    }

