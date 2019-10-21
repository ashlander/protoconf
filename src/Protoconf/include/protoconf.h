#pragma once

#include <regex>
#include <string>
#include <fstream>
#include <google/protobuf/util/json_util.h>
#include <Logger/log.h>

namespace bsw {
namespace protoconf {

   /**
    * @brief Read entire file, and put content in output arg
    * @param path Path to file
    * @param[out] output Content of the file if success
    * @return Success
    */
   bool readEntireFile(const std::string& path, std::string& output)
   {
      LOG(logINFO) << "Open file to read \"" << path << "\"";
      bool result = true;

      std::ifstream infile(path);
      if (!infile.fail())
      {
         infile.seekg(0, std::ios::end);
         output.reserve(infile.tellg());
         infile.seekg(0, std::ios::beg);

         output.assign((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

         infile.close();
      }
      else
      {
         LOG(logERROR) << "Failed to open file to read \"" << path << "\"";
         result = false;
      }//if
      return result;
   }

   /**
    * @brief Write file with content
    * @param path Path to file
    * @param output File content to write
    * @return Success
    */
   bool writeEntireFile(const std::string& path, const std::string& output)
   {
      LOG(logINFO) << "Open file to write \"" << path << "\"";
      bool result = true;

      std::ofstream outfile(path, std::ofstream::out);
      if (!outfile.fail())
      {
         outfile << output;
         outfile.close();
      }
      else
      {
         LOG(logERROR) << "Failed to open file to write \"" << path << "\"";
         result = false;
      }//f
      return result;
   }

   /**
    * @brief Remove c style // comments
    * protobuf can't work with comments in JSON
    * Jet we need comments support for configuration files
    * @param input Raw content
    * @return Updated content
    */
   std::string removeComments(std::string input)
   {
      std::regex rule("//.*");
      return std::regex_replace (input, rule, "");
   }

   /**
    * @brief Load configuration file and fill TProtoConf configuration object
    * @param path Path to JSON configuration file
    * @param conf Configuration object to fill
    * @param templateConfiguration If file missing and createTemplateIfMissing is set, file will be created with this content
    * @param ignoreMissing Specifies to ignore missing fields in configuration, e.g. old client reads newer configuration
    * @param createTemplateIfMissing Create file with template JSON configuration
    * @return Success
    */
   template<class TProtoConf>
   bool load(const std::string& path, TProtoConf& conf, const TProtoConf& templateConfiguration = TProtoConf(), bool ignoreMissing = true, bool createTemplateIfMissing = true)
   {
      bool result = true;

      std::string json;
      if (result)
         result = readEntireFile(path, json);

      if (result)
      {
         result = fromJson(json, conf);
      }
      else if (createTemplateIfMissing) // create conf file if missing
      {
         conf = templateConfiguration;
         result = save(path, conf);
      }//if

      return result;
   }

   /**
    * @brief Save configuration object as JSON file configuration
    * @param path Path to configuration file
    * NOTE: file will be completely overwritten
    * @param conf Configuration object
    * @return Success
    */
   template<class TProtoConf>
   bool save(const std::string& path, const TProtoConf& conf)
   {
      bool result = true;

      std::string json;
      if (result)
         result = toJson(conf, json);

      if (result)
         result = writeEntireFile(path, json);

      return result;
   }

   /**
    * @brief Serialize configuration to JSON
    * @param conf Configuration object
    * @param[out] output Result JSON string
    * @return Success
    */
   template<class TProtoConf>
   bool toJson(const TProtoConf& conf, std::string& output)
   {
      using namespace google::protobuf;
      util::JsonOptions options;
      options.add_whitespace = true;
      options.always_print_primitive_fields = true;
      bool result = util::MessageToJsonString(conf, &output, options).ok();
      if (!result)
      {
         LOG(logERROR) << "Failed to convert protobuf configuration to JSON";
      }//if
      return result;
   }

   /**
    * @brief Construct configuration object from JSON string
    * @param json String in JSON format as input
    * @param[out] conf Configuration object
    * @param ignoreMissing Ignore unknown fields
    * @return Success
    */
   template<class TProtoConf>
   bool fromJson(const std::string& json, TProtoConf& conf, bool ignoreMissing = true)
   {
      using namespace google::protobuf;
      util::JsonParseOptions options;
      if (ignoreMissing)
         options.ignore_unknown_fields = true;
      bool result = util::JsonStringToMessage(removeComments(json), &conf, options).ok();
      if (!result)
      {
         LOG(logERROR) << "Failed to convert JSON to protobuf configuration";
      }//if
      return result;
   }

} /* namespace protoconf */
} /* namespace bsw */

