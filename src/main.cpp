#include <unordered_set>

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

using namespace il2cpp_utils;

static ModInfo modInfo;
bool MenuSceneLoadedFresh = false;
 
Vector3 coverPos;
Vector3 coverScale = {1.0f, 1.0f, 1.0f};
Vector2 sizeDelta = {11.0f, 11.0f};
Vector2 anchorMin = {0.0f, 0.0f};
Vector2 anchorMax = {1.0f, 1.0f};
Vector2 anchoredPosition = {1.5f, 0.0f};
Rect uvRect = {0.0f, 0.0f, 1.0f, 1.0f};
Vector3 localPosition = {-1.0f, -52.0f};
Vector3 newlocalpos = {0.0f, -10.0f};

 
Il2CppObject* FindObjectsOfTypeAllFirstOrDefault(Il2CppReflectionType* Type)
{
    Array<Il2CppObject*>* Objects = *RunMethod<Array<Il2CppObject*>*>(GetClassFromName("UnityEngine","Resources"), "FindObjectsOfTypeAll", Type);
    if(Objects)
        return Objects->values[0];
    else
        return nullptr;
}

MAKE_HOOK_OFFSETLESS(Internal_ActiveSceneChanged, void, Scene previousActiveScene, Scene newActiveScene)
{  
    Internal_ActiveSceneChanged(previousActiveScene, newActiveScene);
    std::string Menu = "MenuViewControllers";
    std::string EmptyTransition = "EmptyTransition";
 
    Il2CppString* previousScene = *RunMethod<Il2CppString*>("UnityEngine.SceneManagement", "Scene", "GetNameInternal", previousActiveScene.m_Handle);
    if(previousScene == nullptr)
        return;
   
    std::string previousSceneStr  = to_utf8(csstrtostr(previousScene));
    Il2CppString* activeScene = *RunMethod<Il2CppString*>("UnityEngine.SceneManagement", "Scene", "GetNameInternal", newActiveScene.m_Handle);
    if(activeScene == nullptr)
        return;
   
    std::string activeSceneStr  = to_utf8(csstrtostr(activeScene));
 
    if(activeSceneStr == Menu)
    {
        if(previousSceneStr == EmptyTransition)
        {
            MenuSceneLoadedFresh = true;
        } else
        {
            MenuSceneLoadedFresh = false;
        }
    }
   
}
 
MAKE_HOOK_OFFSETLESS(HandleMainMenuViewControllerDidFinish, void, Il2CppObject* self, Il2CppObject* viewController, int subMenuType){
   
    Il2CppObject* sldv = FindObjectsOfTypeAllFirstOrDefault(GetSystemType("", "StandardLevelDetailView"));
 

    HandleMainMenuViewControllerDidFinish(self, viewController, subMenuType);
    if(!MenuSceneLoadedFresh)
        return;

    Il2CppObject* cover = *GetFieldValue(sldv, "_coverImage");
    Il2CppObject* bdscc = *GetFieldValue(sldv, "_beatmapDifficultySegmentedControlController");
   
    Il2CppObject* coverTransform = *RunMethod(cover, "get_transform");
    Il2CppObject* parent = *RunMethod(coverTransform, "get_parent");

    Il2CppObject* playerStatsContainer = *GetFieldValue(sldv, "_playerStatsContainer");
    Il2CppObject* playerStatsContainerLayout = *RunMethod(playerStatsContainer, "GetComponent", GetSystemType("UnityEngine.UI", "LayoutElement"));
   
    Il2CppObject* bdsccTransform = *RunMethod(bdscc, "get_transform");
    Il2CppObject* playContainer = *RunMethod(bdsccTransform, "get_parent");
   
    Il2CppObject* parentLayout = *RunMethod(parent, "GetComponent", GetSystemType("UnityEngine.UI", "LayoutElement"));
   
    Vector3 coverTransformPos = *RunMethod<Vector3>(coverTransform, "get_localPosition");
   
    coverPos = {0, 0, coverTransformPos.z};
   
    RunMethod(coverTransform, "set_localPosition", coverPos);
   
    RunMethod(coverTransform, "set_localScale", coverScale);
   
    Il2CppObject* coverRect = *RunMethod(cover, "get_rectTransform");
   
    RunMethod(coverRect, "set_sizeDelta", sizeDelta);
   
    RunMethod(coverRect, "set_anchorMin", anchorMin);
   
    RunMethod(coverRect, "set_anchorMax", anchorMax);
   
    RunMethod(coverRect, "set_anchoredPosition", anchoredPosition);
   
    RunMethod(cover, "set_uvRect", uvRect);
   
    RunMethod(playContainer, "SetParent", parent);
   
    RunMethod(playContainer, "set_localPosition", localPosition);
   
    Il2CppObject* image = *RunMethod(playContainer, "GetComponent", GetSystemType("UnityEngine.UI", "Image"));
   
    RunMethod(image, "set_enabled", false);
   
    Il2CppObject* playerStatsContainerLayoutTransform = *RunMethod(playerStatsContainerLayout, "get_transform");
   
    RunMethod(playerStatsContainerLayoutTransform, "SetParent", parent);
   
    RunMethod(playerStatsContainerLayoutTransform, "set_localPosition", newlocalpos);
   
    RunMethod(parentLayout, "set_preferredHeight", 60.0f);
    MenuSceneLoadedFresh = false;
}

extern "C" void setup(ModInfo& info) {
    info.id = "SongCoverExpander";
    info.version = "0.1.1";
    modInfo = info;
}
 
extern "C" void load() {
    il2cpp_functions::Init();
    INSTALL_HOOK_OFFSETLESS(HandleMainMenuViewControllerDidFinish, il2cpp_utils::FindMethodUnsafe("", "MainFlowCoordinator", "HandleMainMenuViewControllerDidFinish", 2));
    INSTALL_HOOK_OFFSETLESS(Internal_ActiveSceneChanged, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement","SceneManager","Internal_ActiveSceneChanged", 2));
}
