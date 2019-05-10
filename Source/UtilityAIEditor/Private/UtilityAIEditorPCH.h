#pragma once

#include "UtilityAIGraph.h"
#include "UtilityAINode.h"
#include "UtilityAIEdge.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "IUtilityAIEditor.h"

#define LOG_INFO(FMT, ...) UE_LOG(UtilityAIEditor, Display, (FMT), ##__VA_ARGS__)
#define LOG_WARNING(FMT, ...) UE_LOG(UtilityAIEditor, Warning, (FMT), ##__VA_ARGS__)
#define LOG_ERROR(FMT, ...) UE_LOG(UtilityAIEditor, Error, (FMT), ##__VA_ARGS__)
