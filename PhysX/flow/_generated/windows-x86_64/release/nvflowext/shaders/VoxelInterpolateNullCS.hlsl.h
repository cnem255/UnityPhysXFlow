#include "VoxelInterpolateNullCS_vulkan.hlsl.h"
#include "VoxelInterpolateNullCS_d3d12.hlsl.h"

struct VoxelInterpolateNullCS_Pipeline
{
	NvFlowContextInterface* contextInterface;
	NvFlowComputePipeline* pipeline;
};

inline void VoxelInterpolateNullCS_init(NvFlowContextInterface* contextInterface, NvFlowContext* context, VoxelInterpolateNullCS_Pipeline* ptr)
{
	ptr->contextInterface = contextInterface;
	NvFlowContextConfig config = {};
	ptr->contextInterface->getContextConfig(context, &config);
	NvFlowBindingDesc bindingDescs[16] = {};
	NvFlowComputePipelineDesc desc = {};
	if (config.api == eNvFlowContextApi_vulkan)
	{
		bindingDescs[0].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[0].bindingDesc.vulkan.binding = 0;
		bindingDescs[0].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[0].bindingDesc.vulkan.set = 0;

		bindingDescs[1].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[1].bindingDesc.vulkan.binding = 1;
		bindingDescs[1].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[1].bindingDesc.vulkan.set = 0;

		bindingDescs[2].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[2].bindingDesc.vulkan.binding = 2;
		bindingDescs[2].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[2].bindingDesc.vulkan.set = 0;

		bindingDescs[3].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[3].bindingDesc.vulkan.binding = 3;
		bindingDescs[3].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[3].bindingDesc.vulkan.set = 0;

		bindingDescs[4].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[4].bindingDesc.vulkan.binding = 4;
		bindingDescs[4].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[4].bindingDesc.vulkan.set = 0;

		bindingDescs[5].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[5].bindingDesc.vulkan.binding = 5;
		bindingDescs[5].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[5].bindingDesc.vulkan.set = 0;

		bindingDescs[6].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[6].bindingDesc.vulkan.binding = 6;
		bindingDescs[6].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[6].bindingDesc.vulkan.set = 0;

		bindingDescs[7].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[7].bindingDesc.vulkan.binding = 7;
		bindingDescs[7].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[7].bindingDesc.vulkan.set = 0;

		bindingDescs[8].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[8].bindingDesc.vulkan.binding = 8;
		bindingDescs[8].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[8].bindingDesc.vulkan.set = 0;

		bindingDescs[9].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[9].bindingDesc.vulkan.binding = 9;
		bindingDescs[9].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[9].bindingDesc.vulkan.set = 0;

		bindingDescs[10].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[10].bindingDesc.vulkan.binding = 10;
		bindingDescs[10].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[10].bindingDesc.vulkan.set = 0;

		bindingDescs[11].type = eNvFlowDescriptorType_rwTexture;
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

		bindingDescs[14].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[14].bindingDesc.vulkan.binding = 14;
		bindingDescs[14].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[14].bindingDesc.vulkan.set = 0;

		bindingDescs[15].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[15].bindingDesc.vulkan.binding = 15;
		bindingDescs[15].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[15].bindingDesc.vulkan.set = 0;

		desc.numBindingDescs = 16;
		desc.bindingDescs = bindingDescs;
		desc.bytecode.data = VoxelInterpolateNullCS_vulkan_hlsl;
		desc.bytecode.sizeInBytes = sizeof(VoxelInterpolateNullCS_vulkan_hlsl);
	}
	if (config.api == eNvFlowContextApi_d3d12)
	{
		bindingDescs[0].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[0].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[0].bindingDesc.d3d12.registerBegin = 0;
		bindingDescs[0].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[0].bindingDesc.d3d12.space = 0;

		bindingDescs[1].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[1].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[1].bindingDesc.d3d12.registerBegin = 1;
		bindingDescs[1].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[1].bindingDesc.d3d12.space = 0;

		bindingDescs[2].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[2].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[2].bindingDesc.d3d12.registerBegin = 2;
		bindingDescs[2].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[2].bindingDesc.d3d12.space = 0;

		bindingDescs[3].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[3].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[3].bindingDesc.d3d12.registerBegin = 3;
		bindingDescs[3].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[3].bindingDesc.d3d12.space = 0;

		bindingDescs[4].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[4].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[4].bindingDesc.d3d12.registerBegin = 4;
		bindingDescs[4].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[4].bindingDesc.d3d12.space = 0;

		bindingDescs[5].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[5].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[5].bindingDesc.d3d12.registerBegin = 5;
		bindingDescs[5].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[5].bindingDesc.d3d12.space = 0;

		bindingDescs[6].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[6].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[6].bindingDesc.d3d12.registerBegin = 6;
		bindingDescs[6].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[6].bindingDesc.d3d12.space = 0;

		bindingDescs[7].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[7].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[7].bindingDesc.d3d12.registerBegin = 7;
		bindingDescs[7].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[7].bindingDesc.d3d12.space = 0;

		bindingDescs[8].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[8].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[8].bindingDesc.d3d12.registerBegin = 8;
		bindingDescs[8].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[8].bindingDesc.d3d12.space = 0;

		bindingDescs[9].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[9].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[9].bindingDesc.d3d12.registerBegin = 9;
		bindingDescs[9].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[9].bindingDesc.d3d12.space = 0;

		bindingDescs[10].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[10].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[10].bindingDesc.d3d12.registerBegin = 10;
		bindingDescs[10].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[10].bindingDesc.d3d12.space = 0;

		bindingDescs[11].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[11].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[11].bindingDesc.d3d12.registerBegin = 11;
		bindingDescs[11].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[11].bindingDesc.d3d12.space = 0;

		bindingDescs[12].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[12].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[12].bindingDesc.d3d12.registerBegin = 12;
		bindingDescs[12].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[12].bindingDesc.d3d12.space = 0;

		bindingDescs[13].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[13].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[13].bindingDesc.d3d12.registerBegin = 13;
		bindingDescs[13].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[13].bindingDesc.d3d12.space = 0;

		bindingDescs[14].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[14].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[14].bindingDesc.d3d12.registerBegin = 14;
		bindingDescs[14].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[14].bindingDesc.d3d12.space = 0;

		bindingDescs[15].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[15].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[15].bindingDesc.d3d12.registerBegin = 15;
		bindingDescs[15].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[15].bindingDesc.d3d12.space = 0;

		desc.numBindingDescs = 16;
		desc.bindingDescs = bindingDescs;
		desc.bytecode.data = VoxelInterpolateNullCS_d3d12_hlsl;
		desc.bytecode.sizeInBytes = sizeof(VoxelInterpolateNullCS_d3d12_hlsl);
	}
	ptr->pipeline = ptr->contextInterface->createComputePipeline(context, &desc);
};

inline void VoxelInterpolateNullCS_destroy(NvFlowContext* context, VoxelInterpolateNullCS_Pipeline* ptr)
{
	ptr->contextInterface->destroyComputePipeline(context, ptr->pipeline);
}

struct VoxelInterpolateNullCS_PassParams
{
	NvFlowTextureTransient* value0;
	NvFlowTextureTransient* value1;
	NvFlowTextureTransient* value2;
	NvFlowTextureTransient* value3;
	NvFlowTextureTransient* value4;
	NvFlowTextureTransient* value5;
	NvFlowTextureTransient* value6;
	NvFlowTextureTransient* value7;
	NvFlowTextureTransient* weight0;
	NvFlowTextureTransient* weight1;
	NvFlowTextureTransient* weight2;
	NvFlowTextureTransient* weight3;
	NvFlowTextureTransient* weight4;
	NvFlowTextureTransient* weight5;
	NvFlowTextureTransient* weight6;
	NvFlowTextureTransient* weight7;
};

inline void VoxelInterpolateNullCS_addPassCompute(NvFlowContext* context, VoxelInterpolateNullCS_Pipeline* ptr, NvFlowUint3 gridDim, const VoxelInterpolateNullCS_PassParams* params)
{
	NvFlowContextConfig config = {};
	ptr->contextInterface->getContextConfig(context, &config);
	NvFlowDescriptorWrite descriptorWrites[16] = {};
	NvFlowResource resources[16] = {};
	NvFlowPassComputeParams passComputeParams = {};
	passComputeParams.pipeline = ptr->pipeline;
	passComputeParams.gridDim = gridDim;
	if (config.api == eNvFlowContextApi_vulkan)
	{
		descriptorWrites[0].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[0].write.vulkan.binding = 0;
		descriptorWrites[0].write.vulkan.arrayIndex = 0;
		descriptorWrites[0].write.vulkan.set = 0;
		resources[0].textureTransient = params->value0;

		descriptorWrites[1].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[1].write.vulkan.binding = 1;
		descriptorWrites[1].write.vulkan.arrayIndex = 0;
		descriptorWrites[1].write.vulkan.set = 0;
		resources[1].textureTransient = params->value1;

		descriptorWrites[2].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[2].write.vulkan.binding = 2;
		descriptorWrites[2].write.vulkan.arrayIndex = 0;
		descriptorWrites[2].write.vulkan.set = 0;
		resources[2].textureTransient = params->value2;

		descriptorWrites[3].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[3].write.vulkan.binding = 3;
		descriptorWrites[3].write.vulkan.arrayIndex = 0;
		descriptorWrites[3].write.vulkan.set = 0;
		resources[3].textureTransient = params->value3;

		descriptorWrites[4].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[4].write.vulkan.binding = 4;
		descriptorWrites[4].write.vulkan.arrayIndex = 0;
		descriptorWrites[4].write.vulkan.set = 0;
		resources[4].textureTransient = params->value4;

		descriptorWrites[5].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[5].write.vulkan.binding = 5;
		descriptorWrites[5].write.vulkan.arrayIndex = 0;
		descriptorWrites[5].write.vulkan.set = 0;
		resources[5].textureTransient = params->value5;

		descriptorWrites[6].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[6].write.vulkan.binding = 6;
		descriptorWrites[6].write.vulkan.arrayIndex = 0;
		descriptorWrites[6].write.vulkan.set = 0;
		resources[6].textureTransient = params->value6;

		descriptorWrites[7].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[7].write.vulkan.binding = 7;
		descriptorWrites[7].write.vulkan.arrayIndex = 0;
		descriptorWrites[7].write.vulkan.set = 0;
		resources[7].textureTransient = params->value7;

		descriptorWrites[8].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[8].write.vulkan.binding = 8;
		descriptorWrites[8].write.vulkan.arrayIndex = 0;
		descriptorWrites[8].write.vulkan.set = 0;
		resources[8].textureTransient = params->weight0;

		descriptorWrites[9].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[9].write.vulkan.binding = 9;
		descriptorWrites[9].write.vulkan.arrayIndex = 0;
		descriptorWrites[9].write.vulkan.set = 0;
		resources[9].textureTransient = params->weight1;

		descriptorWrites[10].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[10].write.vulkan.binding = 10;
		descriptorWrites[10].write.vulkan.arrayIndex = 0;
		descriptorWrites[10].write.vulkan.set = 0;
		resources[10].textureTransient = params->weight2;

		descriptorWrites[11].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[11].write.vulkan.binding = 11;
		descriptorWrites[11].write.vulkan.arrayIndex = 0;
		descriptorWrites[11].write.vulkan.set = 0;
		resources[11].textureTransient = params->weight3;

		descriptorWrites[12].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[12].write.vulkan.binding = 12;
		descriptorWrites[12].write.vulkan.arrayIndex = 0;
		descriptorWrites[12].write.vulkan.set = 0;
		resources[12].textureTransient = params->weight4;

		descriptorWrites[13].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[13].write.vulkan.binding = 13;
		descriptorWrites[13].write.vulkan.arrayIndex = 0;
		descriptorWrites[13].write.vulkan.set = 0;
		resources[13].textureTransient = params->weight5;

		descriptorWrites[14].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[14].write.vulkan.binding = 14;
		descriptorWrites[14].write.vulkan.arrayIndex = 0;
		descriptorWrites[14].write.vulkan.set = 0;
		resources[14].textureTransient = params->weight6;

		descriptorWrites[15].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[15].write.vulkan.binding = 15;
		descriptorWrites[15].write.vulkan.arrayIndex = 0;
		descriptorWrites[15].write.vulkan.set = 0;
		resources[15].textureTransient = params->weight7;

		passComputeParams.numDescriptorWrites = 16;
		passComputeParams.descriptorWrites = descriptorWrites;
		passComputeParams.resources = resources;
		passComputeParams.debugLabel = "VoxelInterpolateNullCS";
	}
	if (config.api == eNvFlowContextApi_d3d12)
	{
		descriptorWrites[0].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[0].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[0].write.d3d12.registerIndex = 0;
		descriptorWrites[0].write.d3d12.space = 0;
		resources[0].textureTransient = params->value0;

		descriptorWrites[1].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[1].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[1].write.d3d12.registerIndex = 1;
		descriptorWrites[1].write.d3d12.space = 0;
		resources[1].textureTransient = params->value1;

		descriptorWrites[2].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[2].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[2].write.d3d12.registerIndex = 2;
		descriptorWrites[2].write.d3d12.space = 0;
		resources[2].textureTransient = params->value2;

		descriptorWrites[3].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[3].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[3].write.d3d12.registerIndex = 3;
		descriptorWrites[3].write.d3d12.space = 0;
		resources[3].textureTransient = params->value3;

		descriptorWrites[4].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[4].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[4].write.d3d12.registerIndex = 4;
		descriptorWrites[4].write.d3d12.space = 0;
		resources[4].textureTransient = params->value4;

		descriptorWrites[5].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[5].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[5].write.d3d12.registerIndex = 5;
		descriptorWrites[5].write.d3d12.space = 0;
		resources[5].textureTransient = params->value5;

		descriptorWrites[6].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[6].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[6].write.d3d12.registerIndex = 6;
		descriptorWrites[6].write.d3d12.space = 0;
		resources[6].textureTransient = params->value6;

		descriptorWrites[7].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[7].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[7].write.d3d12.registerIndex = 7;
		descriptorWrites[7].write.d3d12.space = 0;
		resources[7].textureTransient = params->value7;

		descriptorWrites[8].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[8].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[8].write.d3d12.registerIndex = 8;
		descriptorWrites[8].write.d3d12.space = 0;
		resources[8].textureTransient = params->weight0;

		descriptorWrites[9].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[9].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[9].write.d3d12.registerIndex = 9;
		descriptorWrites[9].write.d3d12.space = 0;
		resources[9].textureTransient = params->weight1;

		descriptorWrites[10].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[10].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[10].write.d3d12.registerIndex = 10;
		descriptorWrites[10].write.d3d12.space = 0;
		resources[10].textureTransient = params->weight2;

		descriptorWrites[11].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[11].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[11].write.d3d12.registerIndex = 11;
		descriptorWrites[11].write.d3d12.space = 0;
		resources[11].textureTransient = params->weight3;

		descriptorWrites[12].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[12].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[12].write.d3d12.registerIndex = 12;
		descriptorWrites[12].write.d3d12.space = 0;
		resources[12].textureTransient = params->weight4;

		descriptorWrites[13].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[13].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[13].write.d3d12.registerIndex = 13;
		descriptorWrites[13].write.d3d12.space = 0;
		resources[13].textureTransient = params->weight5;

		descriptorWrites[14].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[14].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[14].write.d3d12.registerIndex = 14;
		descriptorWrites[14].write.d3d12.space = 0;
		resources[14].textureTransient = params->weight6;

		descriptorWrites[15].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[15].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[15].write.d3d12.registerIndex = 15;
		descriptorWrites[15].write.d3d12.space = 0;
		resources[15].textureTransient = params->weight7;

		passComputeParams.numDescriptorWrites = 16;
		passComputeParams.descriptorWrites = descriptorWrites;
		passComputeParams.resources = resources;
		passComputeParams.debugLabel = "VoxelInterpolateNullCS";
	}
	ptr->contextInterface->addPassCompute(context, &passComputeParams);
};
