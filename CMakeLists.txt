cmake_minimum_required(VERSION 3.1)
project(AWSBedrockExample)

set(CMAKE_CXX_STANDARD 11)

# Find the AWS SDK for C++ package
find_package(AWSSDK REQUIRED COMPONENTS core bedrock bedrock-runtime s3)

# Add LLMInterface files
add_executable(aws_bedrock_example bedrock_sample.cpp LLMInterface.cpp)
add_executable(aws_bedrock_async_example bedrock_async_sample.cpp LLMInterface.cpp)

add_executable(aws_s3_example s3_sample.cpp)

target_link_libraries(aws_bedrock_example ${AWSSDK_LINK_LIBRARIES})
target_link_libraries(aws_bedrock_async_example ${AWSSDK_LINK_LIBRARIES})

target_link_libraries(aws_s3_example ${AWSSDK_LINK_LIBRARIES})

# Include the AWS SDK headers
target_include_directories(aws_bedrock_example PRIVATE ${AWSSDK_INCLUDE_DIRS})
target_include_directories(aws_bedrock_async_example PRIVATE ${AWSSDK_INCLUDE_DIRS})

target_include_directories(aws_s3_example PRIVATE ${AWSSDK_INCLUDE_DIRS})

# Include current directory for LLMInterface.h
target_include_directories(aws_bedrock_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(aws_bedrock_async_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
