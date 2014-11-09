////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// (C) Andy Thomason 2012-2014
/// Modular Framework for OpenGLES2 rendering on multiple platforms. 
///@author Himanshu Chablani 
///@brief Minecraft Wars 2014 : A simple project in which the player has to collect elements, build a fort, and defend his fort as long as he can 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace octet {

  ///The Number of elements in the game ie : Mud, Wood, Stone, Iron
#define Nu_Elements 4 
  
  ///The Number of weapons in the game ie : gun 
#define Nu_Weapon 1   

  enum ID_Object
  {
    Plane_ID = -1,
    Player_ID = 0,
    Element_ID = 1,
    AI_ID = 2,
    Pickup_ID = 3
  };
  /**
  * @enum ID_Object
  * @brief This enum contains the IDs for the game objects. Used to check what kind of an object the rigid body is from the user pointer.
  */

  enum ID_Pickup
  {
    Health_ID = 0,
    Ammo_ID = 1
  };
  /**
  * @enum ID_Pickup
  * @brief This enum contains the IDs for the items the player can pick
  */

  enum ID_Element
  {
    Mud_ID = 0,
    Wood_ID = 1,
    Stone_ID = 2,
    Iron_ID = 3
  };
  /**
  * @enum ID_Element
  * @brief This enum contains the IDs for the elements the game will have
  */

  enum ID_weapon
  {
    Gun_ID = 0
  };
  /**
  * @enum ID_weapon
  * @brief This enum contains the IDs for the weapons the game will have
  */

  enum Hits_to_extract_Element
  {
    Mud_Hits = 2,
    Wood_Hits = 3,
    Stone_Hits = 5,
    Iron_Hits = 7
  };
  /**
  * @enum Hits_to_extract_Element
  * @brief This enum contains the number of gather hits required by the player to gather a perticular element
  */


  class CollisionObjects
  {
    int id;
    /** @var int id
    *   @brief The ID of the Object ie player, element , AI , plane , or pickup
    */
    btRigidBody *body;
    /** @var btRigidBody *body
    *   @brief The Rigid body of the Object
    */
    void *objectclass;
    /** @var void *objectclass
    *   @brief The Object class of the Object
    */
  public:
    CollisionObjects(int _id, btRigidBody *_body, void *_objectclass)
    {
      id = _id;
      body = _body;
      objectclass = _objectclass;
    }
    /** @fn CollisionObjects(int _id, btRigidBody *_body, void *_objectclass)
    *   @brief The constructor for the class
    *   @param int _id an int this is the id of the object to define if the object is mud,wood,stone,or iron ie  ID_Object
    *   @param btRigidBody *_body is a pointer to the rigid body of the object.
    *   @param void *_objectclass is a void pointer to the object class which is specific to the object ie player,element,AI...
    */

    int getid()
    {
      return id;
    }
    /** @fn int getid()
    *   @brief getter for the ID
    *   @return an int this is the ID of the game object
    */
    btRigidBody* getbody()
    {
      return body;
    }
    /** @fn btRigidBody* getbody()
    *   @brief getter for the Rigid body
    *   @return a btRigidBody* this is the Rigid body of the game object
    */

    void* getobjectclass()
    {
      return objectclass;
    }
    /** @fn void* getobjectclass()
    *   @brief getter for the Object class
    *   @return a  void* this is the object class of the game object
    */

  };
  /**
  * @class CollisionObjects
  * @brief this class stores the basic common information about the game objects ie ID, Rigidbody, and the pointer to the specific object.
  * all game object that are rigid bodies are objects of this class.
  */


  struct Player
  {
    float health;
    /** @var float health
    *   @brief The current health of the player
    */
    int elements_held[Nu_Elements];
    /** @var int elements_held[Nu_Elements]
    *   @brief The current elements held by the player
    */
    dynarray<int> elements_held_ids[Nu_Elements];
    /** @var dynarray<int> elements_held_ids[Nu_Elements]
    *   @brief The IDs of the current elements held by the player
    */
    int ammo[Nu_Weapon];
    /** @var ammo[Nu_Weapon]
    *   @brief The ammon for the weapon held by the player
    */
    float weapon_damage[Nu_Weapon];
    /** @var float weapon_damage[Nu_Weapon]
    *   @brief The damage for the weapon held by the player
    */
    int current_element;
    /** @var int current_element
    *   @brief The current element that the player can place
    */
    int current_weapon;
    /** @var int current_weapon
    *   @brief The current weapon that the player can use
    */
    int index;
    /** @var  int index
    *   @brief The index of the player in game object dynarray
    */

    Player()
    {
      index = 0;
      health = 100.0f;
      for (int i = 0; i < Nu_Elements; i++)
      {
        elements_held[i] = 0;
      }
      for (int i = 0; i < Nu_Weapon; i++)
      {
        ammo[i] = 100;
      }
      weapon_damage[0] = 20.0f;

      current_element = ID_Element::Mud_ID;
      current_weapon = ID_weapon::Gun_ID;

    }
    /** @fn Player()
    *   @brief The constructor for the player struct
    */

    bool Got_Hit(float damage)
    {
      if (health - damage < 0.0f)
      {
        return false;
      }

      else
      {
        health -= damage;
        return true;
      }
    }
    /** @fn bool Got_Hit(float damage)
    *   @brief The ammont of damage the player has taken from the AI returns false if player dies
    *   @return a bool true if player is alive false if dead
    */

    void Element_Picked(int element_number, int element_index)
    {
      elements_held[element_number]++;
      elements_held_ids[element_number].push_back(element_index);
    }
    /** @fn  void Element_Picked(int element_number, int element_index)
    *   @brief used to manage the elemets_held and the ID of those elements when the player picks up an element
    *   @param int element_number The ID of the element the player picked
    *   @param int element_index The Index of that element in the game object dynarray
    */
    int Element_Placed(int element_number)
    {
      dynarray<int>::iterator tempind = elements_held_ids[element_number].end();
      tempind--;
      int returnint = *tempind;
      --elements_held[element_number];
      elements_held_ids[element_number].pop_back();
      return returnint;
    }
    /** @fn  int Element_Placed(int element_number)
    *   @brief used to manage the elemets_held and the ID of those elements when the player places an element
    *   @param int element_number The ID of the element the player picked
    *   @return an int the index of the element in the dynarray
    */

    void Ammo_Picked(int weapon_nu, int amount)
    {
      ammo[weapon_nu] += amount;
    }
    /** @fn  void Ammo_Picked(int weapon_nu, int amount)
    *   @brief used to manage the ammo when the player picks up ammo for a weapon
    *   @param int weapon_nu The ID for the weapon that the player has picked the ammo for
    *   @param int amount the amount of ammo he has picked
    */
    bool Weapon_fired(int weapon_nu)
    {
      if (ammo[weapon_nu] > 0)
      {
        --ammo[weapon_nu];
        return true;
      }
      return false;
    }
    /** @fn  bool Weapon_fired(int weapon_nu)
    *   @brief used to manage the ammo when the player fires a weapon
    *   @param int weapon_nu The ID for the weapon that the player has picked the ammo for
    *   @return a bool true if the player has ammo to fire that weapon false if he is out of ammo
    */
  };
  /**
  * @struct Player
  * @brief The Player struct contains all the relevant info about the player
  * This is the object class for the player
  */

  struct AI
  {
    float health;
    /** @var float health
    *   @brief The current health of the AI
    */
    int index;
    /** @var  int index
    *   @brief The index of the AI in game object dynarray
    */
    float damage;
    /** @var float damage
    *   @brief The damage for the AI
    */
    bool canattack;
    /** @var  bool canattack
    *   @brief if true the AI can attack if false AI has to wait for some time till he can attack again
    */
    float framestoattack;
    /** @var  float framestoattack
    *   @brief The ammount of frames the AI has to wait for the next time he can attack
    */
    float tempframes;
    /** @var float tempframes
    *   @brief The frames lasped since the AI's last attack
    */
    bool isdead;
    /** @var  bool isdead
    *   @brief the status of the AI dead or alive
    */
    AI(int _index)
    {
      health = 50.0f;
      damage = 15;
      index = _index;
      canattack = true;
      isdead = false;
    }
    /** @fn AI(int _index)
    *   @brief The constructor for the player struct
    *   @param int _index The index of the AI in the game object dynarray
    */
    bool Got_Hit(float damage)
    {
      if (health - damage > 0.0f)
      {
        health -= damage;
        return true;
      }
      else
      {
        isdead = true;
        return false;
      }

    }
    /** @fn bool Got_Hit(float damage)
    *   @brief The ammont of damage the AI has taken from the Player returns false if AI dies
    *   @return a bool true if AI is alive false if dead
    */
    bool updatecanattack()
    {
      if (!canattack)
      {
        if (tempframes > framestoattack)
        {
          canattack = true;
          return true;
        }
        else
        {
          tempframes++;
        }
        return false;
      }
      return true;
    }
    /** @fn bool updatecanattack()
    *   @brief If the AI can't attack returns false and till the tempframes > framestoattack and then the AI can attack again
    *   @return a bool true if AI is can attack false if not
    */
    float attacked()
    {
      if (canattack)
      {
        canattack = false;
        framestoattack = 30;
        tempframes = 0;
        return damage;
      }
      else
        return 0.0f;
    }
    /** @fn  float attacked()
    *   @brief If the AI can't attack returns 0 and if AI can returns the damgae
    *   @return a float the damage of AI attack
    */
  };
  /**
  * @struct AI
  * @brief The AI struct contains all the relevant info about the AI
  * This is the object class for the AI
  */


  struct Elements
  {
    int id;
    /** @var int id
    *   @brief The ID of the element ie mud, stone , wood, iron
    */
    string tag;
    /** @var string tag
    *   @brief The tag of the element
    */
    float health;
    /** @var float health
    *   @brief The current health of the element
    */
    int hit_to_extraxt;
    /** @var int hit_to_extraxt
    *   @brief The number of hits needed to extract the element
    */
    vec4 color;
    /** @var vec4 color
    *   @brief the colour to be put out on the mesh
    */
    int index;
    /** @var  int index
    *   @brief The Index of this class in GameObject dynarry
    */

    Elements(int _id, string _tag, int _index, float _health, int _hit_to_extract, vec4 _color)
    {
      id = _id;
      tag = _tag;
      index = _index;
      health = _health;
      hit_to_extraxt = _hit_to_extract;
      color = _color;
    }
    /** @fn Elements(int _id, string _tag, int _index, float _health, int _hit_to_extract, vec4 _color)
    *   @brief The constructor for the element struct
    *   @param int _id an int this is the id of the object to define if the object is mud,wood,stone,or iron ie  ID_Object
    *   @param string _tag is the tag of the element 
    *   @param int _index the index in the gameobejct dynarray
    *   @param float _health the health of the element
    *   @param int _hit_to_extract the number of hits neede to extract
    *   @param  vec4 _color the colour to be put out on the mesh
    */
    bool Got_Hit(float damage)
    {
      if (health - damage > 0.0f)
      {
        health -= damage;
        return true;
      }
      return false;
    }
    /** @fn bool Got_Hit(float damage)
    *   @brief The ammont of damage the element has taken from the AI returns false if element has health <0
    *   @return a bool true if element has some health else false
    */
  };
  /**
  * @struct Elements
  * @brief The base struct for all the elements.Use this in the userpointer and call the derived class constructors to init this
  * This is the object class for all the elements
  */


  struct Mud : public Elements
  {

    Mud(int index) : Elements(ID_Element::Mud_ID, "Mud", index, 20.0f, 2, vec4(0.4078f, 0.196f, 0.0352f, 1.0f))
    {
    }

  };
  /**
  * @struct Mud : public Elements
  * @brief The Mud struct derived from the element struc. Only use to init the element
  * This is use to init the element as a mud element
  */


  struct Wood : public Elements
  {
    Wood(int index) : Elements(ID_Element::Wood_ID, "Wood", index, 35.0f, 3, vec4(0.9098f, 0.666f, 0.4784f, 1.0f))
    {

    }

  };
  /**
  * @struct Wood : public Elements
  * @brief The Wood struct derived from the element struc. Only use to init the element
  * This is use to init the element as a wood element
  */


  struct Stone : public Elements
  {
    Stone(int index) : Elements(ID_Element::Stone_ID, "Stone", index, 45.0f, 5, vec4(0.3529f, 0.3529f, 0.3529f, 1.0f))
    {}
  };
  /**
  * @struct Stone : public Elements
  * @brief The Stone struct derived from the element struc. Only use to init the element
  * This is use to init the element as a stone element
  */

  struct Iron : public Elements
  {
    Iron(int index) : Elements(ID_Element::Iron_ID, "Iron", index, 65.0f, 7, vec4(0.5529f, 0.5529f, 0.5529f, 1.0f))
    {}
  };
  /**
  * @struct Stone : public Elements
  * @brief The Iron struct derived from the element struc. Only use to init the element
  * This is use to init the element as an iron element
  */

  bool isgameover;
  /** @var    bool isgameover
  *   @brief true if the player dies.
  */

  bool AIisfacingobject(btRigidBody *AI, btRigidBody *obj)
  {
    btQuaternion btq = AI->getOrientation();
    quat q(btq[0], btq[1], btq[2], btq[3]);
    mat4t objm = q;

    btVector3 AIlookdir = btVector3(objm.z().x()*(-1),
      objm.z().y()*(-1),
      objm.z().z()*(-1));
    AIlookdir.normalize();

    if ((obj->getCenterOfMassPosition().x() - (AI->getCenterOfMassPosition().x() + AIlookdir.x()) <= 2) && (obj->getCenterOfMassPosition().x() - (AI->getCenterOfMassPosition().x() + AIlookdir.x()) >= -2) && (obj->getCenterOfMassPosition().z() - (AI->getCenterOfMassPosition().z() + AIlookdir.z()) <= 2) && (obj->getCenterOfMassPosition().z() - (AI->getCenterOfMassPosition().z() + AIlookdir.z()) >= -2))
      return true;
    else
      return false;
  }
  /**
  * @fn bool AIisfacingobject(btRigidBody *AI, btRigidBody *obj)
  * @brief This fucntion is called when the AI collides with an element or the player to check if the AI is facing then to attack
  * @param btRigidBody *AI The rigid body of the AI
  * @param btRigidBody *obj The rigid body of the object the AI collided with
  * @return a bool true if the AI was facing  the object else false
  */

  bool minecraftcollision(btManifoldPoint &cp, const btCollisionObjectWrapper *obj1, int id1, int index1, const btCollisionObjectWrapper *obj2, int id2, int index2)
  {
    CollisionObjects *Obj1userpointer = (CollisionObjects*)obj1->getCollisionObject()->getUserPointer();
    CollisionObjects *Obj2userpointer = (CollisionObjects*)obj2->getCollisionObject()->getUserPointer();
    Elements *The_Element;
    AI *The_AI;
    Player *The_Player;
    if ((Obj1userpointer->getid() == ID_Object::AI_ID || Obj2userpointer->getid() == ID_Object::AI_ID) && (Obj1userpointer->getid() != ID_Object::Plane_ID && Obj2userpointer->getid() != ID_Object::Plane_ID))
    {

      if (Obj1userpointer->getid() == ID_Object::AI_ID)
      {
        The_AI = (AI*)Obj1userpointer->getobjectclass();
        float AI_dmage;
        AI_dmage = The_AI->attacked();
        if (Obj2userpointer->getid() == ID_Object::Player_ID && (AIisfacingobject(Obj1userpointer->getbody(), Obj2userpointer->getbody())))
        {

          if (AI_dmage > 0)
          {
            The_Player = (Player *)Obj2userpointer->getobjectclass();
            printf("AI HIT PLAYER \n");
            if (!The_Player->Got_Hit(AI_dmage))
            {
             isgameover=true; 
            }
          }
        }

        if (Obj2userpointer->getid() == ID_Object::Element_ID && (AIisfacingobject(Obj1userpointer->getbody(), Obj2userpointer->getbody())))
        {
          if (AI_dmage > 0)
          {
            The_Element = (Elements *)Obj2userpointer->getobjectclass();
            printf("AI HIT Element \n");

            if (!The_Element->Got_Hit(AI_dmage))
            {
              Obj2userpointer->getbody()->setMassProps(0, btVector3(0.0f, 0.0f, 0.0f));
              Obj2userpointer->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
              Obj2userpointer->getbody()->translate(btVector3(0, -100, 0));

            }
          }
        }
      }

      if (Obj2userpointer->getid() == ID_Object::AI_ID)
      {
        The_AI = (AI*)Obj2userpointer->getobjectclass();
        float AI_dmage;
        AI_dmage = The_AI->attacked();
        if (Obj1userpointer->getid() == ID_Object::Player_ID && (AIisfacingobject(Obj2userpointer->getbody(), Obj1userpointer->getbody())))
        {
          if (AI_dmage > 0)
          {
            The_Player = (Player *)Obj1userpointer->getobjectclass();
            printf("AI HIT PLAYER \n");
            if (!The_Player->Got_Hit(AI_dmage))
            {
              printf("PLAYER DIED");
            }
          }
        }

        if (Obj1userpointer->getid() == ID_Object::Element_ID && (AIisfacingobject(Obj2userpointer->getbody(), Obj1userpointer->getbody())))
        {
          if (AI_dmage > 0)
          {
            The_Element = (Elements *)Obj1userpointer->getobjectclass();
            printf("AI HIT Element \n");

            if (!The_Element->Got_Hit(The_AI->attacked()))
            {
              Obj1userpointer->getbody()->setMassProps(0, btVector3(0.0f, 0.0f, 0.0f));
              Obj1userpointer->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
              Obj1userpointer->getbody()->translate(btVector3(0, -100, 0));

            }
          }
        }
      }

    }
    return false;
  }
  /**
  * @fn bool minecraftcollision(btManifoldPoint &cp, const btCollisionObjectWrapper *obj1, int id1, int index1, const btCollisionObjectWrapper *obj2, int id2, int index2)
  * @brief The collision custom callback fucntion.This is called when a collion takes place.
  * @param btManifoldPoint &cp The point of collision
  * @param const btCollisionObjectWrapper *obj1 The rigid body wrapper of the first object
  * @param int id1 the id of the first object( The Bt physics id not the game id)
  * @param int id1 the index of the first object(The Bt physics index not the game index)
  * @param const btCollisionObjectWrapper *obj2 The rigid body wrapper of the second object
  * @param int id1 the id of the second object( The Bt physics id not the game id)
  * @param int id1 the index of the second object(The Bt physics index not the game index)
  * @return a bool ture if the attribute of the collion have changed else false
  */



  class minecraft_wars : public app {
    
    ref<visual_scene> app_scene;
    /** @var ref<visual_scene> app_scene
    *   @brief scene for drawing box
    */

    //The Bullet Physics    
    btDefaultCollisionConfiguration config;
    /** @var btDefaultCollisionConfiguration config
    *   @brief setup for the world
    */        
    btCollisionDispatcher *dispatcher; 
    /** @var btCollisionDispatcher *dispatcher
    *   @brief handler for collisions between objects
    */            
    btDbvtBroadphase *broadphase;    
    /** @var btDbvtBroadphase *broadphase
    *   @brief handler for broadphase (rough) collision
    */              
    btSequentialImpulseConstraintSolver *solver;
    /** @var  btSequentialImpulseConstraintSolver *solver
    *   @brief handler to resolve collisions
    */   
    btDiscreteDynamicsWorld *world; 
    /** @var   btDiscreteDynamicsWorld *world
    *   @brief physics world, contains rigid bodies
    */      

    Player The_Player;
    /** @var Player The_Player
    *   @brief the player (which will be linked to the user pointer of the rigid bodies of the player)
    */    
    dynarray<CollisionObjects*> Game_objects; 
    /** @var dynarray<CollisionObjects*> Game_objects
    *   @brief the game objects this contains the rigidbodies gameobj class and the id for the object.
    */    
    dynarray<scene_node*> nodes; 
    /** @var  dynarray<scene_node*> nodes
    *   @brief the nodes for the game just used for the rendering
    */
    dynarray<CollisionObjects*> Enemies;
    /** @var  dynarray<CollisionObjects*> Enemies
    *   @brief the CollisionObjects for the enemies, used to contr the enemies
    */   

    bool gathering;
    /** @var  bool gathering
    *   @brief this indicates that the player is gather so wait for some frames till the next time player can gather
    */
    bool placing;
    /** @var  bool placing
    *   @brief this indicates that the player is placing something 
    */
    bool canshoot;
    /** @var  bool canshoot
    *   @brief true if player can shoot; if he has shot then make this false and true again after set frames
    */
    float  framecount_gathering;
    /** @var  float  framecount_gathering
    *   @brief multiplay this with 30*nu of sec to collect the item and then chk of frams
    */
    float tempframe_gathering;
    /** @var  float tempframe_gathering
    *   @brief set =0 when gathering chk with framecount to see if can gather or not 
    */
    float  framecount_shooting;
    /** @var float tempframe_gathering
    *   @brief multiplay this with 30*nu of sec to shoot then chk of frams
    */
    float tempframe_shooting;
    /** @var float tempframe_shooting
    *   @brief set =0 when shooting chk with framecount to see if can gather or not 
    */
    float  framecount_placing;
    /** @var float tempframe_shooting
    *   @brief multiplay this with 30*nu of sec to place the item and then chk of frams
    */
    float tempframe_placing;
    /** @var float tempframe_placing
    *   @brief set =0 when placing chk with framecount to see if can gather or not 
    */
    bool canjump;
    /** @var bool canjump
    *   @brief true if the player can jump false if he cant
    */
    float framecount_jump;
    /** @var  float framecount_jump
    *   @brief multiplay this with 30*nu of sec to jump and then chk of frams
    */
    float tempframecount_jump;
    /** @var float tempframecount_jump
    *   @brief set =0 when jump chk with framecount to see if can jump or not
    */
    
    float tempframe_place_enemy;
    /** @var  float tempframe_place_enemy
    *   @brief used as a seed to ramdomize the position of the enemy
    */
    float nu_enemies;
    /** @var   float nu_enemies
    *   @brief number of enemies to make 
    */
    bool activate_enemies;
    /** @var   bool activate_enemies
    *   @brief activate the enemies in the night and deactivate in the morning
    */

    Elements *element;
    /** @var Elements *element
    *   @brief the element we are gathering
    */
    btCollisionShape *elementshape;
    /** @var  btCollisionShape *elementshape
    *   @brief the collision shape used to make elements 
    */
    btCollisionShape *enemyshape;
    /** @var  btCollisionShape *enemyshape
    *   @brief the collision shape used to make enemies
    */
   
    param_shader *b;
    /** @var   param_shader *b
    *   @brief the lighting shader 
    */    
    ref<param_uniform> m_amb;
    /** @var  ref<param_uniform> m_amb
    *   @brief the material ambient paramerter for the lighting shader.
    */
    ref<param_uniform> l_amb;
    /** @var  ref<param_uniform> l_amb
    *   @brief the light ambient paramerter for the lighting shader.
    */
    ref<param_uniform> m_dif;
    /** @var  ref<param_uniform> m_dif
    *   @brief the material diffuse paramerter for the lighting shader.
    */
    ref<param_uniform> l_dif;
    /** @var  ref<param_uniform> l_dif
    *   @brief the light diffuse paramerter for the lighting shader.
    */
    ref<param_uniform> m_spec;
    /** @var ref<param_uniform> m_spec
    *   @brief the material specular paramerter for the lighting shader.
    */
    ref<param_uniform> l_spec;
    /** @var  ref<param_uniform> l_spec
    *   @brief the light specular paramerter for the lighting shader.
    */
    ref<param_uniform> shnn;
    /** @var  ref<param_uniform> shnn
    *   @brief the shining paramerter for the lighting shader.
    */
    ref<param_uniform> lightpos;
    /** @var  ref<param_uniform> lightpos
    *   @brief the light position paramerter for the lighting shader.
    */
    ref<material> shiningbox;
    /** @var  ref<material> shiningbox
    *   @brief the material for the lighting shader.
    */  
    float shnnval = 32;
    ///parameter value for the lighting shader.
    vec3 amb;
    ///parameter value for the lighting shader.
    vec3 lamb;
    ///parameter value for the lighting shader.
    vec3 diff;
    ///parameter value for the lighting shader.
    vec3 ldiff;
    ///parameter value for the lighting shader.
    vec3 spec;
    ///parameter value for the lighting shader.
    vec3 lspec;
    ///parameter value for the lighting shader.
    vec3 light_pos; 
    ///parameter value for the lighting shader.

    ref<material> enemy_material;
    /** @var  ref<material> enemy_material
    *   @brief the material for the enemy
    */

    //The UI
    text_overlay *overlay;
    /** @var  text_overlay *overlay
    *   @brief text overlay for the UI.
    */    
    mesh_text *UI_top;
    /** @var   mesh_text *UI_top
    *   @brief text mesh for the top UI.
    */
    mesh_text *UI_bot;
    /** @var   mesh_text *UI_bot
    *   @brief text mesh for the bottom UI.
    */
    mesh_text *UI_popup;
    /** @var  mesh_text *UI_popup
    *   @brief text mesh for the popup UI.
    */
    mesh_text *target;
    /** @var  mesh_text *target
    *   @brief text mesh for the target UI.
    */
    int popupmsg;
    /** @var int popupmsg
    *   @brief The msg number for the pop up UI
    */
    int tempframpopup;
    /** @var int tempframpopup
    *   @brief The frames lapsed since the popup UI was on
    */
    int framepopup;
    /** @var int framepopup
    *   @brief The number of frames the popup UI should show up
    */

    //sound
    ALuint Sound_shoot;
    /** @var ALuint Sound_shoot
    *   @brief The sound for the gun
    */
    ALuint Sound_place;
    /** @var ALuint Sound_place
    *   @brief The sound for placing element
    */
    ALuint Sound_gather;
    /** @var  ALuint Sound_gather
    *   @brief The sound for gathering element
    */
    ALuint Sound_zombieattak;
    /** @var ALuint Sound_zombieattak
    *   @brief The sound for zombie attack
    */
    ALuint sources[4];
    /** @var ALuint sources[4]
    *   @brief The source for the sounds 
    */

    //day night cycle
    bool isday;
    /** @var  bool isday
    *   @brief true is its day, false if night
    */
    int nightnumber;
    /** @var  int nightnumber
    *   @brief the number of nights that have passed
    */
    bool enemiesrecycled;
    /** @var  bool enemiesrecycled
    *   @brief true if the enemies have been placed in the game , false if not (only used a trigger)
    */
    float gamedaytime;
    /** @var   float gamedaytime
    *   @brief The game day night time (0-360)
    */    
    scene_node *daynightnode;
    /** @var   scene_node *daynightnode
    *   @brief The scene node for the day night cycle objects (sun and moon)
    */
        
    enum Num_sound
    {
      Num_shoot = 0,
      Num_place,
      Num_gather,
      Num_zombieattack
    };
    /** @enum Num_sound
    *   @brief The enum for the sound
    */
    enum Popup_Msg
    {
      Msg_None = 0,
      Msg_CantPlace,
      Msg_OutOfAmmo,
      Msg_ZombiesActive,
      Msg_NotEnoughElements
    };
    /** @enum Popup_Msg
    *   @brief The enum for the popup msg
    */
    ALuint get_sound_source(int s)
    {
      return sources[s];
    }     
    /** @fn  ALuint get_sound_source(int s)
    *   @brief returns the sound to play
    *   @return a ALuint the sound to play
    */

    void mapmaker()
    {
      mat4t modelToWorld;
      elementshape = new btBoxShape(get_btVector3(vec3(1.0f, 1.0f, 1.0f)));
      btDefaultMotionState *motionState;
      btScalar mass = 100.0f;
      btVector3 inertiaTensor;
      btRigidBody * rigid_body;
      CollisionObjects *element_object;

      material *mud_mat;
      material *wood_mat;
      material *stone_mat;
      unsigned int seed = 0x948fab;
      for (int i = 0; i < 60; i++)
      {
        modelToWorld.loadIdentity();
        modelToWorld.translate(0, 0, 0);
        btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld));
        btVector3 pos(get_btVector3(modelToWorld[3].xyz()));
        btTransform transform(matrix, pos);
        motionState = new btDefaultMotionState(transform);
        elementshape->calculateLocalInertia(mass, inertiaTensor);
        rigid_body = new btRigidBody(mass, motionState, elementshape, inertiaTensor);
        rigid_body->setFriction(1.0f);
        rigid_body->setCollisionFlags(rigid_body->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK);
        world->addRigidBody(rigid_body);
        rigid_body->setActivationState(DISABLE_DEACTIVATION);

        scene_node *node = new scene_node(modelToWorld, atom_);
        mesh_box *box = new mesh_box(vec3(1, 1, 1));
        Elements *newele;

        if (i < 15)
        {
          rigid_body->translate(btVector3(5, i + 1, 5));
          newele = new Mud(Game_objects.size());
          element_object = new CollisionObjects(ID_Object::Element_ID, rigid_body, newele);
          rigid_body->setUserPointer(element_object);
          Game_objects.push_back(element_object);
          mud_mat = new material(newele->color);
          nodes.push_back(node);
          app_scene->add_child(node);
          app_scene->add_mesh_instance(new mesh_instance(node, box, mud_mat));
        }
        if (i >= 15 && i < 30)
        {
          rigid_body->translate(btVector3(-5, i - 14, 5));
          newele = new Wood(Game_objects.size());
          element_object = new CollisionObjects(ID_Object::Element_ID, rigid_body, newele);
          rigid_body->setUserPointer(element_object);
          Game_objects.push_back(element_object);
          wood_mat = new material(newele->color);
          nodes.push_back(node);
          app_scene->add_child(node);
          app_scene->add_mesh_instance(new mesh_instance(node, box, wood_mat));
        }

        if (i >= 30 && i < 45)
        {
          rigid_body->translate(btVector3(-5, i - 29, -5));
          newele = new Stone(Game_objects.size());
          element_object = new CollisionObjects(ID_Object::Element_ID, rigid_body, newele);
          rigid_body->setUserPointer(element_object);
          Game_objects.push_back(element_object);
          stone_mat = new material(newele->color);
          nodes.push_back(node);
          app_scene->add_child(node);
          app_scene->add_mesh_instance(new mesh_instance(node, box, stone_mat));
        }
        if (i >= 45 && i < 60)
        {
          rigid_body->translate(btVector3(5, i - 44, -5));
          newele = new Iron(Game_objects.size());
          element_object = new CollisionObjects(ID_Object::Element_ID, rigid_body, newele);
          rigid_body->setUserPointer(element_object);
          Game_objects.push_back(element_object);
          nodes.push_back(node);
          app_scene->add_child(node);
          app_scene->add_mesh_instance(new mesh_instance(node, box, shiningbox));
        }
      }
    }
    /** @fn  void mapmaker()
    *   @brief this is used to make the map. ie set all the elements in the map
    */
    void Add_Rigidbody_to_Cam()
    {
      mat4t cammat = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      cammat.loadIdentity();
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      modelToWorld.translate(15, 4, 15);
      btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld));
      btVector3 pos(get_btVector3(modelToWorld[3].xyz()));
      btCollisionShape *shape = new btBoxShape(get_btVector3(vec3(1.0f, 4.0f, 1.0f)));
      btTransform transform(matrix, pos);
      btDefaultMotionState *motionState = new btDefaultMotionState(transform);
      btScalar mass = 10.0f;
      btVector3 inertiaTensor;
      shape->calculateLocalInertia(mass, inertiaTensor);
      btRigidBody * rigid_body = new btRigidBody(mass, motionState, shape, inertiaTensor);
      rigid_body->setAngularFactor(btVector3(0, 0, 0));
      rigid_body->setFriction(1.0f);
      rigid_body->setActivationState(DISABLE_DEACTIVATION);
      rigid_body->setCollisionFlags(rigid_body->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK);
      world->addRigidBody(rigid_body);

      CollisionObjects *Player_object = new CollisionObjects(ID_Object::Player_ID, rigid_body, &The_Player);
      rigid_body->setUserPointer(Player_object);
      Game_objects.push_back(Player_object);
      The_Player.index = Game_objects.size() - 1;
      nodes.push_back(app_scene->get_camera_instance(0)->get_node());
    }
    /** @fn  void Add_Rigidbody_to_Cam()
    *   @brief Attach a rigid body to the camera so that we can interact with the physics world
    */
    void Create_base()
    {
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      modelToWorld.translate(0, -4, 0);
      btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld));
      btVector3 pos(get_btVector3(modelToWorld[3].xyz()));
      btCollisionShape *shape = new btBoxShape(get_btVector3(vec3(1500, 4, 1500)));
      btTransform transform(matrix, pos);
      btDefaultMotionState *motionState = new btDefaultMotionState(transform);
      btScalar mass = 0.0f;
      btVector3 inertiaTensor;
      shape->calculateLocalInertia(mass, inertiaTensor);
      btRigidBody * rigid_body = new btRigidBody(mass, motionState, shape, inertiaTensor);
      world->addRigidBody(rigid_body);

      CollisionObjects *Plane = new CollisionObjects(ID_Object::Plane_ID, rigid_body, NULL);
      rigid_body->setUserPointer(Plane);
      Game_objects.push_back(Plane);

      mesh_box *box = new mesh_box(vec3(1500, 4, 1500));
      scene_node *node = new scene_node(modelToWorld, atom_);
      nodes.push_back(node);
      app_scene->add_child(node);
      material *floor_mat = new material(vec4(0, 1, 0, 1));
      app_scene->add_mesh_instance(new mesh_instance(node, box, floor_mat));
    }
    /** @fn  void Create_base()
    *   @brief create a rigid body base for the game
    */
    vec3 matrixmult(mat4t objm, vec3 direction)
    {
      vec3 temp(objm.x().x()*direction.x() + objm.y().x()*direction.y() + objm.z().x()*direction.z(),
        objm.x().y()*direction.x() + objm.y().y()*direction.y() + objm.z().y()*direction.z(),
        objm.x().z()*direction.x() + objm.y().z()*direction.y() + objm.z().z()*direction.z());
      return temp;
    }
    /** @fn  vec3 matrixmult(mat4t objm, vec3 direction)
    *   @brief returns the postion of the point according to the object and the direction 
        @param mat4t objm The parent matrix of the object
        @param vec3 direction The direction we need to find ie front back left right
        @return a vec3 the postion of the point according to the object and the direction 
    */    
    void Player_cont()
    {
      vec3 forworddir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(0, 0, -1));
      vec3 backdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(0, 0, 1));
      vec3 rightdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(1, 0, 0));
      vec3 leftdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(-1, 0, 0));
      vec3 lookdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(0, 0, -1));

      if (!gathering)
      {
        if (is_key_down(key::key_rmb))
        {
          btCollisionWorld::ClosestRayResultCallback gatherray(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()),

            btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 8 * lookdir.x(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + 8 * lookdir.y(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 8 * lookdir.z()));


          world->rayTest(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()),

            btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 8 * lookdir.x(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + 8 * lookdir.y(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 8 * lookdir.z()), gatherray);

          if (gatherray.hasHit())
          {
            CollisionObjects *Objuserpointer = (CollisionObjects*)gatherray.m_collisionObject->getUserPointer();
            printf("gathering object : %d \n", Objuserpointer->getid());
            if (Objuserpointer->getid() == ID_Object::Element_ID)
            {
              ALuint source = get_sound_source(Num_sound::Num_gather);
              alSourcei(source, AL_BUFFER, Sound_gather);
              alSourcePlay(source);

              element = (Elements*)Objuserpointer->getobjectclass();
              if (element->hit_to_extraxt > 1)
              {
                gathering = true;
                element->hit_to_extraxt--;

                tempframe_gathering = 0;
                printf("gathering element : %s  Index : %d\n", element->tag, element->index);
                printf("hits remaining %d\n", element->hit_to_extraxt);
              }
              else
              {
                Game_objects[element->index]->getbody()->setMassProps(0, btVector3(0.0f, 0.0f, 0.0f));
                Game_objects[element->index]->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
                btVector3 pos = Game_objects[element->index]->getbody()->getCenterOfMassPosition();
                Game_objects[element->index]->getbody()->translate(btVector3(-pos.x(), -100, -pos.z()));
                The_Player.Element_Picked(element->id, element->index);
              }
            }
          }
        }
      }

      if (is_key_down(key::key_lmb) && canshoot)
      {
        if (The_Player.Weapon_fired(ID_weapon::Gun_ID))
        {
          canshoot = false;
          tempframe_shooting = 0;

          ALuint source = get_sound_source(Num_sound::Num_shoot);
          alSourcei(source, AL_BUFFER, Sound_shoot);
          alSourcePlay(source);

          btCollisionWorld::ClosestRayResultCallback shootray(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()),

            btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 500 * lookdir.x(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + 500 * lookdir.y(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 500 * lookdir.z()));


          world->rayTest(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()),

            btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 500 * lookdir.x(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + 500 * lookdir.y(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 500 * lookdir.z()), shootray);
          if (shootray.hasHit())
          {
            CollisionObjects *Objuserpointer = (CollisionObjects*)shootray.m_collisionObject->getUserPointer();
            printf("shootobject object : %d \n", Objuserpointer->getid());
            if (Objuserpointer->getid() != ID_Object::Plane_ID)
            {
              btRigidBody *tempbody = Objuserpointer->getbody();
              btVector3 pos = tempbody->getCenterOfMassPosition();
              btVector3 hitpoint = shootray.m_hitPointWorld;
              btVector3 normaldir = btVector3(pos.x() - hitpoint.x(), pos.y() - hitpoint.y(), pos.z() - hitpoint.z());
              normaldir.normalize();
              printf("body pos : %f %f %f \n shot pos : %f %f %f \n normal force pos %f %f %f\n\n ", pos.x(), pos.y(), pos.z(), hitpoint.x(), hitpoint.y(), hitpoint.z(), normaldir.x(), normaldir.y(), normaldir.z());

              float forcemult = 2000.0f;
              if (Objuserpointer->getid() == ID_Object::AI_ID)
              {

                AI *hitAI = (AI *)Objuserpointer->getobjectclass();
                printf("AI Health %f", hitAI->health);
                if (!hitAI->Got_Hit(The_Player.weapon_damage[The_Player.current_weapon]))
                {
                  Game_objects[hitAI->index]->getbody()->setMassProps(0, btVector3(0.0f, 0.0f, 0.0f));
                  Game_objects[hitAI->index]->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
                  btVector3 pos = Game_objects[hitAI->index]->getbody()->getCenterOfMassPosition();
                  Game_objects[hitAI->index]->getbody()->translate(btVector3(-pos.x(), -100, -pos.z()));

                  recycle_enemies(hitAI->index);
                  printf("Enemy died %d", hitAI->index);
                }
              }
              tempbody->applyImpulse(btVector3(forcemult*normaldir.x(), forcemult*normaldir.y(), forcemult*normaldir.z()), normaldir);
            }
          }
        }
        else
        {
          
          popupmsg = Popup_Msg::Msg_OutOfAmmo;
          tempframpopup = 0;
          framepopup = 45;
        }
      }
            
      if (is_key_down(key::key_tab) && !placing)
      {
        if (The_Player.elements_held[The_Player.current_element] > 0)
        {
          dynarray<int>::iterator tempele = The_Player.elements_held_ids[The_Player.current_element].begin();
          btCollisionWorld::ClosestRayResultCallback placeray(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()), btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 2 * lookdir.x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 2 * lookdir.z()));
          world->rayTest(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()), btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 2 * lookdir.x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 2 * lookdir.z()), placeray);

          if (placeray.hasHit())
          {
            popupmsg = Popup_Msg::Msg_CantPlace;
            tempframpopup = 0;
            framepopup = 45;
            placing = false;
          }
          else
          {
            placing = true;
            tempframe_placing = 0;
          }
        }
        else
        {
          popupmsg = Popup_Msg::Msg_NotEnoughElements;
          tempframpopup = 0;
          framepopup = 45;
        }
      }
      if (is_key_down(key::key_f1))
      {
        The_Player.current_element = ID_Element::Mud_ID;
      }
      if (is_key_down(key::key_f2))
      {
        The_Player.current_element = ID_Element::Wood_ID;
      }
      if (is_key_down(key::key_f3))
      {
        The_Player.current_element = ID_Element::Stone_ID;
      }
      if (is_key_down(key::key_f4))
      {
        The_Player.current_element = ID_Element::Iron_ID;
      }

      if (canjump && is_key_down(key::key_space) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->applyCentralImpulse(btVector3(0, 180, 0));
        tempframecount_jump = 0;
        canjump = false;
      }
      if (is_key_down(key::key_up) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->setLinearVelocity(btVector3(15 * forworddir.x(), 0, 15 * forworddir.z()));
      }
      if (is_key_down(key::key_down) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->setLinearVelocity(btVector3(15 * backdir.x(), 0, 15 * backdir.z()));
      }
      if (is_key_down(key::key_right) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->setLinearVelocity(btVector3(15 * rightdir.x(), 0, 15 * rightdir.z()));
      }
      if (is_key_down(key::key_left) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->setLinearVelocity(btVector3(15 * leftdir.x(), 0, 15 * leftdir.z()));
      }
      int vx = 0, vy = 0;
      int x, y;
      get_viewport_size(vx, vy);
      get_mouse_pos(x, y);
      btRigidBody *rigid_body = Game_objects[The_Player.index]->getbody();

      rigid_body->applyTorqueImpulse(btVector3(0, (float)-x*2.0f, 0));
      btQuaternion btq = rigid_body->getOrientation();
      btVector3 pos = rigid_body->getCenterOfMassPosition();
      quat q(btq[0], btq[1], btq[2], btq[3]);
      mat4t modelToWorld;
      modelToWorld = q;
      modelToWorld[3] = vec4(pos[0], pos[1], pos[2], 1);
      modelToWorld.rotateY((float)-x*2.0f);
      if (vy / 2 - y < 40 && vy / 2 - y > -40)
        modelToWorld.rotateX(vy / 2 - y);
      if (vy / 2 - y >= 40)
        modelToWorld.rotateX(40);
      if (vy / 2 - y <= -40)
        modelToWorld.rotateX(-40);
      nodes[The_Player.index]->access_nodeToParent() = modelToWorld;//apply to the node
    }
    /** @fn  void Player_cont()
    *   @brief The player controls ie gathering placing shooting walking jumping rotationg camera....
    */
    void gatheringelement()
    {
      if (gathering)
      {
        if (tempframe_gathering >= framecount_gathering)
        {
          gathering = false;
        }
        else
        {
          tempframe_gathering++;
        }
      }
    }
    /** @fn  void gatheringelement()
    *   @brief Used to count down the frames since the last gathering key was pressed so that the player can gather again
    */
    void placingelement()
    {
      if (placing && The_Player.elements_held[The_Player.current_element] > 0)
      {

        if (tempframe_placing >= framecount_placing)
        {

          vec3 lookdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(0, 0, -1));
          btCollisionWorld::ClosestRayResultCallback placeray(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()), btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 2 * lookdir.x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 2 * lookdir.z()));
          world->rayTest(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()), btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 2 * lookdir.x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 2 * lookdir.z()), placeray);
          printf(" from %f,%f,%f to %f,%f,%f \n\n", app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 2 * lookdir.x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 2 * lookdir.z());
          if (placeray.hasHit())
          {
            printf("Cant place here");
          }
          else
          {
            ALuint source = get_sound_source(Num_sound::Num_place);
            alSourcei(source, AL_BUFFER, Sound_place);
            alSourcePlay(source);
            int element_index = The_Player.Element_Placed(The_Player.current_element);
            element = (Elements *)Game_objects[element_index]->getobjectclass();
            switch (The_Player.current_element)
            {
            case 0:
              element->hit_to_extraxt = Hits_to_extract_Element::Mud_Hits;
              break;
            case 1:
              element->hit_to_extraxt = Hits_to_extract_Element::Wood_Hits;
              break;
            case 2:
              element->hit_to_extraxt = Hits_to_extract_Element::Stone_Hits;
              break;
            case 3:
              element->hit_to_extraxt = Hits_to_extract_Element::Iron_Hits;
              break;

            }
            btVector3 pos = Game_objects[element_index]->getbody()->getCenterOfMassPosition();
            Game_objects[element_index]->getbody()->translate(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 1 * lookdir.x() - pos.x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + lookdir.y() - pos.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 1 * lookdir.z() - pos.z()));

            btScalar mass = 100.0f;
            btVector3 inertiaTensor;
            elementshape->calculateLocalInertia(mass, inertiaTensor);
            Game_objects[element_index]->getbody()->setMassProps(mass, inertiaTensor);
            Game_objects[element_index]->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK);
          }
          placing = false;
        }
        else
        {
          tempframe_placing++;
        }

      }
    }
    /** @fn  void placingelement()
    *   @brief Used to count down the frames since the last placing key was pressed. after the timer elapses the object is placed.
    */
    void shootreload()
    {
      if (!canshoot)
      {
        if (tempframe_shooting >= framecount_shooting)
        {
          canshoot = true;
        }
        else
        {
          tempframe_shooting++;
        }
      }
    }
    /** @fn  void shootreload()
    *   @brief Used to count down the frames since the last shoot was fired
    */
    void waitforjump()
    {
      if (!canjump)
      {
        if (tempframecount_jump >= framecount_jump)
        {
          canjump = true;
        }
        else
        {
          tempframecount_jump++;
        }
      }
    }
    /** @fn  void waitforjump()
    *   @brief Used to count down the frames since the last jump key was pressed so that the player can jump again
    */

    void  activateenemies()
    {
      if (!activate_enemies)
      {
        popupmsg = Popup_Msg::Msg_ZombiesActive;
        tempframpopup = 0;
        framepopup = 90;

        activate_enemies = true;
        btVector3 playerpos = Game_objects[The_Player.index]->getbody()->getCenterOfMassPosition();
        random randpos;

        mat4t modelToWorld;
        enemyshape = new btBoxShape(get_btVector3(vec3(1.0f, 1.0f, 1.0f)));
        btDefaultMotionState *motionState;
        btScalar mass = 50.0f;
        btVector3 inertiaTensor;
        btRigidBody * rigid_body;
        CollisionObjects *enemy_object;
        for (int i = 0; i < nu_enemies; i++)
        {
          modelToWorld.loadIdentity();

          randpos.set_seed(((unsigned int)(tempframe_gathering + tempframe_placing + tempframe_place_enemy + tempframe_shooting)) ^ 0x948fab);
          tempframe_place_enemy += 399959;

          float pos_x;
          float pos_z;
          if (i < 3)
          {
            pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
            pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);
          }
          else if (i < 6)

          {
            pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
            pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
          }

          else if (i < 8)
          {
            pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
            pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
          }
          else
          {
            pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
            pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);

          }
          printf("pos_x %f, pos_z %f \n\n", pos_x, pos_z);
          modelToWorld.translate(pos_x, 10, pos_z);//make this acc to random

          btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld));
          btVector3 pos(get_btVector3(modelToWorld[3].xyz()));
          btTransform transform(matrix, pos);
          motionState = new btDefaultMotionState(transform);
          elementshape->calculateLocalInertia(mass, inertiaTensor);
          rigid_body = new btRigidBody(mass, motionState, elementshape, inertiaTensor);
          rigid_body->setFriction(1.0f);
          rigid_body->setCollisionFlags(btCollisionObject::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK);
          world->addRigidBody(rigid_body);
          rigid_body->setActivationState(DISABLE_DEACTIVATION);

          AI *newAI = new AI(Game_objects.size());
          enemy_object = new CollisionObjects(ID_Object::AI_ID, rigid_body, newAI);
          rigid_body->setUserPointer(enemy_object);
          Enemies.push_back(enemy_object);
          Game_objects.push_back(enemy_object);

          mesh_box *box = new mesh_box(vec3(1, 1, 1));
          scene_node *node = new scene_node(modelToWorld, atom_);
          nodes.push_back(node);
          app_scene->add_child(node);
          app_scene->add_mesh_instance(new mesh_instance(node, box, enemy_material));
        }

      }
    }
    /** @fn  void  activateenemies()
    *   @brief This function is used to create enemies with random postion. This function is only called on the first night
    */
    void enemyconts()
    {
      if (activate_enemies)
      {

        btVector3 playerpos = Game_objects[The_Player.index]->getbody()->getCenterOfMassPosition();
        AI *thisAI;
        for (int i = 0; i < Enemies.size(); i++)
        {
          thisAI = (AI *)Enemies[i]->getobjectclass();
          if ((Enemies[i]->getbody()->getLinearVelocity().y() < 0.1f && Enemies[i]->getbody()->getLinearVelocity().y() > -0.1f) && !thisAI->isdead)
          {

            btVector3 enpos = Enemies[i]->getbody()->getCenterOfMassPosition();
            btVector3 relpos = btVector3(playerpos - enpos);
            relpos.normalize();
            Enemies[i]->getbody()->applyCentralForce(2000 * btVector3(relpos.x(), 0, relpos.z()));
            if (thisAI->updatecanattack())
            {
              ALuint source = get_sound_source(Num_sound::Num_zombieattack);
              alSourcei(source, AL_BUFFER, Sound_zombieattak);
              alSourcePlay(source);
            }
          }
        }

      }
    }
    /** @fn  void enemyconts()
    *   @brief The AI controls ie run after the player and attack
    */   
    void killallenemies()
    {
      for (int _index = 0; _index < Enemies.size(); _index++)
      {
        Enemies[_index]->getbody()->setMassProps(0, btVector3(0.0f, 0.0f, 0.0f));
        Enemies[_index]->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
        btVector3 pos = Enemies[_index]->getbody()->getCenterOfMassPosition();
        Enemies[_index]->getbody()->translate(btVector3(-pos.x(), -100, -pos.z()));
      }
    }   
    /** @fn void killallenemies()
    *   @brief This function is used to kill all the enemies when the night ends
    */
    void recycle_enemies(int _index)
    {
      if (!isday)
      {
        btVector3 pos = Game_objects[_index]->getbody()->getCenterOfMassPosition();
        random randpos;
        btVector3 playerpos = Game_objects[The_Player.index]->getbody()->getCenterOfMassPosition();

        btScalar mass = 100.0f;
        btVector3 inertiaTensor;
        elementshape->calculateLocalInertia(mass, inertiaTensor);
        Game_objects[_index]->getbody()->setMassProps(mass, inertiaTensor);
        Game_objects[_index]->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK);

        AI *deadAI = (AI *)Game_objects[_index]->getobjectclass();
        deadAI = new AI(_index);
        printf("AI Health %f", deadAI->health);

        randpos.set_seed((unsigned int)_index ^ 0xf963af);
        float pos_x;
        float pos_z;
        if (_index < 3)
        {
          pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
          pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);
        }
        else if (_index < 6)

        {
          pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
          pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
        }

        else if (_index < 8)
        {
          pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
          pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
        }
        else
        {
          pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
          pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);
        }
        Game_objects[_index]->getbody()->translate(btVector3(pos_x - pos.x(), 110, pos_z - pos.z()));
        deadAI->isdead = false;
      }
    }
    /** @fn void recycle_enemies(int _index)
    *   @brief This fucntion is used to reiniti the dead enemy and place him at a random loaction this function is only called in night time.
    *   @param int _index the index the enemy has in the game object dynarray
    */
    void recycleallenemies()
    {
      for (int _index = 0; _index < Enemies.size(); _index++)
      {
        random randpos;
        btVector3 playerpos = Game_objects[The_Player.index]->getbody()->getCenterOfMassPosition();
        AI *deadAI = (AI *)Enemies[_index]->getobjectclass();
        deadAI = new AI(_index);
        printf("AI Health %f", deadAI->health);

        btScalar mass = 100.0f;
        btVector3 inertiaTensor;
        elementshape->calculateLocalInertia(mass, inertiaTensor);
        Enemies[_index]->getbody()->setMassProps(mass, inertiaTensor);
        Enemies[_index]->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK);

        randpos.set_seed((unsigned int)_index ^ 0xf963af);
        float pos_x;
        float pos_z;
        if (_index < 3)
        {
          pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
          pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);
        }
        else if (_index < 6)

        {
          pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
          pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
        }

        else if (_index < 8)
        {
          pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
          pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
        }
        else
        {
          pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
          pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);
        }
        Enemies[_index]->getbody()->translate(btVector3(pos_x, 110, pos_z));
        deadAI->isdead = false;
      }
    }
    /** @fn  void recycleallenemies()
    *   @brief This function is called from the second night to reiniti all the dead enemy and place him at a random loaction this function is only called in night time.
    */

    void init_shingingbox()
    {
      b = new param_shader("src/examples/minecraft_wars/base_vertex.vs", "src/examples/minecraft_wars/shining.fs");
      shiningbox = new material(vec4(1.0f, 1.0f, 1.0f, 1.0f), b);
      atom_t atom_shinn = app_utils::get_atom("shnn");
      shnn = shiningbox->add_uniform(&shnnval, atom_shinn, GL_FLOAT, 1, param::stage_fragment);

      atom_t atom_m_amb = app_utils::get_atom("m_ambient");
      amb = vec3(0.4f, 0.4f, 0.4f);
      m_amb = shiningbox->add_uniform(&amb, atom_m_amb, GL_FLOAT_VEC3, 1, param::stage_fragment);

      atom_t atom_l_amb = app_utils::get_atom("l_ambient");
      lamb = vec3(0.55f, 0.55f, 0.55f);
      l_amb = shiningbox->add_uniform(&lamb, atom_l_amb, GL_FLOAT_VEC3, 1, param::stage_fragment);

      atom_t atom_m_diff = app_utils::get_atom("m_diffuse");
      diff = vec3(0.6f, 0.6f, 0.6f);
      m_dif = shiningbox->add_uniform(&diff, atom_m_diff, GL_FLOAT_VEC3, 1, param::stage_fragment);

      atom_t atom_l_diff = app_utils::get_atom("l_diffuse");
      ldiff = vec3(0.6f, 0.6f, 0.6f);
      l_dif = shiningbox->add_uniform(&ldiff, atom_l_diff, GL_FLOAT_VEC3, 1, param::stage_fragment);

      atom_t atom_m_spec = app_utils::get_atom("m_specular");
      spec = vec3(0.9f, 0.9f, 0.9f);
      m_spec = shiningbox->add_uniform(&spec, atom_m_spec, GL_FLOAT_VEC3, 1, param::stage_fragment);

      atom_t atom_l_spec = app_utils::get_atom("l_specular");
      lspec = vec3(1.0f, 1.0f, 1.0f);
      l_spec = shiningbox->add_uniform(&lspec, atom_l_spec, GL_FLOAT_VEC3, 1, param::stage_fragment);

      atom_t atom_lightpos = app_utils::get_atom("lightpos");
      light_pos = vec3(app_scene->get_light_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_light_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_light_instance(0)->get_node()->access_nodeToParent().w().z());
      lightpos = shiningbox->add_uniform(&light_pos, atom_lightpos, GL_FLOAT_VEC3, 1, param::stage_fragment);
    }
    /** @fn  void init_shingingbox()
    *   @brief This function is used to init the lighing shader
    */
    void init_player_var()
    {
      isgameover = false;
      canjump = true;
      framecount_jump = 20.0f;

      gathering = false;
      framecount_gathering = 15;

      placing = false;
      framecount_placing = 15.0f;

      canshoot = true;
      framecount_shooting = 15.0f;
      
      activate_enemies = false;
    }
    /** @fn  void init_player_var()
    *   @brief This function is used to init the player vars ie gathering palcing shooting canjump canshoot......
    */

    void clearpopup()
    {
      if (popupmsg != Popup_Msg::Msg_None)
      {
        if (tempframpopup > framepopup)
        {
          popupmsg = Popup_Msg::Msg_None;
        }
        else
        {
          tempframpopup++;
        }
      }
    }
    /** @fn   void clearpopup()
    *   @brief This function is used clear the popup UI after the framecount elapse
    */
    void initUI()
    {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      printf(" x %i , y %i", vx, vy);
      overlay = new text_overlay();
      // get the defualt font.
      bitmap_font *font = overlay->get_default_font();
      // create a box containing text (in pixels)
      aabb bb_bot(vec3(vx*7/8, -550, 0.0f), vec3(vx, 200.0f, 0.0f));
      UI_bot = new mesh_text(font, "", &bb_bot);

      aabb bb_top(vec3(vx*7/8, 200, 0.0f), vec3(vx, 200.0f, 0.0f));
      UI_top = new mesh_text(font, "", &bb_top);

      aabb bb_popup(vec3(vx * 7 / 8, -240, 0.0f), vec3(vx, 200.0f, 0.0f));
      UI_popup = new mesh_text(font, "", &bb_popup);

      aabb bb_target(vec3(vx, -100, 0.0f), vec3(vx, 100.0f, 0.0f));
      target = new mesh_text(font, "+", &bb_target);
      // add the mesh to the overlay.

      overlay->add_mesh_text(UI_bot);
      overlay->add_mesh_text(UI_top);
      overlay->add_mesh_text(UI_popup);
      overlay->add_mesh_text(target);
    }
    /** @fn  void initUI()
    *   @brief This function is used to init the UI variables ie overlay textmesh....
    */
    void updateUI(int vx, int vy)
    {
      string current_element;
      switch (The_Player.current_element)
      {
      case 0: current_element = "Mud";
        break;

      case 1: current_element = "Wood";
        break;

      case 2: current_element = "Stone";
        break;

      case 3: current_element = "Iron";
        break;
      }
      UI_bot->clear();
      UI_bot->format(
        "Health : %f          Ammo : %i         Current Element : %s", The_Player.health, The_Player.ammo[ID_weapon::Gun_ID], current_element
        );
      // convert it to a mesh.
      UI_bot->update();


      UI_top->clear();
      UI_top->format(
        "Mud : %i          Wood : %i         Stone : %i          Iron : %i", The_Player.elements_held[ID_Element::Mud_ID], The_Player.elements_held[ID_Element::Wood_ID], The_Player.elements_held[ID_Element::Stone_ID], The_Player.elements_held[ID_Element::Iron_ID]
        );
      // convert it to a mesh.
      UI_top->update();


      UI_popup->clear();
      switch (popupmsg)
      {
      case Popup_Msg::Msg_None:UI_popup->format("");
        break;
      case Popup_Msg::Msg_ZombiesActive:UI_popup->format("!!!!!Zombies Are Comming!!!!!");
        break;
      case Popup_Msg::Msg_CantPlace:UI_popup->format("!Can't Place Here!");
        break;
      case Popup_Msg::Msg_NotEnoughElements:UI_popup->format("!Not Enough Elements!");
        break;
      case Popup_Msg::Msg_OutOfAmmo:UI_popup->format("!Out Of Ammo!");
        break;
      }
      if (isgameover)
      {
        UI_popup->format("!Game Over!\n press esc to exit");
      }
      UI_popup->update();
      // draw the text overlay
      overlay->render(vx, vy);

    }
    /** @fn  void updateUI(int vx, int vy)
    *   @brief This function is used update the UI
    *   @param int vx The width of the window
    *   @param int vy The height od the window
    */

    void InitDayNightCycle()
    {
      mat4t modelToWorld;
      mat4t modelToWorld_sun;
      mat4t modelToWorld_moon;
      mat4t modelToWorld_sky;
      mat4t modelToWorld_sunlight;

      modelToWorld_sunlight.loadIdentity();
      modelToWorld_sunlight.translate(4900, 0, 0);
      modelToWorld_sunlight.rotateY(-90);

      modelToWorld_sky.loadIdentity();
      modelToWorld.loadIdentity();
      modelToWorld_sun.loadIdentity();
      modelToWorld_sun.translate(5000, 0, 0);
      modelToWorld_moon.loadIdentity();
      modelToWorld_moon.translate(-5000, 0, 0);
      daynightnode = new scene_node(modelToWorld, atom_);
      app_scene->add_child(daynightnode);
      scene_node *sky_node = new scene_node(modelToWorld_sky, atom_);
      scene_node *sun_node = new scene_node(modelToWorld_sun, atom_);
      scene_node *moon_node = new scene_node(modelToWorld_moon, atom_);
      scene_node *sunlight_node = new scene_node(modelToWorld_sunlight, atom_);

      mesh_sphere *sky = new mesh_sphere(vec3(0, 0, 0), 5100);
      mesh_sphere *sun = new mesh_sphere(vec3(5000, 0, 0), 200);
      mesh_sphere *moon = new mesh_sphere(vec3(-5000, 0, 0), 200);
      /*
      SPOT LIGHT IS't WORKING
      light *lighttype = new light(atom_spot);
      lighttype->set_kind(atom_spot);
      lighttype->set_near_far(0.1,150);
      light_instance *sunlight = new light_instance(sunlight_node,lighttype);
      sunlight->set_light(lighttype);
      app_scene->add_light_instance(sunlight);
      sun_node->add_child(sunlight_node);
      */
      material *sun_mat = new material(vec4(0.992, 0.721, 0.074, 1));
      material *moon_mat = new material(vec4(0.266, 0.388, 0.956, 1));
      material *sky_mat = new material(vec4(0.247, 0.3039, 0.580, 1));
      app_scene->add_mesh_instance(new mesh_instance(sun_node, sun, sun_mat));
      app_scene->add_mesh_instance(new mesh_instance(moon_node, moon, moon_mat));
      app_scene->add_mesh_instance(new mesh_instance(sky_node, sky, sky_mat));
      daynightnode->add_child(sun_node);
      daynightnode->add_child(moon_node);
    }  
    /** @fn  void InitDayNightCycle()
    *   @brief This function is used to init the day night cycle
    */
    void DayNightCycle()
    {
      if (!isday && !enemiesrecycled)
      {
        if (nightnumber == 0)
        {
          activateenemies();
        }
        else
        {
          if (!enemiesrecycled)
          {
            recycleallenemies();
          }
        }
        enemiesrecycled = true;
        nightnumber++;
      }  


      daynightnode->access_nodeToParent().rotateZ(0.01);
      app_scene->get_light_instance(0)->get_node()->access_nodeToParent().rotateX(-0.01);
      gamedaytime += 0.01;

      if (gamedaytime > 360)
      {
        gamedaytime -= 360;
      }
      if (gamedaytime > 0 && gamedaytime < 200 && !isday)
      {
        isday = true;
        killallenemies();
      }
      if (gamedaytime < 360 && gamedaytime>200 && isday)
      {
        isday = false;
        enemiesrecycled = false;
      }
    }
    /** @fn  void DayNightCycle()
    *   @brief This function is used to mantain the day night cycle. Ie the movement of the sun and the moon and the spawning and recycling of the enemies
    */
  public:
    /// this is called when we construct the class before everything is initialised.
    minecraft_wars(int argc, char **argv) : app(argc, argv) {
      dispatcher = new btCollisionDispatcher(&config);
      broadphase = new btDbvtBroadphase();
      solver = new btSequentialImpulseConstraintSolver();
      world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, &config);
    }

    ~minecraft_wars() {
      delete world;
      delete solver;
      delete broadphase;
      delete dispatcher;
      delete elementshape;
      delete daynightnode;
      for (unsigned int i = 0; i < Game_objects.size(); i++)
        delete Game_objects[i];

      for (unsigned int i = 0; i < Enemies.size(); i++)
        delete Enemies[i];
    }
    /// this is called once OpenGL is initialized
    void app_init() {
      ShowCursor(false);
      world->setGravity(btVector3(0.0f, -50.0f, 0.0f));
      app_scene = new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_light_instance(0)->get_node()->access_nodeToParent().loadIdentity();
      app_scene->get_camera_instance(0)->set_far_plane(5500);
      initUI();
      init_shingingbox();
      init_player_var();
      enemy_material = new material(vec4(0, 0, 0, 1));
      InitDayNightCycle();

      app_scene->get_light_instance(0)->get_node()->access_nodeToParent().rotateY(90);
      isday = true;     
      gamedaytime = 0;
      nightnumber = 0;
      enemiesrecycled = false;


      Add_Rigidbody_to_Cam();
      Create_base();
      mapmaker();
      gContactAddedCallback = minecraftcollision;

      tempframe_place_enemy = 0;
      nu_enemies = 10;
      popupmsg = Popup_Msg::Msg_None;

      Sound_shoot = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "src/examples/minecraft_wars/sounds/shot.wav");
      Sound_zombieattak = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "src/examples/minecraft_wars/sounds/zombieatt.wav");
      Sound_place = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "src/examples/minecraft_wars/sounds/drop.wav");
      Sound_gather = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "src/examples/minecraft_wars/sounds/hammer.wav");
      alGenSources(4, sources);
      
    }
    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);
      updateUI(vx, vy);

      world->stepSimulation(1.0f / 30);
      //for each rigid body in the world we will find the position of the cube and refresh the position of the rendered cube.
      for (unsigned i = 0; i != Game_objects.size(); ++i) {
        btRigidBody *rigid_body = Game_objects[i]->getbody();
        btQuaternion btq = rigid_body->getOrientation();
        btVector3 pos = rigid_body->getCenterOfMassPosition();
        quat q(btq[0], btq[1], btq[2], btq[3]);
        //forming the modelToWorld matrix
        mat4t modelToWorld;
        if (i != The_Player.index)
        {
          modelToWorld = q;//rotation but not for the camera
          modelToWorld[3] = vec4(pos[0], pos[1], pos[2], 1);//position
          nodes[i]->access_nodeToParent() = modelToWorld;//apply to the node        
        }
      }

      // update matrices. assume 30 fps.
      app_scene->update(1.0f / 30);
      if (is_key_down(key::key_esc))
      {
        exit(1);
      }
      app_scene->render((float)vx / vy);

      if (!isgameover)
      {
      Player_cont();
      placingelement();
      gatheringelement();
      shootreload();
      DayNightCycle();
      enemyconts();
      waitforjump();
      clearpopup();
      }
    }
  };
  /**
  * @class minecraft_wars : public app
  * @brief this is the main class of this project.
  */
}
