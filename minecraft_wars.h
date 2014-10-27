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
#define Nu_Weapon 2
  ///The ID for the different objects. Used to check what kind of an object the rigid body is from the user pointer.
  enum ID_Object
  {
    Plane_ID = -1,
    Player_ID = 0,
    Element_ID = 1,
    Player_Element_ID = 2,
    AI_ID = 3
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
  ///The Weapons in the game.
  enum weapon
  {
    pickaxe = 0,
    gun = 1
  };
  ///The Player struct contains all the relivent info about the player
  struct Player
  {
    float health;
    int elements_held[Nu_Elements];
    dynarray<int> elements_held_ids[Nu_Elements];
    int ammo[Nu_Weapon];
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
      current_weapon = weapon::pickaxe;
    }

    void Got_Hit(float damage)
    {
      if (health - damage < 0.0f)
      {
        //dead go to end screen
      }

      else
      {
        health -= damage;
      }
    }

    void Element_Picked(int element_number)
    {
      elements_held[element_number]++;
    }

    void Element_Placed(int element_number)
    {
      --elements_held[element_number];
    }

    void Ammo_Picked(int weapon_nu, int amount)
    {
      ammo[weapon_nu] += amount;
    }

    void Weapon_fired(int weapon_nu)
    {
      --ammo[weapon_nu];
    }
  };
  ///The base struct for all the elements.Use this in the userpointer and call the derived class constr to init this.
  struct Elements
  {
    string tag;
    float health;
    float time_to_extraxt;
    vec4 color;
    int index;//The Index of this class in GameObject dynarry use game_object.size()-1
    //texture_shader texture; 
    Elements(string _tag, int _index, float _health, float _time_to_extract, vec4 _color)
    {
      tag = _tag;
      index = _index;
      health = _health;
      time_to_extraxt = _time_to_extract;
      color = _color;

    }
  };

  ///The Mud struct derived from the element struc. Only use to init the element
  struct Mud : public Elements
  {

    Mud(int index) : Elements("Mud", index, 20.0f, 2.0f, vec4(0.4078f, 0.196f, 0.0352f, 1.0f))
    {
    }

  };

  ///The Wood struct derived from the element struc. Only use to init the element
  struct Wood : public Elements
  {
    Wood(int index) : Elements("Wood", index, 35.0f, 2.8f, vec4(0.9098f, 0.666f, 0.4784f, 1.0f))
    {}

  };
  ///The Stone struct derived from the element struc. Only use to init the element
  struct Stone : public Elements
  {
    Stone(int index) : Elements("Stone", index, 45.0f, 4.0f, vec4(0.3529f, 0.3529f, 0.3529f, 1.0f))
    {}
  };
  ///The Iron struct derived from the element struc. Only use to init the element
  struct Iron : public Elements
  {
    Iron(int index) : Elements("Iron", index, 65.0f, 7.0f, vec4(0.5529f, 0.5529f, 0.5529f, 1.0f))
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
    Player *The_player;
    if ((Obj1userpointer->getid() == ID_Object::Player_ID || Obj1userpointer->getid() == ID_Object::AI_ID) || (Obj2userpointer->getid() == ID_Object::Player_ID || Obj2userpointer->getid() == ID_Object::AI_ID))
    {
      switch (Obj1userpointer->getid())
      {
      case ID_Object::Plane_ID:
        break;
      case ID_Object::Player_ID:
        The_player = (Player*)Obj1userpointer->getobjectclass();

      }

      switch (Obj2userpointer->getid())
      {
      case ID_Object::Plane_ID:
        break;
      case ID_Object::Player_ID:
        The_player = (Player*)Obj2userpointer->getobjectclass();

      }

      //The_player->Got_Hit(1);
     // printf("collision b/w Object id : %i, Object id : %i, The Player health is : %f \n", Obj1userpointer->getid(), Obj2userpointer->getid(), The_player->health);
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
    int map_grid[100][20][100];

    bool gathering;//this indicates that the player is gather something so deactivate all conts
    bool canshoot;//true if player can shoot; if he has shot then make this false and true again after set frames
    int  framecount;//multiplay this with 30*nu of sec to collect the item and then chk of frams
    int tempframe;//set =0 when gathering or shooting and chk with framecount to see if can shoot or not 
    Elements *element;//the element we are gathering 

    param_shader *b;

    void mapmaker()
    {
      mat4t modelToWorld;       
      btCollisionShape *shape = new btBoxShape(get_btVector3(vec3(2.0f, 2.0f, 2.0f)));     
      btDefaultMotionState *motionState;
      btScalar mass = 100.0f;
      btVector3 inertiaTensor;
      btRigidBody * rigid_body;
      CollisionObjects *element_object;
      for (int i =0; i<10 ; i++)
      {
        modelToWorld.loadIdentity();
        modelToWorld.translate(0,0,-4-8*i);
        btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld));
        btVector3 pos(get_btVector3(modelToWorld[3].xyz()));
        btTransform transform(matrix, pos);
        motionState = new btDefaultMotionState(transform);
        shape->calculateLocalInertia(mass, inertiaTensor);
        rigid_body = new btRigidBody(mass, motionState, shape, inertiaTensor);        
        rigid_body->setFriction(1.0f);
        rigid_body->setCollisionFlags(rigid_body->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK);
        world->addRigidBody(rigid_body);      
        
        Elements *newele = new Iron(Game_objects.size());
        element_object = new CollisionObjects(ID_Object::Element_ID, rigid_body, newele);
        rigid_body->setUserPointer(element_object);
        Game_objects.push_back(element_object);

        mesh_box *box = new mesh_box(vec3(2, 2, 2));
        scene_node *node = new scene_node(modelToWorld, atom_);
        nodes.push_back(node);
        app_scene->add_child(node);

        
        b = new param_shader("src/examples/minecraft_wars/test.vs", "src/examples/minecraft_wars/test.fs");
                  
        material *floor_mat = new material(vec4(newele->color),b);
       
        app_scene->add_mesh_instance(new mesh_instance(node, box, floor_mat));
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
      btCollisionShape *shape = new btBoxShape(get_btVector3(vec3(400, 4, 400)));
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

      mesh_box *box = new mesh_box(vec3(400, 4, 400));
      scene_node *node = new scene_node(modelToWorld, atom_);
      nodes.push_back(node);
      app_scene->add_child(node);
      material *floor_mat = new material(vec4(0, 1, 0, 1));
      app_scene->add_mesh_instance(new mesh_instance(node, box, floor_mat));
    }

    
    vec3 matrixmult(mat4t objm,vec3 direction)
    {
      vec3 temp(objm.x().x()*direction.x() + objm.y().x()*direction.y() + objm.z().x()*direction.z(), 
                objm.x().y()*direction.x() + objm.y().y()*direction.y() + objm.z().y()*direction.z(),  
                objm.x().z()*direction.x() + objm.y().z()*direction.y() + objm.z().z()*direction.z());
      return temp;
    }
    void Player_cont()
    {

      vec3 forworddir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(0, 0, -1));
      vec3 backdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(0, 0, 1));
      vec3 rightdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(1, 0, 0));
      vec3 leftdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(-1, 0, 0));
      vec3 lookdir = matrixmult(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent(), vec3(0, 1, -1));

      //if (!gathering)
      { 
      if (is_key_down(key::key_tab))
      {
        btCollisionWorld::ClosestRayResultCallback gatherray(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()), btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 8 * lookdir.x(), 4 * lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 8 * lookdir.z()));
        world->rayTest(btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()), btVector3(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 8*lookdir.x(), 4 * lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z() + 8 * lookdir.z()), gatherray);
        printf(" from %f,%f,%f to %f,%f,%f \n\n", app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().x() + 8 * lookdir.x(), 4 * lookdir.y(), app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().w().z()+ 8 * lookdir.z());
        if (gatherray.hasHit())
        {
          CollisionObjects *Objuserpointer = (CollisionObjects*)gatherray.m_collisionObject->getUserPointer();
          printf("gathering object : %d \n",Objuserpointer->getid());
          if (Objuserpointer->getid() == ID_Object::Element_ID )
          {
            element = (Elements*)Objuserpointer->getobjectclass();
            gathering=true;
            framecount = element->time_to_extraxt*30;
            tempframe=0;
            printf("gathering element : %s  Index : %d\n",element->tag,element->index);
          }           

        }
      }
      }
      if (is_key_down(key::key_space) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->applyCentralImpulse(btVector3(0, 250, 0));
      }
      if (is_key_down(key::key_up) && (Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() < 0.1f && Game_objects[The_Player.index]->getbody()->getLinearVelocity().y() > -0.1f))
      {
        Game_objects[The_Player.index]->getbody()->setLinearVelocity(btVector3(10*forworddir.x(),0,10*forworddir.z()));
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
      for (unsigned int i = 0; i < Game_objects.size(); i++)
        delete Game_objects[i];
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      ShowCursor(false);
      world->setGravity(btVector3(0.0f, -50.0f, 0.0f));
      app_scene = new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->set_far_plane(1000);
      Add_Rigidbody_to_Cam();
      Create_base();
      mapmaker();
      gContactAddedCallback = minecraftcollision;
      gathering=false;
      printf("Nodes Size : %i  Game_object Size : %i",nodes.size(),Game_objects.size());
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
      // draw the scene
      if (is_key_down(key::key_esc))
      {
        exit(1);
      }
      app_scene->render((float)vx / vy);
      //ask about shaders and parama class and how to set the attributes and uniforms for the shaders.
      //b->render();
      Player_cont();
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
