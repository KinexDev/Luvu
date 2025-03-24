#include "../include/Graphics.h"

void Graphics::Register(lua_State* L)
{
	lua_newtable(L);
	PUSH_FUNCTION_AS_TABLE_KEY(L, &DrawImage, "DrawImage");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &DrawRectangle, "DrawRectangle");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &DrawLine, "DrawLine");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &GetWidth, "GetWidth");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &GetHeight, "GetHeight");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &SetWindowTitle, "SetWindowTitle");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &SetWindowSize, "SetWindowSize");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &SetWindowPos, "SetWindowPos");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &SetWindowResizable, "SetWindowResizable");
	PUSH_FUNCTION_AS_TABLE_KEY(L, &SetVSync, "SetVSync");
	lua_setglobal(L, "Graphics");
}

int Graphics::DrawImage(lua_State* L)
{
	if (!lua_isuserdata(L, 1) || !lua_isuserdata(L, 2) || !lua_isnumber(L, 3) || !lua_isuserdata(L, 4) || !lua_isuserdata(L, 5))
		return 0;

	LuaTexture* texture = static_cast<LuaTexture*>(lua_touserdata(L, 1));
	LuaVec2* position = static_cast<LuaVec2*>(lua_touserdata(L, 2));
	float rotation = (float)lua_tonumber(L, 3);
	LuaVec2* scale = static_cast<LuaVec2*>(lua_touserdata(L, 4));
	LuaColor* color = static_cast<LuaColor*>(lua_touserdata(L, 5));

	if (texture && position && scale && color)
	{
		glActiveTexture(GL_TEXTURE0);
		Game& instance = Game::Instance();
		
		mat4 mat;
		glm_mat4_identity(mat);
		glm_translate(mat, vec3{ position->vec2Val[0], position->vec2Val[1], 0});
		glm_rotate(mat, glm_rad(rotation), vec3{ 0, 0, 1 });
		glm_scale(mat, vec3{ scale->vec2Val[0], scale->vec2Val[1], 1 });

		glActiveTexture(GL_TEXTURE0);
		texture->tex->Bind();

		instance.defaultShader->SetUniform("hasTexture", 1);
		instance.defaultShader->SetUniform("color", color->vec4Val);
		instance.defaultShader->SetUniform("model", mat);
		instance.defaultShader->SetUniform("projection", instance.p);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return 0;
}

int Graphics::GetWidth(lua_State* L)
{
	Game& instance = Game::Instance();
	lua_pushnumber(L, instance.screenWidth);
	return 1;
}

int Graphics::GetHeight(lua_State* L)
{
	Game& instance = Game::Instance();
	lua_pushnumber(L, instance.screenHeight);
	return 1;
}

int Graphics::DrawRectangle(lua_State* L)
{
	if (!lua_isuserdata(L, 1) || !lua_isnumber(L, 2) || !lua_isuserdata(L, 3) || !lua_isuserdata(L, 4))
		return 0;
	LuaVec2* position = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	float rotation = (float)lua_tonumber(L, 2);
	LuaVec2* scale = static_cast<LuaVec2*>(lua_touserdata(L, 3));
	LuaColor* color = static_cast<LuaColor*>(lua_touserdata(L, 4));

	if (position && scale && color)
	{
		Game& instance = Game::Instance();

		mat4 mat;
		glm_mat4_identity(mat);
		glm_translate(mat, vec3{ position->vec2Val[0], position->vec2Val[1], 0 });
		glm_rotate(mat, glm_rad(rotation), vec3{ 0, 0, 1 });
		glm_scale(mat, vec3{ scale->vec2Val[0], scale->vec2Val[1], 1 });

		instance.defaultShader->SetUniform("hasTexture", 0);
		instance.defaultShader->SetUniform("color", color->vec4Val);
		instance.defaultShader->SetUniform("model", mat);
		instance.defaultShader->SetUniform("projection", instance.p);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	return 0;
}

int Graphics::DrawLine(lua_State* L)
{
	if (!lua_isuserdata(L, 1) || !lua_isuserdata(L, 2) || !lua_isnumber(L, 3) || !lua_isuserdata(L, 4))
		return 0;
	LuaVec2* start = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	LuaVec2* end = static_cast<LuaVec2*>(lua_touserdata(L, 2));
	float width = (float)lua_tonumber(L, 3);
	LuaColor* color = static_cast<LuaColor*>(lua_touserdata(L, 4));

	if (start && end && color)
	{
		vec2 dir;

		glm_vec2_sub(end->vec2Val, start->vec2Val, dir);

		float length = sqrtf(dir[0] * dir[0] +
			dir[1] * dir[1]);

		vec2 offset;

		glm_vec2_divs(dir, 2, offset);

		glm_vec2_normalize(dir);

		float rotation = atan2(dir[1], dir[0]);

		vec2 pos { start->vec2Val[0], start->vec2Val[1] };
		glm_vec2_add(pos, offset, pos);

		Game& instance = Game::Instance();

		mat4 mat;
		glm_mat4_identity(mat);
		glm_translate(mat, vec3{ pos[0], pos[1], 0 });
		glm_rotate(mat, rotation, vec3{ 0, 0, 1 });
		glm_scale(mat, vec3{ length, width, 1 });

		instance.defaultShader->SetUniform("hasTexture", 0);
		instance.defaultShader->SetUniform("color", color->vec4Val);
		instance.defaultShader->SetUniform("model", mat);
		instance.defaultShader->SetUniform("projection", instance.p);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	return 0;
}

int Graphics::SetWindowTitle(lua_State* L)
{
	if (!lua_isstring(L, 1))
		return -1;
	const char* title = lua_tostring(L, 1);
	Game& instance = Game::Instance();
	glfwSetWindowTitle(instance.window, title);
	return 0;
}

int Graphics::SetWindowSize(lua_State* L)
{
	if (!lua_isuserdata(L, 1))
		return -1;
	LuaVec2* vec2 = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	if (vec2)
	{
		Game& instance = Game::Instance();
		glfwSetWindowSize(instance.window, vec2->vec2Val[0], vec2->vec2Val[1]);
	}
	return 0;
}

int Graphics::SetWindowPos(lua_State* L)
{
	if (!lua_isuserdata(L, 1))
		return -1;
	LuaVec2* vec2 = static_cast<LuaVec2*>(lua_touserdata(L, 1));
	if (vec2)
	{
		Game& instance = Game::Instance();
		glfwSetWindowPos(instance.window, vec2->vec2Val[0], vec2->vec2Val[1]);
	}
	return 0;
}

int Graphics::SetWindowResizable(lua_State* L)
{
	if (!lua_isboolean(L, 1))
		return -1;
	int boolean = lua_toboolean(L, 1);
	Game& instance = Game::Instance();
	glfwSetWindowAttrib(instance.window, GLFW_RESIZABLE, boolean);
	return 0;
}

int Graphics::SetFullscreen(lua_State* L)
{
	return 0;
}

int Graphics::SetVSync(lua_State* L)
{
	if (!lua_isboolean(L, 1))
		return -1;
	int val = lua_toboolean(L, 1);
	glfwSwapInterval(val);
	return 0;
}
