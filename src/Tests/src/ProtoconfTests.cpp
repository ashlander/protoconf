#include <boost/filesystem.hpp>

#include "GTestGroups.h"
#include <Protoconf/module.h>
#include <Protoconf/protoconf.h>
#include <Schema/schema.pb.h>

    using namespace boost;
    using namespace bsw;
    //using namespace protoconf; // DO NOT USE using protoconf !!! Really don't

    class ProtoconfTests: public LogTest
    {
        public:
            ProtoconfTests()
            {
               testConfiguration.set_allocated_v1conf( new pconf::v1() );

               // Note: nested messages will not be displayed, unless explicitly added
               auto v2instance = new pconf::v2();
               testConfiguration.set_allocated_v2conf( v2instance );

               for (int32_t i = 0; i < 5; ++i)
               {
                  testConfiguration.add_statisticsint(i);
                  testConfiguration.add_statisticsfloat(float(i) - 0.5f);
               }//for

               v2instance->add_datas(); // add default data

               auto data = v2instance->add_datas(); // add custom data
               data->set_id(222222222);
               data->set_hash("AAAAAAA");
               data->set_value(111111);
            }

            virtual ~ProtoconfTests() {}

        public:
            virtual void SetUp() override
            {
            }

            pconf::configuration testConfiguration;
    };//class

    TEST_F(ProtoconfTests, DefaultConfigurationToJson)
    {
       pconf::configuration conf;
       std::string json;
       bool result = protoconf::toJson(conf, json);
       ASSERT_TRUE(result);
       ASSERT_TRUE(!json.empty());

       printMessage(std::string("JSON configuration: \n") + json);
    }

    TEST_F(ProtoconfTests, ConfigurationToJson)
    {
       std::string json;
       bool result = protoconf::toJson(testConfiguration, json);
       ASSERT_TRUE(result);
       ASSERT_TRUE(!json.empty());
       printMessage(std::string("JSON configuration: \n") + json);
    }

    TEST_F(ProtoconfTests, JsonToConfiguration)
    {
       pconf::configuration conf;

       std::string json;
       bool result = protoconf::toJson(conf, json);
       ASSERT_TRUE(result);
       ASSERT_TRUE(!json.empty());

       pconf::configuration confRecovered;
       result = protoconf::fromJson(json, confRecovered);
       ASSERT_TRUE(result);

       std::string json2;
       result = protoconf::toJson(conf, json2);
       ASSERT_TRUE(result);
       ASSERT_TRUE(!json2.empty());

       ASSERT_EQ(json, json2);
    }

    TEST_F(ProtoconfTests, LoadMissingConfiguration)
    {
       const std::string path("/tmp/something_not_exist.json");
       pconf::configuration conf;
       bool result = protoconf::load(path, conf, pconf::configuration(), true, false /*do not create missing configuration*/);
       ASSERT_FALSE(result);
    }

    TEST_F(ProtoconfTests, SaveTemplateConfiguration)
    {
       const std::string path("/tmp/something_not_exist.json");
       pconf::configuration conf;

       std::string json;
       bool result = protoconf::toJson(testConfiguration, json);
       ASSERT_TRUE(result);

       result = protoconf::load(path, conf, testConfiguration);
       std::remove(path.c_str());
       ASSERT_TRUE(result);

       std::string json2;
       result = protoconf::toJson(conf, json2);
       ASSERT_TRUE(result);

       ASSERT_EQ(json, json2);
    }

    TEST_F(ProtoconfTests, SaveLoadConfiguration)
    {
       const std::string path("/tmp/protoconfiguration.json");
       pconf::configuration conf;

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

    TEST_F(ProtoconfTests, BigStringSaveLoadConfiguration)
    {
       const std::string path("/tmp/protoconfiguration.json");
       pconf::configuration conf;

       const char* name = R"_(Some big and heavy string with
multiple new lines
line
line
and line
!@$@$%^$$%^&^&&*)((
)_";
       conf.set_descr_big(name);

       std::string json;
       bool result = protoconf::toJson(conf, json);
       ASSERT_TRUE(result);

       printMessage(std::string("DST JSON configuration: \n") + json);
       printMessage(conf.descr_big());

       result = protoconf::save(path, conf);
       ASSERT_TRUE(result);

       result = protoconf::load(path, conf);
       ASSERT_TRUE(result);

       std::string json2;
       result = protoconf::toJson(conf, json2);
       ASSERT_TRUE(result);

       ASSERT_EQ(json, json2);
    }

const char* partialConfig = R"_({
 "descrValName": "This is simple value description",
 "valName": "Name",
})_";

    TEST_F(ProtoconfTests, PartialJsonToConfiguration)
    {
       std::string json = partialConfig;
       printMessage(std::string("SRC JSON configuration: \n") + json);

       pconf::configuration conf;
       bool result = protoconf::fromJson(json, conf);
       ASSERT_TRUE(result);

       std::string json2;
       result = protoconf::toJson(conf, json2);
       ASSERT_TRUE(result);
       ASSERT_TRUE(!json2.empty());

       printMessage(std::string("DST JSON configuration: \n") + json2);
    }

const char* switchPosition = R"_({
 "valName": "Moved up",
 "descrValName": "This is simple value description",
})_";

    TEST_F(ProtoconfTests, SwitchPositionsJsonToConfiguration)
    {
       std::string json = switchPosition;
       printMessage(std::string("SRC JSON configuration: \n") + json);

       pconf::configuration conf;
       bool result = protoconf::fromJson(json, conf);
       ASSERT_TRUE(result);

       std::string json2;
       result = protoconf::toJson(conf, json2);
       ASSERT_TRUE(result);
       ASSERT_TRUE(!json2.empty());

       printMessage(std::string("DST JSON configuration: \n") + json2);
    }

const char* removeOptional = R"_({
 "descrValName": "This is simple value description",
 "valName": "",
 "descrV1conf": "This describes protocol version 1",
 "descrV2conf": "This describes protocol version 2"
})_";

    TEST_F(ProtoconfTests, RemoveLinesJsonToConfiguration)
    {
       std::string json = removeOptional;
       printMessage(std::string("SRC JSON configuration: \n") + json);

       pconf::configuration conf;
       bool result = protoconf::fromJson(json, conf);
       ASSERT_TRUE(result);

       std::string json2;
       result = protoconf::toJson(conf, json2);
       ASSERT_TRUE(result);
       ASSERT_TRUE(!json2.empty());

       printMessage(std::string("DST JSON configuration: \n") + json2);
    }

const char* commentOptional = R"_({
 "descrValName": "This is simple value description",
 //         "valName": "Something i do not want to remove, but temporary disabled",    <- line comment with some spaces
 //"descrValUint": "This is dedicated to test integer value",                          <- line comment
 "valUint": 50,                                                                     // <- just ignore this comment
 "descrStatisticsInt": "Statistics with integer values",
 "statisticsInt": [],
 "descrStatisticsFloat": "Statistics with float values",
 "statisticsFloat": [],
 "descrV1conf": "This describes protocol version 1",
 "descrV2conf": "This describes protocol version 2"
})_";

    TEST_F(ProtoconfTests, CommentLinesJsonToConfiguration)
    {
       std::string json = commentOptional;
       printMessage(std::string("SRC JSON configuration: \n") + json);

       pconf::configuration conf;
       bool result = protoconf::fromJson(json, conf);
       ASSERT_TRUE(result);

       std::string json2;
       result = protoconf::toJson(conf, json2);
       ASSERT_TRUE(result);
       ASSERT_TRUE(!json2.empty());

       printMessage(std::string("DST JSON configuration: \n") + json2);
    }

