#include "module.h"

#include "ProtoconfConfig.h"

#include "protoconf.h" // For code coverage

#include <Logger/log.h>
#include <Common/macro.h>

namespace bsw {
namespace protoconf {
namespace module {

std::string version()
{
   const std::string name( STRINGIFYVALUE(Protoconf_NAME) );
   std::string version( STRINGIFYVALUE(Protoconf_VERSION) );
   LOG(logDEBUG) << version;
   const std::string buildType( STRINGIFYVALUE(Protoconf_BUILD_TYPE) );
   const std::string buildOS( STRINGIFYVALUE(Protoconf_OS) );
   const std::string buildArch( STRINGIFYVALUE(Protoconf_ARCH) );
   const std::string buildNum( STRINGIFYVALUE(Protoconf_BUILD_NUM) );
   const std::string gitHash( STRINGIFYVALUE(Protoconf_GIT_HASH) );
   if (!buildNum.empty()) // update version with build num
   {
      size_t pos = version.find_last_of(".");
      if (pos != std::string::npos)
      {
         version = version.substr(0, pos) + "." + buildNum;
      }//if
   }//if
   return name +
      " " + version +
      " " + buildType +
      " " + buildOS +
      " " + buildArch +
      " / build: [" + (!buildNum.empty() ? buildNum : "unknown") +
      "] hash: [" + (!gitHash.empty() ? gitHash : "unknown") + "] /";
}

} /* namespace module */
} /* namespace protoconf */
} /* namespace bsw */

