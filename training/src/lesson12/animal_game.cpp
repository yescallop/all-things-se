#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>
#include <string>

using std::string;

class Animal {
  protected:
    string name;
    int age;

  private:
    static int animalNum;

  public:
    Animal(string name = "Animal") : name(name) { animalNum++; }

    virtual void move() const {};
    virtual void shout() const {};
    string getName() const { return name; }
    int getAge() const { return age; }
    void setName(string value) { name = value; }
    static int getNum() { return animalNum; }
};

int Animal::animalNum = 0;

class Dog : public Animal {
    static int dogNum;

  public:
    Dog(string name = "Dog") : Animal(name) { age = ++dogNum; }

    void move() const override {
        printf("跑 %.1f 英尺！ ", 5 + 0.1 * getAge());
    }
    void shout() const override {
        printf("汪汪汪，它是 %s（年龄 %d 岁）\n\n", name.c_str(), getAge());
    }
    static int getNum() { return dogNum; }
};

int Dog::dogNum = 0;

class Frog : public Animal {
    static int frogNum;

  public:
    Frog(string name = "Frog") : Animal(name) { age = ++frogNum; }

    void move() const override {
        printf("跳跃 %.1f 英尺！ ", 1 + 0.1 * getAge());
    }
    void shout() const override {
        printf("呱呱呱，它是 %s（年龄 %d 岁）\n\n", name.c_str(), getAge());
    }
    static int getNum() { return frogNum; }
};

int Frog::frogNum = 0;

class Bird : public Animal {
    static int birdNum;

  public:
    Bird(string name = "Bird") : Animal(name) { age = ++birdNum; }

    void move() const override {
        printf("飞行 %.1f 英尺！ ", 10 + 0.1 * getAge());
    }
    void shout() const override {
        printf("啾啾啾，它是 %s（年龄 %d 岁）\n\n", name.c_str(), getAge());
    }
    static int getNum() { return birdNum; }
};

int Bird::birdNum = 0;

void showAnimal(Animal *animal) {
    animal->move();
    animal->shout();
}

int main() {
    Animal *animalList[30];

    for (int i = 0; i < 10; i++)
        animalList[i] = new Dog();
    for (int i = 0; i < 8; i++)
        animalList[10 + i] = new Frog();
    for (int i = 0; i < 12; i++)
        animalList[18 + i] = new Bird();

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(animalList, animalList + 30, g);

    printf("*** 动物园里共有 %d 只动物", Animal::getNum());
    printf("（%d 只狗，%d 只青蛙，%d 只鸟 ***\n\n", Dog::getNum(),
           Frog::getNum(), Bird::getNum());

    printf("------------------------------\n\n");
    printf("先学习学习……\n\n");
    for (int i = 0; i < 5; i++)
        showAnimal(animalList[i]);
    for (int i = 5; i < 10; i++)
        animalList[i]->setName("Animal");
    printf("------------------------------\n\n");
    printf("让小朋友来猜猜（英语）……");
#ifdef _WIN32
    fflush(stdout);
    system("pause");
#else
    printf("请按任意键继续……");
    fflush(stdout);
    system("read");
#endif

    printf("\n");

    int score = 0;
    string ans;

    for (int i = 5; i < 10; i++) {
        showAnimal(animalList[i]);
        printf("猜猜它是什么动物？：");
        std::getline(std::cin, ans);
        const char *name = typeid(*animalList[i]).name();
        if (strstr(name, ans.c_str())) {
            printf("\n✓ 答对了！\n\n");
            score += 20;
        } else {
            printf("\n𐄂 答错了！\n\n");
        }
    }

    printf("*** 你的最终得分：%d ***\n", score);

    return 0;
}
