#include "../include/main.hpp"

using namespace il2cpp_utils;
 
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
 
Color c{
    c.r = 0.0f,
    c.g = 0.0f,
    c.g = 0.0f,
    c.a = 0.5f
};
 
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
    string Menu = "MenuViewControllers";
    string EmptyTransition = "EmptyTransition";

    Il2CppString* previousScene = *RunMethod<Il2CppString*>("UnityEngine.SceneManagement", "Scene", "GetNameInternal", previousActiveScene.m_Handle);
    if(previousScene == nullptr)
        return;
    
    string previousSceneStr  = to_utf8(csstrtostr(previousScene));
    Il2CppString* activeScene = *RunMethod<Il2CppString*>("UnityEngine.SceneManagement", "Scene", "GetNameInternal", newActiveScene.m_Handle);
    if(activeScene == nullptr)
        return;
    
    string activeSceneStr  = to_utf8(csstrtostr(activeScene));

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
    if(MenuSceneLoadedFresh == false)
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
   
}
 
extern "C" void load() {
    log(INFO, "Hello from il2cpp_init!");
    log(INFO, "Installing hooks...");
    INSTALL_HOOK_OFFSETLESS(HandleMainMenuViewControllerDidFinish, FindMethodUnsafe("", "MainFlowCoordinator", "HandleMainMenuViewControllerDidFinish", 2));
    INSTALL_HOOK_OFFSETLESS(Internal_ActiveSceneChanged, FindMethodUnsafe("UnityEngine.SceneManagement","SceneManager","Internal_ActiveSceneChanged", 2));
    log(INFO, "Installed all hooks!");
}