#include "ShadowNullCS_vulkan.hlsl.h"
#include "ShadowNullCS_d3d12.hlsl.h"

struct ShadowNullCS_Pipeline
{
	NvFlowContextInterface* contextInterface;
	NvFlowComputePipeline* pipeline;
};

inline void ShadowNullCS_init(NvFlowContextInterface* contextInterface, NvFlowContext* context, ShadowNullCS_Pipeline* ptr)
{
	ptr->contextInterface = contextInterface;
	NvFlowContextConfig config = {};
	ptr->contextInterface->getContextConfig(context, &config);
	NvFlowBindingDesc bindingDescs[1] = {};
	NvFlowComputePipelineDesc desc = {};
	if (config.api == eNvFlowContextApi_vulkan)
	{
		bindingDescs[0].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[0].bindingDesc.vulkan.binding = 0;
		bindingDescs[0].bindingDesc.vulkan.descriptorCount = 1;
		bindingDescs[0].bindingDesc.vulkan.set = 0;

		desc.numBindingDescs = 1;
		desc.bindingDescs = bindingDescs;
		desc.bytecode.data = ShadowNullCS_vulkan_hlsl;
		desc.bytecode.sizeInBytes = sizeof(ShadowNullCS_vulkan_hlsl);
	}
	if (config.api == eNvFlowContextApi_d3d12)
	{
		bindingDescs[0].type = eNvFlowDescriptorType_rwTexture;
		bindingDescs[0].bindingDesc.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		bindingDescs[0].bindingDesc.d3d12.registerBegin = 0;
		bindingDescs[0].bindingDesc.d3d12.numRegisters = 1;
		bindingDescs[0].bindingDesc.d3d12.space = 0;

		desc.numBindingDescs = 1;
		desc.bindingDescs = bindingDescs;
		desc.bytecode.data = ShadowNullCS_d3d12_hlsl;
		desc.bytecode.sizeInBytes = sizeof(ShadowNullCS_d3d12_hlsl);
	}
	ptr->pipeline = ptr->contextInterface->createComputePipeline(context, &desc);
};

inline void ShadowNullCS_destroy(NvFlowContext* context, ShadowNullCS_Pipeline* ptr)
{
	ptr->contextInterface->destroyComputePipeline(context, ptr->pipeline);
}

struct ShadowNullCS_PassParams
{
	NvFlowTextureTransient* valueOut;
};

inline void ShadowNullCS_addPassCompute(NvFlowContext* context, ShadowNullCS_Pipeline* ptr, NvFlowUint3 gridDim, const ShadowNullCS_PassParams* params)
{
	NvFlowContextConfig config = {};
	ptr->contextInterface->getContextConfig(context, &config);
	NvFlowDescriptorWrite descriptorWrites[1] = {};
	NvFlowResource resources[1] = {};
	NvFlowPassComputeParams passComputeParams = {};
	passComputeParams.pipeline = ptr->pipeline;
	passComputeParams.gridDim = gridDim;
	if (config.api == eNvFlowContextApi_vulkan)
	{
		descriptorWrites[0].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[0].write.vulkan.binding = 0;
		descriptorWrites[0].write.vulkan.arrayIndex = 0;
		descriptorWrites[0].write.vulkan.set = 0;
		resources[0].textureTransient = params->valueOut;

		passComputeParams.numDescriptorWrites = 1;
		passComputeParams.descriptorWrites = descriptorWrites;
		passComputeParams.resources = resources;
		passComputeParams.debugLabel = "ShadowNullCS";
	}
	if (config.api == eNvFlowContextApi_d3d12)
	{
		descriptorWrites[0].type = eNvFlowDescriptorType_rwTexture;
		descriptorWrites[0].write.d3d12.registerHlsl = eNvFlowRegisterHlsl_u;
		descriptorWrites[0].write.d3d12.registerIndex = 0;
		descriptorWrites[0].write.d3d12.space = 0;
		resources[0].textureTransient = params->valueOut;

		passComputeParams.numDescriptorWrites = 1;
		passComputeParams.descriptorWrites = descriptorWrites;
		passComputeParams.resources = resources;
		passComputeParams.debugLabel = "ShadowNullCS";
	}
	ptr->contextInterface->addPassCompute(context, &passComputeParams);
};
