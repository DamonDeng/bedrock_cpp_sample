#include "LLMInterface.h"

using namespace Aws;
using namespace Aws::Client;
using namespace Aws::Auth;
using namespace Aws::Utils::Json;
using namespace Aws::BedrockRuntime;
using namespace Aws::BedrockRuntime::Model;

std::string LLMInterface::invoke(const std::string& input_message) {
    // Initialize the SDK
    SDKOptions options;
    InitAPI(options);
    std::string result_text = "Error";
    {
        // Set up client configuration
        ClientConfiguration clientConfig;
        clientConfig.region = "us-west-2"; // Replace with your region

        // Create a Bedrock client
        BedrockRuntimeClient bedrockRuntimeClient(clientConfig);

        ConverseRequest converseRequest;

        converseRequest.SetModelId("anthropic.claude-3-sonnet-20240229-v1:0");

        Aws::Vector<Message> input_messages;

        Message message;

        message.SetRole(ConversationRole::user);

        Aws::Vector<ContentBlock> input_contentBlocks;

        ContentBlock input_contentBlock;

        input_contentBlock.WithText(input_message);

        input_contentBlocks.push_back(input_contentBlock);

        message.SetContent(input_contentBlocks);

        input_messages.push_back(message);

        converseRequest.SetMessages(input_messages);

        auto outcome = bedrockRuntimeClient.Converse(converseRequest);

        if (outcome.IsSuccess()) {
            // Process the result
            ConverseResult result = outcome.GetResultWithOwnership();

            auto output = result.GetOutput();

            Message resultMessage = output.GetMessage();

            Aws::Vector<ContentBlock> contentBlocks = resultMessage.GetContent();

            for (auto contentBlock : contentBlocks) {
                result_text = contentBlock.GetText();
            }

        } else {
            // Handle error
            std::cerr << "Failed to converse: " << outcome.GetError().GetMessage() << std::endl;
        }
    }
    // Shutdown the SDK
    ShutdownAPI(options);
    return result_text;
}

std::string LLMInterface::player_say(const std::string& player_message) {
    std::string formatted_message = "我是魔兽世界里的NPC 维里副队长，在新手村 北郡山谷里，游戏玩家给我发了下面这个消息：" + player_message + " 请帮我生成一个合适的符合魔兽世界设定的回复, 魔兽世界的NPC说话都比较简短，所以回复不要太长。";
    return invoke(formatted_message);
}
