#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class Info{
public:

    void draw(sf::RenderTarget &rt, int score){

        text.setString("Zdobyte punkty: " + std::to_string(score));


        //rt.draw(rectangle);
        rt.draw(text);
    }

    Info(){

        if(!font.loadFromFile("D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/Roboto-Black.ttf")){
            std::cout<<"blad czcionki";
        }
        text.setFont(font);
        text.setString("0");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        text.setPosition(pos_);

        rectangle = sf::RectangleShape(sf::Vector2f(120, 50));
        rectangle.setSize(sf::Vector2f(150, 50));
        rectangle.setPosition(0,0);
        rectangle.setFillColor(sf::Color(185,122,86,255));
    }
private:
    sf::Vector2f pos_={20,0};
    sf::RectangleShape rectangle;
    sf::Text text;
    sf::Font font;
};


class Platform{
public:
    Platform(std::string path, float sizeX, float sizeY, bool half)
        :pos_(sizeX, sizeY){
        if (!texture.loadFromFile(path)) {
            std::cout<<"Unable to upload your texture";
            return;
        }


        sprite.setTexture(texture);
        if(half){
            float a=size_.x*0.5, b=size_.y;
            size_ = {a, b};
            sprite.setScale(0.5,1);
        }
        sprite.setPosition(pos_);

    }

    sf::Vector2f getPosition(){
        return pos_;
    }


    void drawPlatform(sf::RenderTarget &rt){
        sprite.setPosition(pos_);
        rt.draw(sprite);
    }

    sf::Vector2f getSize(){
        return size_;
    }





private:
    sf::Sprite sprite;
    sf::Texture texture;

    sf::Vector2f pos_ = {0,400}, size_={560,42};
};



class Animation{
public:
    Animation()=default;
    Animation(int x, int y, int width, int height, std::string path, int frames1){
        nFrames=frames1;

        if (!texture.loadFromFile(path)) {
            std::cout<<"Unable to upload your texture";
            return;
        }else{
            for(int i=0; i<nFrames; i++){
                frames.push_back({x+i*width,y,width,height});
            }
        }
    }

    void ApplyToSprite(sf::Sprite& s, const float scale)const{
        s.setTexture(texture);
        s.setTextureRect(frames[iFrame]);
        s.setScale(scale,scale);
    }
    void Update(float dt){
        time+=dt;
        while(time>=holdTime){
            time-=holdTime;
            Advance();
        }
    }
~Animation()=default;
private:
    void Advance(){
        if(++iFrame>=nFrames){
            iFrame = 0;
        }
    }



private:
    int nFrames=8;
    static constexpr float holdTime=0.1f;
    sf::Texture texture;
    std::vector<sf::IntRect> frames;
    int iFrame=0;
    float time=0.0f;
};




class Character {

private:
    enum class AniIndex{
        wUp,
        wDown,
        wLeft,
        wRight,
        wStandRight,
        wStandLeft,
        shotRight,
        shotLeft,
        Count
    };

    enum class AniIndexBullet{
        right,
        left,
        count,
        cur
    };

public:
    Character(const std::string& name, int& hp, sf::Vector2f &pos, std::string path)
            : name_(name), hp_(hp), pos_(pos){


        if(!font.loadFromFile("D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/Roboto-Black.ttf")){
            std::cout<<"blad czcionki";
        }
        text.setFont(font);
        text.setString(std::to_string(hp));
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::Blue);


        sprite_.setTextureRect({0,0,218,228});
        constantAnimations[int(AniIndex::wRight)] = Animation(0,height_int*3,width_int,height_int, path, 8);
        constantAnimations[int(AniIndex::wLeft)] = Animation(0,height_int*2,width_int,height_int, path, 8);
        constantAnimations[int(AniIndex::wStandLeft)] = Animation(0,height_int,width_int,height_int, path, 8);
        constantAnimations[int(AniIndex::wStandRight)] = Animation(0,0,width_int,height_int, path, 8);
        constantAnimations[int(AniIndex::shotLeft)] = Animation(0,height_int*4,width_int,height_int, path, 8);
        constantAnimations[int(AniIndex::shotRight)] = Animation(0,height_int*5,width_int,height_int, path, 8);

    }



    float getHeight(){
        return height_int;
    }

    float getWidth(){
        return width_int;
    }


    void setPosition(float y){
        pos_.y=y;
    }

    void Draw(sf::RenderTarget& rt, bool hero){
        sprite_.setPosition(pos_);

        rt.draw(sprite_);
        if(hero){
            text.setPosition(pos_.x+(width/2)-(fontSize/2), pos_.y-40);
            rt.draw(text);
        }
    }

    void SetDirection(const sf::Vector2f& dir, bool &shot){
        vel_ = dir*speed;
        if(shot && (curAnimation==AniIndex::wRight || curAnimation==AniIndex::wStandRight)){
            curAnimation=AniIndex::shotRight;
        }else if(shot && (curAnimation==AniIndex::wLeft || curAnimation==AniIndex::wStandLeft)){
            curAnimation=AniIndex::shotLeft;
        }else if(dir.x>0.0f && !shot){
            curAnimation = AniIndex::wRight;
        }else if(dir.x<0.0f && !shot){
            curAnimation = AniIndex::wLeft;
        }else if(dir.x==0 && !shot && (curAnimation==AniIndex::wLeft || curAnimation==AniIndex::shotLeft)){
            curAnimation=AniIndex::wStandLeft;
        }else if(dir.x==0 && !shot && (curAnimation==AniIndex::wRight || curAnimation==AniIndex::shotRight)){
            curAnimation=AniIndex::wStandRight;
        }
    }



    void colide(sf::Vector2f& dir1, sf::RenderWindow &window, sf::Vector2f platPos_, sf::Vector2f platSize, bool &skok2, bool &skok, sf::Vector2f startPos_){
        //left
        if(pos_.x <= 0){
            pos_.x=0;
            dir1.x = 0.0f;
        }

        //right
        if(pos_.x >= window.getSize().x-width){
            pos_.x=window.getSize().x-width;
            dir1.x = 0.0f;
        }

        //top
        if(pos_.y <= 0){
            dir1.y = 0.0f;

        }

        //bottom
        if(pos_.y >= window.getSize().y-height){
            pos_.y = window.getSize().y-height;
            dir1.y = 0.0f;
        }


//kolizje z platformą

        bool prawo=false, lewo=false, gora=false;

        gora = pos_.y+height>=platPos_.y;
        prawo = pos_.x<=platPos_.x+platSize.x;
        lewo = pos_.x+width>=platPos_.x;


        if(gora && pos_.y+height<=platPos_.y+(platSize.y/5) && prawo &&lewo && !skok){
            skok2=false;
            dir1.y=0;
            pos_.y = platPos_.y+1-height;
        }


        if(!skok && startPos_.y!=pos_.y
                && ((pos_.x>platPos_.x+platSize.x && pos_.x<platPos_.x+platSize.x+width && pos_.y+height>=platPos_.y && pos_.y<=platPos_.y+platSize.y)
                    || (pos_.x+width>platPos_.x-width && pos_.x+width<platPos_.x && pos_.y+height>=platPos_.y && pos_.y<=platPos_.y+platSize.y))){
            skok2=true;
        }

    }

    void Update(float dt){
        pos_+=vel_*dt;
        constantAnimations[int (curAnimation)].Update(dt);
        constantAnimations[int (curAnimation)].ApplyToSprite(sprite_, scale);
        sprite_.setPosition(pos_);
    }



    sf::Vector2f getPos(){return pos_;}

    void createBullet(){
        animation_bullet[int(AniIndexBullet::right)] = Animation(0,0,101,40, "D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/pocisk.png", 2);
        animation_bullet[int(AniIndexBullet::left)] = Animation(0,40,101,40, "D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/pocisk.png", 2);

        animation_bullet[int (curAnimationBullet)].ApplyToSprite(sprite_bullet, 0.0f);

    }

    void setBulletPosition(){

        if(curAnimation==AniIndex::wRight || curAnimation==AniIndex::wStandRight || curAnimation==AniIndex::shotRight){
            curAnimationBullet=AniIndexBullet::right;
            vel_bullet_={100.0f*4, 0.0f};
            sprite_bullet.setPosition(pos_.x+width/2, pos_.y+height/6);
        }else{
            curAnimationBullet=AniIndexBullet::left;
            vel_bullet_={-100.0f*4, 0.0f};
            sprite_bullet.setPosition(pos_.x-width/2, pos_.y+height/6);
        }
    }

    void doShot(sf::RenderTarget &rt, float dt){
        animation_bullet[int (curAnimationBullet)].ApplyToSprite(sprite_bullet, 1);
        sprite_bullet.move(vel_bullet_*dt);
        animation_bullet[int (curAnimationBullet)].Update(dt);
        rt.draw(sprite_bullet);
    }

protected:
    int fontSize=20;
    sf::Font font;
    sf::Text text;
    const float scale = 0.65f;
    const float width = 218.0f*scale, height = 228.0f*scale;
    const float width_int = 218, height_int=228, width_bullet=202, height_bullet=40;
    int ktory=0;
    static constexpr float speed = 100.0f*4;
    sf::Vector2f pos_, pos_bullet_;
    sf::Vector2f vel_ = {0.0f, 0.0f}, vel_bullet_={100.0f*4, 0.0f};
    std::string name_;
    int hp_;
    sf::Texture texture_, texture_bullet;
    sf::Sprite sprite_, sprite_bullet;
    Animation constantAnimations[int(AniIndex::Count)], animation_bullet[int(AniIndexBullet::count)];
    AniIndex curAnimation = AniIndex::wRight;
    AniIndexBullet curAnimationBullet = AniIndexBullet::right;
};



class Money{
public:

    Money(std::string path){
        if (!texture.loadFromFile(path)) {
            std::cout<<"Unable to upload your texture";
            return;
        }

        sprite.setTexture(texture);
        sprite.setPosition(pos_);
    }

    void drawMoney(sf::RenderTarget &rt, sf::Vector2f heroPos, float wid, float hei){
        genRandomMoneyPos(heroPos, wid, hei);
        sprite.setPosition(pos_);
        rt.draw(sprite);
    }

    int getScore(){return score;}


    bool collide(sf::Vector2f charPos_, float wid, float hei){

        if(pos_.x>=charPos_.x && pos_.x+width<=charPos_.x+wid && pos_.y>=charPos_.y && pos_.y+height<=charPos_.y+hei){
            score++;
            return true;
        }
        return false;
    }


    void genRandomMoneyPos(sf::Vector2f charPos_, float wid, float hei){
        if(collide(charPos_, wid, hei)){
            int randd=rand()%1401;
            int randd2=rand()%601;
            pos_={float(randd), float(randd2)};
        }
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float width=100, height=100;
    sf::Vector2f pos_ = {300,300};
    int score=0;
};


//class Enemy{

//private:
//    enum class AniIndex{
//        wUp,
//        wDown,
//        wLeft,
//        wRight,
//        wStandRight,
//        wStandLeft,
//        shotRight,
//        shotLeft,
//        Count
//    };

//public:

//    Enemy(const std::string& name, int& hp, sf::Vector2f &pos, std::string path)
//        : name_(name), hp_(hp), pos_(pos){



//    sprite_.setTextureRect({0,0,218,228});
//    constantAnimations[int(AniIndex::wRight)] = Animation(0,height_int*3,width_int,height_int, path, 8);
//    constantAnimations[int(AniIndex::wLeft)] = Animation(0,height_int*2,width_int,height_int, path, 8);
//    constantAnimations[int(AniIndex::wStandLeft)] = Animation(0,height_int,width_int,height_int, path, 8);
//    constantAnimations[int(AniIndex::wStandRight)] = Animation(0,0,width_int,height_int, path, 8);
//    constantAnimations[int(AniIndex::shotLeft)] = Animation(0,height_int*4,width_int,height_int, path, 8);
//    constantAnimations[int(AniIndex::shotRight)] = Animation(0,height_int*5,width_int,height_int, path, 8);

//}

//    void direction(sf::Vector2f velOfHero){
//        if
//    }



//private:
//    sf::Sprite sprite_;
//    Animation constantAnimations[int(AniIndex::Count)];
//    int hp_, speed=1000;
//    sf::Vector2f pos_;
//    std::string name_;
//    const float scale = 0.65f;
//    const float width = 218.0f*scale, height = 228.0f*scale;
//    const float width_int = 218, height_int=228, width_bullet=202, height_bullet=40;

//};
