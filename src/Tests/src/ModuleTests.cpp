#include <boost/filesystem.hpp>

#include "GTestGroups.h"
#include <Protoconf/module.h>

    using namespace boost;
    using namespace bsw;
    using namespace protoconf;

    class ModuleTests: public LogTest
    {
        public:
            ModuleTests() {}
            virtual ~ModuleTests() {}

        public:
            virtual void SetUp() override
            {
            }
    };


    TEST_F(ModuleTests, CheckVersion)
    {
        std::string version = module::version();
        printMessage(version);
        ASSERT_TRUE(!version.empty());
    }
