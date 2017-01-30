#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
void HelloWorld::SetPhysicsWorld(PhysicsWorld *world)
{
	sceneWorld = world;
	//this->sceneWorld->setGravity(Vec2(0.0f, -98.0f));
}
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
	//layer->SetPhysicsWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

	auto spriteA = Sprite::create("sprite_a.png");
	spriteA->setScale(0.3);
	spriteA->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	addChild(spriteA);

	auto spriteB = Sprite::create("sprite_b.png");
	spriteB->setScale(0.3);
	spriteB->setPosition(visibleSize.width * 0.5f, spriteB->getBoundingBox().size.height);
	addChild(spriteB);

	//Exaple on how to create a custom material
	float desiredDensity = 1.0f;
	float desiredRestitution = 1.0f;
	float desiredFriction = 1.0f;
	auto customMaterial = PhysicsMaterial(desiredDensity, desiredRestitution, desiredFriction);
	//Use the custom material to create the body for spriteA
	auto bodyA = PhysicsBody::createBox(spriteA->getTextureRect().size, customMaterial);
	
	//Use the default material to create the PhysicsBody for spriteB
	auto bodyB = PhysicsBody::createBox(spriteB->getTextureRect().size, PHYSICSBODY_MATERIAL_DEFAULT);

	//Select your category bitmask slots for each sprite PhysicsBody group
	                        //                 11111111112222222222333
							//Slots	  12345678901234567890123456789012
	                        //=========================================
	uint32_t bodyACategoryBitmask = 0x00000000000000000000000000000001b; //And switch the selected 
	//																 ^     slot to a 1
	//          Select slot 32 for bodyA ============================|
	uint32_t bodyBCategoryBitmask = 0x00000000000000000000000000000010b; //Do the same for bodyB
	//                                                              ^
	//          Select slot 31 for bodyB ===========================|
	//Set the category bitmasks for each PhysicsBody group
	bodyA->setCategoryBitmask(bodyACategoryBitmask);
	bodyB->setCategoryBitmask(bodyBCategoryBitmask);
	//Set the collision bitmask for each PhysicsBody group
	//We want bodyA to collide with bodyB so we set bodyA's collision bitmask to bodyB's category
	bodyA->setCollisionBitmask(bodyBCategoryBitmask);
	//And vice-versa
	bodyB->setCollisionBitmask(bodyACategoryBitmask);
	
	spriteA->setPhysicsBody(bodyA);
	spriteB->setPhysicsBody(bodyB);

	auto boundaryBody = PhysicsBody::createEdgeBox(this->getBoundingBox().size, PHYSICSBODY_MATERIAL_DEFAULT);
	this->setPhysicsBody(boundaryBody);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
