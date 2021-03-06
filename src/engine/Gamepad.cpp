#include <iostream>
#include "engine/Gamepad.hpp"

void Gamepad::setLayout(LAYOUT layout)
{

    this->layout = layout;
    switch(layout){
        case LAYOUT::KEYBOARD:
            kbutton_map["A"]     = KBUTTON_S{sf::Keyboard::Key::Z,     false};
            kbutton_map["B"]     = KBUTTON_S{sf::Keyboard::Key::X,     false};
            kbutton_map["X"]     = KBUTTON_S{sf::Keyboard::Key::C,     false};
            kbutton_map["Y"]     = KBUTTON_S{sf::Keyboard::Key::V,     false};
            kbutton_map["UP"]    = KBUTTON_S{sf::Keyboard::Key::Up,    false};
            kbutton_map["LEFT"]  = KBUTTON_S{sf::Keyboard::Key::Left,  false};
            kbutton_map["RIGHT"] = KBUTTON_S{sf::Keyboard::Key::Right, false};
            kbutton_map["DOWN"]  = KBUTTON_S{sf::Keyboard::Key::Down,  false};
            break;
        case LAYOUT::PS4:
            button_map["A"]     = BUTTON_S{ 1,  false};
            button_map["B"]     = BUTTON_S{ 2,  false};
            button_map["Y"]     = BUTTON_S{ 3,  false};
            button_map["X"]     = BUTTON_S{ 0,  false};
            button_map["UP"]    = BUTTON_S{-1,  false};
            button_map["LEFT"]  = BUTTON_S{-1,  false};
            button_map["RIGHT"] = BUTTON_S{-1,  false};
            button_map["DOWN"]  = BUTTON_S{-1,  false};
            break;
        case LAYOUT::XB360:
        case LAYOUT::XB1:
            button_map["A"]     = BUTTON_S{ 1,  false};
            button_map["B"]     = BUTTON_S{ 2,  false};
            button_map["Y"]     = BUTTON_S{ 3,  false};
            button_map["X"]     = BUTTON_S{ 0,  false};
            button_map["UP"]    = BUTTON_S{-1,  false};
            button_map["LEFT"]  = BUTTON_S{-1,  false};
            button_map["RIGHT"] = BUTTON_S{-1,  false};
            button_map["DOWN"]  = BUTTON_S{-1,  false};
            break;
        case LAYOUT::GENERIC:
        default:
            break;
    };
}

Gamepad::LAYOUT Gamepad::guessLayout()
{
    // If no index for this gamepad, fail soft(?)
    if(controllerIndex < 0)
        return LAYOUT::GENERIC;

    std::cout << "Guessing Layout for controller at index " << controllerIndex << std::endl;

    sf::Joystick::Identification id = sf::Joystick::getIdentification(controllerIndex);

    switch(id.vendorId){
        // Playstation
        case 0x054c:
            switch(id.productId){
                case 0x05c4: // ps4 conroller
                    std::cout << "PS4 Controller(?)" << std::endl;
                    return LAYOUT::PS4;
                    break;
            }
            break;
        // Microsoft
        case 0x045e:
            switch(id.productId){
                case 0x02ea:
                    std::cout << "XBONE Controller(?)" << std::endl;
                    return LAYOUT::XB1;
                    break;
            }
            break;
        default:
            std::cout << "Couldn't Determine Specific Controller. Consider adding it." << std::endl;
            std::cout << "Vendor ID: " << id.vendorId << " Product ID: " << id.productId << std::endl;
            std::cout << "Defaulting to generic controller layout." << std::endl;
            return LAYOUT::GENERIC;
            break;
    }
}

void Gamepad::update()
{
    if(this->layout == LAYOUT::KEYBOARD){
        for (auto it = kbutton_map.begin(); it != kbutton_map.end(); it++)
        {
            std::string button = it->first;
            bool isPressed = sf::Keyboard::isKeyPressed(it->second.button);

            bool wasPressed = it->second.isDown; 
            if( isPressed && !wasPressed ) // If button pressed, but not already pressed
            {
                // Send button pressed event
                auto event = std::make_shared<GamepadEvent>();
                event->button = button;
                event->type = GamepadEvent::TYPE::PRESSED;
                event->index = controllerIndex;
                Events::queueEvent("gamepad_event", event);
                it->second.isDown = true;
            }
            else if(!isPressed && wasPressed)
            {
                // Send button released event
                auto event = std::make_shared<GamepadEvent>();
                event->button = button;
                event->type = GamepadEvent::TYPE::RELEASED;
                event->index = controllerIndex;
                Events::queueEvent("gamepad_event", event);
                it->second.isDown = false;
            }
            else if(!isPressed)
            {
                it->second.isDown = false;
            }
        }
    }
    else if(!sf::Joystick::isConnected(controllerIndex)){
        if(this->isConnected()){
            std::cout << "CONTROLLER DISCONNECTED" << std::endl;
            this->isConnected_b = false;
        }
    }
    else{
        if(!this->isConnected()){
            this->isConnected_b = true;
            std::cout << "CONTROLLER CONNECTED AT INDEX " << controllerIndex << std::endl;
        }
        // SFML treats the d-pad as an axis for some reason....
        float povx = sf::Joystick::getAxisPosition(controllerIndex, sf::Joystick::PovX);
        float povy = sf::Joystick::getAxisPosition(controllerIndex, sf::Joystick::PovY);

        for (auto it = button_map.begin(); it != button_map.end(); it++)
        {
            std::string button = it->first;
            bool isPressed = false;
            // Special cases for up, down, left, and right
            if( (button == "UP" && povy == -100)     || 
                (button == "DOWN" && povy == 100)    ||
                (button == "LEFT" && povx == -100)   || 
                (button == "RIGHT" && povx == 100)  
            )
                isPressed = true;
            else{
                if(it->second.button >= 0)
                    isPressed = sf::Joystick::isButtonPressed(controllerIndex, it->second.button);
            }
            bool wasPressed = it->second.isDown; 
            if( isPressed && !wasPressed ) // If button pressed, but not already pressed
            {
                // Send button pressed event
                auto event = std::make_shared<GamepadEvent>();
                event->button = button;
                event->type = GamepadEvent::TYPE::PRESSED;
                event->index = controllerIndex;
                Events::queueEvent("gamepad_event", event);
                it->second.isDown = true;
            }
            else if(!isPressed && wasPressed)
            {
                // Send button released event
                auto event = std::make_shared<GamepadEvent>();
                event->button = button;
                event->type = GamepadEvent::TYPE::RELEASED;
                event->index = controllerIndex;
                Events::queueEvent("gamepad_event", event);
                it->second.isDown = false;
            }
            else if(!isPressed)
            {
                it->second.isDown = false;
            }
        }
    }
}

int GamepadController::addGamepads()
{
    // Set up events
    
    // Events::addEventListener("gamepad_button_released", [=](base_event_type e){});
    // Make sure connected joysticks up to date 
    sf::Joystick::update(); 
    std::cout << "Searching for Gamepads..." << std::endl;
    int count = 0;
    for(int i = 0; i < sf::Joystick::Count; i++){
        if(sf::Joystick::isConnected(i)){
            std::cout << "Gamepad found at index " << i << std::endl;
            // find any available gamepad slot
            gamepads[i] = Gamepad(i);
            count++;
        }
    }

    // Default last gamepad to keyboard secretly
    gamepads[count] = Gamepad();
    gamepads[count].setIndex(count);
    gamepads[count].setLayout(Gamepad::LAYOUT::KEYBOARD);
    this->count = count;
    return count;
}

void GamepadController::removeGamepad(int id)
{
    
}

void GamepadController::update()
{
    // try to determine disconnects and connects
    // update controller
    for(auto it = gamepads.begin(); it != gamepads.end(); it++){
        (*it).second.update();
    }
}