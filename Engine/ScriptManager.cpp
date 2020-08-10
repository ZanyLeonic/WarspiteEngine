#include "ScriptManager.h"
#include <iostream>

#include "ScriptWrappers.h"
#include "WarspiteUtil.h"
#include <pybind11/embed.h>
#include "WarspiteObject.h"

// Exposing some classes to Python
PYBIND11_MODULE(engine, m) {
	// define these so they can be assigned later.
	m.attr(LEVELOBJECT_NAME)		= py::none();
	m.attr(CAMERAOBJECT_NAME)	= py::none();
	m.attr(INPUTOBJECT_NAME)		= py::none();
	m.attr(GAMEOBJECT_NAME)		= py::none();

	py::class_<CPtrWrapper<CWarspiteObject>>(m, "pWO")
		.def("get_ptr", &CPtrWrapper<CWarspiteObject>::get);
	
	py::class_<CVector2D>(m, "Vector2D", "A two dimensional vector object")
		.def(py::init<>())
		.def(py::init<float,float>())
		.def("set_x", &CVector2D::SetX, "Sets the X value from inside this Vector")
		.def("set_y", &CVector2D::SetY, "Sets the Y value from inside this Vector")
		.def("get_x", &CVector2D::GetX, "Returns the X value from inside this vector")
		.def("get_y", &CVector2D::GetY, "Returns the Y value from inside this vector")
		.def("norm", &CVector2D::Normalize, "Returns a normalization of the vector")
		.def("length", &CVector2D::Length, "Returns the length of the vector (as a float)")
		.def("__len__", &CVector2D::Length_i, "Returns the length of the vector (truncated as an int)")
		.def("__repr__", [](const CVector2D& v)
			{
				return "<CVector2D with point (" +
					std::to_string(v.GetX()) + ", "
					+ std::to_string(v.GetY()) + ")>";
			}
	);

	py::class_<CWarspiteObject>(m, "WarspiteObject", "An object that exists as the base for the majority of GameObjects in levels")
		.def(py::init<>())
		.def("get_position", &CWarspiteObject::GetPosition, "Returns the position of the specified object")
		.def("set_position", &CWarspiteObject::SetPosition, "Sets the position of the specified object")
		.def("get_type", &CWarspiteObject::GetFactoryID, "Returns the factory ID of the object")
		.def("get_name", &CWarspiteObject::GetName, "Returns the name of the object")
		.def("get_texid", &CWarspiteObject::GetTextureID, "Returns the texture ID associated with the object")
		.def("get_velocity", &CWarspiteObject::GetVelocity, "Returns the current velocity")
		.def("set_velocity", &CWarspiteObject::SetVelocity, "Sets the current velocity")
		.def("add_velocity", &CWarspiteObject::AddVelocity, "Adds the specified velocity to the current")
		.def("get_acceleration", &CWarspiteObject::GetAcceleration, "Returns the current acceleration")
		.def("set_acceleration", &CWarspiteObject::SetAcceleration, "Sets the current acceleration")
		.def("add_acceleration", &CWarspiteObject::AddAcceleration, "Adds to the current acceleration")
		.def("get_size", &CWarspiteObject::GetSize, "Returns a Vector2D with the dimensions of the object")
		.def("get_current_anim_row", &CWarspiteObject::GetCurrentAnimRow, "Returns the current row of animation on the sprite sheet the object is on")
		.def("set_anim_row", &CWarspiteObject::SetAnimRow, "Sets the current row of animation on the sprite sheet the object is on")
		.def("get_current_anim_frame", &CWarspiteObject::GetCurrentAnimFrame, "Returns the current frame of animation the object is on")
		.def("set_anim_frame", &CWarspiteObject::SetAnimFrame, "Sets the current animation frame")
		.def("get_total_anim_frames", &CWarspiteObject::GetTotalAnimFrames, "Returns the total amount of animation frames the object has")
		.def("should_collide", &CWarspiteObject::ShouldCollide, "Returns whether the object should collide with other objects")
		.def("set_collision", &CWarspiteObject::SetCollision, "Sets whether this object should collide with other objects")
		.def("__repr__", [](CWarspiteObject& o)
			{
				return "<CWarspiteObject \"" + (std::string(o.GetName()).empty() ? "UNDEFINED" : std::string(o.GetName())) +
					"\" of type \"" + (o.GetFactoryID().empty() ? "UNDEFINED" : o.GetFactoryID()) + "\">";
			}
	);

	py::class_<CGameStateBase>(m, "GameState", "An object that supplies custom functionality for the Engine depending on scenario")
		.def("get_loaded_textures", &CGameStateBase::GetLoadedTextures, "Returns a vector of loaded texture IDs related to the state")
		.def("get_loaded_scripts", &CGameStateBase::GetLoadedScripts, "Returns a vector of loaded script IDs related to the state")
		.def("should_be_ticking", &CGameStateBase::ShouldBeTicking, "Returns whether the state should be ticking or not")
		.def("should_be_drawing", &CGameStateBase::ShouldBeDrawing, "Returns whether the state should be drawing or not");
	
	py::class_<SLevelObject>(m, "LevelObject", "A container that allows interaction with the currently loaded level. (Do not call this - use engine.level)")
		.def(py::init<CLevel*>())
		.def("get_name", &SLevelObject::GetName, "Returns the name of the map currently loaded. (Extension removed.)")
		.def("get_level_size", &SLevelObject::GetLevelSize, "Returns a Vector2D of the map's dimensions")
		.def("create_object", &SLevelObject::CreateObject, "Creates the object specified - returns the object in success and nullptr when failed")
		.def("find_wobject", &SLevelObject::FindGameObject, "Returns a WarspiteObject that matches the specified ID")
		.def("__repr__", [](SLevelObject& o)
			{
				return "<SLevelObject for map \"" + (o.GetName().empty() ? "UNDEFINED" : o.GetName()) + "\">";
			}
	);

	py::class_<SCameraObject>(m, "CameraObject", "A container that allows interaction with the camera in use. (Do not call this - use engine.camera)")
		.def(py::init<CCamera*>())
		.def("get_position", &SCameraObject::GetPosition, "Returns a Vector2D of the current position of the Camera")
		.def("get_position_with_offset", &SCameraObject::GetOffsetedPosition, "Returns a Vector2D of the current position of the Camera offset by the viewport")
		.def("get_target", &SCameraObject::GetTarget, "Returns a Vector2D of the current target of the Camera")
		.def("set_position", &SCameraObject::SetPosition, "Sets the position of the Camera")
		.def("set_target", &SCameraObject::SetTarget, "Sets the target of the Camera")
		.def("get_level_size", &SCameraObject::GetLevelSize, "Returns a Vector2D of the level size")
		.def("__repr__", [](SCameraObject& o)
			{
				return "<SCameraObject with target (" +
					std::to_string(o.GetTarget()->GetX()) + ", " +
					std::to_string(o.GetTarget()->GetY()) +
					")>";
			}
	);

	py::class_<SInputObject>(m, "InputObject", "A container that allows interaction input handler. (Do not call this - use engine.inputh)")
		.def(py::init<CInputHandler*>())
		.def("get_button_state", &SInputObject::GetButtonState, "Returns the button state of the specified controller and button")
		.def("get_mouse_button_state", &SInputObject::GetMouseButtonState, "Returns the state of the specified mouse button")
		.def("is_keydown", &SInputObject::IsKeyDown, "Returns whether the specified key is down")
		.def("get_x_axis", &SInputObject::GetXAxis, "Returns the value of the specified X value on the specified axis on the specified controller")
		.def("get_y_axis", &SInputObject::GetYAxis, "Returns the value of the specified Y value on the specified axis on the specified controller")
		.def("set_release_state", &SInputObject::SetReleaseState, "Sets the release state of the specified key")
		.def("add_action_keydown", &SInputObject::AddActionKeyDown, "Binds the execution of the specified method when the specified key is down")
		.def("add_action_keyup", &SInputObject::AddActionKeyUp, "Binds the execution of the specified method when the specified key is released");

	py::class_<SGameObject>(m, "GameObject", "A container that allows interaction with more misc aspects of the Engine's APIs. (Do not call this - use engine.game)")
		.def(py::init<CGame*>())
		.def("get_current_state", &SGameObject::GetCurrentState, "Returns the state that is currently loaded")
		.def("change_state", &SGameObject::ChangeState, "Changes the current state to the specified state")
		// .def("get_player", &SGameObject::GetPlayer<CPlayer>, "Returns the currently registered player object")
		.def("load_texture", &SGameObject::LoadTexture, "Loads the specified texture into the manager with the specified ID");

	py::class_<CObjectParams>(m, "ObjectParams", "A container used to initialise objects")
		.def(py::init<float, float>(), "Used for certain objects that just need to be spawned")
		.def(py::init<float, float, const char*>(), "Used for objects that use scripts")
		.def(py::init<float, float, int, int, std::string, int, int, int, int, int, std::string, std::string, std::string>()
			, "Used for most object initialisation");

	py::class_<CWarspiteUtil>(m, "util", "Commonly used methods that get reused throughout the codebase")
		.def("get_file_ext", CWarspiteUtil::GetFileExtenstion, "Returns the file extension from the provided path")
		.def("get_file_name", CWarspiteUtil::GetFileName, "Returns the filename from the provided path")
		.def("read_all_text", CWarspiteUtil::ReadAllText, "Returns the text from the path provided");

	py::class_<CEngineFileSystem> fs(m, "fs", "Filesystem methods that help locating files and commonly used IO");
	fs.def("resolve_path", CEngineFileSystem::ResolvePath, "Returns a path depending on the resource type");

	py::enum_<CEngineFileSystem::EPathType>(fs, "PathType")
		.value("NONE", CEngineFileSystem::EPathType::NONE)
		.value("TEXTURE", CEngineFileSystem::EPathType::TEXTURE)
		.value("MAP", CEngineFileSystem::EPathType::MAP)
		.value("SCRIPT", CEngineFileSystem::EPathType::SCRIPT)
		.value("TILESET", CEngineFileSystem::EPathType::TILESET)
		.value("STATE", CEngineFileSystem::EPathType::STATE)
		.value("SOUND", CEngineFileSystem::EPathType::SOUND)

		.export_values();	
	
	py::class_<SKeyScancodes> sc(m, "WS_Scancode", "A wrapper for specifying some of SDL_Scancode. (For use with engine.inputh methods)");
	
	py::enum_<SDL_Scancode>(sc, "SDL_Scancode")
		.value("UNKNOWN", SDL_SCANCODE_UNKNOWN)
	
		.value("A", SDL_SCANCODE_A)
		.value("B", SDL_SCANCODE_B)
		.value("C", SDL_SCANCODE_C)
		.value("D", SDL_SCANCODE_D)
		.value("E", SDL_SCANCODE_E)
		.value("F", SDL_SCANCODE_F)
		.value("G", SDL_SCANCODE_G)
		.value("H", SDL_SCANCODE_H)
		.value("I", SDL_SCANCODE_I)
		.value("J", SDL_SCANCODE_J)
		.value("K", SDL_SCANCODE_K)
		.value("L", SDL_SCANCODE_L)
		.value("M", SDL_SCANCODE_M)
		.value("N", SDL_SCANCODE_N)
		.value("O", SDL_SCANCODE_O)
		.value("P", SDL_SCANCODE_P)
		.value("Q", SDL_SCANCODE_Q)
		.value("R", SDL_SCANCODE_R)
		.value("S", SDL_SCANCODE_S)
		.value("T", SDL_SCANCODE_T)
		.value("U", SDL_SCANCODE_U)
		.value("V", SDL_SCANCODE_V)
		.value("W", SDL_SCANCODE_W)
		.value("X", SDL_SCANCODE_X)
		.value("Y", SDL_SCANCODE_Y)
		.value("Z", SDL_SCANCODE_Z)

		.value("0", SDL_SCANCODE_0)
		.value("1", SDL_SCANCODE_1)
		.value("2", SDL_SCANCODE_2)
		.value("3", SDL_SCANCODE_3)
		.value("4", SDL_SCANCODE_4)
		.value("5", SDL_SCANCODE_5)
		.value("6", SDL_SCANCODE_6)
		.value("7", SDL_SCANCODE_7)
		.value("8", SDL_SCANCODE_8)
		.value("9", SDL_SCANCODE_9)

		.value("RETURN", SDL_SCANCODE_RETURN)
		.value("ESCAPE", SDL_SCANCODE_ESCAPE)
		.value("BACKSPACE", SDL_SCANCODE_BACKSPACE)
		.value("TAB", SDL_SCANCODE_TAB)
		.value("SPACE", SDL_SCANCODE_SPACE)

		.value("MINUS", SDL_SCANCODE_MINUS)
		.value("EQUALS", SDL_SCANCODE_EQUALS)
		.value("LEFTBRACKET", SDL_SCANCODE_LEFTBRACKET)
		.value("RIGHTBRACKET", SDL_SCANCODE_RIGHTBRACKET)
		.value("BACKSLASH", SDL_SCANCODE_BACKSLASH)
		.value("NONUSHASH", SDL_SCANCODE_NONUSHASH)

		.value("SEMICOLON", SDL_SCANCODE_SEMICOLON)
		.value("APOSTROPHE", SDL_SCANCODE_APOSTROPHE)
		.value("GRAVE", SDL_SCANCODE_GRAVE)
		.value("COMMA", SDL_SCANCODE_COMMA)
		.value("PERIOD", SDL_SCANCODE_PERIOD)
		.value("SLASH", SDL_SCANCODE_SLASH)
		.value("CAPSLOCK", SDL_SCANCODE_CAPSLOCK)

		.value("F1", SDL_SCANCODE_F1)
		.value("F2", SDL_SCANCODE_F2)
		.value("F3", SDL_SCANCODE_F3)
		.value("F4", SDL_SCANCODE_F4)
		.value("F5", SDL_SCANCODE_F5)
		.value("F6", SDL_SCANCODE_F6)
		.value("F7", SDL_SCANCODE_F7)
		.value("F8", SDL_SCANCODE_F8)
		.value("F9", SDL_SCANCODE_F9)
		.value("F10", SDL_SCANCODE_F10)
		.value("F11", SDL_SCANCODE_F11)
		.value("F12", SDL_SCANCODE_F12)

		.value("PRINTSCREEN", SDL_SCANCODE_PRINTSCREEN)
		.value("SCROLLLOCK", SDL_SCANCODE_SCROLLLOCK)
		.value("PAUSE", SDL_SCANCODE_PAUSE)
		.value("INSERT", SDL_SCANCODE_INSERT)

		.value("HOME", SDL_SCANCODE_HOME)
		.value("PAGEUP", SDL_SCANCODE_PAGEUP)
		.value("DELETE", SDL_SCANCODE_DELETE)
		.value("END", SDL_SCANCODE_END)
		.value("PAGEDOWN", SDL_SCANCODE_PAGEDOWN)
		.value("RIGHT", SDL_SCANCODE_RIGHT)
		.value("LEFT", SDL_SCANCODE_LEFT)
		.value("DOWN", SDL_SCANCODE_DOWN)
		.value("UP", SDL_SCANCODE_UP)

		.value("NUMLOCKCLEAR", SDL_SCANCODE_NUMLOCKCLEAR)

		.value("KP_DIVIDE", SDL_SCANCODE_KP_DIVIDE)
		.value("KP_MULTIPLY", SDL_SCANCODE_KP_MULTIPLY)
		.value("KP_MINUS", SDL_SCANCODE_KP_MINUS)
		.value("KP_PLUS", SDL_SCANCODE_KP_PLUS)
		.value("KP_ENTER", SDL_SCANCODE_KP_ENTER)
		.value("KP_1", SDL_SCANCODE_KP_1)
		.value("KP_2", SDL_SCANCODE_KP_2)
		.value("KP_3", SDL_SCANCODE_KP_3)
		.value("KP_4", SDL_SCANCODE_KP_4)
		.value("KP_5", SDL_SCANCODE_KP_5)
		.value("KP_6", SDL_SCANCODE_KP_6)
		.value("KP_7", SDL_SCANCODE_KP_7)
		.value("KP_8", SDL_SCANCODE_KP_8)
		.value("KP_9", SDL_SCANCODE_KP_9)
		.value("KP_0", SDL_SCANCODE_KP_0)
		.value("KP_PERIOD", SDL_SCANCODE_KP_PERIOD)
		.value("NONUSBACKSLASH", SDL_SCANCODE_NONUSBACKSLASH)

		.value("LCTRL", SDL_SCANCODE_LCTRL)
		.value("LSHIFT", SDL_SCANCODE_LSHIFT)
		.value("LALT", SDL_SCANCODE_LALT)
		.value("LGUI", SDL_SCANCODE_LGUI)
		.value("RCTRL", SDL_SCANCODE_RCTRL)
		.value("RSHIFT", SDL_SCANCODE_RSHIFT)
		.value("RALT", SDL_SCANCODE_RALT)
		.value("RGUI", SDL_SCANCODE_RGUI)
	
		.export_values();
}

CScriptManager* CScriptManager::s_pInstance = 0;

CScriptManager::CScriptManager()
{
	std::cout << "Initialising ScriptManager..." << std::endl;
	
	try 
	{
		// Initialize Python interpreter and import bar module
		PyImport_AppendInittab("engine", PyInit_engine);
		py::initialize_interpreter();
		
		main_module = py::module::import("__main__");
		engine_module = py::module::import("engine");
		main_namespace = main_module.attr("__dict__");
	}
	catch (pybind11::error_already_set const& e)
	{
		std::cerr << "***AN ERROR OCCURRED DURING INITIALISATION OF PYTHON!***" << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << "***************************END**************************" << std::endl;
	}

	// Show that the ScriptManager is ready
	SGameScript* test = SGameScript::source("test", "import sys\nprint(\"Using Python Runtime %s.%s.%s\" % (sys.version_info.major, sys.version_info.minor, sys.version_info.micro))\nprint(\"Script Manager is ready!\")");

	Run(test);
}

void CScriptManager::Destroy()
{
	// Destroy our interpreter
	py::finalize_interpreter();
}

void CScriptManager::Load(SGameScript* script)
{
	loadedScripts[script->GetScriptName()] = script;
}

bool CScriptManager::Remove(const char* scriptRef)
{
	auto search = loadedScripts.find(scriptRef);
	if (search != loadedScripts.end())
	{
		loadedScripts.erase(search);
		return true;
	}
	return false;
}

void CScriptManager::RemoveAll()
{
	loadedScripts.clear();
}

bool CScriptManager::Run(SGameScript* script, py::object* ns)
{
	try
	{
		// Use the namespace provided (if there is one)
		switch (script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			py::exec(script->GetSource().c_str(), ns != nullptr ? *ns : main_namespace);
			break;
		case EGameScriptType::SCRIPT_FILE:
			py::eval_file(script->GetFilename().c_str(), ns != nullptr ? *ns : main_namespace);
			break;
		default:
			return false; // No type defined? what?
		}
		
		return true;
	}
	catch(py::error_already_set const & e)
	{
		switch(script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			std::cerr << "An internal error occurred when executing an inline script named: \"" + script->GetScriptName() + "\"" << std::endl;
			break;
		case EGameScriptType::SCRIPT_FILE:
			std::cerr << "An internal error occurred when executing an external script named: \"" + script->GetScriptName() + "\"" << std::endl;
			break;
		default:
			std::cerr << "An internal error occurred when executing script named: \"" + script->GetScriptName() + "\"" << std::endl;
		}

		// Print what happened to not make debugging hell.
		std::cerr << "Error:\n " << e.what() << std::endl;
		std::cerr << "***Error End***" << std::endl;
	}
	return false;
}

bool CScriptManager::RunFromRef(std::string scriptRef, py::object* ns)
{
	return Run(loadedScripts[scriptRef], ns);
}