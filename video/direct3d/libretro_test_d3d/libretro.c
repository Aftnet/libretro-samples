#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "libretro_d3d.h"
#include "Renderer.h"

static struct retro_hw_render_callback hw_render;
static const struct retro_hw_render_interface_d3d11 *d3d11;

const unsigned int fb_width = 800;
const unsigned int fb_height = 600;

void retro_init(void)
{}

void retro_deinit(void)
{}

unsigned retro_api_version(void)
{
	return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
	(void)port;
	(void)device;
}

void retro_get_system_info(struct retro_system_info *info)
{
	memset(info, 0, sizeof(*info));
	info->library_name = "TestCore D3D11";
	info->library_version = "v1";
	info->need_fullpath = false;
	info->valid_extensions = NULL; // Anything is fine, we don't care.
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
	static struct retro_system_timing coreTimings;
	coreTimings.fps = 60.0;
	coreTimings.sample_rate = 24000;

	static struct retro_game_geometry coreGrometry;
	coreGrometry.base_width = fb_width;
	coreGrometry.base_height = fb_height;
	coreGrometry.max_width = fb_width;
	coreGrometry.max_height = fb_height;
	coreGrometry.aspect_ratio = fb_width / fb_height;

	info->timing = coreTimings;
	info->geometry = coreGrometry;
}

static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

void retro_set_environment(retro_environment_t cb)
{
	environ_cb = cb;

	struct retro_variable variables[] = {
		{ NULL, NULL },
	};

	bool no_rom = true;
	cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);
	cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
	audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
	audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
	input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
	input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
	video_cb = cb;
}

static void update_variables(void)
{
	static struct retro_variable var;
	var.key = "testgl_resolution";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
	}
}

static unsigned frame_count;

void retro_run(void)
{
	bool updated = false;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
		update_variables();

	input_poll_cb();

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))
	{
	}

	//Render code here
	renderer_render_frame();

	frame_count++;

	video_cb(RETRO_HW_FRAME_BUFFER_VALID, fb_width, fb_height, 0);
}

static void context_reset(void)
{
	if (!environ_cb(RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE, (void**)&d3d11) || !d3d11)
	{
		//Failed to get HW rendering interface
		return;
	}

	if (d3d11->interface_version != RETRO_HW_RENDER_INTERFACE_D3D11_VERSION)
	{
		//Wrong rendering interface version
		d3d11 = NULL;
		return;
	}

	//Recreate d3d resources here
	renderer_init(d3d11->device, d3d11->context, fb_width, fb_height);
}

static void context_destroy(void)
{
	renderer_deinit();
}

static bool retro_init_hw_context(void)
{
	hw_render.context_type = RETRO_HW_CONTEXT_DIRECT3D;
	hw_render.version_major = 11;
	hw_render.version_minor = 0;
	hw_render.context_reset = context_reset;
	hw_render.context_destroy = context_destroy;
	hw_render.depth = true;
	hw_render.stencil = true;
	hw_render.bottom_left_origin = false;

	if (!environ_cb(RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render))
		return false;

	return true;
}

bool retro_load_game(const struct retro_game_info *info)
{
	update_variables();

	enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
	if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
	{
		//XRGB8888 is not supported
		return false;
	}

	if (!retro_init_hw_context())
	{
		//HW Context could not be initialized, exiting...
		return false;
	}

	(void)info;
	return true;
}

void retro_unload_game(void)
{}

unsigned retro_get_region(void)
{
	return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
	(void)type;
	(void)info;
	(void)num;
	return false;
}

size_t retro_serialize_size(void)
{
	return 0;
}

bool retro_serialize(void *data, size_t size)
{
	(void)data;
	(void)size;
	return false;
}

bool retro_unserialize(const void *data, size_t size)
{
	(void)data;
	(void)size;
	return false;
}

void *retro_get_memory_data(unsigned id)
{
	(void)id;
	return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
	(void)id;
	return 0;
}

void retro_reset(void)
{}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
	(void)index;
	(void)enabled;
	(void)code;
}

