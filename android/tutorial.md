PART 1:
we can use MT Manager for example a cracked version of it obv,
we click the roblox apk -> View -> hold click classes2.dex -> dex editor plus -> find com->roblox->client->ActivityNativeMain and click it to open it and then -> find OnCreate that also has .registers 13 aka this

.method public onCreate(Landroid/os/Bundle;)V
     .registers 13
.

and to load ur own library u would do:

const-string v4, "test"

invoke-static {v4}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

the "test" stands for libtest.so which is placed in roblox -> view -> lib folder and either arm64-v8a or x86_64 depending on what ur emulator is, 
what is required to make our own library .so:
installing android studio -> new project -> native cpp -> in the cpp folder theres a cpp main native-lib.cpp file that will be the main file for everything,
ill add printsploit tut in part 2 soon,

to find offsets we go to roblox -> View -> lib -> arm64-v8a/x86_64 -> libroblox.so put it in a shared folder with the pc and the emulator, we open it in ida and we in the "load a new file" popup we choose:
fill segment gaps, manual load, then click the processor settings and uncheck simplify intrusctions and disable bl jumps, them edit arm architechture and choose ur arm64-v8a or the one u have etc then let it analyze fully, 
the offset strings are down below:

Print = "Current asset id is %lld"
ScriptContextResume = "$Script" pretty straight forward actually
KTable = "Trying to call method on object of type: `%s` with incorrect arguments." its a qword_xxxxxxx

Luau_Execute = "C stack overflow" 4th xref takes only 1 arg
LuaO_NilObject = "Attempt to migrate WeakObjectRef across VM boundary" go inside the last return its an unk_xxxxxxx

taskschedulerpointer = "Out of arbiter nodes: Increase the FInt::TaskSchedulerMaxNumOfArbiters value" first qword
fireproximityprompt = "ProximityPrompt_Triggered" xref + string
getlspointer = "GetStyled(): Property %s is write-only" figure it out urself
requirebypass = "Cannot require a non-RobloxScript module from a RobloxScript" figure it out urself
loadsafe = "%s: bytecode version mismatch (expected [%d..%d], got %d)" figure it out urself 
tasksynchronize = "task.synchronize() may only be called from a script that is a descendant of an Actor"
taskwait = "task.wait is not available for AuroraScripts"
taskdesynchronize = "task.desynchronize() may only be called from a script that is a descendant of an Actor",
gameloaded = "onGameLoaded() SessionReporterState_GameLoaded placeId:%lld" string,

for example the gameloaded is:
__int64 __fastcall sub_2290F9C(__int64 a1, __int64 a2) // gameloaded offset
{
  __int64 v3; // x0
  __int64 v4; // x20
  __int64 v5; // x21
  __int64 v6; // x0
  _BYTE v8[8]; // [xsp+8h] [xbp-18h] BYREF
  __int64 v9; // [xsp+10h] [xbp-10h]

  v3 = sub_5FE11A4(v8);
  v4 = v9;
  v5 = sub_228FF74(v3);
  v6 = sub_2291054();
  sub_21854D4(v4, v5, v6, a2);
  if ( a2 )
    __android_log_print(3, "rbx.jni", "onGameLoaded() SessionReporterState_GameLoaded placeId:%lld", a2);
  return sub_5FE129C(v8);
}

so it would be 
const uintptr_t ongameloaded = memory::getaddr(0x2290F9C);, // this is old NOT updated,

the memory i use is in the same folder as this in memory.hpp
