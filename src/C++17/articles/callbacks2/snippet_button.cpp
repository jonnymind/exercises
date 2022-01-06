// Ploymorphism through inheritance.
struct VirtualButton {
  virtual void onClick() {}
};

struct OkButton : public VirtualButton {
  virtual void onClick() override {
    std::cout << "You Clicked OK!\n";
  }
};

// Polymorphism through callbacks.
struct CallbackButton {
  using ClickHandler = std::function<void()>;
  CallbackButton(ClickHandler onClick): onClick_{onClick} {}

private:
  ClickHandler onClick_;
};

CallbackButton OkCallbackButton{ []{std::cout << "You clicked OK\n";} };
