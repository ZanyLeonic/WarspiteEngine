#include "ScriptWrappers.h"

#include <iostream>
#include <pybind11/embed.h>

#include "Camera.h"
#include "InputHandler.h"
#include "Level.h"
#include "GameStateBase.h"
#include "GameObjectDictionary.h"
#include "PlayState.h"
#include "ObjectLayer.h"
#include "TextureManager.h"
#include "WarspiteObject.h"
#include "WarspiteUtil.h"
#include "GameStateDictionary.h"
#include "GameStateManager.h"
#include "Game.h"

// Exposing some classes to Python
PYBIND11_MODULE(engine, m) {
	// define these so they can be assigned later.
	m.attr(LEVELOBJECT_NAME) = py::none();
	m.attr(CAMERAOBJECT_NAME) = py::none();
	m.attr(INPUTOBJECT_NAME) = py::none();
	m.attr(GAMEOBJECT_NAME) = py::none();

	py::class_<CVector2D>(m, "Vector2D", "A two dimensional vector object")
		.def(py::init<>())
		.def(py::init<float, float>())
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

	py::class_<SWarObject>(m, "WarObject", "An object that exists as the base for the majority of GameObjects in levels")
		.def(py::init<CWarspiteObject*>())
		.def("get_position", &SWarObject::GetPosition, "Returns the position of the specified object")
		.def("set_position", &SWarObject::SetPosition, "Sets the position of the specified object")
		.def("get_type", &SWarObject::GetFactoryID, "Returns the factory ID of the object")
		.def("get_name", &SWarObject::GetName, "Returns the name of the object")
		.def("get_texid", &SWarObject::GetTextureID, "Returns the texture ID associated with the object")
		.def("get_velocity", &SWarObject::GetVelocity, "Returns the current velocity")
		.def("set_velocity", &SWarObject::SetVelocity, "Sets the current velocity")
		.def("add_velocity", &SWarObject::AddVelocity, "Adds the specified velocity to the current")
		.def("get_acceleration", &SWarObject::GetAcceleration, "Returns the current acceleration")
		.def("set_acceleration", &SWarObject::SetAcceleration, "Sets the current acceleration")
		.def("add_acceleration", &SWarObject::AddAcceleration, "Adds to the current acceleration")
		.def("get_size", &SWarObject::GetSize, "Returns a Vector2D with the dimensions of the object")
		.def("get_current_anim_row", &SWarObject::GetCurrentAnimRow, "Returns the current row of animation on the sprite sheet the object is on")
		.def("set_anim_row", &SWarObject::SetAnimRow, "Sets the current row of animation on the sprite sheet the object is on")
		.def("get_current_anim_frame", &SWarObject::GetCurrentAnimFrame, "Returns the current frame of animation the object is on")
		.def("set_anim_frame", &SWarObject::SetAnimFrame, "Sets the current animation frame")
		.def("get_total_anim_frames", &SWarObject::GetTotalAnimFrames, "Returns the total amount of animation frames the object has")
		.def("should_collide", &SWarObject::ShouldCollide, "Returns whether the object should collide with other objects")
		.def("set_collision", &SWarObject::SetCollision, "Sets whether this object should collide with other objects")
		.def("should_overlap", &SWarObject::ShouldOverlap, "Returns whether the object should overlap with other objects")
		.def("set_overlap", &SWarObject::SetOverlap, "Sets whether this object should overlap with other objects")
		.def("__repr__", [](SWarObject& o)
			{
				return "<SWarObject \"" + (std::string(o.GetName()).empty() ? "UNDEFINED" : std::string(o.GetName())) +
					"\" of type \"" + (o.GetFactoryID().empty() ? "UNDEFINED" : o.GetFactoryID()) + "\">";
			}
	);

	py::class_<SWarState>(m, "GameState", "An object that supplies custom functionality for the Engine depending on scenario")
		.def("get_loaded_textures", &SWarState::GetLoadedTextures, "Returns a vector of loaded texture IDs related to the state")
		.def("get_loaded_scripts", &SWarState::GetLoadedScripts, "Returns a vector of loaded script IDs related to the state")
		.def("should_be_ticking", &SWarState::ShouldBeTicking, "Returns whether the state should be ticking or not")
		.def("should_be_drawing", &SWarState::ShouldBeDrawing, "Returns whether the state should be drawing or not");

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
		.def("get_player", &SGameObject::GetPlayer, "Returns the currently registered player object")
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

bool CScriptWrappers::Init_Engine()
{
	try
	{
		// Import the engine module
		PyImport_AppendInittab("engine", PyInit_engine);
		
		return true;
	}
	catch (pybind11::error_already_set const& e)
	{
		std::cerr << "***AN ERROR OCCURRED DURING INITIALISATION OF PYTHON!***" << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << "***************************END**************************" << std::endl;
	}
	return false;
}

CVector2D SWarObject::GetPosition() const
{
	if (!IsValid()) return CVector2D();
	return m_inst->GetPosition();
}

void SWarObject::SetPosition(CVector2D& nPos) const
{
	if (!IsValid()) return;
	m_inst->SetPosition(nPos);
}

CVector2D SWarObject::GetVelocity() const
{
	if (!IsValid()) return CVector2D();
	return m_inst->GetVelocity();
}

void SWarObject::SetVelocity(CVector2D nV) const
{
	if (!IsValid()) return;
	m_inst->SetVelocity(nV);
}

void SWarObject::AddVelocity(CVector2D aV) const
{
	if (!IsValid()) return;
	m_inst->AddVelocity(aV);
}

CVector2D SWarObject::GetAcceleration() const
{
	if (!IsValid()) return CVector2D();
	return m_inst->GetAcceleration();
}

void SWarObject::SetAcceleration(CVector2D nV) const
{
	if (!IsValid()) return;
	m_inst->SetAcceleration(nV);
}

void SWarObject::AddAcceleration(CVector2D aV) const
{
	if (!IsValid()) return;
	m_inst->AddAcceleration(aV);
}

CVector2D SWarObject::GetSize() const
{
	if (!IsValid()) return CVector2D();
	return m_inst->GetSize();
}

int SWarObject::GetCurrentAnimRow() const
{
	if (!IsValid()) return 0;
	return m_inst->GetCurrentAnimRow();
}

void SWarObject::SetAnimRow(int nR) const
{
	if (!IsValid()) return;
	m_inst->SetAnimRow(nR);
}

int SWarObject::GetCurrentAnimFrame() const
{
	if (!IsValid()) return 0;
	return m_inst->GetCurrentAnimFrame();
}

void SWarObject::SetAnimFrame(int nR) const
{
	if (!IsValid()) return;
	m_inst->SetAnimFrame(nR);
}

int SWarObject::GetTotalAnimFrames() const
{
	if (!IsValid()) return 0;
	return m_inst->GetTotalAnimFrames();
}

const char* SWarObject::GetName() const
{
	if (!IsValid()) return "";
	return m_inst->GetName();
}

std::string SWarObject::GetTextureID() const
{
	if (!IsValid()) return "";
	return m_inst->GetTextureID();
}

std::string SWarObject::GetFactoryID() const
{
	if (!IsValid()) return "";
	return m_inst->GetFactoryID();
}

bool SWarObject::ShouldCollide() const
{
	if (!IsValid()) return false;
	return m_inst->ShouldCollide();
}

void SWarObject::SetCollision(bool nC) const
{
	if (!IsValid()) return;
	m_inst->SetCollision(nC);
}

bool SWarObject::ShouldOverlap() const
{
	if (!IsValid()) return false;
	return m_inst->ShouldOverlap();
}

void SWarObject::SetOverlap(bool nO) const
{
	if (!IsValid()) return;
	m_inst->SetOverlap(nO);
}

std::vector<std::string> SWarState::GetLoadedTextures() const
{
	if (!IsValid()) return std::vector<std::string>();
	return m_inst->GetLoadedTextures();
}

std::vector<std::string> SWarState::GetLoadedScripts() const
{
	if (!IsValid()) return std::vector<std::string>();
	return m_inst->GetLoadedScripts();
}

bool SWarState::ShouldBeTicking() const
{
	if (!IsValid()) return false;
	return m_inst->ShouldBeTicking();
}

bool SWarState::ShouldBeDrawing() const
{
	if (!IsValid()) return false;
	return m_inst->ShouldBeDrawing();
}

std::string SLevelObject::GetName() const
{
	if (!IsValid()) return "";

	return m_inst->GetName();
}

CVector2D SLevelObject::GetLevelSize() const
{
	if (!IsValid()) return CVector2D(0, 0);

	return m_inst->m_LevelSize;
}

std::unique_ptr<SWarObject> SLevelObject::CreateObject(std::string factID, CObjectParams params) const
{
	if (!IsValid()) nullptr;
	// Create the object and load the params provided.
	IGameObject* pObj = CGameObjectDictionary::Instance()->Create(factID);
	if (pObj == nullptr) return nullptr; // a bit redundant but it prevents a reference of a nullptr

	pObj->Load(&params);

	std::vector<IGameObject*>* pSL = m_inst->GetScriptLayer()->GetGameObjects();

	pSL->push_back(pObj);
	pObj->OnPlay();

	auto* pNew = new SWarObject(dynamic_cast<CWarspiteObject*>(pObj));
	if (pNew == nullptr) return nullptr;

	// lul will this work? nope.
	return std::unique_ptr<SWarObject>(pNew);
}

std::unique_ptr<SWarObject> SLevelObject::FindGameObject(std::string id)
{
	if (!IsValid()) return nullptr;

	IGameObject* found = CWarspiteUtil::FindGameObject(m_inst, id);

	if (found)
	{
		auto* pNew = new SWarObject(dynamic_cast<CWarspiteObject*>(found));
		if (pNew == nullptr) return nullptr;

		return std::unique_ptr<SWarObject>(pNew);
	}

	return nullptr;
}

std::vector<std::vector<IGameObject*>*> SLevelObject::GetGameObjects() const
{
	if (!IsValid()) return std::vector<std::vector<IGameObject*>*>();

	return m_inst->GetGameObjects();
}

CVector2D SCameraObject::GetPosition() const
{
	if (!IsValid()) return CVector2D(0, 0);

	return m_inst->GetPosition();
}

CVector2D SCameraObject::GetOffsetedPosition() const
{
	if (!IsValid()) return CVector2D(0, 0);

	return m_inst->GetPositionT();
}

CVector2D* SCameraObject::GetTarget() const
{
	if (!IsValid()) return nullptr;

	return m_inst->GetTarget();
}

bool SCameraObject::SetPosition(CVector2D pPos) const
{
	if (!IsValid()) return false;

	m_inst->SetPosition(pPos);
	return true;
}

bool SCameraObject::SetTarget(CVector2D* pTar) const
{
	if (!IsValid()) return false;

	m_inst->SetTarget(pTar);
	return true;
}

CVector2D SCameraObject::GetLevelSize() const
{
	if (!IsValid()) return CVector2D();

	return m_inst->GetLevelSize();
}

bool SInputObject::GetButtonState(int joy, int buttonNumber) const
{
	if (!IsValid()) return false;
	return m_inst->GetButtonState(joy, buttonNumber);
}

bool SInputObject::GetMouseButtonState(int buttonNumber) const
{
	if (!IsValid()) return false;
	return m_inst->GetMouseButtonState(buttonNumber);
}

bool SInputObject::IsKeyDown(SDL_Scancode key) const
{
	if (!IsValid()) return false;
	return m_inst->IsKeyDown(key);
}

float SInputObject::GetXAxis(int joy, int stick) const
{
	if (!IsValid()) return 0.f;
	return m_inst->GetXAxis(joy, stick);
}

float SInputObject::GetYAxis(int joy, int stick) const
{
	if (!IsValid()) return 0.f;
	return m_inst->GetYAxis(joy, stick);
}

bool SInputObject::SetReleaseState(SDL_Scancode key, bool state) const
{
	if (!IsValid()) return false;
	m_inst->SetReleaseState(key, state);
	return true;
}

bool SInputObject::AddActionKeyDown(SDL_Scancode key, const KeyCallback callBack) const
{
	if (!IsValid()) return false;
	m_inst->AddActionKeyDown(key, callBack);
	return true;
}

bool SInputObject::AddActionKeyUp(SDL_Scancode key, const KeyCallback callBack) const
{
	if (!IsValid()) return false;
	m_inst->AddActionKeyUp(key, callBack);
	return true;
}

std::unique_ptr<SWarState> SGameObject::GetCurrentState() const
{
	if (!IsValid()) return nullptr;
	return std::unique_ptr<SWarState>(new SWarState(m_inst->GetStateManager()->GetCurrentState()));
}

bool SGameObject::ChangeState(std::string stateID) const
{
	if (m_inst->GetStateManager()->GetCurrentState()->GetStateID() != stateID)
	{
		m_inst->GetStateManager()->ModifyState(CGameStateDictionary::Instance()->Create(stateID));
		return true;
	}
	return false;
}

std::unique_ptr<SWarObject> SGameObject::GetPlayer() const
{
	if (m_inst->GetPlayer() == nullptr) return nullptr;

	CPlayState* pState = dynamic_cast<CPlayState*>(m_inst->GetStateManager()->GetCurrentState());
	if (!pState) return nullptr;
	
	IGameObject* found = CWarspiteUtil::FindGameObject(pState->GetLoadedLevel(), "Player");

	if (found)
	{
		auto* pNew = new SWarObject(dynamic_cast<CWarspiteObject*>(found));
		if (pNew == nullptr) return nullptr;

		return std::unique_ptr<SWarObject>(pNew);
	}

	return nullptr;
}

bool SGameObject::LoadTexture(std::string texPath, std::string texID) const
{
	if (!IsValid()) return false;
	return CTextureManager::Instance()->Load(texPath, texID, m_inst->GetRenderer());
}
