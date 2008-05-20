#include "StdAfx.h"
#include "Setup.h"
#ifdef WIN32
#pragma warning(disable:4305)		// warning C4305: 'argument' : truncation from 'double' to 'float'
#pragma warning(disable:4018)		// warning C4018: '<' : signed/unsigned mismatch
#endif

    class OrbOfCommand : public GameObjectAIScript
      {
      public:
              OrbOfCommand(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                    if(pPlayer->HasFinishedQuest(7761) && pPlayer->getLevel() >= 58 && pPlayer->InGroup() == true)
                    {
                      pPlayer->SafeTeleport(469, 0, -7672.939941, -1107.307617, 396.649994, 0.616532);
                    }
                    else if(pPlayer->getLevel() <= 57 || pPlayer->HasFinishedQuest(7761) == false)
                    {
                    pPlayer->BroadcastMessage("You need to be level 58 and have completed the quest : Blackhand's Command");
                    }
                    else if(pPlayer->HasFinishedQuest(7761) == true && pPlayer->getLevel() >= 58 && pPlayer->InGroup() == false)
                    {
                    pPlayer->BroadcastMessage("You need to be in a raid group to be able to enter this instance");
                    }
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new OrbOfCommand(GO); }
      };
      GameObjectAIScript * create_go45(GameObject * GO) { return new OrbOfCommand(GO); }

    class OrbOfTransLocUC : public GameObjectAIScript
      {
      public:
              OrbOfTransLocUC(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                      pPlayer->SafeTeleport(530, 0, 10021.254883, -7014.893066, 49.717690, 4.00532);           
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new OrbOfTransLocUC(GO); }
      };
      GameObjectAIScript * create_go(GameObject * GO) { return new OrbOfTransLocUC(GO); }

    class OrbOfTransLocSILVM : public GameObjectAIScript
      {
      public:
              OrbOfTransLocSILVM(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                      pPlayer->SafeTeleport(0, 0, 1805.823975, 334.009796, 70.397469, 4.724170);                 
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new OrbOfTransLocSILVM(GO); }
      };
      GameObjectAIScript * create_go2(GameObject * GO) { return new OrbOfTransLocSILVM(GO); }
	  
    class FalconWatchbtm : public GameObjectAIScript
      {
      public:
              FalconWatchbtm(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                      pPlayer->SafeTeleport(530, 0, -592.9, 4068.4, 143.2, 1.25);                   
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new FalconWatchbtm(GO); }
      };
      GameObjectAIScript * create_go3(GameObject * GO) { return new FalconWatchbtm(GO); }
	  
    class FalconWatchtop : public GameObjectAIScript
      {
      public:
              FalconWatchtop(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                      pPlayer->SafeTeleport(530, 0, -595.0, 4081.1, 93.8, 2.07);                
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new FalconWatchtop(GO); }
      };
      GameObjectAIScript * create_go4(GameObject * GO) { return new FalconWatchtop(GO); }
	  
    class firewing_point_orb1 : public GameObjectAIScript
      {
      public:
              firewing_point_orb1(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                      pPlayer->SafeTeleport(530, 0, -2260.65, 3186.5, 23.67, 1.51);                   
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new firewing_point_orb1(GO); }
      };
      GameObjectAIScript * create_go5(GameObject * GO) { return new firewing_point_orb1(GO); }
	  
    class firewing_point_orb2 : public GameObjectAIScript
      {
      public:
              firewing_point_orb2(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                      pPlayer->SafeTeleport(530, 0, -2250.4, 3120.7, 27.38 , 3.72);                   
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new firewing_point_orb2(GO); }
      };
      GameObjectAIScript * create_go6(GameObject * GO) { return new firewing_point_orb2(GO); }
	  
    class firewing_point_orb3 : public GameObjectAIScript
      {
      public:
              firewing_point_orb3(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                      pPlayer->SafeTeleport(530, 0, -2311.486, 3079.403, 158.384, 0.62);                   
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new firewing_point_orb3(GO); }
      };
      GameObjectAIScript * create_go7(GameObject * GO) { return new firewing_point_orb3(GO); }
	  
    class firewing_point_orb4 : public GameObjectAIScript
      {
      public:
              firewing_point_orb4(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                      pPlayer->SafeTeleport(530, 0, -2260.321, 3226.028, 5.344, 6.28);                   
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new firewing_point_orb4(GO); }
      };
      GameObjectAIScript * create_go8(GameObject * GO) { return new firewing_point_orb4(GO); }
	  
    class firewing_point_orb5 : public GameObjectAIScript
      {
      public:
              firewing_point_orb5(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
              void OnActivate(Player * pPlayer)
              {
                      pPlayer->SafeTeleport(530, 0, 9291.155, -7913.556, 176.130, 5.33);                   
              }
              static GameObjectAIScript *Create(GameObject * GO) { return new firewing_point_orb5(GO); }
      };
      GameObjectAIScript * create_go9(GameObject * GO) { return new firewing_point_orb5(GO); }



      void SetupGameobjects(ScriptMgr * mgr)
      {
		  mgr->register_gameobject_script(184503, &OrbOfTransLocUC::Create);            
          mgr->register_gameobject_script(184502, &OrbOfTransLocSILVM::Create);       
          mgr->register_gameobject_script(179879, &OrbOfCommand::Create);
		  mgr->register_gameobject_script(184501, &FalconWatchbtm::Create);
		  mgr->register_gameobject_script(184500, &FalconWatchtop::Create);
		  mgr->register_gameobject_script(182543, &firewing_point_orb1::Create);
		  mgr->register_gameobject_script(182548, &firewing_point_orb2::Create);
		  mgr->register_gameobject_script(182547, &firewing_point_orb3::Create);
		  mgr->register_gameobject_script(182546, &firewing_point_orb4::Create);
		  mgr->register_gameobject_script(180911, &firewing_point_orb5::Create);
      }
	  
	  
	  
	  
	  