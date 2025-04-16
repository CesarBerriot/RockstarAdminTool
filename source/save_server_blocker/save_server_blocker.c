#include "save_server_blocker.h"
#include <windows.h>
#include <fwpmu.h>
#include <hard_assert.h>
#include <stdint.h>

// missing symbols on MinGW
// see
#ifdef __MINGW32__
	DEFINE_GUID(
		FWPM_LAYER_OUTBOUND_TRANSPORT_V4,
		0x09e61aea,
		0xd214,
		0x46e2,
		0x9b, 0x21, 0xb2, 0x6b, 0x0b, 0x2f, 0x28, 0xc8
	);

	DEFINE_GUID(
	   FWPM_CONDITION_IP_REMOTE_ADDRESS,
	   0xb235ae9a,
	   0x1d64,
	   0x49b8,
	   0xa4, 0x4c, 0x5f, 0xf3, 0xd9, 0x09, 0x50, 0x45
	);
#endif

static GUID FILTER_GUID = { 0x1a6242ad, 0x77d4, 0x4726, { 0x80, 0x3c, 0x10, 0x35, 0x68, 0xee, 0x6a, 0x1e } };

static bool compute_state(void);
static void create_filter(void);
static void delete_filter(void);
static HANDLE create_filtering_engine(void);
static void cleanup_filtering_engine(HANDLE);

bool @is_blocking_save_servers(void)
{	return compute_state();
}

void @toggle_save_server_blocking(void)
{	compute_state() ? delete_filter() : create_filter();
}

static bool compute_state(void)
{	HANDLE filtering_engine = create_filtering_engine();
	bool result = false;
	FWPM_FILTER0 * filter;
	switch(FwpmFilterGetByKey0(filtering_engine, &FILTER_GUID, &filter))
	{	case ERROR_SUCCESS:
			FwpmFreeMemory0((void**)&filter);
			result = true;
		case FWP_E_FILTER_NOT_FOUND:
			break;
		default:
			ha_abort(MODULE_NAME, "Filter lookup operation failure.");
	}

	cleanup_filtering_engine(filtering_engine);
	return result;
}

static void create_filter(void)
{	HANDLE filtering_engine = create_filtering_engine();
	FWPM_FILTER0 filter =
	{	.filterKey = FILTER_GUID,
		.displayData =
		{	.name = L"" MODULE_NAME " R* Save Servers Outbound Filter",
			.description = L"https://github.com/CesarBerriot/RockstarAdminTool"
		},
		.layerKey = FWPM_LAYER_OUTBOUND_TRANSPORT_V4,
		.subLayerKey = IID_NULL,
		.weight =
		{	.type = FWP_UINT64,
			.uint64 = (UINT64*)(UINT32[2]){ UINT32_MAX, UINT32_MAX },
		},
		.numFilterConditions = 1,
		.filterCondition = (FWPM_FILTER_CONDITION0[])
		{	{	.fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS,
				.matchType = FWP_MATCH_EQUAL,
				.conditionValue =
				{	.type = FWP_UINT32,
					.uint32 = htonl(inet_addr("192.81.241.171"))
				}
			}
		},
		.action.type = FWP_ACTION_BLOCK
	};
	ha_assert(!FwpmFilterAdd0(filtering_engine, &filter, NULL, NULL), MODULE_NAME, "Filtering engine filter creation failure.");
	cleanup_filtering_engine(filtering_engine);
}

static void delete_filter(void)
{	HANDLE filtering_engine = create_filtering_engine();
	ha_assert(!FwpmFilterDeleteByKey0(filtering_engine, &FILTER_GUID), MODULE_NAME, "Filtering engine filter deletion failure.");
	cleanup_filtering_engine(filtering_engine);
}

static HANDLE create_filtering_engine(void)
{	HANDLE filtering_engine;
	ha_assert(!FwpmEngineOpen0(NULL, RPC_C_AUTHN_DEFAULT, NULL, NULL, &filtering_engine), MODULE_NAME, "Filtering engine initialization failure.");
	return filtering_engine;
}

static void cleanup_filtering_engine(HANDLE filtering_engine)
{	ha_assert(!FwpmEngineClose0(filtering_engine), MODULE_NAME, "Filtering engine cleanup failure.");
}
