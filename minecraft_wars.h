////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
// Modular Framework for OpenGLES2 rendering on multiple platforms. 
//Himanshu Chablani Minecraft Wars 2014 : A simple project in which the player has to collect elements, build a fort, and defend his fort as long as he can 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace octet {
  ///The Number of elements in the game ie : Mud, Wood, Stone, Iron
#define Nu_Elements 4 
  ///The Number of weapons in the game ie : Pickaxe, gun  
#define Nu_Weapon 1
  ///The ID for the different objects. Used to check what kind of an object the rigid body is from the user pointer.
  enum ID_Object
  {
    Plane_ID = -1,
    Player_ID = 0,
    Element_ID = 1,
    AI_ID = 2,
    Pickup_ID =3
  };

  enum ID_Pickup
  {
    Health_ID =0,
    Ammo_ID =1
  };
  enum ID_Element
  {
    Mud_ID = 0,
    Wood_ID = 1,
    Stone_ID = 2,
    Iron_ID = 3
  };

  enum ID_weapon
  {
    Gun_ID = 0
  };

  enum Hits_to_extract_Element
  {
    Mud_Hits = 2,
    Wood_Hits = 3,
    Stone_Hits = 5,
    Iron_Hits = 7
  };
  ///The class that stores the basic common information about the game objects ie ID, Rigidbody, and the pointer to the specific object.
  class CollisionObjects
  {
    int id;//The ID of the Object ie player, mud, stone , wood, iron , AI
    btRigidBody *body;
    void *objectclass;
  public:
    CollisionObjects(int _id, btRigidBody *_body, void *_objectclass)
    {
      id = _id;
      body = _body;
      objectclass = _objectclass;
    }

    int getid()
    {
      return id;
    }

    btRigidBody* getbody()
    {
      return body;
    }

    void* getobjectclass()
    {
      return objectclass;
    }
  };

  ///The Player struct contains all the relivent info about the player
  struct Player
  {
    float health;
    int elements_held[Nu_Elements];
    dynarray<int> elements_held_ids[Nu_Elements];
    int ammo[Nu_Weapon];
    float weapon_damage[Nu_Weapon];
    int current_element;
    int current_weapon;
    int index;//The Index of this class in GameObject dynarry

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
        ammo[i] = 0;
      }
      weapon_damage[0]=20.0f;

      current_element = ID_Element::Mud_ID;
      current_weapon = ID_weapon::Gun_ID;
      
    }

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

    void Element_Picked(int element_number, int element_index)
    {
      elements_held[element_number]++;
      elements_held_ids[element_number].push_back(element_index);
    }

    int Element_Placed(int element_number)
    {
      dynarray<int>::iterator tempind = elements_held_ids[element_number].end();;
      --elements_held[element_number];
      elements_held_ids[element_number].pop_back();
      return *tempind;
    }

    void Ammo_Picked(int weapon_nu, int amount)
    {
      ammo[weapon_nu] += amount;
    }

    bool Weapon_fired(int weapon_nu)
    {
      if (ammo[weapon_nu]>0)
      { 
      --ammo[weapon_nu];
      return true;
      }
      return false;
    }
  };
  
  struct AI
  {
    float health;
    int index;
    float damage;
    bool canattack;
    float framestoattack;
    float tempframes;
    AI(int _index)
    {
      health = 50.0f;
      damage = 15;
      index = _index;
    }
   
    bool Got_Hit(float damage)
    {
      if (health - damage > 0.0f)
      {
        health -= damage;
        return true;
      }      
        return false;
      
    }

    void updatecanattack()
    {
      if (!canattack)
      { 
        if (tempframes > framestoattack)
        {
          canattack=true;
        }
        else
        {
          tempframes++;
        }
      }    
    }
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

  };

  ///The base struct for all the elements.Use this in the userpointer and call the derived class constr to init this.
  struct Elements
  {
    int id;//the id of the element
    string tag;
    float health;
    int hit_to_extraxt;
    vec4 color;
    int index;//The Index of this class in GameObject dynarry use game_object.size()-1
    
    Elements(int _id, string _tag, int _index, float _health, int _hit_to_extract, vec4 _color)
    {
      id = _id;
      tag = _tag;
      index = _index;
      health = _health;
      hit_to_extraxt = _hit_to_extract;
      color = _color;
      
    }
    bool Got_Hit(float damage)
    {
      if (health - damage > 0.0f)
      {
        health -= damage;
        return true;
      }
      return false;

    }

  };

  ///The Mud struct derived from the element struc. Only use to init the element
  struct Mud : public Elements
  {

    Mud(int index) : Elements(ID_Element::Mud_ID, "Mud", index, 20.0f, 2, vec4(0.4078f, 0.196f, 0.0352f, 1.0f))
    {
    }

  };

  ///The Wood struct derived from the element struc. Only use to init the element
  struct Wood : public Elements
  {
    Wood(int index) : Elements(ID_Element::Wood_ID, "Wood", index, 35.0f, 3, vec4(0.9098f, 0.666f, 0.4784f, 1.0f))
    {

    }

  };
  ///The Stone struct derived from the element struc. Only use to init the element
  struct Stone : public Elements
  {
    Stone(int index) : Elements(ID_Element::Stone_ID, "Stone", index, 45.0f, 5, vec4(0.3529f, 0.3529f, 0.3529f, 1.0f))
    {}
  };
  ///The Iron struct derived from the element struc. Only use to init the element
  struct Iron : public Elements
  {
    Iron(int index) : Elements(ID_Element::Iron_ID, "Iron", index, 65.0f, 7, vec4(0.5529f, 0.5529f, 0.5529f, 1.0f))
    {}
  };

  ///This class handels the in game UI //haveto create for all elements and the player health weapon... 
  class game_UI : public resource {
  private:
    // helper for drawing text
    text_overlay *overlay;
    // text mesh object for overlay.
    mesh_text *text;

  public:

    game_UI()
    {


    }


    void init()
    {
      // create the overlay
      overlay = new text_overlay();
      // get the defualt font.
      bitmap_font *font = overlay->get_default_font();
      // create a box containing text (in pixels)
      aabb bb(vec3(64.5f, -200.0f, 0.0f), vec3(256, 64, 0));
      text = new mesh_text(font, "", &bb);
      // add the mesh to the overlay.
      overlay->add_mesh_text(text);
    }

    // make for the rest of the elements aswell just in a line 
    void display_elements(int vx, int vy, int player_mud)
    {
      text->clear();
      text->format(
        "mud : %i \n", player_mud
        );
      // convert it to a mesh.
      text->update();
      // draw the text overlay
      overlay->render(vx, vy);

    }

  };

  ///The collision callback fucntion this is called when a collion takes place.
  bool minecraftcollision(btManifoldPoint &cp, const btCollisionObjectWrapper *obj1, int id1, int index1, const btCollisionObjectWrapper *obj2, int id2, int index2)
  {
    CollisionObjects *Obj1userpointer = (CollisionObjects*)obj1->getCollisionObject()->getUserPointer();
    CollisionObjects *Obj2userpointer = (CollisionObjects*)obj2->getCollisionObject()->getUserPointer();
    Elements *The_Element;
    AI *The_AI;
    Player *The_Player;
    if ((Obj1userpointer->getid() == ID_Object::AI_ID || Obj2userpointer->getid() == ID_Object::AI_ID) && (Obj1userpointer->getid() != ID_Object::Plane_ID || Obj2userpointer->getid() != ID_Object::Plane_ID))
    {
      if (Obj1userpointer->getid() == ID_Object::AI_ID)
      {
        The_AI = (AI*)Obj1userpointer->getobjectclass();
        if (Obj2userpointer->getid() == ID_Object::Player_ID)
        {         
          The_Player = (Player *)Obj2userpointer->getobjectclass();
          if (!The_Player->Got_Hit(The_AI->attacked()))
          {
            //Game Over 
          }
        }

        if (Obj2userpointer->getid() == ID_Object::Element_ID)
        {
          The_Element = (Elements *)Obj2userpointer->getobjectclass();
          if(The_Element->Got_Hit(The_AI->attacked()))
          {
            Obj2userpointer->getbody()->setMassProps(0, btVector3(0.0f, 0.0f, 0.0f));
            Obj2userpointer->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
            Obj2userpointer->getbody()->translate(btVector3(0, -100, 0));

          }
        }



      }

    }
    return false;
  }

  class minecraft_wars : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;

    //the player , elements in the world (which will be linked to the user pointer of the rigid bodies of the objects), the UI for the game(currently Text)
    Player The_Player;
    dynarray<Elements*> Element_list;
    game_UI UI;

    //The Bullet Physics    
    btDefaultCollisionConfiguration config;       /// setup for the world
    btCollisionDispatcher *dispatcher;            /// handler for collisions between objects
    btDbvtBroadphase *broadphase;                 /// handler for broadphase (rough) collision
    btSequentialImpulseConstraintSolver *solver;  /// handler to resolve collisions
    btDiscreteDynamicsWorld *world;             /// physics world, contains rigid bodies


    dynarray<CollisionObjects*> Game_objects; //the game objects this contains the rigidbodies gameobj class and the id for the object.
    dynarray<scene_node*> nodes; // the nodes for the game just used for the rendering

    dynarray<CollisionObjects*> Enemies;
    int map_grid[100][20][100];

    bool gathering;//this indicates that the player is gather something so deactivate all conts
    bool placing;//this indicates that the player is placing something so deactive his place funtion and reactive after a set amount of frmes say 30 = 1 sec;
    bool canshoot;//true if player can shoot; if he has shot then make this false and true again after set frames
    float  framecount_gathering;//multiplay this with 30*nu of sec to collect the item and then chk of frams
    float tempframe_gathering;//set =0 when gathering chk with framecount to see if can gather or not 
    float  framecount_shooting;//multiplay this with 30*nu of sec to shoot then chk of frams
    float tempframe_shooting;//set =0 when shooting chk with framecount to see if can gather or not 
    float  framecount_placing;//multiplay this with 30*nu of sec to place the item and then chk of frams
    float tempframe_placing;//set =0 when placing chk with framecount to see if can gather or not 

    float framecount_place_enemy;//the min frams after which the enemies will come to attack the player
    float tempframe_place_enemy;
    float nu_enemies;//number of enemies to make 
    bool activate_enemies;//activate the enemies after the framecount is complete

    bool canjump;
    float framecount_jump;
    float tempframecount_jump;

    Elements *element;//the element we are gathering 


    //the lighting shader 
    param_shader *b;

    //the paramerter set for the shader need to add at set up and modify in draw
    ref<param_uniform> m_amb;
    ref<param_uniform> l_amb;
    ref<param_uniform> m_dif;
    ref<param_uniform> l_dif;
    ref<param_uniform> m_spec;
    ref<param_uniform> l_spec;
    ref<param_uniform> shnn;
    ref<param_uniform> lightpos;

    //the material
    ref<material> shiningbox;
    ref<material> enemy_material;
    //parameter value
    float shnnval = 32;
    vec3 amb;
    vec3 lamb;
    vec3 diff;
    vec3 ldiff;
    vec3 spec;
    vec3 lspec;
    vec3 light_pos;

    //test 
    bool b_light;


    btCollisionShape *elementshape;
    btCollisionShape *enemyshape;

    void mapmaker()
    {
      mat4t modelToWorld;
      elementshape = new btBoxShape(get_btVector3(vec3(1.0f, 1.0f, 1.0f)));
      btDefaultMotionState *motionState;
      btScalar mass = 100.0f;
      btVector3 inertiaTensor;
      btRigidBody * rigid_body;
      CollisionObjects *element_object;
      for (int i = 0; i < 10; i++)
      {
        modelToWorld.loadIdentity();
        modelToWorld.translate(0, 0, -4 - 8 * i);
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
        Elements *newele = new Iron(Game_objects.size());
        element_object = new CollisionObjects(ID_Object::Element_ID, rigid_body, newele);
        rigid_body->setUserPointer(element_object);
        Game_objects.push_back(element_object);
        mesh_box *box = new mesh_box(vec3(1, 1, 1));
        scene_node *node = new scene_node(modelToWorld, atom_);
        nodes.push_back(node);
        app_scene->add_child(node);

        app_scene->add_mesh_instance(new mesh_instance(node, box, shiningbox));


      }
    }

    void Add_Rigidbody_to_Cam()
    {
      mat4t cammat = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      cammat.loadIdentity();
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
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


    vec3 matrixmult(mat4t objm, vec3 direction)
    {
      vec3 temp(objm.x().x()*direction.x() + objm.y().x()*direction.y() + objm.z().x()*direction.z(),
        objm.x().y()*direction.x() + objm.y().y()*direction.y() + objm.z().y()*direction.z(),
        objm.x().z()*direction.x() + objm.y().z()*direction.y() + objm.z().z()*direction.z());
      return temp;
    }

    //if include xbox cont add or for the keys and fun
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
          app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + 8*lookdir.y(),
          app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 8 * lookdir.z()));


          world->rayTest(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(),
           app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(),
            app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()),

             btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 8 * lookdir.x(), 
             app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + 8* lookdir.y(),
              app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 8 * lookdir.z()), gatherray);

          if (gatherray.hasHit())
          {
            CollisionObjects *Objuserpointer = (CollisionObjects*)gatherray.m_collisionObject->getUserPointer();
            printf("gathering object : %d \n", Objuserpointer->getid());
            if (Objuserpointer->getid() == ID_Object::Element_ID)
            {
              element = (Elements*)Objuserpointer->getobjectclass();
              if (element->hit_to_extraxt > 1)
              {
                gathering = true;
                element->hit_to_extraxt--;
                
                tempframe_gathering = 0;
                printf("gathering element : %s  Index : %d\n", element->tag, element->index);
                printf("hits remaining %d\n",  element->hit_to_extraxt);
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
        canshoot = false;
        tempframe_shooting=0;
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
            printf("body pos : %f %f %f \n shot pos : %f %f %f \n normal force pos %f %f %f\n\n ",pos.x(),pos.y(),pos.z(),hitpoint.x(),hitpoint.y(),hitpoint.z(),normaldir.x(),normaldir.y(),normaldir.z());
            
            float forcemult = 2000.0f;
            if (Objuserpointer->getid()==ID_Object::AI_ID)
            {
            
              AI *hitAI= (AI *)Objuserpointer->getobjectclass();
              printf("AI Health %f",hitAI->health);
              if (!hitAI->Got_Hit(The_Player.weapon_damage[The_Player.current_weapon]))
              {
                recycle_enemies(hitAI->index);
                printf("Enemy died %d",hitAI->index);
              }
            }
            tempbody->applyImpulse(btVector3(forcemult*normaldir.x(), forcemult*normaldir.y(), forcemult*normaldir.z()), normaldir);
            
          }
        }
      
      }

      //test change the key 
      if (is_key_down(key::key_tab) && !placing)
      {
        if (The_Player.elements_held[The_Player.current_element] > 0)
        {
          dynarray<int>::iterator tempele = The_Player.elements_held_ids[The_Player.current_element].begin();
          
         
          btCollisionWorld::ClosestRayResultCallback placeray(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()), btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 2 * lookdir.x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 2 * lookdir.z()));
          world->rayTest(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()), btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 2* lookdir.x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 2 * lookdir.z()), placeray);
          
          if (placeray.hasHit())
          { 
            //replace with UI
            printf("Cant place here");
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
          //replace with UI
          printf("not enough %d element \n\n", The_Player.current_element);
        }
      }

      if (canjump && is_key_down(key::key_space) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->applyCentralImpulse(btVector3(0, 150, 0));
        tempframecount_jump=0;
        canjump=false;
      }
      if (is_key_down(key::key_up) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->setLinearVelocity(btVector3(10 * forworddir.x(), 0, 10 * forworddir.z()));
      }
      if (is_key_down(key::key_down) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->setLinearVelocity(btVector3(10 * backdir.x(), 0, 10 * backdir.z()));
      }
      if (is_key_down(key::key_right) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->setLinearVelocity(btVector3(10 * rightdir.x(), 0, 10 * rightdir.z()));
      }
      if (is_key_down(key::key_left) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->setLinearVelocity(btVector3(10 * leftdir.x(), 0, 10 * leftdir.z()));
      }

      /*
      int vx = 0, vy = 0;
      int x,y;
      get_viewport_size(vx, vy);
      get_mouse_pos(x,y);
      btRigidBody *rigid_body = Game_objects[The_Player.index]->getbody();
      btQuaternion btq = rigid_body->getOrientation();
      btVector3 pos = rigid_body->getCenterOfMassPosition();
      quat q(btq[0], btq[1], btq[2], btq[3]);
      mat4t modelToWorld;

      modelToWorld = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      modelToWorld[3] = vec4(pos[0], pos[1], pos[2], 1);//position
      modelToWorld.loadIdentity();
      modelToWorld.rotateY((float)-x*2.0f);

      if (vy / 2 - y < 40 && vy / 2 - y > -40)
      modelToWorld.rotateX(vy / 2 - y);
      if (vy / 2 - y >= 40)
      modelToWorld.rotateX(40);
      if (vy / 2 - y <= -40)
      modelToWorld.rotateX(-40);

      modelToWorld[3] = vec4(pos[0], pos[1], pos[2], 1);//position
      nodes[The_Player.index]->access_nodeToParent() = modelToWorld;//apply to the node
      */

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
      modelToWorld[3] = vec4(pos[0], pos[1], pos[2], 1);//position      


      modelToWorld.rotateY((float)-x*2.0f);
      if (vy / 2 - y < 40 && vy / 2 - y > -40)
        modelToWorld.rotateX(vy / 2 - y);
      if (vy / 2 - y >= 40)
        modelToWorld.rotateX(40);
      if (vy / 2 - y <= -40)
        modelToWorld.rotateX(-40);

      nodes[The_Player.index]->access_nodeToParent() = modelToWorld;//apply to the node
    }

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
              int element_index = The_Player.Element_Placed(The_Player.current_element);
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
            btVector3 pos = Game_objects[element->index]->getbody()->getCenterOfMassPosition();
            Game_objects[element->index]->getbody()->translate(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() +1 * lookdir.x() - pos.x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y() + lookdir.y() - pos.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 1*lookdir.z() -  pos.z()));
           
            btScalar mass = 100.0f;
            btVector3 inertiaTensor;
            elementshape->calculateLocalInertia(mass, inertiaTensor);
            Game_objects[element->index]->getbody()->setMassProps(mass, inertiaTensor);
            Game_objects[element->index]->getbody()->setCollisionFlags(btCollisionObject::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK);           
            }
            placing = false;
          }
          else
          {
            tempframe_placing++;
          }       

      }
    }

    void shootreload()
    {
      if (!canshoot)
      {
        if (tempframe_shooting>= framecount_shooting)
        {
          canshoot = true;
        }
        else
        {
          tempframe_shooting++;
        }
      }
    }

    void activateenemies()
    {
      if (!activate_enemies)
      {
        
        if (tempframe_place_enemy >= framecount_place_enemy)
        {
          activate_enemies=true;  
          printf("enemies activated !!!");
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
            tempframe_place_enemy+=399959;

            float pos_x;
            float pos_z;
            if (i<3)
            {
              pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
              pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);
            }
            else if (i<6)
              
            {
              pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
              pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
            }

            else if (i<8)
            {
              pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
              pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
            }
            else
            {
              pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
              pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);

            }
            printf("pos_x %f, pos_z %f \n\n",pos_x,pos_z);
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

            mesh_box *box = new mesh_box(vec3(1, 1,1));
            scene_node *node = new scene_node(modelToWorld, atom_);
            nodes.push_back(node);
            app_scene->add_child(node);
            app_scene->add_mesh_instance(new mesh_instance(node, box, enemy_material));


          }          
        }
        else
        {
        tempframe_place_enemy++;
        }
      }
    }

    void enemyconts()
    {
      if (activate_enemies)
      {
        btVector3 playerpos = Game_objects[The_Player.index]->getbody()->getCenterOfMassPosition();
        for (int i=0;i<Enemies.size();i++)
        {
          if (Enemies[i]->getbody()->getLinearVelocity().y() < 0.1f && Enemies[i]->getbody()->getLinearVelocity().y() > - 0.1f)
          { 
          btVector3 enpos = Enemies[i]->getbody()->getCenterOfMassPosition();
          btVector3 relpos = btVector3(playerpos - enpos);
          relpos.normalize();          
          Enemies[i]->getbody()->applyCentralForce(2000 * btVector3(relpos.x(), 0, relpos.z()));
          AI *thisAI = (AI *)Enemies[i]->getobjectclass();
          thisAI->updatecanattack();
          }
        }
        
      }
    }

    void waitforjump()
    {
      if (!canjump)
      {
        if (tempframecount_jump >= framecount_jump)
        {
          canjump=true;
        }
        else
        {
        tempframecount_jump++;
        }
      }
    }

    void recycle_enemies(int _index)
    {
      btVector3 pos = Game_objects[_index]->getbody()->getCenterOfMassPosition();
      random randpos;
      btVector3 playerpos = Game_objects[The_Player.index]->getbody()->getCenterOfMassPosition();
      AI *deadAI = (AI *)Game_objects[_index]->getobjectclass();
      deadAI = new AI(_index);
      printf("AI Health %f",deadAI->health);
            
      randpos.set_seed((unsigned int)_index ^ 0xf963af);
      float pos_x;
      float pos_z;
      if (_index<3)
      {
        pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
        pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);
      }
      else if (_index<6)

      {
        pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
        pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
      }

      else if (_index<8)
      {
        pos_x = randpos.get(playerpos.x() + 200.0f, playerpos.x() + 500.0f);
        pos_z = randpos.get(playerpos.z() - 200.0f, playerpos.z() - 500.0f);
      }
      else
      {
        pos_x = randpos.get(playerpos.x() - 200.0f, playerpos.x() - 500.0f);
        pos_z = randpos.get(playerpos.z() + 200.0f, playerpos.z() + 500.0f);

      }

      Game_objects[_index]->getbody()->translate(btVector3(pos_x,10,pos_z));      
    }

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
      for (unsigned int i = 0; i < Game_objects.size(); i++)
        delete Game_objects[i];

      for (unsigned int i = 0; i < Enemies.size(); i++)
        delete Enemies[i];
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      //ShowCursor(false);
      world->setGravity(btVector3(0.0f, -50.0f, 0.0f));
      app_scene = new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_light_instance(0)->get_node()->access_nodeToParent().loadIdentity();
      app_scene->get_light_instance(0)->get_node()->access_nodeToParent().translate(0, 40, 0);
      app_scene->get_camera_instance(0)->set_far_plane(1000);
      b = new param_shader("src/examples/minecraft_wars/base_vertex.vs", "src/examples/minecraft_wars/shining.fs");
      shiningbox = new material(vec4(1.0f, 1.0f, 1.0f, 1.0f), b);
      enemy_material = new material(vec4(0, 0, 0, 1));

      Add_Rigidbody_to_Cam();
      Create_base();
      mapmaker();
      gContactAddedCallback = minecraftcollision;

      canjump = true;
      framecount_jump=20.0f;

      gathering = false;
      framecount_gathering = 15;
      
      placing = false; 
      framecount_placing = 15.0f;

      canshoot=true;
      framecount_shooting = 15.0f;

      framecount_place_enemy = 300;//make this 3000 or so 
      activate_enemies=false;
      tempframe_place_enemy =0;
      nu_enemies=10;
      

      printf("Nodes Size : %i  Game_object Size : %i", nodes.size(), Game_objects.size());

      printf("\n%d",'e');
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


      //test
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      //modelToWorld.translate(0, 0, 20);
      mesh_box *box = new mesh_box(vec3(4, 4, 4));
      scene_node *node = new scene_node();
      app_scene->add_child(node);
      material *floor_mat = new material(vec4(1, 1, 0, 1));
      app_scene->add_mesh_instance(new mesh_instance(node, box, floor_mat));
      node->translate(vec3(0, 0, -40));

      {mat4t modelToWorld;
      modelToWorld.loadIdentity();
      //modelToWorld.translate(0, 0, 20);
      mesh_box *box = new mesh_box(vec3(4, 4, 4));
      scene_node *node = new scene_node();
      app_scene->add_child(node);
      material *floor_mat = new material(vec4(1, 0, 0, 1));
      app_scene->add_mesh_instance(new mesh_instance(node, box, floor_mat));
      node->translate(vec3(0, 0, 40)); }

       {mat4t modelToWorld;
       modelToWorld.loadIdentity();
       //modelToWorld.translate(0, 0, 20);
       mesh_box *box = new mesh_box(vec3(4, 4, 4));
       scene_node *node = new scene_node();
       app_scene->add_child(node);
       material *floor_mat = new material(vec4(0, 1, 0, 1));
       app_scene->add_mesh_instance(new mesh_instance(node, box, floor_mat));
       node->translate(vec3(40, 0, 0)); }

        {mat4t modelToWorld;
        modelToWorld.loadIdentity();
        // modelToWorld.translate(0, 0, 20);
        mesh_box *box = new mesh_box(vec3(4, 4, 4));
        scene_node *node = new scene_node();
        app_scene->add_child(node);
        material *floor_mat = new material(vec4(0, 0, 1, 1));
        app_scene->add_mesh_instance(new mesh_instance(node, box, floor_mat));
        node->translate(vec3(-40, 0, 0)); }
      //test end 

    }


    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);

      app_scene->begin_render(vx, vy);

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
      /*
      if (b_light)
      {
      light_pos = vec3(light_pos.x()+1.0f,10.0f,0.0f);
      }
      else
      {
      light_pos = vec3(light_pos.x() - 1.0f, 10.0f, 0.0f);
      }
      if (light_pos.x() > 10.0f)
      {
      b_light = false;
      }
      if (light_pos.x() < -10.0f)
      {
      b_light = true;
      }

      shiningbox->set_uniform(lightpos,&light_pos,sizeof(vec3));
      */
      // draw the scene
      if (is_key_down(key::key_esc))
      {
        exit(1);
      }

      app_scene->render((float)vx / vy);
      //ask about shaders and parama class and how to set the attributes and uniforms for the shaders.
      //b->render();
      Player_cont();

      placingelement();
      gatheringelement();
      shootreload();
      activateenemies();
      enemyconts();
      waitforjump();
    }

    void cam_mouse(int x, int y, HWND *hnd)
    {
      /*
      float senc=5.0f;
      float dx, dy;
      dx = x - vx / 2;
      dy = y - vy / 2;
      dx*=senc;
      dy*=senc;
      camrot.x()+=dx;
      camrot.y()+=dy;
      //test
      printf("The camrot vec2 x : %f y : %f \n",camrot.x(),camrot.y());
      tagPOINT t;
      t.x = vx / 2;
      t.y = vy / 2;
      //ClientToScreen(*hnd, &t);
      SetCursorPos(t.x, t.y);
      */
    }
  };
}
