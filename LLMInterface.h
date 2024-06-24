#ifndef LLMINTERFACE_H
#define LLMINTERFACE_H

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/bedrock-runtime/BedrockRuntimeClient.h>
#include <aws/bedrock-runtime/model/InvokeModelRequest.h>
#include <aws/bedrock-runtime/model/InvokeModelResult.h>
#include <aws/bedrock-runtime/model/ConverseRequest.h>
#include <aws/bedrock-runtime/model/ConverseResult.h>
#include <aws/bedrock-runtime/model/Message.h>
#include <aws/bedrock-runtime/model/ConversationRole.h>
#include <iostream>
#include <string>

class LLMInterface {
public:
    static std::string invoke(const std::string& input_message);
    static std::string player_say(const std::string& player_message);
};

#endif // LLMINTERFACE_H
