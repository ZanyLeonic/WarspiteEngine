#include "ScriptManager.h"
#include <iostream>
#include <pybind11/embed.h>

#include "WarspiteObject.h"
#include "WarspiteUtil.h"

CScriptManager* CScriptManager::s_pInstance = 0;

// Exposing some classes to Python
PYBIND11_MODULE(game, m) {
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

	py::class_<CWarspiteObject>(m, "WarspiteObject")
		.def(py::init<>())
		.def("get_position", &CWarspiteObject::GetPosition)
		.def("set_position", &CWarspiteObject::SetPosition)
		.def("get_type", &CWarspiteObject::GetFactoryID)
		.def("get_name", &CWarspiteObject::GetName)
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
		.def("should_collide", &CWarspiteObject::ShouldCollide)
		.def("set_collision", &CWarspiteObject::SetCollision)
		.def("__repr__", [](CWarspiteObject& o)
			{
				return "<CWarspiteObject \"" + std::string(o.GetName()) + 
					"\"of type \"" + o.GetFactoryID() + "\">";
			}
	);
}

CScriptManager::CScriptManager()
{
	std::cout << "Initialising ScriptManager..." << std::endl;

	try {
		// Initialize Python interpreter and import bar module
		PyImport_AppendInittab("game", PyInit_game);
		Py_Initialize();
		
		main_module = py::module::import("__main__");
		main_namespace = main_module.attr("__dict__");
	}
	catch(pybind11::error_already_set const&)
	{
		PyErr_Print();
	}

	// Show that the ScriptManager is ready
	SGameScript* test = SGameScript::source("test", "import sys\nprint(\"Using Python Runtime %s.%s.%s\" % (sys.version_info.major, sys.version_info.minor, sys.version_info.micro))\nprint(\"Script Manager is ready!\")");

	Run(test);
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
			PyRun_SimpleString(script->GetSource().c_str());
			break;
		case EGameScriptType::SCRIPT_FILE:
			PyRun_SimpleString(WarspiteUtil::ReadAllText(script->GetFilename()).c_str());
			break;
		default:
			return false; // No type defined? what?
		}
		
		return true;
	}
	catch(pybind11::error_already_set const &)
	{
		PyErr_Print(); // print all errors for now
	}
	return false;
}

bool CScriptManager::RunFromRef(std::string scriptRef)
{
	return Run(loadedScripts[scriptRef]);
}