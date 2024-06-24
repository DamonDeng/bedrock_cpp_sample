#include <iostream>
#include <string>

#include "LLMInterface.h"


using namespace Aws;
using namespace Aws::Client;
using namespace Aws::Auth;
using namespace Aws::Utils::Json;
using namespace Aws::BedrockRuntime;
using namespace Aws::BedrockRuntime::Model;

// The following is the sample implementation of Converse, to call the LLM synchronously.
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

// The following is the sample implementation of ConverseAsync, to call the LLM asynchronously.

// at first , define a custom async caller context
// the sender and receiver are only used for demonstration purpose
// assuming that we need to handle sender and receiver in the async callback



class Sender {
public:
    void Say(const std::string& msg) {
        std::cout << "Sender: " << msg << std::endl;
    }
};

class Receiver {
public:
    void Say(const std::string& msg) {
        std::cout << "Receiver: " << msg << std::endl;
    }
};


class CustomAsyncCallerContext : public Aws::Client::AsyncCallerContext
{
public:
    CustomAsyncCallerContext(void* sender, void* receiver)
    {
        m_sender = sender;
        m_receiver = receiver;
    }

    void* GetSender() const { return m_sender; }
    void* GetReceiver() const { return m_receiver; }

private:
    void* m_sender;
    void* m_receiver;
};


void HandlerFunction(const BedrockRuntimeClient* client,
                     const Model::ConverseRequest& request,
                     const Model::ConverseOutcome& outcome,
                     const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context)
{

    // now, outcome is handled here in the callback function

    if (outcome.IsSuccess()) {
            // retrieve the custom async caller context

            const auto& customContext = static_cast<const CustomAsyncCallerContext&>(*context);
            Sender* sender = static_cast<Sender*>(customContext.GetSender());
            Receiver* receiver = static_cast<Receiver*>(customContext.GetReceiver());
            
            // Process the result
            // ConverseResult result = outcome.GetResultWithOwnership();

            const auto& result = outcome.GetResult();

            auto output = result.GetOutput();

            Message resultMessage = output.GetMessage();

            Aws::Vector<ContentBlock> contentBlocks = resultMessage.GetContent();

            for (auto contentBlock : contentBlocks) {
                auto result_text = contentBlock.GetText();

                // send the result to the sender
                const std::string& result_text_to_sender = "For Sender: the LLM response is: " + result_text;

                // start a new line to separate the result from the previous messages
                std::cout << std::endl;

                sender->Say(result_text_to_sender);

                receiver->Say(result_text);
            }

    } else {
        // Handle error
        std::cerr << "Failed to converse: " << outcome.GetError().GetMessage() << std::endl;
    }


    
}




void LLMInterface::invokeAsync(const std::string& input_message) {
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


        // Create a custom async caller context
        Sender mySender;
        Receiver myReceiver;

        std::shared_ptr<CustomAsyncCallerContext> context =
            std::make_shared<CustomAsyncCallerContext>(&mySender, &myReceiver);


        // auto outcome = bedrockRuntimeClient.Converse(converseRequest);
        // instead of calling the Converse method directly, call the ConverseAsync method

        bedrockRuntimeClient.ConverseAsync(converseRequest, HandlerFunction, context);

        // now we need a way to wait for the async call to complete, or else the program will exit before the async call completes

        // for demonstration purpose, we setup a loop to wait for any key press to exit the program
        // at the same time print a "waiting" message, following "." to indicate the program is still running

        std::cout << "Waiting for the async call to complete";

        
        while (true) {
            std::cout << ".";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (std::cin.get() == '\n') {
                break;
            }
        }

        
    }
    // Shutdown the SDK
    ShutdownAPI(options);
    
}


std::string LLMInterface::player_say(const std::string& player_message) {
    std::string formatted_message = "我是魔兽世界里的NPC 维里副队长，在新手村 北郡山谷里，游戏玩家给我发了下面这个消息：" + player_message + " 请帮我生成一个合适的符合魔兽世界设定的回复, 魔兽世界的NPC说话都比较简短，所以回复不要太长。";
    return invoke(formatted_message);
}
