#include "EmitterPoint3CS_vulkan.hlsl.h"
#include "EmitterPoint3CS_d3d12.hlsl.h"

struct EmitterPoint3CS_Pipeline
{
	NvFlowContextInterface* contextInterface;
	NvFlowComputePipeline* pipeline;
};

inline void EmitterPoint3CS_init(NvFlowContextInterface* contextInterface, NvFlowContext* context, EmitterPoint3CS_Pipeline* ptr)
{
	ptr->contextInterface = contextInterface;
	NvFlowContextConfig config = {};
	ptr->contextInterface->getContextConfig(context, &config);
	NvFlowBindingDesc bindingDescs[14] = {};
	NvFlowComputePipelineDesc desc = {};
	if (config.api == eNvFlowContextApi_vulkan)
	{
		bindingDescs[0].type = eNvFlowDescriptorType_constantBuffer;
		bindingDescs[0].bindingDesc.vulkan.binding = 0;
		bindingDescs[0].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[0].bindingDesc.vulkan.set = 0;

		bindingDescs[1].type = eNvFlowDescriptorType_constantBuffer;
		bindingDescs[1].bindingDesc.vulkan.binding = 1;
		bindingDescs[1].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[1].bindingDesc.vulkan.set = 0;

		bindingDescs[2].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[2].bindingDesc.vulkan.binding = 2;
		bindingDescs[2].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[2].bindingDesc.vulkan.set = 0;

		bindingDescs[3].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[3].bindingDesc.vulkan.binding = 3;
		bindingDescs[3].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[3].bindingDesc.vulkan.set = 0;

		bindingDescs[4].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[4].bindingDesc.vulkan.binding = 4;
		bindingDescs[4].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[4].bindingDesc.vulkan.set = 0;

		bindingDescs[5].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[5].bindingDesc.vulkan.binding = 5;
		bindingDescs[5].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[5].bindingDesc.vulkan.set = 0;

		bindingDescs[6].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[6].bindingDesc.vulkan.binding = 6;
		bindingDescs[6].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[6].bindingDesc.vulkan.set = 0;

		bindingDescs[7].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[7].bindingDesc.vulkan.binding = 7;
		bindingDescs[7].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[7].bindingDesc.vulkan.set = 0;

		bindingDescs[8].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[8].bindingDesc.vulkan.binding = 8;
		bindingDescs[8].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[8].bindingDesc.vulkan.set = 0;

		bindingDescs[9].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[9].bindingDesc.vulkan.binding = 9;
		bindingDescs[9].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[9].bindingDesc.vulkan.set = 0;

		bindingDescs[10].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[10].bindingDesc.vulkan.binding = 10;
		bindingDescs[10].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[10].bindingDesc.vulkan.set = 0;

		bindingDescs[11].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[11].bindingDesc.vulkan.binding = 11;
		bindingDescs[11].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[11].bindingDesc.vulkan.set = 0;

		bindingDescs[12].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[12].bindingDesc.vulkan.binding = 12;
		bindingDescs[12].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[12].bindingDesc.vulkan.set = 0;

		bindingDescs[13].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[13].bindingDesc.vulkan.binding = 13;
		bindingDescs[13].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[13].bindingDesc.vulkan.set = 0;

		desc.numBindingDescs = 14;
		desc.bindingDescs = bindingDescs;
		desc.bytecode.data = EmitterPoint3CS_vulkan_hlsl;
		desc.bytecode.sizeInBytes = sizeof(EmitterPoint3CS_vulkan_hlsl);
	}
	if (config.api == eNvFlowContextApi_d3d12)
	{
		bindingDescs[0].type = eNvFlowDescriptorType_constantBuffer;
		bindingDescs[0].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_b;
		bindingDescs[0].bindingDesc.d3d12.registerBegin = 0;
		bindingDescs[0].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[0].bindingDesc.d3d12.space = 0;

		bindingDescs[1].type = eNvFlowDescriptorType_constantBuffer;
		bindingDescs[1].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_b;
		bindingDescs[1].bindingDesc.d3d12.registerBegin = 1;
		bindingDescs[1].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[1].bindingDesc.d3d12.space = 0;

		bindingDescs[2].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[2].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[2].bindingDesc.d3d12.registerBegin = 0;
		bindingDescs[2].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[2].bindingDesc.d3d12.space = 0;

		bindingDescs[3].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[3].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[3].bindingDesc.d3d12.registerBegin = 1;
		bindingDescs[3].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[3].bindingDesc.d3d12.space = 0;

		bindingDescs[4].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[4].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[4].bindingDesc.d3d12.registerBegin = 2;
		bindingDescs[4].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[4].bindingDesc.d3d12.space = 0;

		bindingDescs[5].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[5].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[5].bindingDesc.d3d12.registerBegin = 3;
		bindingDescs[5].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[5].bindingDesc.d3d12.space = 0;

		bindingDescs[6].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[6].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[6].bindingDesc.d3d12.registerBegin = 4;
		bindingDescs[6].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[6].bindingDesc.d3d12.space = 0;

		bindingDescs[7].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[7].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[7].bindingDesc.d3d12.registerBegin = 5;
		bindingDescs[7].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[7].bindingDesc.d3d12.space = 0;

		bindingDescs[8].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[8].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[8].bindingDesc.d3d12.registerBegin = 6;
		bindingDescs[8].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[8].bindingDesc.d3d12.space = 0;

		bindingDescs[9].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[9].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[9].bindingDesc.d3d12.registerBegin = 7;
		bindingDescs[9].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[9].bindingDesc.d3d12.space = 0;

		bindingDescs[10].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[10].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[10].bindingDesc.d3d12.registerBegin = 8;
		bindingDescs[10].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[10].bindingDesc.d3d12.space = 0;

		bindingDescs[11].type = eNvFlowDescriptorType_structuredBuffer;
		bindingDescs[11].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		bindingDescs[11].bindingDesc.d3d12.registerBegin = 9;
		bindingDescs[11].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[11].bindingDesc.d3d12.space = 0;

		bindingDescs[12].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[12].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[12].bindingDesc.d3d12.registerBegin = 0;
		bindingDescs[12].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[12].bindingDesc.d3d12.space = 0;

		bindingDescs[13].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[13].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[13].bindingDesc.d3d12.registerBegin = 1;
		bindingDescs[13].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[13].bindingDesc.d3d12.space = 0;

		desc.numBindingDescs = 14;
		desc.bindingDescs = bindingDescs;
		desc.bytecode.data = EmitterPoint3CS_d3d12_hlsl;
		desc.bytecode.sizeInBytes = sizeof(EmitterPoint3CS_d3d12_hlsl);
	}
	ptr->pipeline = ptr->contextInterface->createComputePipeline(context, &desc);
};

inline void EmitterPoint3CS_destroy(NvFlowContext* context, EmitterPoint3CS_Pipeline* ptr)
{
	ptr->contextInterface->destroyComputePipeline(context, ptr->pipeline);
}

struct EmitterPoint3CS_PassParams
{
	NvFlowBufferTransient* gParams;
	NvFlowBufferTransient* gSubStepParams;
	NvFlowBufferTransient* gTable;
	NvFlowBufferTransient* arrayValuesIn;
	NvFlowBufferTransient* oldValueIn;
	NvFlowBufferTransient* keyLowIn;
	NvFlowBufferTransient* keyIn;
	NvFlowBufferTransient* valIn;
	NvFlowBufferTransient* scanTarget1In;
	NvFlowBufferTransient* scanCouple1In;
	NvFlowBufferTransient* scanWeight1In;
	NvFlowBufferTransient* key1In;
	NvFlowTextureTransient* valueOut;
	NvFlowTextureTransient* voxelWeightOut;
};

inline void EmitterPoint3CS_addPassCompute(NvFlowContext* context, EmitterPoint3CS_Pipeline* ptr, NvFlowUint3 gridDim, const EmitterPoint3CS_PassParams* params)
{
	NvFlowContextConfig config = {};
	ptr->contextInterface->getContextConfig(context, &config);
	NvFlowDescriptorWrite descriptorWrites[14] = {};
	NvFlowResource resources[14] = {};
	NvFlowPassComputeParams passComputeParams = {};
	passComputeParams.pipeline = ptr->pipeline;
	passComputeParams.gridDim = gridDim;
	if (config.api == eNvFlowContextApi_vulkan)
	{
		descriptorWrites[0].type = eNvFlowDescriptorType_constantBuffer;
		descriptorWrites[0].write.vulkan.binding = 0;
		descriptorWrites[0].write.vulkan.arrayIndex = 0;
		descriptorWrites[0].write.vulkan.set = 0;
		resources[0].bufferTransient = params->gParams;

		descriptorWrites[1].type = eNvFlowDescriptorType_constantBuffer;
		descriptorWrites[1].write.vulkan.binding = 1;
		descriptorWrites[1].write.vulkan.arrayIndex = 0;
		descriptorWrites[1].write.vulkan.set = 0;
		resources[1].bufferTransient = params->gSubStepParams;

		descriptorWrites[2].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[2].write.vulkan.binding = 2;
		descriptorWrites[2].write.vulkan.arrayIndex = 0;
		descriptorWrites[2].write.vulkan.set = 0;
		resources[2].bufferTransient = params->gTable;

		descriptorWrites[3].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[3].write.vulkan.binding = 3;
		descriptorWrites[3].write.vulkan.arrayIndex = 0;
		descriptorWrites[3].write.vulkan.set = 0;
		resources[3].bufferTransient = params->arrayValuesIn;

		descriptorWrites[4].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[4].write.vulkan.binding = 4;
		descriptorWrites[4].write.vulkan.arrayIndex = 0;
		descriptorWrites[4].write.vulkan.set = 0;
		resources[4].bufferTransient = params->oldValueIn;

		descriptorWrites[5].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[5].write.vulkan.binding = 5;
		descriptorWrites[5].write.vulkan.arrayIndex = 0;
		descriptorWrites[5].write.vulkan.set = 0;
		resources[5].bufferTransient = params->keyLowIn;

		descriptorWrites[6].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[6].write.vulkan.binding = 6;
		descriptorWrites[6].write.vulkan.arrayIndex = 0;
		descriptorWrites[6].write.vulkan.set = 0;
		resources[6].bufferTransient = params->keyIn;

		descriptorWrites[7].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[7].write.vulkan.binding = 7;
		descriptorWrites[7].write.vulkan.arrayIndex = 0;
		descriptorWrites[7].write.vulkan.set = 0;
		resources[7].bufferTransient = params->valIn;

		descriptorWrites[8].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[8].write.vulkan.binding = 8;
		descriptorWrites[8].write.vulkan.arrayIndex = 0;
		descriptorWrites[8].write.vulkan.set = 0;
		resources[8].bufferTransient = params->scanTarget1In;

		descriptorWrites[9].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[9].write.vulkan.binding = 9;
		descriptorWrites[9].write.vulkan.arrayIndex = 0;
		descriptorWrites[9].write.vulkan.set = 0;
		resources[9].bufferTransient = params->scanCouple1In;

		descriptorWrites[10].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[10].write.vulkan.binding = 10;
		descriptorWrites[10].write.vulkan.arrayIndex = 0;
		descriptorWrites[10].write.vulkan.set = 0;
		resources[10].bufferTransient = params->scanWeight1In;

		descriptorWrites[11].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[11].write.vulkan.binding = 11;
		descriptorWrites[11].write.vulkan.arrayIndex = 0;
		descriptorWrites[11].write.vulkan.set = 0;
		resources[11].bufferTransient = params->key1In;

		descriptorWrites[12].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[12].write.vulkan.binding = 12;
		descriptorWrites[12].write.vulkan.arrayIndex = 0;
		descriptorWrites[12].write.vulkan.set = 0;
		resources[12].textureTransient = params->valueOut;

		descriptorWrites[13].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[13].write.vulkan.binding = 13;
		descriptorWrites[13].write.vulkan.arrayIndex = 0;
		descriptorWrites[13].write.vulkan.set = 0;
		resources[13].textureTransient = params->voxelWeightOut;

		passComputeParams.numDescriptorWrites = 14;
		passComputeParams.descriptorWrites = descriptorWrites;
		passComputeParams.resources = resources;
		passComputeParams.debugLabel = "EmitterPoint3CS";
	}
	if (config.api == eNvFlowContextApi_d3d12)
	{
		descriptorWrites[0].type = eNvFlowDescriptorType_constantBuffer;
		descriptorWrites[0].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_b;
		descriptorWrites[0].write.d3d12.registerIndex = 0;
		descriptorWrites[0].write.d3d12.space = 0;
		resources[0].bufferTransient = params->gParams;

		descriptorWrites[1].type = eNvFlowDescriptorType_constantBuffer;
		descriptorWrites[1].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_b;
		descriptorWrites[1].write.d3d12.registerIndex = 1;
		descriptorWrites[1].write.d3d12.space = 0;
		resources[1].bufferTransient = params->gSubStepParams;

		descriptorWrites[2].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[2].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[2].write.d3d12.registerIndex = 0;
		descriptorWrites[2].write.d3d12.space = 0;
		resources[2].bufferTransient = params->gTable;

		descriptorWrites[3].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[3].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[3].write.d3d12.registerIndex = 1;
		descriptorWrites[3].write.d3d12.space = 0;
		resources[3].bufferTransient = params->arrayValuesIn;

		descriptorWrites[4].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[4].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[4].write.d3d12.registerIndex = 2;
		descriptorWrites[4].write.d3d12.space = 0;
		resources[4].bufferTransient = params->oldValueIn;

		descriptorWrites[5].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[5].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[5].write.d3d12.registerIndex = 3;
		descriptorWrites[5].write.d3d12.space = 0;
		resources[5].bufferTransient = params->keyLowIn;

		descriptorWrites[6].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[6].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[6].write.d3d12.registerIndex = 4;
		descriptorWrites[6].write.d3d12.space = 0;
		resources[6].bufferTransient = params->keyIn;

		descriptorWrites[7].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[7].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[7].write.d3d12.registerIndex = 5;
		descriptorWrites[7].write.d3d12.space = 0;
		resources[7].bufferTransient = params->valIn;

		descriptorWrites[8].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[8].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[8].write.d3d12.registerIndex = 6;
		descriptorWrites[8].write.d3d12.space = 0;
		resources[8].bufferTransient = params->scanTarget1In;

		descriptorWrites[9].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[9].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[9].write.d3d12.registerIndex = 7;
		descriptorWrites[9].write.d3d12.space = 0;
		resources[9].bufferTransient = params->scanCouple1In;

		descriptorWrites[10].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[10].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[10].write.d3d12.registerIndex = 8;
		descriptorWrites[10].write.d3d12.space = 0;
		resources[10].bufferTransient = params->scanWeight1In;

		descriptorWrites[11].type = eNvFlowDescriptorType_structuredBuffer;
		descriptorWrites[11].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_t;
		descriptorWrites[11].write.d3d12.registerIndex = 9;
		descriptorWrites[11].write.d3d12.space = 0;
		resources[11].bufferTransient = params->key1In;

		descriptorWrites[12].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[12].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[12].write.d3d12.registerIndex = 0;
		descriptorWrites[12].write.d3d12.space = 0;
		resources[12].textureTransient = params->valueOut;

		descriptorWrites[13].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[13].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[13].write.d3d12.registerIndex = 1;
		descriptorWrites[13].write.d3d12.space = 0;
		resources[13].textureTransient = params->voxelWeightOut;

		passComputeParams.numDescriptorWrites = 14;
		passComputeParams.descriptorWrites = descriptorWrites;
		passComputeParams.resources = resources;
		passComputeParams.debugLabel = "EmitterPoint3CS";
	}
	ptr->contextInterface->addPassCompute(context, &passComputeParams);
};
