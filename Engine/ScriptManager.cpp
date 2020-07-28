#include "ScriptManager.h"
#include <iostream>
#include <pybind11/embed.h>

#include "ScriptWrappers.h"
#include "WarspiteObject.h"
#include "WarspiteUtil.h"

CScriptManager* CScriptManager::s_pInstance = 0;

PYBIND11_MAKE_OPAQUE(std::vector<std::vector<IGameObject*>*>&);
PYBIND11_MAKE_OPAQUE(std::vector<IGameObject*>&);

// Exposing some classes to Python
PYBIND11_MODULE(engine, m) {
	//py::class_<std::vector<std::vector<IGameObject*>*>>(m, "GOVector")
	//	.def(py::init<>())
	//	.def("__iter__", [](std::vector<std::vector<IGameObject*>*>& v)
	//		{
	//			return py::make_iterator(v.begin(), v.end());
	//		}, py::keep_alive<0, 1>()
	//);

	//py::class_<std::vector<IGameObject*>*>(m, "InGOVector")
	//	.def(py::init<>())
	//	.def("__iter__", [](std::vector<IGameObject*>& v)
	//		{
	//			return py::make_iterator(v.begin(), v.end());
	//		}, py::keep_alive<0, 1>()
	//);

	//py::class_<IGameObject, std::unique_ptr<IGameObject, py::nodelete>>(m, "GameObject")
	//	.def(py::init<>())
	//	.def("get_name", &IGameObject::GetName)
	//	.def("should_collide", &IGameObject::ShouldCollide)
	//	.def("__repr__", [](IGameObject& o)
	//		{
	//			return "<IGameObject \"" + std::string(o.GetName()) + "\">";
	//		}
	//);

	py::class_<CVector2D>(m, "Vector2D")
		.def(py::init<>())
		.def("set_x", &CVector2D::SetX)
		.def("set_y", &CVector2D::SetY)
		.def("get_y", &CVector2D::GetY)
		.def("get_x", &CVector2D::GetX)
		.def("norm", &CVector2D::Normalize)
		.def("length", &CVector2D::Length)
		.def("__len__", &CVector2D::Length_i)
		.def("__repr__",[](const CVector2D &v)
			{
				return "<CVector2D with point (" + 
					std::to_string(v.GetX()) + ", "
					+ std::to_string(v.GetY()) + ")>";
			}
	);
	
	py::class_<CWarspiteObject, IGameObject>(m, "WarspiteObject")
		.def(py::init<>())
		.def("get_position", &CWarspiteObject::GetPosition)
		.def("set_position", &CWarspiteObject::SetPosition)
		.def("get_type", &CWarspiteObject::GetFactoryID)
		.def("get_texid", &CWarspiteObject::GetTextureID)
		.def("get_velocity", &CWarspiteObject::GetVelocity)
		.def("set_velocity", &CWarspiteObject::SetVelocity)
		.def("add_velocity", &CWarspiteObject::AddVelocity)
		.def("get_acceleration", &CWarspiteObject::GetAcceleration)
		.def("set_acceleration", &CWarspiteObject::SetAcceleration)
		.def("add_acceleration", &CWarspiteObject::AddAcceleration)
		.def("get_size", &CWarspiteObject::GetSize)
		.def("get_current_anim_row", &CWarspiteObject::GetCurrentAnimRow)
		.def("set_anim_row", &CWarspiteObject::SetAnimRow)
		.def("get_current_anim_frame", &CWarspiteObject::GetCurrentAnimFrame)
		.def("set_anim_row", &CWarspiteObject::SetAnimRow)
		.def("get_total_anim_frames", &CWarspiteObject::GetTotalAnimFrames)
		.def("set_collision", &CWarspiteObject::SetCollision)
		.def("__repr__", [](CWarspiteObject& o)
			{
				return "<CWarspiteObject \"" + std::string(o.GetName()) + 
					   "\" of type \"" + o.GetFactoryID() + "\">";
			}
	);
	
	py::class_<SLevelObject>(m, "LevelObject")
		.def(py::init<CLevel*>())
		.def("get_name", &SLevelObject::GetName)
		.def("get_level_size", &SLevelObject::GetLevelSize)
		//.def("find_wobject", &SLevelObject::FindGameObject<CWarspiteObject>)
		.def("__repr__", [](SLevelObject& o)
			{
				return "<SLevelObject named \"" + o.GetName() + "\">";
			}
	);

	py::class_<SCameraObject>(m, "CameraObject")
		.def(py::init<CCamera*>())
		.def("get_position", &SCameraObject::GetPosition)
		.def("get_position_with_offset", &SCameraObject::GetOffsetedPosition)
		.def("get_target", &SCameraObject::GetTarget)
		.def("set_position", &SCameraObject::SetPosition)
		.def("set_target", &SCameraObject::SetTarget)
		.def("get_level_size", &SCameraObject::GetLevelSize)
		.def("__repr__", [](SCameraObject& o)
			{
				return "<SCameraObject with target (" + 
						std::to_string(o.GetTarget()->GetX()) + ", " + 
						std::to_string(o.GetTarget()->GetY()) + 
						")>";
			}
	);

	py::class_<SInputObject>(m, "InputObject")
		.def(py::init<CInputHandler*>())
		.def("get_button_state", &SInputObject::GetButtonState)
		.def("get_mouse_button_state", &SInputObject::GetMouseButtonState)
		.def("is_keydown", &SInputObject::IsKeyDown)
		.def("get_x_axis", &SInputObject::GetXAxis)
		.def("get_y_axis", &SInputObject::GetYAxis)
		.def("set_release_state", &SInputObject::SetReleaseState)
		.def("add_action_keydown", &SInputObject::AddActionKeyDown)
		.def("add_action_keyup", &SInputObject::AddActionKeyUp
	);
}

CScriptManager::CScriptManager()
{
	std::cout << "Initialising ScriptManager..." << std::endl;

	try {
		// Initialize Python interpreter and import bar module
		PyImport_AppendInittab("engine", PyInit_engine);
		Py_Initialize();
		
		main_module = py::module::import("__main__");
		engine_module = py::module::import("engine");
		main_namespace = main_module.attr("__dict__");
	}
	catch (pybind11::error_already_set const&)
	{
		std::cout << "***AN ERROR OCCURRED DURING INITIALISATION OF PYTHON!***" << std::endl;
		PyErr_Print();
		std::cout << "***************************END**************************" << std::endl;
	}

	// Show that the ScriptManager is ready
	SGameScript* test = SGameScript::source("test", "import sys\nprint(\"Using Python Runtime %s.%s.%s\" % (sys.version_info.major, sys.version_info.minor, sys.version_info.micro))\nprint(\"Script Manager is ready!\")");

	Run(test);
}

void CScriptManager::Destroy()
{
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

bool CScriptManager::Run(SGameScript* script)
{
	try
	{
		// Use the namespace provided (if there is one)
		switch (script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			exec(script->GetSource().c_str(), main_namespace);
			break;
		case EGameScriptType::SCRIPT_FILE:
			PyRun_SimpleString(WarspiteUtil::ReadAllText(script->GetFilename()).c_str());
			break;
		default:
			return false; // No type defined? what?
		}
		
		return true;
	}
	catch(py::error_already_set const &)
	{
		switch(script->GetScriptType())
		{
		case EGameScriptType::SCRIPT_INLINE:
			std::cout << "An internal error occurred when executing an inline script named: \"" + script->GetScriptName() + "\"" << std::endl;
			PyErr_Print(); // print all errors for now
			break;
		case EGameScriptType::SCRIPT_FILE:
			std::cout << "An internal error occurred when executing an external script named: \"" + script->GetScriptName() + "\"" << std::endl;
			PyErr_Print(); // print all errors for now
			break;
		default:
			std::cout << "An internal error occurred when executing script named: \"" + script->GetScriptName() + "\"" << std::endl;
		}
	}
	return false;
}

bool CScriptManager::RunFromRef(std::string scriptRef)
{
	return Run(loadedScripts[scriptRef]);
}