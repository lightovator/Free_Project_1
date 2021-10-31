/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if (!Scene::init()) return false;

    Button* button01 = Button::create("res/btn_up.png", "res/btn_down.png", "res/btn_disabled.png");
    button01->setPosition(Vec2(120, 150));
    button01->setTitleText("Start");
    button01->setTitleFontSize(24.0f);
    button01->setTitleColor(Color3B(200, 144, 232));
    button01->setTitleFontName("fonts/04b20.ttf");
    button01->setContentSize(Size(200, 50));
    button01->setScale9Enabled(true);
    this->addChild(button01, 1);

    Sprite* spriteBG = Sprite::create("res/galaxy.jpg");
    spriteBG->setPosition(Vec2(1920 / 2, 1080 / 2));
    this->addChild(spriteBG);

    auto evt = EventListenerTouchOneByOne::create();
    evt->onTouchBegan = [=](Touch* t, Event* e)->bool {
        CCLOG("touch: x: %f, y: %f", t->getLocation().x, t->getLocation().y);
        //사실 지금 꼭 필요한 코드는 아닌, 위치 로그를 작성해주는 정도입니다.

        position_x = t->getLocation().x;
        position_y = t->getLocation().y;

        //클릭 한 곳에 새를 생성하는 함수를 입력해야 합니다.
        create_bird(position_x, position_y, birds_collection);

        return true;
    };

    getEventDispatcher()->addEventListenerWithSceneGraphPriority(evt, this);

    return true;
}

void HelloWorld::create_bird(float position_x, float position_y, std::vector<Sprite*> &birds_collection){

    birds_collection.push_back(Sprite::create("res/frame-1.png"));
    birds_collection.back()->setPosition(position_x, position_y);
    birds_collection.back()->setScale(0.2);
    this->addChild(birds_collection.back());

    Animation* animation = Animation::create();
    animation->addSpriteFrameWithFile("res/frame-1.png");
    animation->addSpriteFrameWithFile("res/frame-2.png");

    animation->setDelayPerUnit(0.1);

    Animate* animate = Animate::create(animation);

    birds_collection.back()->runAction(RepeatForever::create(animate));

    //birds_collection.back()->runAction(Sequence::create(DelayTime::create(10.0f), RemoveSelf::create(), nullptr));
    //노드가 아예 사라지면 그 다음 새가 갈 위치를 찾지 못해서 버그가 생기므로 FadeTo의 Opacity를 0으로 하는 식으로 바꿉니다.

    birds_collection.back()->runAction(FadeTo::create(10.0, 0));

    if (birds_collection.size() >= 2) {
        birds_collection.back()->runAction(
            RepeatForever::create(
                Sequence::create(
                    CallFunc::create([=]() {birds_collection.back()->setFlippedX(false); }),
                    EaseExponentialIn::create(
                        MoveTo::create(3.0f, Vec2(birds_collection.at(birds_collection.size() - 2)->getPosition()))
                        //직전 클릭 위치까지 다녀오도록 설정해봅니다.
                    ),
                    CallFunc::create([=]() {birds_collection.back()->setFlippedX(true); }),
                    EaseExponentialIn::create(
                        MoveTo::create(3.0f, Vec2(position_x, position_y))
                    ),
                    nullptr
                )
            )
        );
    }
    else {
        birds_collection.back()->runAction(
            RepeatForever::create(
                Sequence::create(
                    CallFunc::create([=]() {birds_collection.back()->setFlippedX(false); }),
                    EaseExponentialIn::create(
                        MoveTo::create(3.0f, Vec2(1920, position_y))
                        //직전 클릭 위치까지 다녀오도록 설정해봅니다.
                    ),
                    CallFunc::create([=]() {birds_collection.back()->setFlippedX(true); }),
                    EaseExponentialIn::create(
                        MoveTo::create(3.0f, Vec2(position_x, position_y))
                    ),
                    nullptr
                )
            )
        );
    }

}
