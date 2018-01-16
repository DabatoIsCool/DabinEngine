#include <iostream>
#include <string>

#include <allegro5/allegro.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "Globals.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT ev;
ALLEGRO_TIMEOUT timeout;
bool got_event;

int consoleoutput(lua_State *L)
{
	std::string consolestring = lua_tostring(L, 1);
	std::cout << consolestring << std::endl;
	return 0;
}

int initDabinEngine(lua_State *L)
{
	if (!al_init())
	{
		std::cout << "Error initializing Allegro" << std::endl;
		return -1;
	}
	
	std::cout << "DabinEngine v" << DABINENGINE_VERSION << " successfully initialized" << std::endl;

	return 0;
}

int createWindow(lua_State *L)
{
	if (display != NULL)
	{
		al_destroy_display(display);
	}

	int windowwidth = lua_tointeger(L, 1);
	int windowheight = lua_tointeger(L, 2);
	std::string windowtitle = lua_tostring(L, 3);

	display = al_create_display(windowwidth, windowheight);
	al_set_window_title(display, windowtitle.c_str());

	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));

	return 0;
}

int getEvents(lua_State *L)
{
	

	al_init_timeout(&timeout, 0.06);

	got_event = al_wait_for_event_until(event_queue, &ev, &timeout);

	return 0;
}

int updateWindow(lua_State *L)
{
	al_flip_display();

	return 0;
}

int clearWindow(lua_State *L)
{
	al_clear_to_color(al_map_rgb(0, 0, 0));

	return 0;
}

int destroyWindow(lua_State *L)
{
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

int isCloseRequested(lua_State *L)
{

	std::cout << "Working!" << std::endl;

	if (got_event && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

lua_State *initLua()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	lua_register(L, "print", consoleoutput);
	lua_register(L, "dabin_init", initDabinEngine);
	lua_register(L, "dabin_createwindow", createWindow);
	lua_register(L, "dabin_updatewindow", updateWindow);
	lua_register(L, "dabin_clearwindow", clearWindow);
	lua_register(L, "dabin_getevents", getEvents);
	lua_register(L, "dabin_iscloserequested", isCloseRequested);
	lua_register(L, "dabin_destroywindow", destroyWindow);

	luaL_dofile(L, "lua/main.lua");

	return L;
}

int main()
{
	lua_State *L = initLua();
	

	lua_close(L);

	std::system("pause");

	return 0;
}